#ifndef BASE_HANDLER_H
#define BASE_HANDLER_H

#include <string>

#include <muduo/net/Buffer.h>

namespace bytree
{
class BaseHandler
{
 public:
  BaseHandler(const muduo::net::TcpConnectionPtr& conn)
    : conn_(conn)
  {}
  virtual ~BaseHandler() {}
  virtual void OnData(muduo::net::Buffer* buf) {}

  muduo::net::TcpConnectionPtr GetConn() { return conn_; }
 private:
  muduo::net::TcpConnectionPtr conn_;
};
typedef boost::shared_ptr<BaseHandler> BaseHandlerPtr;

class BaseHandlerFactory
{
 public:
  virtual BaseHandler* CreateHandler(const std::string& name,
                                     const muduo::net::TcpConnectionPtr& conn) = 0;
};
typedef boost::shared_ptr<BaseHandlerFactory> BaseHandlerFactoryPtr;
}

#endif
