#include <boost/bind.hpp>

#include "http_transaction.h"
#include "http_request.h"
#include "http_response.h"

using namespace std;
using namespace bytree;

void HTTPTransaction::OnRequest(HTTPRequest& info)
{
  HTTPResponse resp;
  Handle(info, resp);
  Send(resp);
  Detach();
}

void HTTPTransaction::Send(HTTPResponse& resp)
{
  send_callback_(resp);
}

void HTTPTransaction::Detach()
{
  detach_callback_(shared_from_this());
}
