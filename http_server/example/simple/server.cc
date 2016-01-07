#include <muduo/base/Logging.h>
#include <muduo/net/EventLoop.h>
#include <muduo/net/InetAddress.h>

#include "httpserver/http_server.h"
#include "httpserver/http_handler.h"
#include "httpserver/http_transaction.h"

using namespace muduo::net;
using namespace bytree;

class SimpleTransaction : public HTTPTransaction
{
 public:
  SimpleTransaction(HTTPCodec::StreamID id)
    : HTTPTransaction(id)
  {
  }
  ~SimpleTransaction() {}

  void Handle(HTTPRequest& req, HTTPResponse& resp)
  {
    LOG_INFO << "req methond: "<< req.GetMethod();
    resp.SetStatus(200, "OK")
        .AddHeader("hello", "world")
        .AddHeader("Content-Type", "text/plain")
        .AddBody("I am body");
  }
};
class SimpleFactory : public HTTPTransactionFactory
{
 public:
  HTTPTransaction* CreateTransaction(HTTPCodec::StreamID id)
  {
    return new SimpleTransaction(id);
  }
};

int main()
{
  muduo::Logger::setLogLevel(muduo::Logger::DEBUG);
  EventLoop loop;

  HTTPServer server(loop,
                    InetAddress(5011),
                    "simple_server",
                    new HTTPHandlerFactory(new SimpleFactory())) ;
  server.Start();
  loop.loop();
}

