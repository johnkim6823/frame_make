#--compiler----
CXX = g++

#--Flages----

SIGN_VER_FLAGS = -lssl -lcrypto -L /usr/local/opt/openssl/lib

OPENCV = `pkg-config opencv4 --cflags --libs`
LIBS = $(OPENCV) -lpthread
#--sources----
SOURCES :=  Logger.cpp

#--Object files----
OBJS = $(SOURCES:.cpp=.o)

all: Logger

Logger: $(OBJS)
	$(CXX)  $(OBJS) -o $@ $(LIBS) $(SIGN_VER_FLAGS)

Logger.o: ./Logger.cpp
	${CXX} -c -o $@  $< $(LIBS) $(SIGN_VER_FLAGS )

clean:
	$(RM) *.o  Logger
