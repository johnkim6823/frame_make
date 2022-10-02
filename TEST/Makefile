CXX = g++

#--Flages----
SIGN_VER_FLAGS = -lssl -lcrypto -L /usr/local/opt/openssl/lib
MYSQL_FLAGS =  -I/usr/include/mysql -L/usr/lib64/mysql -lmysqlclient

mktree_test: mktree_test.o
	$(CXX) -o mktree_test mktree_test.o 

%.o: ./%.cpp
	$(CXX) -c $< -o $@  $(MYSQL_FLAGS) $(SIGN_VER_FLAGS)


clean:
	$(RM) *.o mktree_test