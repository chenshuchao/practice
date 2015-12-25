#include "http_codec.h"

explicit HTTPCodec::HTTPCodec()
  : is_headers_complete_(false),
    is_body_complete_(false),
    is_upgrade_complete_(false)
{
}

static void HTTPCodec::initParserSettings() {
  kParserSettings.on_message_begin = HTTPCodec::OnMessageBeginCallback;
  kParserSettings.on_url = HTTPCodec::OnUrlCallback;
  kParserSettings.on_header_field = HTTPCodec::OnHeaderFieldCallback;
  kParserSettings.on_header_value = HTTPCodec::OnHeaderValueCallback;
  kParserSettings.on_headers_complete = HTTPCodec::OnHeadersCompleteCallback;
  kParserSettings.on_body = HTTPCodec::OnBodyCallback;
  kParserSettings.on_message_complete = HTTPCodec::OnMessageCompleteCallback;
  kParserSettings.on_reason = HTTPCodec::OnReasonCallback;
  kParserSettings.on_chunk_header = HTTPCodec::OnChunkHeaderCallback;
  kParserSettings.on_chunk_complete = HTTPCodec::OnChunkCompleteCallback;
}

size_t HTTPCodec::Parse(const Buffer& buf)
{
  if (is_upgrade_complete_)
  {
    // skip to excute other protocol handle
    
    return;
  }
  size_t bytes_parsed = http_parse_excute(&parser_,
                                          &kParserSettings,
                                          buf.begin(),
                                          buf.readableBytes());

  return bytes_parsed;
}

void HTTPCodec::OnHeadersComplete()
{
  is_headers_complete_ = true;  
}

void HTTPCodec::OnMessageComplete()
{
}



static void HTTPCodec::OnHeadersCompleteCallback(http_parser *parser)
{
  HTTPCodec* codec = static_cast<HTTPCodec*>(parser->data);
  codec->OnHeadersComplete();
}

static void HTTPCodec::OnMessageCompleteCallback(http_parser *parser)
{
  HTTPCodec* codec = static_cast<HTTPCodec*>(parser->data);
  codec->OnMessageComplete();
}


