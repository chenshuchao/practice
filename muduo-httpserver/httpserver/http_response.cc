#include "http_response.h"

using namespace bytree;
using namespace std;

HTTPResponse& HTTPResponse::SetStatus(int code, const string& message)
{
  status_code_ = code;
  status_message_ = message;
  return *this;
}

HTTPResponse& HTTPResponse::AddHeader(const string& name, const string& value)
{
  headers_map_.insert(pair<string, string>(name, value));
  return *this;
}

HTTPResponse& HTTPResponse::AddBody(const string& body)
{
  body_ = body;
  return *this;
}

string HTTPResponse::GetHeaderValue(const string& name)
{
  if (!headers_map_.count(name)) return "";
  return headers_map_[name];
}

void HTTPResponse::GetAllHeaders(vector<string>& name, vector<string>& value)
{
  name.clear();
  value.clear();
  for (map<string, string>::iterator it = headers_map_.begin();
       it != headers_map_.end();
       it ++)
  {
    name.push_back(it->first);
    value.push_back(it->second);
  }
}

