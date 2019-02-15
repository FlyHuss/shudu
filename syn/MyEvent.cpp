#include "MyEvent.h"
#include <iostream>
#include "MyMutex.h"
#include <assert.h>

MyEvent::MyEvent():m_Flag(0),IsRecordNum(false){
	mcondition_=new Condition(mmutex_);
}

MyEvent::~MyEvent(){
	delete mcondition_;
}

bool MyEvent::Set(){
	MyMutex mutex_lock(mmutex_);
	mcondition_->notify();
	
	m_Flag++;
	
	return true;
}

bool MyEvent::Wait(){
	MyMutex mutex_lock(mmutex_);	
	
	while(m_Flag==0){
		mcondition_->wait();
	}
	
	if(IsRecordNum){
		m_Flag--;
	}
	else{
		m_Flag=0;
	}

	return true;
}

bool MyEvent::WaitForSecond(double second){
	MyMutex mutex_lock(mmutex_);
	mcondition_->WaitForTime(second);
	if(IsRecordNum){
		m_Flag--;
	}
	else{
		m_Flag=0;
	}
	return true;
}

void MyEvent::SetRecordNum(bool RecordNum){
	IsRecordNum=RecordNum;
}
