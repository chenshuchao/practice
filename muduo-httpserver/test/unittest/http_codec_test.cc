#include <string>
#include <gtest/gtest.h>
#include <httpserver/http_codec.h>
#include <httpserver/http_request.h>

using namespace std;
using namespace bytree;

class HTTPCodecCallbackTest : public HTTPCodec::Callback
{
 public:
  HTTPCodecCallbackTest()
    : is_message_begin(false),
      is_headers_complete(false)
  {
  }
  void OnMessageBegin(HTTPCodec::StreamID id)
  {
    is_message_begin = true;
  }
  void OnHeadersComplete()
  {
    is_headers_complete = true;
  }
  bool is_message_begin;
  bool is_headers_complete;
};

string CreateSimpleRequest()
{
  string req("GET / HTTP/1.1\nHost: www.baidu.com\n\n");
  return req;
}

TEST(OnDataTest, simple)
{
  HTTPCodec codec;
  HTTPCodecCallbackTest *callback = new HTTPCodecCallbackTest();
  codec.SetCallback(callback);
  string req = CreateSimpleRequest();
  codec.OnData(req);
  ASSERT_EQ(callback->is_message_begin, true);
  ASSERT_EQ(codec.GetHeaderName(), "hello");
  ASSERT_EQ(codec.GetHeaderValue(), "world");
  ASSERT_EQ(callback->is_headers_complete, false);
}

