#ifndef HTTP_CODEC_H
#define HTTP_CODEC_H

namespace
{
class HTTPCodec
{
 public:
  static void InitParserSettings();
  size_t OnIngress(const Buffer&);
   
  
 private:
  static OnMessageBeginCallback(http_parser *parser);
  static OnUrlCallback(http_parser *parser, const char* buf, size_t len);
  static OnHeaderFieldCallback(http_parser *parser, const char* buf, size_t len);
  static OnHeaderValueCallback(http_parser *parser, const char* buf, size_t len);
  static OnHeadersCompleteCallback(http_parser *parser);
  static OnBodyCallback(http_parser *parser, const char* buf, size_t len);
  static OnMessageCompleteCallback(http_parser *parser);
  static OnChunkHeaderCallback(http_parser *parser);
  static OnChunkHeaderCompleteCallback(http_parser *parser);

  bool is_header_complete_;
  bool is_body_complete_;
  http_parse parser_;
  static http_parse_settings kParserSettings;
};
}

#endif
