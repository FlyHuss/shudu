#include "MyMutexPre.h"
#include <iostream>
#include <assert.h>

MyMutexPre::MyMutexPre(){
	int r=pthread_mutex_init(&mutex_,NULL);
	assert(r==0);
}

MyMutexPre::~MyMutexPre(){
	//assert(holder_==0)
	int r=pthread_mutex_destroy(&mutex_);
	assert(r==0);
}

void MyMutexPre::Lock(){
	int r=pthread_mutex_lock(&mutex_);
	assert(r==0);
}

void MyMutexPre::Unlock(){
	//holder_=0;
	int r=pthread_mutex_unlock(&mutex_);
	assert(r==0);
}

/*bool MyMutexPre::isLockedByThisThread(){
	return holder_==boost::this_thread::get_id();
}*/

pthread_mutex_t * MyMutexPre::GetMutex(){
	return &mutex_;
}
