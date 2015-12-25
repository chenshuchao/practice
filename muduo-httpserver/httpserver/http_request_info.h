#ifndef HTTP_REQUEST_INFO_H
#define HTTP_REQUEST_INFO_H
namespace bytree
{
class HTTPRequestInfo
{
 public:
  static const char* kMethod = "METHOD";
  static const char* kURL = "URL";
  static const char* kProtocol = "PROTOCOL";
 public:
  HTTPRequestInfo();
  void ParseHeader(std::string& request);
  void ParseUrl(std:string& path, unordered_map<std::string, std::string>& mp);

 private:
  bool is_header_finished_;
  bool is_body_finished_;
  unordered_map<string, string> header_params_;
  unordered_map<string, string> method_get_params_;
  std::string request_body_;
}
};
}
#endif
