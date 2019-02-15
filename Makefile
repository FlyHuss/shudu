CXXFLAGS := -std=c++11

servertest :./syn/MyMutexPre.o ./syn/Condition.o ./syn/MyMutex.o ./syn/MyEvent.o \
shudu.pb.o FdIoBuffer.o Dispacher.o ThreadPoll.o SolveServer.o EventThread.o test.o 
	g++ ./syn/MyMutexPre.o ./syn/Condition.o ./syn/MyMutex.o ./syn/MyEvent.o \
shudu.pb.o FdIoBuffer.o Dispacher.o ThreadPoll.o SolveServer.o EventThread.o test.o -o servertest \
-I /usr/local/include/ -L /usr/local/lib/ -lprotobuf -lpthread

./syn/MyMutexPre.o:./syn/MyMutexPre.h

./syn/Condition.o:./syn/Condition.h

./syn/MyMutex.o:./syn/MyMutex.h

./syn/MyEvent.o:./syn/MyEvent.h

shudu.pb.o:shudu.pb.h

FdIoBuffer.o:FdIoBuffer.h

Dispacher.o:Dispacher.h

ThreadPoll.o:ThreadPoll.h

SolveServer.o:SolveServer.h

EventThread.o:EventThread.h

test.o:

.PHONY:clean
clean:
	-rm *.o ./syn/*.o
