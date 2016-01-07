#ifndef HTTP_SESSION_H
#define HTTP_SESSION_H

#include <boost/function.hpp>
#include <muduo/net/TcpConnection.h>

#include "http_transaction.h"
#include "http_codec.h"

namespace bytree
{
class HTTPHandler;
typedef boost::shared_ptr<HTTPHandler> HTTPHandlerPtr;

class HTTPSession : public boost::enable_shared_from_this<HTTPSession>
{
 public:
  typedef boost::shared_ptr<HTTPSession> HTTPSessionPtr;
  typedef boost::function<void (HTTPSessionPtr)> CloseCallback;
  explicit HTTPSession(const std::string& name,
                       const HTTPHandlerPtr& handler,
                       const muduo::net::TcpConnectionPtr& conn,
                       const HTTPTransactionFactoryPtr& factory)
    : name_(name),
      handler_(handler),
      conn_(conn),
      transaction_factory_(factory)
  {
    SetCodecCallback();
  }

  std::string GetName() { return name_; }
  muduo::net::TcpConnectionPtr GetConnection() { return conn_; }
  
  // TODO
  void SetCodecCallback();
/*
  void SetCloseCallback(const CloseCallback& cb)
  { close_callback_ = cb; }
*/
  void OnData(muduo::net::Buffer* buf);
  void OnMessageBegin(HTTPCodec::StreamID id);
  void OnMessageComplete(HTTPCodec::StreamID id, HTTPRequest& request);

  void RemoveTransaction(const HTTPTransactionPtr& tran);

  void FormatResponse(HTTPResponse& resp); 
  void SendResponse(HTTPResponse& response);
  void Send(std::string& message);

  // 主动关闭
  void HandleClose();
  // call when HTTPServer remove me
  void Destory();

 private:
  std::string name_;
  boost::weak_ptr<HTTPHandler> handler_;
  muduo::net::TcpConnectionPtr conn_;
  HTTPCodec codec_;
  std::map<HTTPCodec::StreamID, HTTPTransactionPtr> transaction_map_;
  boost::shared_ptr<HTTPTransactionFactory> transaction_factory_;
};

typedef boost::shared_ptr<HTTPSession> HTTPSessionPtr;
}

#endif
