#include <muduo/net/EventLoop.h>
#include <muduo/net/TcpServer.h>

#include "http_server.h"
#include "http_codec.h"

HTTPServer::HTTPServer(EventLoop* loop,
                       const string name,
                       const HTTPServerConfig& config,
                       TcpServer::Option option)
  : config_(config),
  : tcpserver_(loop, InetAddr(config.port), name, option)
{
  tcpServer.setMessageCallback(boost::bind(OnRequst, this, _1, _2, _3));
}

void HTTPServer::Start()
{
  tcpServer.Start();
}

void HTTPServer::Stop()
{
}

void HTTPServer::OnConnection(const TcpConnectionPtr& conn)
{
 
}
void HTTPServer::OnDisconnection(const TcpConnectionPtr& conn)
{

}
void HTTPServer::OnRequest(const TcpConnectionPtr& conn, Buffer* buf, Timestamp)
{
  HTTPSessionPtr session = session_manager_.GetSession(conn->name());
  if (!conn) //error

  session.OnRequest();
}
