#--compiler----
CXX = g++

#--Flages----

SIGN_VER_FLAGS = -lssl -lcrypto -L /usr/local/opt/openssl/lib
MYSQL_FLAGS =  -I/usr/include/mysql -L/usr/lib64/mysql -lmysqlclient
CFLAGS = -I ../Merkle_Tree
#--sources----
SOURCES :=   command_parser.cpp Logger.cpp

#--Object files----
OBJS = $(SOURCES:.cpp=.o)
LOGGER := Logger

OPENCV = `pkg-config opencv4 --cflags --libs`
LIBS = $(OPENCV) -lpthread

Verifier: Verifier.o 
	$(CXX) -o Verifier Verifier.o  $(LIBS) $(SIGN_VER_FLAGS) $(MYSQL_FLAGS)

command_parser.o: ./../Client/command_parser.cpp
	$(CXX) -c -o $@ $<

Verifier.o: ./Verifier.cpp
	${CXX} -c $< -o $@ $(LIBS) $(MYSQL_FLAGS)


clean:
	$(RM) *.o Verifier
