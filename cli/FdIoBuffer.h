#ifndef FDIOBUFFER_H_
#define FDIOBUFFER_H_

#include <boost/noncopyable.hpp>
#include <errno.h>
//#include <vector>
#include <string>
#include <stdint.h>

#define BUFSIZE 512
#define MAX_EMPTY_LEN 512

/*
因为用户io可能数据只收到部分，不完整，而io线程使用的epoll管理所有的io fd ，所以不能只用一个buf来缓存数据，必须多个buf
要么多个io线程，一个线程一个io buf;要么一个io线程，但是每个fd映射一个buf
我选择方案二。因为我觉得本程序io任务的比重并不大

由于每个buf和一个fd绑定且只在io线程使用，所以不需要考虑线程安全
*/

class FdIoBuffer:boost::noncopyable
{
public:
	FdIoBuffer();
	~FdIoBuffer();

	int ReadFd(int fd);//从fd中读取已到达的所有数据放入自带的buf中。同时具备扩容功能，保证能容纳所有数据。
	int BufDataSize();//返回buf目前所有数据量
	void MoveReadPtr(int len);//因为数据的序列化和反序列化由protobuf负责，所以buffer只需要移动读指针就可以了
	//void MoveWritePtr(int len);
	void Clear();
	
	//以下是专门为了取数据的协议而设的。  数据为： data总长度(4)+name长度(4)+ name(以proto.开头,以\0结尾) + protdata
	uint32_t GetDataLen();//可以用来取前面两个4字节的数据(不会自己移动数据指针，因为需要有一个检测判断数据是否完整，不完整则取消本次读取)
	std::string GetProtoName(int len);//取name（会自己移动数据指针，因为读取name就说明数据检测已经通过）
	char *GetDataPtr();//获取protodata数据的首地址，送给protobuf的反序列化api来使用。
	//以下为了发送数据而设（发数据也需要控制，如果套接字缓存空间不够写的，因为是非阻塞io，只能写部分进去，而不是等待）
	/*char *GetWritePtr();
	char *GetHeadPtr();//把数据写入buf的时候，会先空出4个字节，留出最后写数据总长度
	int BufLeftSize();
	void SetDataEnd();//把数据头的数据总长度写完后调用这个，表示一条数据写完了
	int WriteToBuf(std::string &str,int len);
	int WriteToFd(int fd);//把缓存数据写到套接字里(由poll提示可写的时候就往fd写，但是能写多少就返回多少，如果发送缓冲区满了，应该由用户配合BufSize来控制。*/
private:
	char* m_buf;
	int cur_buf_size;//当前分配的m_buf总大小
	int write_ptr;
	//int head_ptr;//for write data len,and only use at write buf
	int read_ptr;
};

#endif
