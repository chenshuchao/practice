#ifndef HTTP_SESSION_MANAGER_H
#define HTTP_SESSION_MANAGER_H

#include "http_session.h"

namespace
{
class HTTPSessionManager
{
 public:
  explicit HTTPSessionManager();
  ~HTTPSessionManager();

  void removeSession(const std::string&);
 
 private:
  typedef std::map<std::string, HTTPSessionPtr> SessionMap;

  SessionMap session_map_;
};
}

#endif
