CXX = g++

LFLAGS := -g -Wall
SIGN_VER_FLAGS = -I $(HOME)/cryptopp-CRYPTOPP_8_6_0 -Wall -g3 -O2
CFLAGS = -Wall -g -I ./include

SOURCES :=  Logger.cpp command_parser.cpp

OBJS = $(SOURCES:.cpp=.o)
LOGGER := Logger

OPENCV = `pkg-config opencv4 --cflags --libs`
LIBS = $(OPENCV) -lpthread

SDIR = src

all: Logger

sign_verify: sign_verify.o
	$(CXX) -o sign_verify $(SIGN_VER_FLAGS) sign_verify.o -lcryptopp

sign_verify.o: src/sign_verify.cpp
	$(CXX) $(SIGN_VER_FLAGS) -c $< -o $@

Logger: $(OBJS)
	$(CXX) $(CFLAGS) $(OBJS) -o $@ $(LIBS)

Verifier: Verifier.o merkle_tree.o node.o
	$(CXX) $(CFLAGS) -o Verifier Verifier.o merkle_tree.o node.o $(LIBS)

merkle_tree.o: src/merkle_tree.cpp
	${CXX} ${CFLAGS} -c $< -o $@
	
node.o: src/node.cpp
	${CXX} ${CFLAGS} -c $< -o $@	

Verifier.o: ./Verifier.cpp
	${CXX} ${CFLAGS} -c $< -o $@ $(LIBS)

Logger.o: ./Logger.cpp
	${CXX} ${CFLAGS} -c $< -o $@ $(LIBS) -lpthread

clean:
	$(RM) *.o Verifier Logger sign_verify privkey PubkeyBase64.key
