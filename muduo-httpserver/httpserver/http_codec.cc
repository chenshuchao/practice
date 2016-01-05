#include <string>

#include <muduo/base/Logging.h>
#include <muduo/net/Buffer.h>
#include "http_codec.h"
#include "string_util.h"

using namespace std;
using namespace bytree;

http_parser_settings HTTPCodec::kParserSettings;

HTTPCodec::HTTPCodec()
  : parseState_(kParseHeaderNothing),
    is_headers_complete_(false),
    is_body_complete_(false),
    is_upgrade_complete_(false),
    cur_stream_id_(0)
{
  http_parser_init(&parser_, HTTP_REQUEST);
  parser_.data = this;
  InitParserSettings();
}

void HTTPCodec::InitParserSettings()
{
  kParserSettings.on_message_begin = HTTPCodec::OnMessageBeginCallback;
  kParserSettings.on_url = HTTPCodec::OnUrlCallback;
  kParserSettings.on_header_field = HTTPCodec::OnHeaderFieldCallback;
  kParserSettings.on_header_value = HTTPCodec::OnHeaderValueCallback;
  kParserSettings.on_headers_complete = HTTPCodec::OnHeadersCompleteCallback;
  //kParserSettings.on_body = HTTPCodec::OnBodyCallback;
  kParserSettings.on_message_complete = HTTPCodec::OnMessageCompleteCallback;
// kParserSettings.on_chunk_header = HTTPCodec::OnChunkHeaderCallback;
 // kParserSettings.on_chunk_complete = HTTPCodec::OnChunkCompleteCallback;
}

size_t HTTPCodec::OnData(const string& data)
{
  LOG_DEBUG << "HTTPCodec::OnData [" << name_
            << "], data:\n" << data;
  if (is_upgrade_complete_)
  {
    // skip to excute other protocol handle
    return 0;
  }
  size_t bytes_parsed = http_parser_execute(&parser_,
                                            &kParserSettings,
                                            data.c_str(),
                                            data.size());
  
  LOG_DEBUG << "HTTPCodec::OnData [" << name_
            << "], parse " << bytes_parsed << " bytes.";
  return bytes_parsed;
}

int HTTPCodec::OnMessageBegin()
{
  parseState_ = kParseHeaderBegin;
  LOG_INFO << "HTTPCodec::OnMessageBegin [].";
  callback_->OnMessageBegin(cur_stream_id_);
  return 0;
}

int HTTPCodec::OnUrl(const char* buf, size_t len)
{
  LOG_INFO << "HTTPCodec::OnUrl [" << name_<< "]."
           << "len = " << len;
  request_.SetUrl(string(buf, len));
  return 0;
}

int HTTPCodec::OnHeaderField(const char* buf, size_t len)
{
  LOG_INFO << "HTTPCodec::OnHeaderField";
  if (parseState_ == kParseHeaderValue)
  {
    request_.AddHeader(cur_header_name_, cur_header_value_);
    cur_header_name_.clear();
    cur_header_name_.append(buf, len);
  } else if (parseState_ == kParseHeaderName
             || parseState_ == kParseHeaderBegin)
  {
    cur_header_name_.append(buf, len);
  }
  parseState_ = kParseHeaderName;
  LOG_INFO << "HTTPCodec::OnHeaderField [" << name_
           << "] - cur_header_name_: " << cur_header_name_;
  return 0;
}

int HTTPCodec::OnHeaderValue(const char *buf, size_t len)
{
  LOG_INFO << "HTTPCodec::OnHeaderValue";
  if (parseState_ == kParseHeaderName)
  {
    cur_header_value_.clear();
    cur_header_value_.append(buf, len);
  }
  else if (parseState_ == kParseHeaderValue)
  {
    cur_header_value_.append(buf, len);
  }
  parseState_ = kParseHeaderValue;

  LOG_INFO << "HTTPCodec::OnHeaderValue [" << name_
         << "] - cur_header_value_: " << cur_header_value_;
  return 0;
}

int HTTPCodec::OnHeadersComplete()
{
  LOG_INFO << "HTTPCodec::OnHeadersComplete [" << name_ << "].";
  is_headers_complete_ = true;
  string method(http_method_str(static_cast<http_method>(parser_.method)));
  request_.SetMethod(method);
  if (parser_.upgrade == 1)
  {
    request_.SetUpgrade();
  }
  callback_->OnHeadersComplete();
  return 0;
}

int HTTPCodec::OnMessageComplete()
{
  LOG_INFO << "HTTPCodec::OnMessageComplete [" << name_ << "].";
  callback_->OnMessageComplete(cur_stream_id_, request_);
  return 0;
}

int HTTPCodec::OnMessageBeginCallback(http_parser *parser)
{
  HTTPCodec* codec = static_cast<HTTPCodec*>(parser->data);
  assert (codec != NULL);
  return codec->OnMessageBegin();
}

int HTTPCodec::OnUrlCallback(http_parser *parser, const char* buf, size_t len)
{
  HTTPCodec* codec = static_cast<HTTPCodec*>(parser->data);
  return codec->OnUrl(buf, len);
}

int HTTPCodec::OnHeaderFieldCallback(http_parser *parser, const char* buf, size_t len)
{
  HTTPCodec* codec = static_cast<HTTPCodec*>(parser->data);
  return codec->OnHeaderField(buf, len);
}

int HTTPCodec::OnHeaderValueCallback(http_parser *parser, const char* buf, size_t len)
{
  HTTPCodec* codec = static_cast<HTTPCodec*>(parser->data);
  return codec->OnHeaderValue(buf, len);
}

int HTTPCodec::OnHeadersCompleteCallback(http_parser *parser)
{
  HTTPCodec* codec = static_cast<HTTPCodec*>(parser->data);
  return codec->OnHeadersComplete();
}

int HTTPCodec::OnMessageCompleteCallback(http_parser *parser)
{
  HTTPCodec* codec = static_cast<HTTPCodec*>(parser->data);
  return codec->OnMessageComplete();
}

string HTTPCodec::ConvertResponseToString(HTTPResponse& resp) const
{  
  string first_line = "";
  int status_code = resp.GetStatusCode();
  string status_message = resp.GetStatusMessage();
  first_line = "HTTP/1.1 " + int_to_string(status_code) + " " + status_message;
  
  string headers = "";
  vector<string> name, value;
  resp.GetAllHeaders(name, value);
  for (int i = 0, len = name.size(); i < len; i ++)
  {
    headers += name[i] + ": " + value[i] + "\r\n";
  }

  string body = resp.GetBody();

  string msg = first_line + "\r\n" +
               headers + "\r\n" +
               body;
  return msg;
}
