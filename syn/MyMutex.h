#ifndef MYMUTEX_H
#define MYMUTEX_H
#include "MyMutexPre.h"

class MyMutex : boost::noncopyable{
public:
	explicit MyMutex(MyMutexPre& m);

	~MyMutex();
private:
	MyMutexPre& mutexpre_;//引用可以防止拷贝后多次析构，这里用智能指针也是可以的
};

#endif
