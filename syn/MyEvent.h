#ifndef MYEVENT_H_
#define MYEVENT_H_
#include "Condition.h"

class MyEvent :boost::noncopyable
{
public:
	MyEvent();
	~MyEvent();
	bool Set();
	bool Wait();
	bool WaitForSecond(double second);
	void SetRecordNum(bool RecordNum);
private:
	MyMutexPre mmutex_;
	Condition* mcondition_;
	int m_Flag;
	bool IsRecordNum;
};

#endif

//这个是阻塞队列用到的，不需要和外界的互斥量合作
