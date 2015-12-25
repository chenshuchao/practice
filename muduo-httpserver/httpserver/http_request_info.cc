#include "http_request_info.h"

using namespace std;
HTTPRequestInfo::HTTPRequestInfo()
  : is_header_finished_(false),
    is_body_finished_(false)
{
}

void HTTPRequestInfo::ParseHeader(const string& request)
{
  size_t lpos = 0, rpos = 0, len_request = request.size();

  //parse firstline begin
  rpos = request.find("\n", lpos);
  if (string::npos == rpos)
  {
    // error
  }
  string first_line = string(request, lpos, rpos-lpos);
  vector<string> first_line_vector;
  // todo if double space
  split(first_line_vector, first_line, is_any_of(" "));
  if (first_line_vector.size != 3)
  {
    // error
  }
  header_params_[kMethod] = first_line_vector[0];
  header_params_[kURL] = first_line_vector[1];
  header_params_[kProtocol] = first_line_vector[2];

  parseURL(header_params_[kURL], method_get_params);
  lpos = rpos + 1;
  if (lpos >= len_request)
  {
    //error
  }
  //parse firstline end

  //parse headerbody begin
  do {
    rpos = request.find("\n", lpos);
    if (string::npos == rpos)
    {
      //error
    }
    string s = string(request, lpos, rpos-lpos);
    trim(s);
    if (s.empty())
    {
      break;
    }
    size_t pos = s.find(":");
    if (string::npos == pos)
    {
      //error
    }
    string k = string(s, lpos, pos-lpos);
    string v = string(s, lpos+1);
    trim(k);
    trim(v);
    if (k.empty() || v.empty())
    {
      //error
    }
    header_params_[k] = v;
    lpos = rpos + 1;
  } while(lpos < len_request)
  //parse headerbody end
  
  
}


