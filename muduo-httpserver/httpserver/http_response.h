#ifndef HTTPRESPONSE_H
#define HTTPRESPONSE_H

#include <string>
#include <map>
#include <vector>

namespace bytree
{
class HTTPResponse
{
 public:
  HTTPResponse() {}
  HTTPResponse& SetStatus(int code, const std::string& message);
  HTTPResponse& AddHeader(const std::string& name, const std::string& value);
  HTTPResponse& AddBody(const std::string& body);

  int GetStatusCode() { return status_code_; }
  std::string GetStatusMessage() { return status_message_; }
  std::string GetHeaderValue(const std::string& name);
  void GetAllHeaders(std::vector<std::string>& name, std::vector<std::string>& value);
  std::string GetBody() { return body_; }

 private:
  // TODO
  int status_code_;
  std::string status_message_;
  std::map<std::string, std::string> headers_map_;
  std::string body_;
};
}

#endif
