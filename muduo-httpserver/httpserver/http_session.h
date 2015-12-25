#ifndef HTTP_SESSION_H
#define HTTP_SESSION_H

namespace bytree
{
class HTTPCodec;

class HTTPSession
{
 public:
  void OnRequest(); 
 private:
  HTTPCodec codec_;  
};

typedef boost::shared_ptr<HTTPSession> HTTPSessionPtr;

}

#endif
