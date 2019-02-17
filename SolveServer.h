#ifndef SOLVESERVER_H_
#define SOLVESERVER_H_

#include "shudu.pb.h"

class EventThread;//前置声明
/*
该类内部放着各种业务处理函数，新增的业务也写在这
 */
class SolveServer{
public:
	void Init(EventThread *context){
		event_thread_ptr=context;
	};

	void SolveShudu(std::shared_ptr<proto::shudu_data> data_ptr);
	bool SolveShuduHelper(std::string &data, int i, int j);
	bool IsValid( std::string &data, int i, int j );
	void SolveEcho(std::shared_ptr<proto::echo_data> data_ptr);
private:
	EventThread *event_thread_ptr;
};

#endif
