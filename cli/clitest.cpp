#include <iostream>
#include "shudu.pb.h"
#include "FdIoBuffer.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <stdint.h>

#define SERV_LISTEN_PORT 10086
#define SERV_LISTEN_ADDR "127.0.0.1"

using namespace std;

int main(){
	int cli_fd=socket(AF_INET,SOCK_STREAM,NULL);

	sockaddr_in serv_addr;
	bzero(&serv_addr,sizeof(serv_addr));
    serv_addr.sin_family=AF_INET ;
    inet_pton(AF_INET,SERV_LISTEN_ADDR,&serv_addr.sin_addr);
    serv_addr.sin_port=htons(SERV_LISTEN_PORT);

    int r=connect(cli_fd,(sockaddr *)&serv_addr,sizeof(serv_addr));
    assert(r!=-1);

    proto::shudu_data data1;
    data1.set_data("53..7....6..195....98....6.8...6...34..8.3..17...2...6.6....28....419..5....8..79");
    data1.set_time("2018.11.11  11:11:12");
    proto::echo_data data2;
    data2.set_data("hello!sghwn!");

    FdIoBuffer recv_buf;
    string send_buf(8,0);

    //send shudu
    std::string type_name=data1.GetTypeName();
    uint32_t n=htonl(type_name.length()+1);
    memcpy(&send_buf[4],&n,sizeof(uint32_t));
    send_buf+=type_name+"a";// \0 pos
    int pos=send_buf.length()-1;
    if(!data1.AppendToString(&send_buf))
        std::cout<<"AppendToString error!\n";
    n=htonl(send_buf.length()-4);
    memcpy(&send_buf[0],&n,sizeof(uint32_t));
    int len=send_buf.length();
    send_buf[pos]='\0';
    int nwrite=0;
    while(len!=nwrite){// send all
        nwrite+=write(cli_fd,&send_buf[nwrite],len-nwrite);
    }
    cout<<"send shudu len : "<<nwrite<<endl;

    //recv shudu
    recv_buf.ReadFd(cli_fd);
    len=recv_buf.GetDataLen();
    recv_buf.MoveReadPtr(4);
    int namelen=recv_buf.GetDataLen();
    len-=4;
    recv_buf.MoveReadPtr(4);
    len-=namelen;
    string name=recv_buf.GetProtoName(namelen);
    cout<<"received : "<<name<<endl;
    proto::shudu_data data3;
    data3.ParseFromArray(recv_buf.GetDataPtr(),len);
    recv_buf.MoveReadPtr(len);
    cout<<"data : "<<data3.data()<<endl;
    cout<<"time : "<<data3.time()<<endl;

    sleep(2);

    //send echo
    send_buf.resize(8);
    type_name=data2.GetTypeName();
    n=htonl(type_name.length()+1);
    memcpy(&send_buf[4],&n,sizeof(uint32_t));
    send_buf.append(type_name);//append和'+'不同的地方在于这里的n比length多1,把'\0'也append进去了
    send_buf+="a";// \0 pos
    pos=send_buf.length()-1;
    if(!data2.AppendToString(&send_buf))
        std::cout<<"AppendToString error!\n";
    n=htonl(send_buf.length()-4);
    memcpy(&send_buf[0],&n,sizeof(uint32_t));
    len=send_buf.length();
    send_buf[pos]='\0';
    nwrite=0;
    while(len!=nwrite){// send all
        nwrite+=write(cli_fd,&send_buf[nwrite],len-nwrite);
    }
    cout<<"send echo len : "<<nwrite<<endl;

    //recv echo
    recv_buf.Clear();
    len=0;
    recv_buf.ReadFd(cli_fd);
    len=recv_buf.GetDataLen();
    recv_buf.MoveReadPtr(4);
    namelen=recv_buf.GetDataLen();
    len-=4;
    recv_buf.MoveReadPtr(4);
    len-=namelen;
    name=recv_buf.GetProtoName(namelen);
    cout<<"received : "<<name<<endl;
    proto::echo_data data4;
    data4.ParseFromArray(recv_buf.GetDataPtr(),len);
    recv_buf.MoveReadPtr(len);
    cout<<"data : "<<data4.data()<<endl;

    //pause();
	return 0;
}
