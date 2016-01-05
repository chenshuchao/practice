#include <boost/bind.hpp>
#include <boost/bind/placeholders.hpp>

#include <muduo/base/Logging.h>
#include <muduo/net/EventLoop.h>
#include <muduo/net/InetAddress.h>

#include "http_server.h"
#include "http_codec.h"
#include "string_util.h"

using namespace std;
using namespace bytree;

HTTPServer::HTTPServer(muduo::net::EventLoop& loop,
                       const muduo::net::InetAddress& addr,
                       const muduo::string& name,
                       HTTPServer::Config* config)
  : config_(config),
    transaction_factory_(config->transaction_factory),
    tcp_server_(&loop, addr, name, muduo::net::TcpServer::kNoReusePort)
{
  tcp_server_.setConnectionCallback(boost::bind(&HTTPServer::OnConnection, this, _1));
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

void HTTPServer::OnConnection(const muduo::net::TcpConnectionPtr& conn)
{
  LOG_INFO << "HTTPServer::OnConnection [" << conn->name() << "].";
  if (conn->connected())
  {
    CreateSession(conn, transaction_factory_);
  }
  else
  {
    OnDisconnection(conn);
  }
}

void HTTPServer::OnDisconnection(const muduo::net::TcpConnectionPtr& conn)
{
  HTTPSessionPtr session = FindSession(conn);
  if (!session)
  {
    LOG_ERROR << "HTTPServer::OnDisconnection [" << name_
              << "] connection [" << conn->name()
              << " session not found.";
  }
  RemoveSession(session);
}

void HTTPServer::OnData(const muduo::net::TcpConnectionPtr& conn, muduo::net::Buffer* buf, muduo::Timestamp)
{
  HTTPSessionPtr session = FindSession(conn);
  if (!session)
  {
    LOG_ERROR << "HTTPServer::OnRequest [" << name_
              << "] connecton [" << conn->name()
              << " session not found.";
    return;
  }
  session->OnData(buf);
}

void HTTPServer::CreateSession(
         const muduo::net::TcpConnectionPtr& conn,
         const HTTPTransactionFactoryPtr& factory)
{
  string session_name = convert_muduo_string_to_std(conn->name());
  LOG_INFO << "HTTPServer::CreateSession [" << name_
           << "] - new session [" << session_name
           << "]";

  HTTPSessionPtr session(new HTTPSession(session_name, conn, factory));
  session->SetCloseCallback(
      boost::bind(&HTTPServer::HandleSessionClose, this, _1));
  session_map_.insert(pair<string, HTTPSessionPtr>(session_name, session));
}

void HTTPServer::RemoveSession(const HTTPSessionPtr& session)
{
  LOG_INFO << "HTTPSession::RemoveSession [" << name_
           << "] - session [" << session->GetName()
           << "]";
  size_t n = session_map_.erase(session->GetName());
  (void) n; // why void?
  assert(n == 1);
}

HTTPSessionPtr HTTPServer::FindSession(const muduo::net::TcpConnectionPtr& conn)
{
  return session_map_[convert_muduo_string_to_std(conn->name())];
}

void HTTPServer::HandleSessionClose(const HTTPSessionPtr& session)
{
  muduo::net::TcpConnectionPtr conn = session->GetConnection();
  tcp_server_.removeAndDestroyConnection(conn);
}

