#include "Condition.h"
#include <iostream>
#include <time.h>
#include <errno.h>
#include <assert.h>

Condition::Condition(MyMutexPre& m):mutexpre_(m){
	int r=pthread_cond_init(&mcond_,NULL);
	assert(r==0);
}

Condition::~Condition(){
	int r=pthread_cond_destroy(&mcond_);
	assert(r==0);
}

bool Condition::wait(){
	int r=pthread_cond_wait(&mcond_,mutexpre_.GetMutex());
	assert(r==0);
	return true;
}

bool Condition::notify(){
	int r=pthread_cond_signal(&mcond_);
	assert(r==0);
	return true;
}

bool Condition::notifyall(){
	int r=pthread_cond_broadcast(&mcond_);
	assert(r==0);
	return true;
}

bool Condition::WaitForTime(double seconds){
	struct timespec abstime;
	clock_gettime(CLOCK_REALTIME, &abstime);
	const int64_t kNanoSecondsPerSecond = 1e9;
	int64_t nanoseconds = static_cast<int64_t>(seconds * kNanoSecondsPerSecond);//秒转换成纳秒
	abstime.tv_sec += static_cast<time_t>((abstime.tv_nsec + nanoseconds) / kNanoSecondsPerSecond);
	abstime.tv_nsec = static_cast<long>((abstime.tv_nsec + nanoseconds) % kNanoSecondsPerSecond);
	int r=pthread_cond_timedwait(&mcond_, mutexpre_.GetMutex(), &abstime);
	assert(r==0||r==ETIMEDOUT);
	return true;
}
