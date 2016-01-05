#include <boost/bind.hpp>
#include <boost/bind/placeholders.hpp>
#include <boost/enable_shared_from_this.hpp>

#include <muduo/base/Logging.h>
#include <muduo/net/Buffer.h>
#include <muduo/net/TcpConnection.h>

#include "string_util.h"
#include "http_session.h"

using namespace bytree;
using namespace std;

void HTTPSession::SetCodecCallback()
{
  HTTPCodec::Callback *callback = new HTTPCodec::Callback();
  callback->SetMessageBeginCallback(boost::bind(&HTTPSession::OnMessageBegin, this, _1));
  callback->SetMessageCompleteCallback(boost::bind(&HTTPSession::OnMessageComplete, this, _1, _2));
  codec_.SetCallback(callback);
}

void HTTPSession::OnData(muduo::net::Buffer* buf)
{
  // http protocol
  LOG_INFO << "HTTPSession::OnData [" << name_
           << "] - protocol: HTTP.";
  while (buf->readableBytes() > 0)
  {
    string data = convert_muduo_string_to_std(buf->retrieveAllAsString());
    size_t bytes_parsed = codec_.OnData(data);
    if (!bytes_parsed)
    {
      // It means we need to wait for more data.
      break; 
    }
    string unparsed_str(data, bytes_parsed);
    LOG_DEBUG << "unparsed bytes" << unparsed_str.size()
              << "unparsed_str : " << unparsed_str;

    buf->prepend(unparsed_str.c_str(), unparsed_str.size());
    LOG_DEBUG << "readable bytes: " << buf->readableBytes();
  }
}

void HTTPSession::OnMessageBegin(HTTPCodec::StreamID id)
{
  HTTPTransactionPtr tran(transaction_factory_->CreateTransaction(id));
  tran->SetSendCallback(
      boost::bind(&HTTPSession::SendResponse, this, _1));
  tran->SetDetachCallback(
      boost::bind(&HTTPSession::RemoveTransaction, this, _1));

  transaction_map_.insert(pair<HTTPCodec::StreamID, HTTPTransactionPtr>(id, tran));
}

void HTTPSession::OnMessageComplete(HTTPCodec::StreamID id, HTTPRequest& info)
{
  HTTPTransactionPtr tran = transaction_map_[id];
  if (!tran)
  {
    LOG_ERROR << " ";
  }
  assert(tran);
  tran->OnRequest(info);
}

void HTTPSession::RemoveTransaction(const HTTPTransactionPtr& tran)
{
  transaction_map_.erase(tran->GetID());
}

void HTTPSession::FormatResponse(HTTPResponse& resp)
{
  resp.AddHeader("Content-Length",
                 int_to_string(resp.GetBody().size()));
}

void HTTPSession::SendResponse(HTTPResponse& resp)
{
  FormatResponse(resp);
  string msg = codec_.ConvertResponseToString(resp);
  Send(msg);
  HandleClose();
}

void HTTPSession::Send(string& msg)
{
  LOG_INFO << "HTTPSession::Send [" << name_
           << "] - response:\n" << msg;
  conn_->send(msg.c_str(), msg.size());
}

void HTTPSession::HandleClose()
{
  close_callback_(shared_from_this());
}

