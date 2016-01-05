#ifndef HTTP_REQUEST_INFO_H
#define HTTP_REQUEST_INFO_H

#include <boost/shared_ptr.hpp>
#include <map>

namespace bytree
{
class HTTPRequest
{
 public:
  HTTPRequest();
 
  void AddHeader(const std::string name, const std::string& value);
  
  std::string GetHeader(const std::string& name)
  { return headers_map_[name]; }

  void SetMethod(const std::string& method);

  std::string GetMethod() { return method_; }

  void SetUrl(const std::string& url);

  std::string GetUrl() { return url_; }

  bool IsUpgrade() { return is_upgrade_; }

  void SetUpgrade() { is_upgrade_ = true; }

 private:
  bool is_header_finished_;
  bool is_body_finished_;
  bool is_upgrade_;

  std::string method_;
  std::string url_;
  std::string version_;
  std::map<std::string, std::string> headers_map_;
  std::map<std::string, std::string> method_get_map_;
  std::string body_;
};

typedef boost::shared_ptr<HTTPRequest> HTTPRequestPtr;
}

#endif
