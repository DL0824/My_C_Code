LDFLAGS=-L /home/dl/ThirdPartLibForCpp/el7.x86_64/third_part/lib
DEBUGFLAGS=-g
BIN=../bin/oj_server
CC=g++

$(BIN):oj_server.cpp
	$(CC) $^ -o $@  $(DEBUGFLAGS) $(LDFLAGS) -lctemplate -ljsoncpp -lpthread -std=c++11

clean:
	rm -f $(BIN)
