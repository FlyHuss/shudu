#ifndef CONDITION_H
#define CONDITION_H
#include<boost/noncopyable.hpp>
#include "MyMutexPre.h"

class Condition : boost::noncopyable{
public:
	explicit Condition(MyMutexPre &m);
	~Condition();
	bool wait();
	bool notify();
	bool notifyall();
	bool WaitForTime(double seconds);
	//MyMutexPre& GetMutex();

private:
	MyMutexPre& mutexpre_;
	pthread_cond_t mcond_;
};

#endif

//突然发现条件变量有一个坑啊，如果是单线程程序使用wait或者timedwait，会直接返回。多线程才会阻塞
