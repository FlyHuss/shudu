#ifndef MYMUTEXPRE_H
#define MYMUTEXPRE_H
#include<pthread.h>
#include<boost/noncopyable.hpp>

class MyMutexPre : boost::noncopyable{//不允许复制拷贝
friend class MyMutex;
friend class Condition;
public:
	MyMutexPre();
	~MyMutexPre();
	void Lock();
	void Unlock();
	//bool isLockedByThisThread();//防止锁两次产生死锁,或者其他的特殊用处

private:
	pthread_mutex_t *GetMutex();
private:
	pthread_mutex_t mutex_;
	//pid_t holder_;//用来保存当期的线程tid，判断持有这个锁的线程是否是自己
};

#endif
