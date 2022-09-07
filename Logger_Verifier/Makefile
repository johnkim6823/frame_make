#--compiler----
CXX = g++

#--Flages----

SIGN_VER_FLAGS = -lssl -lcrypto -L /usr/local/opt/openssl/lib
CFLAGS = -Wall -g -I ./include

#--sources----
SOURCES :=  Logger.cpp command_parser.cpp 

#--Object files----
OBJS = $(SOURCES:.cpp=.o)
LOGGER := Logger

OPENCV = `pkg-config opencv4 --cflags --libs`
LIBS = $(OPENCV) -lpthread

Logger: $(OBJS)
	$(CXX) $(CFLAGS) $(OBJS) -o $@ $(LIBS) $(SIGN_VER_FLAGS)

Verifier: Verifier.o merkle_tree.o node.o
	$(CXX) $(CFLAGS) -o Verifier Verifier.o merkle_tree.o node.o $(LIBS)

merkle_tree.o: src/merkle_tree.cpp
	${CXX} ${CFLAGS} -c $< -o $@
	
node.o: src/node.cpp
	${CXX} ${CFLAGS} -c $< -o $@	
	
Verifier.o: ./Verifier.cpp
	${CXX} ${CFLAGS} -c $< -o $@ $(LIBS)

sign_verify.o: ./sign.cpp
	${CXX} -c $< -o $@ $(SIGN_VER_FLAGS)

Logger.o: ./Logger.cpp
	${CXX} ${CFLAGS} -c $< -o $@ $(LIBS) 

clean:
	$(RM) *.o Verifier Logger sign_verify privkey PubkeyBase64.key
