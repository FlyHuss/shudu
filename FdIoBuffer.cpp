#include "FdIoBuffer.h"
#include <iostream>
#include <cstring>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>

FdIoBuffer::FdIoBuffer(EventThread *context):cur_buf_size(BUFSIZE),write_ptr(0),read_ptr(0),event_thread_ptr(context){
	m_buf=(char *)malloc(sizeof(char)*BUFSIZE);
};

FdIoBuffer::~FdIoBuffer(){
	delete m_buf;
};

/*
从fd读取数据的时候，因为是非阻塞的，缓存大小有限，当缓存满的时候不能确定到底是读完了全部数据还是只读了一部分，
所以当n==left_size的时候，需要再调用read一次。
此时问题来了，如果刚好读完，且read设置的是非阻塞的，此时read会返回0,这会导致无法确定n==0到底是连接终止还是已经读完了所有数据。
我的解决办法是n==0的时候再read一次，如果是连接终止，则errno==EAGAIN，否则还会返回0,直接return 0;
*/
int FdIoBuffer::ReadFd(int &fd){
	int n;
	int left_size=cur_buf_size-write_ptr;
	if((n=read(fd,m_buf+write_ptr,left_size))<0){//读的fd是非阻塞的
		//出错的情况
		if(errno==EAGAIN){//之前读完了刚好装满缓存，不确定是否真的读完，再读一遍这里真读完了返回EAGAIN错误，返回0
			return 0;
		}
		if(errno==EINTR){//被信号中断，继续
			return ReadFd(fd);
		}
	}
	//EPOLLET需要一直读到返回-1才算读完，
	//如果内核缓存还有数据并且FIN（对端终止连接）紧跟着后面，则需要这里read返回0
	//如果缓存没有数据，对端终止，这里也会检测到
	if(n==0){
		std::cout<<"客户端终止！ fd= "<<fd<<std::endl;
		event_thread_ptr->cli_fd_bufs.erase(fd);
		event_thread_ptr->connect_count--;
		//std::unordered_map<int,FdIoBuffer *>::iterator iter=event_thread->cli_fd_bufs.find(fd);
		epoll_ctl(event_thread_ptr->epfd, EPOLL_CTL_DEL, fd, NULL);
		close(fd);
		fd=-1;//传入的fd是引用，用于通知外界对端关闭连接
		return 0;
	}
	//正常情况
	std::cout<<"ReadFd2 n : "<<n<<std::endl;
	if(n>0){
		write_ptr+=n;//缓存写地址往后挪
		if(n==left_size){//如果读到的数据和left_size相同，则无法区分是否读完内核全部数据，需要递归再读一次
			cur_buf_size*=2;
			m_buf=(char *)realloc(m_buf,cur_buf_size*sizeof(char));
			return n+ReadFd(fd);//递归读，第二次读有可能碰到1.后面还有数据  2.后面没有数据  3.后面跟FIN
		}
	}
	return 0;
}

//当前缓存中有多少数据未取出
int FdIoBuffer::BufDataSize(){
	return write_ptr-read_ptr;
}

//默认取4字节的长度，并从网络序转换成主机序返回
uint32_t FdIoBuffer::GetDataLen(){
	uint32_t *len=(uint32_t *)(m_buf+read_ptr);
	//std::cout<<*len<<std::endl;
	return ntohl(*len);
}

//移动读指针
void FdIoBuffer::MoveReadPtr(int len){
	read_ptr+=len;
	if(read_ptr==write_ptr)//buf全部取完清零
		read_ptr=write_ptr=0;
	
	if(read_ptr>=MAX_EMPTY_LEN){//当buf前面的空白太多了，把后面的数据往前移动。
		memcpy(m_buf,m_buf+read_ptr,BufDataSize());
		read_ptr=0;
		write_ptr-=empty_len;
	}
}

//获取name字符串,并移动read指针
std::string FdIoBuffer::GetProtoName(int len){
	read_ptr+=len;
	return m_buf+read_ptr-len;
}

//获取当前的read指针
char *FdIoBuffer::GetDataPtr(){
	return m_buf+read_ptr;
}

//全部情况
void FdIoBuffer::Clear(){
	write_ptr=read_ptr=0;
}

/*void FdIoBuffer::MoveWritePtr(int len){
	write_ptr+=len;
}*/

/*int FdIoBuffer::WriteToBuf(std::string &str,int len){//因为一定能保证全部写入，所以返回void
	int left_data=BufSize();
	if(len>=left_data){
		cur_buf_size*=2;
		m_buf=(char *)realloc(m_buf,cur_buf_size*sizeof(char));
	}
	memcpy(m_buf+write_ptr,&str[0],len);
	write_ptr+=len;
	return len;
}

int FdIoBuffer::WriteToFd(int fd){
	int left_data=BufSize();
	if(left_data>0){
		int n=write(fd,m_buf+read_ptr,left_data);
		if(n<=0){
			std::cout<<"FdIoBuffer::WriteToFd error!\0";
			return -1;
		}
		read_ptr+=n;
		if(read_ptr==write_ptr){
			read_ptr=head_ptr=0;
			write_ptr=4;
		}

		int empty_len=read_ptr;
		if(empty_len>=MAX_EMPTY_LEN){//当buf前面的空白太多了，把后面的数据往前移动。
			memcpy(m_buf,m_buf+read_ptr,BufSize());
			read_ptr=0;
			write_ptr-=empty_len;
			head_ptr-=empty_len;
		}
		return n;
	}
	return 0;//never
}

char *FdIoBuffer::GetWritePtr(){
	write_ptr==head_ptr?write_ptr=head_ptr+4:0;
	return m_buf+write_ptr;
}

char *FdIoBuffer::GetHeadPtr(){
	return m_buf+head_ptr;
}

int FdIoBuffer::BufLeftSize(){
	return BufSize()-write_ptr;
}

void FdIoBuffer::SetDataEnd(){
	head_ptr=write_ptr;
	write_ptr+=4;
}*/
