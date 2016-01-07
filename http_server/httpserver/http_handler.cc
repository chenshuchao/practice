#include "http_handler.h"

using namespace std;
using namespace bytree;

HTTPHandler::HTTPHandler(const string& name,
                         const muduo::net::TcpConnectionPtr& conn,
                         const HTTPTransactionFactoryPtr& factory)
  : BaseHandler(conn),
    name_(name),
    factory_(factory)
{
}
void HTTPHandler::SetSession() {
  session_.reset(new HTTPSession(name_,
                                 shared_from_this(),
                                 GetConn(),
                                 factory_));
}

void HTTPHandler::OnData(muduo::net::Buffer* buf)
{
  if (!session_)
  {
    SetSession();
  }
  session_->OnData(buf);
}
