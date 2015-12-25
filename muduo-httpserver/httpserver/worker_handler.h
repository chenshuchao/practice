#ifndef WORKER_HANDLER_H
#define WORKER_HANDLER_H

#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>

namespace bytree
{
class TcpConnectionPtr;
class Buffer;
class Timestamp;

class WorkerHandler : boost::noncopyable
{
 public:
  explicit WorkerHandler(RequestHandler&);
  void Handle(const TcpConnectionPtr&, Buffer*, Timestamp);
 private:
  boost::shared_ptr<RequestHandler> req_handler_;
  HTTPRequestInfo req_info_;
};
}
#endif
