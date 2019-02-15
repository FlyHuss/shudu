#include "SolveServer.h"
#include <string>
#include <iostream>
#include <arpa/inet.h>

//数独任务的外部接口函数
void SolveServer::SolveShudu(proto::shudu_data* msg_data){
	//proto::shudu_data* data_ptr=(proto::shudu_data *)msg_data;
	std::cout<<data_ptr->time()<<std::endl;//打印数据的time字符串
	std::string *shudu_data=data_ptr->mutable_data();//返回data的非const
	int fd=thread_poll_ptr->msg_fd[msg_data];//获取对应的fd，用来返回结果

	time_t timep;
	time(&timep);
	data_ptr->set_time(ctime(&timep));//处理的时间写到结果中，shudu任务可以直接在原数据上修改再发回

	if (shudu_data->length()<9*9){
		//执行到这里说明发送的数据有问题
		std::string buf;
		data_ptr->SerializeToString(&buf);//原数据直接序列化到buf中发出
		write(fd,&buf[0],buf.size());//发送
		std::unordered_map<::google::protobuf::Message*,std::function<void(::google::protobuf::Message*)>>::iterator iter=thread_poll_ptr->msg_callback.find(msg_data);
		thread_poll_ptr->msg_callback.erase(iter);
		std::unordered_map<::google::protobuf::Message*,int>::iterator iter2=thread_poll_ptr->msg_fd.find(msg_data);
		thread_poll_ptr->msg_fd.erase(iter2);
		delete msg_data;
		return ;
	}
	//正常数据情况
	SolveShuduHelper( *shudu_data, 0, 0 );//执行解数独程序

	//把正确答案打包发送回给客户
	std::string send_buf(8,0);
    std::string type_name=data_ptr->GetTypeName();
    uint32_t n=htonl(type_name.length()+1);//字符串长度转换成网络字节序
    memcpy(&send_buf[4],&n,sizeof(uint32_t));
    send_buf.append(type_name);//append和'+'不同的地方在于这里的n比length多1,把'\0'也append进去了
    send_buf+="a";// \0 pos
    int pos=send_buf.length()-1;
    if(!data_ptr->AppendToString(&send_buf))//把结果序列化到buf中准备发送
        std::cout<<"AppendToString error!\n";
    n=htonl(send_buf.length()-4);//把数据总长度转换成网络字节序
    memcpy(&send_buf[0],&n,sizeof(uint32_t));
    int len=send_buf.length();
    send_buf[pos]='\0';
    int nwrite=0;
    while(len!=nwrite){// send all
        nwrite+=write(fd,&send_buf[nwrite],len-nwrite);
    }

	std::unordered_map<::google::protobuf::Message*,std::function<void(::google::protobuf::Message*)>>::iterator iter=thread_poll_ptr->msg_callback.find(msg_data);
	thread_poll_ptr->msg_callback.erase(iter);
	std::unordered_map<::google::protobuf::Message*,int>::iterator iter2=thread_poll_ptr->msg_fd.find(msg_data);
	thread_poll_ptr->msg_fd.erase(iter2);
	delete msg_data;
}

//求解数独的算法函数
bool SolveServer::SolveShuduHelper(std::string &data, int i, int j){
	if (i == 9)return true;//已经遍历完全部
	if (j == 9)return SolveShuduHelper( data, i + 1, 0 );//按行遍历

	if (data[i*9+j] == '.') {
		for (int k = 1; k <= 9; k++) {
			data[i*9+j] = k + '0';
			if (IsValid( data, i, j )) {
				if (SolveShuduHelper( data, i, j+1 ))return true;//这里是递归的关键，如果当前的ij值有效，就用if测试下一个，不断往深处，如果是true，递归的过程board已经全部改完，这里直接返回true
			}
			data[i*9+j] = '.';//否则的话就变成. 这里是从深处往回返，所以需要把深处的改变过的改回来，再穷举下一个k
		}
		return false;//递归深处某层遍历完所有都没结果，则false返回到浅层，然后回到44行的if进入下一个k
	}
	else
		return SolveShuduHelper( data, i, j + 1 );//已经填好数字的部分
}

//求解需要用到，计算该数独目前是否合法
bool SolveServer::IsValid( std::string &data, int i, int j ) {
	for (int k = 0; k < 9; k++)
		if (k != j&&data[i*9+j] == data[i*9+k])return false;
	for (int k = 0; k < 9; k++)
		if (k != i&&data[i*9+j] == data[k*9+j])return false;
	for (int m = i / 3 * 3; m < i / 3 * 3+ 3; m++) {
		for (int n = j / 3 * 3; n <j / 3 * 3+ 3; n++) {
			if (( m != i || n != j ) && data[m*9+n] == data[i*9+j])return false;
		}
	}
	return true;
}

//echo任务的外部接口函数,数据原样写回
void SolveServer::SolveEcho(proto::echo_data* data_ptr){
	//proto::shudu_data* data_ptr=(proto::shudu_data *)msg_data;
	
	std::string send_buf(8,0);
    std::string type_name=data_ptr->GetTypeName();
    uint32_t n=htonl(type_name.length()+1);
    memcpy(&send_buf[4],&n,sizeof(uint32_t));
    send_buf.append(type_name);//append和'+'不同的地方在于这里的n比length多1,把'\0'也append进去了
    send_buf+="a";// \0 pos
    int pos=send_buf.length()-1;
    if(!data_ptr->AppendToString(&send_buf))
        std::cout<<"AppendToString error!\n";
    n=htonl(send_buf.length()-4);
    memcpy(&send_buf[0],&n,sizeof(uint32_t));
    int len=send_buf.length();
    send_buf[pos]='\0';
    int nwrite=0;
    int fd=thread_poll_ptr->msg_fd[msg_data];
    while(len!=nwrite){// send all
        nwrite+=write(fd,&send_buf[nwrite],len-nwrite);
    }

	std::unordered_map<::google::protobuf::Message*,std::function<void(::google::protobuf::Message*)>>::iterator iter=thread_poll_ptr->msg_callback.find(msg_data);
	thread_poll_ptr->msg_callback.erase(iter);
	std::unordered_map<::google::protobuf::Message*,int>::iterator iter2=thread_poll_ptr->msg_fd.find(msg_data);
	thread_poll_ptr->msg_fd.erase(iter2);
	delete msg_data;
}
