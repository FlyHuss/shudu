#ifndef DISPACHER_H_
#define DISPACHER_H_
#include <boost/noncopyable.hpp>
#include <memory>//std::shared_ptr
#include "shudu.pb.h"
//#include "EventThread.h"

class EventThread;//前置声明

/*
这个文件一共包含三个class。
第一个是外层包装普通类，第二个是类型派生不同功能的内层模板类，第三个是外部接口注册用的普通类（也可以看成使用Dispacher的类）
为什么不把这些放在第一个类中？因为虚基类最好不要包含成员变量，继承关系转换很复杂，效率也低
仔细看可以发现，第一个和第二个类中没有成员变量。new一个对象也只是一个字节的空对象，主要是包含各种函数调用。
 */

//最外层的普通类，利用指针的多态性质固定对象类型，使相同的基类指向不同的派生类，放入map中
class Dispacher:boost::noncopyable{
public:
	Dispacher(){};
	~Dispacher(){};
	virtual void CallbackFunc(std::shared_ptr<::google::protobuf::Message> &msg_ptr)=0;//纯虚函数，不允许构造虚基类，只能构造派生类
};

//内层的模板类，模板参数T用来萃取具体的message类型
//注意！！！模板类的成员函数一定要放在.h文件里面,因为模板类本质上不是类，只有实例化之后才会编译生成.o文件
template <typename T>
class DispacherT:public Dispacher{
public:
	explicit DispacherT(const std::function<void(std::shared_ptr<T>)> &cb):call_back_fun(cb){};
	//~DispacherT(){};//不需要virtual析构，没有什么要释放的

	//调用对应的回调函数,因为shared_ptr不能自动向下转型，需要手动转成T，而不是在参数里直接传入转成T
	virtual void CallbackFunc(std::shared_ptr<::google::protobuf::Message> &msg_ptr){//调用回调函数
		std::shared_ptr<T> msg_ptrT(std::static_pointer_cast<T>(msg_ptr));
		call_back_fun(msg_ptrT);//传入对应的具体的派生类型进行计算
	};
private:
	//用来保存RegisterCallback传进来的回调函数地址,通过模板实例化，不同的回调函数会实例化出不同的class，保存不同的回调函数
	//里面是传进这个函数的具体new出来的message数据制作，用完需要释放，所以用std::shared_ptr
	std::function<void(std::shared_ptr<T>)> call_back_fun;
};

//第三个类，相当于使用前两个类的用户类,前两个类不被用户使用，其实是可以单独写一个文件拿出去用的，前两个是通用的
class ProtoMessageDispacher{
public:
	//explicit ProtoMessageDispacher(EventThread *context):event_thread_ptr(context){};
	void Init(EventThread *context){
		event_thread_ptr=context;
	}
	//使用一个模板函数，萃取出传入的函数的参数类型。再通过new实例化注册到DispacherT中生成一个具体实例
	template<typename T>
	void RegisterCallback(std::function<void(std::shared_ptr<T>)> cb){
		std::shared_ptr<Dispacher> dispacher_ptr(new DispacherT<T>(cb));//智能指针自动管理实例出来的DispacherT的生命周期
		msg_callback[T::descriptor()]=dispacher_ptr;//每种message type对应的descriptor是相同的，可以用这个多对一的性质给同一类message映射同一个处理函数
	};

	void FindAndUseDispacher(std::shared_ptr<::google::protobuf::Message> &msg_ptr);
	//std::unordered_map<const ::google::protobuf::Descriptor*,std::shared_ptr<Dispacher>>::const_iterator FindMsgCallback();
private:
	//不同的message对应的Descriptor是相同的，所以在这个程序里，实际只用注册两个Descriptor和其对应的回调函数。用完了也不用删除可以重用。
	//map里总共就两个元素，增加其他消息类型元素会增加
	//如果map对应的value是函数对象的话，无法使函数里的参数是具体的类型。所以需要外层一个模板类，类内包含这个模板函数，模板类之上包含一个普通类。
	//把普通类的指针变成value就可以封装该变化点了。（因为map的key和value是确定类型）
	//Dispacher*指针用std::shared_ptr保存，最后退出的时候自动清理掉map，以至于自动析构Dispacher（它是在初始化时Regiser new出来的）
	std::unordered_map<const ::google::protobuf::Descriptor*,std::shared_ptr<Dispacher>> msg_callback;
	EventThread *event_thread_ptr;//可能会处理未知消息类型，所以这里也需要有清除功能
};

#endif
