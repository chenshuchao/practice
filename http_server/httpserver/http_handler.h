#ifndef HTTP_HANDLER_H
#define HTTP_HANDLER_H

#include <boost/enable_shared_from_this.hpp>
#include <boost/bind.hpp>
#include <muduo/net/TcpConnection.h>

#include "base_handler.h"
#include "http_session.h"

namespace bytree
{
class HTTPHandler : public BaseHandler,
                    public boost::enable_shared_from_this<HTTPHandler>
{
 public:
  HTTPHandler(const std::string& name,
              const muduo::net::TcpConnectionPtr&,
              const HTTPTransactionFactoryPtr&);
  ~HTTPHandler() {}

  void OnData(muduo::net::Buffer* buf);
  void SetSession();

 private:
  const std::string name_;
  HTTPTransactionFactoryPtr factory_;
  HTTPSessionPtr session_;
};

class HTTPHandlerFactory : public BaseHandlerFactory
{
 public:
  HTTPHandlerFactory(HTTPTransactionFactory* factory)
    : factory_(factory)
  {
  }
  HTTPHandler* CreateHandler(const std::string& name,
                             const muduo::net::TcpConnectionPtr& conn)
  {
    return new HTTPHandler(name, conn, factory_);
  }
 private:
  HTTPTransactionFactoryPtr factory_;
};
typedef boost::shared_ptr<HTTPHandlerFactory> HTTPHandlerFactoryPtr;
}

#endif
