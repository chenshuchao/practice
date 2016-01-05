#include <string>
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

int main()
{
  HTTPCodec codec;
  HTTPCodecCallbackTest *callback = new HTTPCodecCallbackTest();
  codec.SetCallback(callback);

  string req = "GET /hello/haha/ HTTP/1.1\r\nHost: 10.211.55.6:5011Connection: keep-alive\r\nCache-Control: max-age=0\r\nAccept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8\r\nUpgrade-Insecure-Requests: 1\r\nUser-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10_10_3) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/47.0.2526.106 Safari/537.36\r\nAccept-Encoding: gzip, deflate, sdch\r\nAccept-Language: zh-CN,zh;q=0.8,en;q=0.6,st;q=0.4\r\n\r\n";


  codec.OnData(req);
  cout << "header_name: " << codec.GetHeaderName() << endl;
  cout << "header_value: " << codec.GetHeaderValue() << endl;
  cout << "codec_name: " << codec.GetName() << endl;
}

