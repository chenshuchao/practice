LIB_SRC_PATH = ../../httpserver/
CXXFLAGS = -g  -Wall -I ../../httpserver/lib -I../../  -L/usr/local/lib/muduo -pthread
LDFLAGS = -lmuduo_net -lmuduo_base -lpthread
LIB_SRC = 											\
		$(LIB_SRC_PATH)lib/http_parser/http_parser.c\
		$(LIB_SRC_PATH)http_codec.cc				\
		$(LIB_SRC_PATH)http_request.cc  			\
		$(LIB_SRC_PATH)http_response.cc  			\
		$(LIB_SRC_PATH)http_handler.cc				\
		$(LIB_SRC_PATH)http_server.cc 				\
		$(LIB_SRC_PATH)http_session.cc				\
		$(LIB_SRC_PATH)http_transaction.cc			\
		$(LIB_SRC_PATH)tcp_server.cc

BINARIES = server

all: $(BINARIES)

$(BINARIES):
	g++ $(CXXFLAGS) -o $@ $(LIB_SRC) $(filter %.cc %.c,$^) $(LDFLAGS)

clean:
	rm -f $(BINARIES) core

server: server.cc

