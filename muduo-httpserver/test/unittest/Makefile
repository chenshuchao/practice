
BASE_DIR = ../../
SRC_DIR = ../../httpserver/
GTEST_DIR = ./gtest/

all:
	g++ -g -I${BASE_DIR} -I${SRC_DIR}lib/ -L /usr/local/lib/muduo/ ${GTEST_DIR}src/gtest_main.cc ${SRC_DIR}http_codec.cc ${SRC_DIR}lib/http_parser/http_parser.c ${SRC_DIR}http_request.cc ${SRC_DIR}http_response.cc http_codec_test.cc -o test -pthread -lgtest -lmuduo_net -lmuduo_base

codec:
	g++ -g -I${BASE_DIR} -I${SRC_DIR}lib/ -L /usr/local/lib/muduo/ ${SRC_DIR}http_codec.cc ${SRC_DIR}lib/http_parser/http_parser.c ${SRC_DIR}http_request.cc ${SRC_DIR}http_response.cc codec_test.cc -o codec -pthread -lmuduo_net -lmuduo_base

clean:
	rm -f test codec
