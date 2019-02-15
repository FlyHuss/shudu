CXXFLAGS := -std=c++11

maketest :shudu.pb.o FdIoBuffer.o clitest.o
	g++ shudu.pb.o FdIoBuffer.o clitest.o -o clitest \
-I /usr/local/include/ -L /usr/local/lib/ -lprotobuf -lpthread

shudu.pb.o:shudu.pb.h

FdIoBuffer.o:FdIoBuffer.h

clitest.o:

.PHONY:clean
clean:
	-rm *.o
