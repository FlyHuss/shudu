#ifndef SOLVESERVER_H_
#define SOLVESERVER_H_

#include "ThreadPoll.h"
/*
该类内部放着各种业务处理函数，新增的业务也写在这
 */
class SolveServer{
public:
	explicit void Init(ThreadPoll *context){
		thread_poll_ptr=context;
	};

	void SolveShudu(proto::shudu_data* data);
	bool SolveShuduHelper(std::string &data, int i, int j);
	bool IsValid( std::string &data, int i, int j );
	void SolveEcho(proto::echo_data* data);
private:
	ThreadPoll *thread_poll_ptr;
};

#endif
