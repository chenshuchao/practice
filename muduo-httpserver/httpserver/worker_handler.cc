#include "worker_handler.h"


explicit WorkerHandler::WorkerHandler(RequestHandler& req_handler)
  : req_hander_(req_handler)
{
}

void WorkerHandler::Handle(const TcpConnection& conn, Buffer* buffer, Timestamp timestamp)
{
  string request = buffer->retriveAllAsString(buffer);
  req_info_.ParseHeader(request);
  
  if (req_info.IsGet())
  {
    req_handler_.DoGet(req_info_);
  }
  if (req_info.IsPost())
  {
    req_handler_.DoPost(req_info_);
  }
}
