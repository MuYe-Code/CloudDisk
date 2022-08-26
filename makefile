SRCS:=$(wildcard src/*.cpp)
OBJS:=$(patsubst src/%.cpp, objs/%.o, $(SRCS))

LINKS:= -g -Wall -fno-rtti -lSimpleAmqpClient -lalibabacloud-oss-cpp-sdk -lcurl -lwfrest -lworkflow -lssl -lcrypto -lcrypt -lpthread
CC:=g++

OUTS: server consumer

server: $(OBJS)
	$(CC) $^ -o $@ $(LINKS)

consumer: rabbitmqConsumer/consumer.cpp
	$(CC) rabbitmqConsumer/consumer.cpp -o consumer $(LINKS)

objs/%.o : src/%.cpp
	$(CC) -c $^ -o $@ -g -Wall

.PHONY: clean rebuild

clean: 
	rm -f $(OBJS) OUTS
	
rebuild: clean OUTS

