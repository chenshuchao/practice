#ifndef HTTPSERVER_H
#define HTTPSERVER_H

#include <boost/noncopyable.hpp>

#include "tcp_server.h"
#include "base_handler.h"
#include "http_handler.h"

namespace bytree
{
class HTTPServer : boost::noncopyable
{
 public:
  HTTPServer(muduo::net::EventLoop &loop,
             const muduo::net::InetAddress& addr,
             const muduo::string& name,
             HTTPHandlerFactory* factory);

  void Start();
  void Stop();
  void Bind();

 private:
  void OnCreateOrDestroyConnection(const muduo::net::TcpConnectionPtr& conn);
  void OnConnection(const muduo::net::TcpConnectionPtr& conn);
  void OnDisconnection(const muduo::net::TcpConnectionPtr& conn);
  void OnData(const muduo::net::TcpConnectionPtr& conn, muduo::net::Buffer* buf, muduo::Timestamp);
  
  void CreateHandler(const muduo::net::TcpConnectionPtr& conn);
  void RemoveHandler(const HTTPHandlerPtr& handler);
  HTTPHandlerPtr FindHandler(const muduo::net::TcpConnectionPtr& conn);

  std::string name_;
  HTTPHandlerFactoryPtr handler_factory_;
  muduo::net::TcpServer tcp_server_;
  std::map<std::string, HTTPHandlerPtr> handler_map_;
};

typedef boost::shared_ptr<HTTPServer> HTTPServerPtr;
}

#endif
