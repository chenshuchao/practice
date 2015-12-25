#ifndef REQUEST_HANDLER_H
#define REQUEST_HANDLER_H

#include <string>

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>

namespace
{
class RequestHandler
{
 public:
  explicit RequestHandler();
 
  void DoGet(HTTPRequestInfo&);
  void IsPost(HTTPRequestInfo&);
};
}
#endif
