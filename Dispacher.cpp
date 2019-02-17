#include "EventThread.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <assert.h>
#include <sys/time.h>
#include <stdio.h>
#include <sys/epoll.h>
#include <algorithm>
#include <string.h>
#include <stdlib.h>
#include "FdIoBuffer.h"
#include <functional>

/*
EventThread类主要存放了几乎所有的数据，来使其他线程通过传入它的this指针共享这些数据。
另外它本身就是accept线程，接受客户的连接，并通知io线程有新用户已连接。
同时，通过它启动了IO线程，并初始化线程池，初始化回调等等，是整个程序的发起者。
 */

//构造函数
EventThread::EventThread():accept_looping(false),io_looping(false){
	cli_queue_event.SetRecordNum(true);//初始化自己的条件变量类。true表示是计数模式，即多个signal可以累加，wait的时候如果之前signal过就不会阻塞而是直接取
};

//accept线程的业务循环
//初始化线程的所有资源，以及初试完后进入无限循环accept客户以及把客户fd加入epoll等待数据
void EventThread::Run(){
	int r=pthread_create(&io_thread,NULL,io_thread_func,this);//在accept线程中先启动io线程
	assert(r==0);
	//注册实例化两个不同的处理函数
	m_dispacher.Init(this);
	m_thread_poll.Init(&m_dispacher,this,INIT_THREAD_POLL_NUM);//初始化线程池
	m_solve_server.Init(this);//初始化solve函数
	m_dispacher.RegisterCallback<proto::shudu_data>(std::bind(&SolveServer::SolveShudu, &m_solve_server, std::placeholders::_1));
	m_dispacher.RegisterCallback<proto::echo_data>(std::bind(&SolveServer::SolveEcho, &m_solve_server, std::placeholders::_1));

	accept_looping=true;//线程启动的标志，最后用来设置停止无限循环退出

	int listen_fd=socket(AF_INET,SOCK_STREAM,NULL);
	//就算是epoll也需要accept非阻塞，
	//如果epoll返回和accept调用的之间，用户中断连接rst，待连接队列就会剔除该连接，
	//如果此时队列为空，则可能阻塞。
	r=fcntl(listen_fd,F_GETFL,0);//获取
	assert(r!=-1);
	r=fcntl(listen_fd,F_SETFL,r|O_NONBLOCK);//设置
	assert(r!=-1);

	//绑定套接字和服务器的监听地址
	sockaddr_in serv_addr;
    serv_addr.sin_family=AF_INET ;
    inet_pton(AF_INET,SERV_LISTEN_ADDR,&serv_addr.sin_addr);
    serv_addr.sin_port=htons(SERV_LISTEN_PORT);
    r=bind(listen_fd,(sockaddr *)&serv_addr,sizeof(serv_addr));
    assert(r!=-1);
    r=listen(listen_fd,LISTENQ);
    assert(r!=-1);

    int cli_fd;//保存accept的客户fd

    int nfd;
	epfd = epoll_create(1);//创建epoll
	struct epoll_event ev,events[MAX_EVENTS];
	ev.data.fd = listen_fd;
    ev.events = EPOLLIN|EPOLLET;//可读，边沿触发
    epoll_ctl(epfd, EPOLL_CTL_ADD, listen_fd, &ev);//监听套接字加入epoll

	while(accept_looping){
		nfd=epoll_wait(epfd, events, MAX_EVENTS, -1);//epoll等待可读
		if(nfd<0){//epoll被唤醒，就绪事件<0说明出错
			if(errno==EINTR)//epoll被信号中断,忽略继续
				continue;
			else{
				std::cout<<"epoll_wait error!"<<std::endl;
				exit(-1);
			}
		}

		//epoll正常唤醒
		for(int i=0;i<nfd;++i){
			if(events[i].data.fd==listen_fd){//有客户连接的事件
				cli_fd=accept(listen_fd,NULL,NULL);
				//connect_count++;
				ev.data.fd = cli_fd;
    			ev.events = EPOLLIN|EPOLLET;
    			epoll_ctl(epfd, EPOLL_CTL_ADD, cli_fd, &ev);//新accept的客户fd加入epoll的读事件

    			r=fcntl(cli_fd,F_GETFL,0);
				assert(r!=-1);
				r=fcntl(cli_fd,F_SETFL,r|O_NONBLOCK);//客户的读事件也要设置成非阻塞
				assert(r!=-1);

				{
					MyMutex lock(cli_queue_mutex);//上锁
    				cli_fd_bufs[cli_fd]=new FdIoBuffer(this);//给每个客户fd一个独立的接收缓存
    			}
			}
			else{//已连接的客户发送数据过来了的事件
				MyMutex lock(cli_queue_mutex);//上锁
				cli_queue.push(events[i].data.fd);//客户可读队列放入任务，交给io线程处理
				cli_queue_event.Set();//通知
			}
		}
	}//while无限循环
	close(epfd);//执行到这说明程序结束了，关闭epoll
}

//用来停止主线程循环的
void EventThread::Stop(){
	accept_looping=false;
	io_looping=false;
	pthread_join(io_thread,NULL);
}

//往对应的msg注册fd是io线程的事情，所以还是需要一个set函数存取私有数据
void EventThread::SetMsgFd(std::shared_ptr<::google::protobuf::Message> msg_ptr,int fd){
	MyMutex lock(cli_queue_mutex);//上锁
	msg_fd[msg_ptr]=fd;
}

//由Message找到对应的fd，后面返回消息要用，关闭连接的清理也要用到
int EventThread::FindMsgFd(std::shared_ptr<::google::protobuf::Message> msg_ptr){
	MyMutex lock(cli_queue_mutex);//上锁
	return msg_fd[msg_ptr];
}

//删除Message对应的fd
void EventThread::DeleteMsgFd(std::shared_ptr<::google::protobuf::Message> msg_ptr){
	MyMutex lock(cli_queue_mutex);//上锁
	msg_fd.erase(msg_ptr);
}

//由fd找到对应的buf
FdIoBuffer *EventThread::FindFdBuf(int fd){
	MyMutex lock(cli_queue_mutex);//上锁
	return cli_fd_bufs[fd];
}

//关闭连接或异常情况，删除fd对应的buf
void EventThread::DeleteFdBuf(int fd){
	MyMutex lock(cli_queue_mutex);//上锁
	cli_fd_bufs.erase(fd);
}

//io线程的业务逻辑函数
//这有一个坑
//一个accept reactor+多个io reactor的模式不能分成两种epoll来做。
//不然只能accept得到的描述符放入一个队列，然后io reactor到队列去取，
//这样的话，io reactor还需要一个epoll来处理fd上的事件，如果io事件阻塞不发，io reactor就会一直阻在epoll而得不到accept的新fd
//所以只能由一个epoll循环同时监听accept和描述符的io，区分是accept还是io事件，当有io的时候再派遣到io 线程去
//这也就是为什么 io复用 不能和阻塞操作一起使用的原因了。
void* EventThread::io_thread_func(void *context){
	EventThread *event_thread=(EventThread *)context;//取EventThread类传过来的this指针，共享它的数据

	std::queue<int> temp_cli_queue;//客户可读的任务队列，通过一次性把cli_queue的fd全部copy过来再对它们处理，减少临界区使用时间

	event_thread->io_looping=true;
	while(event_thread->io_looping){
		event_thread->cli_queue_event.Wait();//等待任何客户io数据过来
		{
			MyMutex lock(event_thread->cli_queue_mutex);//上锁
			std::swap(temp_cli_queue,event_thread->cli_queue);//每次用一个空的temp_queue和cli_queue swap，这样然后到临界区外面处理temp_queue,可以减小临界区使用时间
		}
		while(!temp_cli_queue.empty()){//处理所有就绪的io事件的客户fd
			int fd=temp_cli_queue.front();
			temp_cli_queue.pop();

			//codec
			FdIoBuffer* buf_ptr=(event_thread->cli_fd_bufs)[fd];//获取该fd对应的缓存
			int n=buf_ptr->ReadFd(fd);//把用户发来的所有数据装入对应fd的缓存
			if(fd==-1){//上一步的ReadFd内检测到对端关闭了连接
				delete buf_ptr;//删除fd对应的缓存
				continue;
			}
			std::cout<<"ReadFd over\n";
			int buf_size=buf_ptr->BufDataSize();//获取buf内数据的数目
			if(buf_size>HEADER_LEN){//这里是消息处理的步骤，只有缓存收到一条完整的消息之后才会处理。
				int data_len=buf_ptr->GetDataLen();//超过头部才解析头部表示的该条数据字节数
				std::cout<<"data len : "<<data_len<<std::endl;
				if(buf_size>=HEADER_LEN+data_len){//判断是否收到大于等于一条完整数据
					//读取数据,所有数据应该放在最外层的thread线程，由creat传入this指针共享，免得复制数据浪费时间
					buf_ptr->MoveReadPtr(HEADER_LEN);//每条消息的格式是 总长度(4)+name长度(4)+name+protdata
					int name_len=buf_ptr->GetDataLen();
					buf_ptr->MoveReadPtr(HEADER_LEN);
					data_len-=HEADER_LEN;
					std::string prot_name=buf_ptr->GetProtoName(name_len);
					data_len-=name_len;
					
					//通过反射获取protobuf解析数据
					const ::google::protobuf::Descriptor* m_descriptor=::google::protobuf::DescriptorPool::generated_pool()->FindMessageTypeByName(prot_name);
					const ::google::protobuf::Message* proto_type=::google::protobuf::MessageFactory::generated_factory()->GetPrototype(m_descriptor);
					std::shared_ptr<::google::protobuf::Message> m_data_ptr(proto_type->New());//@这里的指针需要自己删除，后面改成智能指针试试
					//从buf中反序列化数据
					m_data_ptr->ParseFromArray(buf_ptr->GetDataPtr(),data_len);
					buf_ptr->MoveReadPtr(data_len);
					event_thread->SetMsgFd(m_data_ptr,fd);//每个消息对应的fd。用来处理完关闭连接
					//可以通过new出来的Message得到Descriptor，所以传入线程池的就是m_data_ptr，
					//在里面再根据m_data_ptr获得Descriptor放入map得到处理函数，直接把m_data_ptr放入对应的处理函数即可
					event_thread->m_thread_poll.Push(m_data_ptr);//把得到的数据和fd放入线程池计算，并由线程池send写回，关闭描述符由用户关闭连接控制。

					/*这一段不再需要了，通过字符串匹配来找对应的处理函数的方法太恶心了
					if(strcmp(prot_name.c_str(),"proto.shudu_data")==0){
						proto::shudu_data* data_ptr=(proto::shudu_data *)m_data;
						data_ptr->ParseFromArray(buf_ptr->GetDataPtr(),data_len);
						buf_ptr->MoveReadPtr(data_len);
						event_thread->m_thread_poll.msg_callback[m_data]=std::bind(&SolveServer::SolveShudu,&(event_thread->m_solve_server),std::placeholders::_1);
						event_thread->m_thread_poll.msg_fd[m_data]=fd;

						event_thread->m_thread_poll.Push(m_data);//把得到的数据和fd放入线程池计算，并由线程池send写回，关闭描述符由用户关闭连接控制。
					}
					else{
						if(strcmp(prot_name.c_str(),"proto.echo_data")==0){
							proto::echo_data* data_ptr=(proto::echo_data *)m_data;
							m_data->ParseFromArray(buf_ptr->GetDataPtr(),data_len);
							buf_ptr->MoveReadPtr(data_len);
							event_thread->m_thread_poll.msg_callback[m_data]=std::bind(&SolveServer::SolveEcho,&(event_thread->m_solve_server),std::placeholders::_1);
							event_thread->m_thread_poll.msg_fd[m_data]=fd;

							event_thread->m_thread_poll.Push(m_data);
						}
						else{//未知数据
							std::cout<<"unknow message!\n";
							std::unordered_map<int,FdIoBuffer *>::iterator iter=event_thread->cli_fd_bufs.find(fd);
							event_thread->cli_fd_bufs.erase(iter);
							event_thread->connect_count--;
							epoll_ctl(event_thread->epfd, EPOLL_CTL_DEL, fd, NULL);
							delete buf_ptr;
							close(fd);
						}
					}*/

				}//can retrive
			}//retrive data
		}//处理所有fd while
	}//loop while
}
