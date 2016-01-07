#ifndef HTTP_CODEC_H
#define HTTP_CODEC_H

#include <map>
#include <boost/bind.hpp>
#include <boost/function.hpp>

#include "lib/http_parser/http_parser.h"
#include "http_request.h"
#include "http_response.h"

namespace bytree
{

class Buffer;
class HTTPCodec
{
 public:
  enum HeaderParseState
  {
    kParseHeaderNothing,
    kParseHeaderBegin,
    kParseHeaderName,
    kParseHeaderValue,
    kParseHeaderComplete
  };

  typedef uint32_t StreamID;
  typedef boost::function<void (StreamID)> MessageBeginCallback;
  typedef boost::function<void (StreamID, HTTPRequest)> MessageCompleteCallback;
  typedef boost::function<void ()> HeadersCompleteCallback; 
  class Callback
  {
   public:
    Callback() {}
    virtual void OnMessageBegin(StreamID id)
    {
      if(message_begin_callback_) message_begin_callback_(id);
    }
    virtual void OnHeadersComplete()
    { 
      if(headers_complete_callback_) headers_complete_callback_();
    }
    virtual void OnMessageComplete(StreamID id, HTTPRequest info)
    { 
      if(message_complete_callback_) message_complete_callback_(id, info);
    }

    void SetMessageBeginCallback(const MessageBeginCallback& cb)
    { message_begin_callback_ = cb; }

    void SetMessageCompleteCallback(const MessageCompleteCallback& cb) 
    { message_complete_callback_ = cb; }
    void SetHeadersCompleteCallback(const HeadersCompleteCallback& cb)
    { headers_complete_callback_ = cb; }

   private:
    MessageBeginCallback message_begin_callback_;
    MessageCompleteCallback message_complete_callback_;
    HeadersCompleteCallback headers_complete_callback_;
  };

  explicit HTTPCodec();

  static void InitParserSettings();

  std::string GetName() { return name_; }

  StreamID CreateStreamID()
  { return ++cur_stream_id_; }

  void SetCallback(Callback* cb)
  { callback_ = cb; }

  size_t OnData(const std::string& data);

  int OnMessageBegin();

  int OnUrl(const char* buf, size_t len);

  int OnHeaderField(const char* buf, size_t len);

  int OnHeaderValue(const char* buf, size_t len);

  int OnHeadersComplete();

  int OnMessageComplete();

  void OnParseError(const char* what);

  std::string GetHeaderName() { return cur_header_name_; }

  std::string GetHeaderValue() { return cur_header_value_; }

  std::string ConvertResponseToString(HTTPResponse& resp) const;
   
 private:
  HeaderParseState parseState_;
  static int OnMessageBeginCallback(http_parser *parser);
  static int OnUrlCallback(http_parser *parser, const char* buf, size_t len);
  static int OnHeaderFieldCallback(http_parser *parser, const char* buf, size_t len);
  static int OnHeaderValueCallback(http_parser *parser, const char* buf, size_t len);
  static int OnHeadersCompleteCallback(http_parser *parser);
  static int OnBodyCallback(http_parser *parser, const char* buf, size_t len);
  static int OnMessageCompleteCallback(http_parser *parser);
  static int OnChunkHeaderCallback(http_parser *parser);
  static int OnChunkHeaderCompleteCallback(http_parser *parser);

  std::string name_;
  http_parser parser_;
  bool is_headers_complete_;
  bool is_body_complete_;
  bool is_upgrade_complete_;
  std::string cur_header_value_;
  std::string cur_header_name_;
  StreamID cur_stream_id_;
  HTTPRequest request_;
  HTTPCodec::Callback* callback_;

  static http_parser_settings kParserSettings;
};
}

#endif
