#include "Dispacher.h"
#include "EventThread.h"
#include <sys/epoll.h>
#include <iostream>

//调用对应的回调函数,因为shared_ptr不能自动向下转型，需要手动转成T，而不是在参数里直接传入转成T
/*template <typename T>
void DispacherT<T>::CallbackFunc(std::shared_ptr<::google::protobuf::Message> msg_ptr){//在传递参数的时候就自动向下转型了，变成了具体的派生类型
	std::shared_ptr<T> msg_ptrT((T*)msg_ptr);
	call_back_fun(msg_ptrT);//传入对应的具体的派生类型进行计算
};*/

//注册具体的DispacherT实例的函数，由用户调用，有几种业务逻辑就注册几次
//第一个传入的参数要是descriptor，而不是proto_message_type，因为descriptor是确定的name得出的，可以直接new出对应的派生实体
//而不再需要字符串判断.
/*template<typename T>
void ProtoMessageDispacher::RegisterCallback(std::function<void(std::shared_ptr<T>)> cb){
	std::shared_ptr<Dispacher> dispacher_ptr(new DispacherT<T>(cb));//智能指针自动管理实例出来的DispacherT的生命周期
	msg_callback[T::descriptor()]=dispacher_ptr;//每种message type对应的descriptor是相同的，可以用这个多对一的性质给同一类message映射同一个处理函数
};*/

//用来查找具体Message对应的DispacherT并调用。第三个类是来使用第一、第二个的。不需要用户调用第一第二的方法
void ProtoMessageDispacher::FindAndUseDispacher(std::shared_ptr<::google::protobuf::Message> &msg_ptr){
	std::unordered_map<const ::google::protobuf::Descriptor*,std::shared_ptr<Dispacher>>::const_iterator iter=msg_callback.find(msg_ptr->GetDescriptor());
	if(iter!=msg_callback.end()){
		iter->second->CallbackFunc(msg_ptr);//实际调用处理函数
	}else{
		//没有注册过的函数,默认关闭连接，清理
		std::cout<<"no RegisterCallback!"<<std::endl;
		int fd=event_thread_ptr->FindMsgFd(msg_ptr);
		FdIoBuffer* buf_ptr=event_thread_ptr->FindFdBuf(fd);
		event_thread_ptr->DeleteFdBuf(fd);
		epoll_ctl(event_thread_ptr->epfd, EPOLL_CTL_DEL, fd, NULL);
		delete buf_ptr;
		close(fd);
	}
	//event_thread_ptr->DeleteMsgFd(msg_ptr);
}
