#include "http_session.h"


void HTTPSession::OnRequest()
{
  codec_.parse();
}
