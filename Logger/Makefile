#--compiler----
CXX = g++

#--Flages----

SIGN_VER_FLAGS = -lssl -lcrypto -L /usr/local/opt/openssl/lib
CFLAGS = -Wall -g -I ../Merkle_Tree

#--sources----
SOURCES :=   command_parser.cpp Logger.cpp

#--Object files----
OBJS = $(SOURCES:.cpp=.o)

OPENCV = `pkg-config opencv4 --cflags --libs`
LIBS = $(OPENCV) -lpthread

all: Logger

Logger: $(OBJS)
	$(CXX)  $(OBJS) -o $@ $(LIBS) $(SIGN_VER_FLAGS)

command_parser.o: ./../Client/command_parser.cpp
	$(CXX) -c -o $@ $<

Logger.o: ./Logger.cpp
	${CXX} -c  -o $@  $< $(LIBS) 

clean:
	$(RM) *.o  Logger
