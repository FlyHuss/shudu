#ifndef EVENTTHREAD_H_
#define EVENTTHREAD_H_

#include <pthread.h>
#include "./syn/MyMutex.h"
#include "./syn/MyEvent.h"
#include <unordered_map>
#include "shudu.pb.h"
#include <queue>
#include "ThreadPoll.h"
#include "SolveServer.h"
#include "Dispacher.h"

class FdIoBuffer;//前置声明

#define INIT_THREAD_POLL_NUM 8
#define HEADER_LEN 4
#define LISTENQ 5
#define MAX_EVENTS 1024
#define SERV_LISTEN_PORT 10086
#define SERV_LISTEN_ADDR "127.0.0.1"

class EventThread:boost::noncopyable
{
public:
	EventThread();
	~EventThread(){};
	static void* io_thread_func(void *context);

	void Run();//启动该线程,内部是一个循环用来accept客户以及把客户fd加入epoll等待数据
	void Stop();//stop以后就不能再Run了，而要重新运行程序
	void SetMsgFd(std::shared_ptr<::google::protobuf::Message> msg_ptr,int fd);
	int FindMsgFd(std::shared_ptr<::google::protobuf::Message> msg_ptr);//由Message找到对应的fd
	void DeleteMsgFd(std::shared_ptr<::google::protobuf::Message> msg_ptr);//关闭连接或异常情况，删除Message对应的fd
	FdIoBuffer *FindFdBuf(int fd);//由fd找到对应的buf
	void DeleteFdBuf(int fd);//关闭连接或异常情况，删除fd对应的buf

	int epfd;//epoll的句柄,懒得写geter了，放在public别乱调用就行
private:
	pthread_t io_thread;//io线程的线程号，回收时用
	bool accept_looping,io_looping;//控制无限循环启动开始的标志
	//int connect_count;//小于等于int的基础类型修改在多线程下不用加锁的,这个是用来控制连接数的，好像没用上，功能没写

	std::queue<int> cli_queue;//io线程取客户读任务的阻塞队列
	MyMutexPre cli_queue_mutex;//自己封装的互斥量类
	MyEvent cli_queue_event;//自己封装的条件变量类

	std::unordered_map<int,FdIoBuffer *> cli_fd_bufs;//每个fd专属的接收buf,每个已连接的用户可能有多条消息，也可能每次只收到半条消息，需要用一个buf存起来，等到够一条完整的消息后再用protobuf反序列化处理
	std::unordered_map<std::shared_ptr<::google::protobuf::Message>,int> msg_fd;//每条message对应一个fd，处理完了就删除，一个用户可能发多个message，但是message不同

	ThreadPoll m_thread_poll;//线程池
	SolveServer m_solve_server;//线程池中用来计算业务的函数
	ProtoMessageDispacher m_dispacher;//用来注册和分派业务函数
};

#endif
