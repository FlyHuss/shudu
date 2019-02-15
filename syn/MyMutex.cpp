#include "MyMutex.h"

MyMutex::MyMutex(MyMutexPre& m):mutexpre_(m){
	mutexpre_.Lock();
}

MyMutex::~MyMutex(){
	mutexpre_.Unlock();
}

#define MyMutex(x) static_assert(false,"MyMutex对象一定要有变量名");
//这条宏是为了防止创建MyMutex()的时候定义一个匿名变量导致锁的作用域提前结束