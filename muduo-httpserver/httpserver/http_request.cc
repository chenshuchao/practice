#include <boost/algorithm/string.hpp>

#include <iostream>
#include "http_request.h"

using namespace std;
using namespace bytree;

HTTPRequest::HTTPRequest()
  : is_header_finished_(false),
    is_body_finished_(false),
    is_upgrade_(false)
{
}

void HTTPRequest::AddHeader(const string name, const string& value)
{
  headers_map_.insert(pair<string, string>(name, value));
}

void HTTPRequest::SetUrl(const std::string& url)
{
  url_ = url;
  return;
}
void HTTPRequest::SetMethod(const string& method)
{
  //todo check method
  string temp(method);
  //method_ = boost::to_upper_copy<string>(temp);
  cout << method_ << endl;
  method_ = temp;
  return;
}

