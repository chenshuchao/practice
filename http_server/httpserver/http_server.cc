#include <boost/bind.hpp>
#include <boost/bind/placeholders.hpp>

#include <muduo/base/Logging.h>

#include "string_util.h"
#include "http_server.h"

using namespace std;
using namespace bytree;

HTTPServer::HTTPServer(muduo::net::EventLoop& loop,
                       const muduo::net::InetAddress& addr,
                       const muduo::string& name,
                       HTTPHandlerFactory* factory)
  : handler_factory_(factory),
    tcp_server_(&loop, addr, name, muduo::net::TcpServer::kNoReusePort)
{
  tcp_server_.setConnectionCallback(
      boost::bind(&HTTPServer::OnCreateOrDestroyConnection, this, _1));
  tcp_server_.setMessageCallback(boost::bind(&HTTPServer::OnData, this, _1, _2, _3));
}

void HTTPServer::Start()
{
  LOG_INFO << "HTTPServer::Start [" << name_ << "].";
  tcp_server_.start();
}

void HTTPServer::Stop()
{
}

void HTTPServer::OnCreateOrDestroyConnection(const muduo::net::TcpConnectionPtr& conn)
{
  if (conn->connected())
  {
    OnConnection(conn);
  }
  else
  {
    OnDisconnection(conn);
  }
}

void HTTPServer::OnConnection(const muduo::net::TcpConnectionPtr& conn)
{
  CreateHandler(conn);
}

void HTTPServer::OnDisconnection(const muduo::net::TcpConnectionPtr& conn)
{
  HTTPHandlerPtr handler = FindHandler(conn);
  if (!handler)
  {
    LOG_ERROR << "HTTPServer::OnDisconnection [" << name_
              << "] connection [" << conn->name()
              << " handler not found.";
  }
  RemoveHandler(handler);
}

void HTTPServer::OnData(const muduo::net::TcpConnectionPtr& conn, muduo::net::Buffer* buf, muduo::Timestamp)
{
  HTTPHandlerPtr handler = FindHandler(conn);
  if (!handler)
  {
    LOG_ERROR << "HTTPServer::OnData [" << name_
              << "] connecton [" << conn->name()
              << " handler not found.";
    return;
  }
  handler->OnData(buf);
}

void HTTPServer::CreateHandler(const muduo::net::TcpConnectionPtr& conn)
{
  HTTPHandlerPtr handler(handler_factory_->CreateHandler(convert_to_std(conn->name()), conn));
  handler_map_.insert(
      pair<string, HTTPHandlerPtr>(convert_to_std(conn->name()), handler));
}

void HTTPServer::RemoveHandler(const HTTPHandlerPtr& handler)
{
  handler_map_.erase(convert_to_std(handler->GetConn()->name()));
}

HTTPHandlerPtr HTTPServer::FindHandler(const muduo::net::TcpConnectionPtr& conn)
{
  return handler_map_[convert_to_std(conn->name())];
}

