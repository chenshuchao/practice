#ifndef HTTPSERVER_H
#define HTTPSERVER_H

#include <boost/noncopyable.hpp>
//#include <muduo/net/TcpServer.h>

#include "tcp_server.h"
#include "http_session.h"
#include "http_transaction.h"

namespace bytree
{
class HTTPServer : boost::noncopyable
{
 public:
  struct Config
  {
    size_t port;
    HTTPTransactionFactory* transaction_factory;
  };
  HTTPServer(muduo::net::EventLoop &loop,
             const muduo::net::InetAddress& addr,
             const muduo::string& name,
             HTTPServer::Config* config);

  void Start();
  void Stop();
  void Bind();

 private:
  void OnConnection(const muduo::net::TcpConnectionPtr& conn);
  void OnDisconnection(const muduo::net::TcpConnectionPtr& conn);
  void OnData(const muduo::net::TcpConnectionPtr& conn, muduo::net::Buffer* buf, muduo::Timestamp);

  void CreateSession(const muduo::net::TcpConnectionPtr& conn,
                     const HTTPTransactionFactoryPtr& factory);
  void RemoveSession(const HTTPSessionPtr& session);
  void HandleSessionClose(const HTTPSessionPtr& session);

  HTTPSessionPtr FindSession(const muduo::net::TcpConnectionPtr& conn);

  std::string name_;
  boost::shared_ptr<HTTPServer::Config> config_;
  HTTPTransactionFactoryPtr transaction_factory_;
  muduo::net::TcpServer tcp_server_;

  typedef std::map<std::string, HTTPSessionPtr> SessionMap;
  SessionMap session_map_;

};
}

#endif
