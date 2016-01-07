#include <muduo/base/Logging.h>

#include "http_session_manager.h"
#include "string_util.h"

using namespace std;
using namespace bytree;

void HTTPSessionManager::CreateSession(
         const muduo::net::TcpConnectionPtr& conn,
         const HTTPTransactionFactoryPtr& factory)
{
  string session_name = convert_muduo_string_to_std(conn->name());
  LOG_INFO << "HTTPSessionManager::CreateSession [" << name_
           << "] - new session [" << session_name
           << "]";
  HTTPSessionPtr session(new HTTPSession(session_name, conn, factory));
  session_map_.insert(pair<string, HTTPSessionPtr>(session_name, session));
}

void HTTPSessionManager::RemoveSession(const HTTPSessionPtr& session)
{
  LOG_INFO << "HTTPSessionManager::RemoveSession [" << name_
           << "] - session [" << session->GetName()
           << "]";
  size_t n = session_map_.erase(session->GetName());
  (void) n; // why void?
  assert(n == 1);
}

HTTPSessionPtr HTTPSessionManager::FindSession(const muduo::net::TcpConnectionPtr& conn)
{
  return session_map_[convert_muduo_string_to_std(conn->name())];
}


