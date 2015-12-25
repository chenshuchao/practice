#ifndef HTTPSERVER_H
#define HTTPSERVER_H

#include <boost/noncopyable.hpp>
#include <muduo/net/TcpServer.h>

namespace bytree
{
class HTTPServer : boost::noncopyable
{
 public:
  explicit HTTPServer(const HTTPServerConfig &config);
  ~HTTPServer();

  void Start();
  void Stop();
  void Bind();

 private:
  boost::shared_ptr<HTTPServerConfig> config_;
  HTTPCodec codec_;
};
}

#endif
