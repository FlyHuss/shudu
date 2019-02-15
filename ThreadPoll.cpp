#include "ThreadPoll.h"
#include <assert.h>

ThreadPoll::ThreadPoll(){
	msg_queue_event.SetRecordNum(true);
}

//初始化线程池内的各种资源，compute线程等
void ThreadPoll::Init(ProtoMessageDispacher * dispacher_ptr,EventThread *event_ptr,int thread_num){
	m_dispacher=dispacher_ptr;
	event_thread_ptr=event_ptr;
	int r;
	pthread_t temp;
	for(int i=0;i<thread_num;++i){//创建线程池内的所有compute线程，一次性创建完，后面不释放也不再创建
		r=pthread_create(&temp,NULL,ThreadPollFun,this);//线程池数量一开始就全部分配好，生命和进程相同，所以不用管理描述符
		assert(r==0);
	}
};

//外接函数，外界调用,往线程池的任务队列放入任务
void ThreadPoll::Push(std::shared_ptr<::google::protobuf::Message> msg){
	{
		MyMutex mlock(msg_queue_mutex);
		msg_queue.push(msg);
	}
	msg_queue_event.Set();//signal
}

//线程池内每个compute线程的业务逻辑
void* ThreadPoll::ThreadPollFun(void *context){
	ThreadPoll *thread_poll_ptr=(ThreadPoll *)context;
	while(1){//无限循环
		thread_poll_ptr->msg_queue_event.Wait();//等待客户任务到来
		std::shared_ptr<::google::protobuf::Message> msg_data_ptr;
		{
			MyMutex mlock(thread_poll_ptr->msg_queue_mutex);
			msg_data_ptr=thread_poll_ptr->msg_queue.front();//从任务队列去除io线程送过来的io数据准备处理
			thread_poll_ptr->msg_queue.pop();
		}
		m_dispacher->FindAndUseDispacher(msg_data_ptr);//调用回调函数处理具体计算逻辑
	}
}