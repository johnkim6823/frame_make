#--compiler----
CXX = g++

#--Flages----
SIGN_VER_FLAGS = -lssl -lcrypto -L/usr/local/opt/openssl/lib
MYSQL_FLAGS =  -I/usr/include/mysql -L/usr/lib64/mysql -lmysqlclient 
OPENCV = `pkg-config opencv4 --cflags --libs`
LIBS = $(OPENCV) -lpthread

#--sources----
SOURCES :=  Verifier.cpp merkle_tree.cpp node.cpp

#--Object files----
OBJS = $(SOURCES:.cpp=.o)

Verifier: $(OBJS)
	$(CXX) $(OBJS) -g -o $@  $(LIBS) $(MYSQL_FLAGS) $(SIGN_VER_FLAGS)

merkle_tree.o: ./../Merkle_Tree/merkle_tree.cpp
	$(CXX) -g -c -o $@ $<

node.o: ./../Merkle_Tree/node.cpp
	$(CXX) -Wall -g -c -o $@ $<

%.o: ./%.cpp
	$(CXX) -c $< -g -o $@  $(LIBS) $(MYSQL_FLAGS) $(SIGN_VER_FLAGS)

clean:
	$(RM) *.o Verifier
