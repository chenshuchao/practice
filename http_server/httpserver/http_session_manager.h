#ifndef HTTP_SESSION_MANAGER_H
#define HTTP_SESSION_MANAGER_H

#include "http_session.h"

namespace bytree
{
class HTTPSessionManager
{
 public:
  HTTPSessionManager(const std::string& name)
    : name_(name)
  {
  }

  void CreateSession(const muduo::net::TcpConnectionPtr& conn,
                     const HTTPTransactionFactoryPtr& factory);
  void RemoveSession(const HTTPSessionPtr& session);
  HTTPSessionPtr FindSession(const muduo::net::TcpConnectionPtr& conn);
  
 private:
  const std::string name_;
  std::map<std::string, HTTPSessionPtr> session_map_;
};
}

#endif
