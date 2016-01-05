#ifndef HTTP_TRANSACTION_H
#define HTTP_TRANSACTION_H

#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/function.hpp>

#include "http_codec.h"

namespace bytree
{
class HTTPResponse;
class HTTPRequest;
typedef boost::shared_ptr<HTTPRequest> HTTPRequestPtr;

class HTTPTransaction : public boost::enable_shared_from_this<HTTPTransaction>
{
 public:
  typedef boost::shared_ptr<HTTPTransaction> HTTPTransactionPtr;
  typedef boost::function<void (HTTPResponse&)> SendCallback;
  typedef boost::function<void (HTTPTransactionPtr)> DetachCallback;

  HTTPTransaction(HTTPCodec::StreamID id)
    : id_(id)
  {
  }
  virtual ~HTTPTransaction() {}

  HTTPCodec::StreamID GetID() { return id_; }

  void SetSendCallback(const SendCallback& cb)
  { send_callback_ = cb; }
  void SetDetachCallback(const DetachCallback& cb)
  { detach_callback_ = cb; }

  void OnRequest(HTTPRequest& info);
  virtual void Handle(HTTPRequest& req, HTTPResponse& resp) = 0;
  void Send(HTTPResponse&);
  void Detach();

 private:
  HTTPCodec::StreamID id_;
  SendCallback send_callback_;
  DetachCallback detach_callback_;
};
typedef boost::shared_ptr<HTTPTransaction> HTTPTransactionPtr;

class HTTPTransactionFactory
{
 public:
   virtual HTTPTransaction* CreateTransaction(HTTPCodec::StreamID id) = 0;
};
typedef boost::shared_ptr<HTTPTransactionFactory> HTTPTransactionFactoryPtr;
}

#endif
