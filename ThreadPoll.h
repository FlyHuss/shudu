#ifndef THREADPOLL_H_
#define THREADPOLL_H_

#include "shudu.pb.h"
#include "./syn/MyMutex.h"
#include "./syn/MyEvent.h"
#include <queue>
#include <functional>
#include <boost/noncopyable.hpp>
#include "Dispacher.h"
//#include "EventThread.h"

/*
线程池，初始化若干个专门用来计算的线程，开始陷入沉睡，等待io线程送入数据进行唤醒处理。
处理完了继续睡眠无限循环
 */

class ThreadPoll:boost::noncopyable{
public:
	ThreadPoll();
	~ThreadPoll(){};
	void Init(void *context,int thread_num);
	void Push(::google::protobuf::Message* msg);//外接函数，用来往任务队列加入任务

	static void* ThreadPollFun(void *context);

	/*//因为要用到对象里的成员函数，需要绑定对象，所以用std::function而不是函数指针
	//std::unordered_map<::google::protobuf::Message*,std::function<void(::google::protobuf::Message*)> > msg_callback;//每条message对应一个回调，处理完了就删除，一个用户可能发多个message，但是message不同*/
	
	std::queue<std::shared_ptr<::google::protobuf::Message>> msg_queue;//io就绪的任务队列
	MyEvent msg_queue_event;//自定义的条件变量
	MyMutexPre msg_queue_mutex;//自定义的互斥量
	ProtoMessageDispacher *m_dispacher;//用来注册和分派业务函数
	EventThread *event_thread_ptr;//处理时可能出现一些异常，用来做清理工作
};

#endif
