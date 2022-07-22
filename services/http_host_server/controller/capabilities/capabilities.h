//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2022-07-21
//		Description : HTTP API接口查询控制器
//
//		History:
//					1. 2022-07-21 由王科威创建
//

#ifndef SERVICE_HTTP_CONTROLLER_CAPABILITIES_H
#define SERVICE_HTTP_CONTROLLER_CAPABILITIES_H

#include <drogon/HttpController.h>
using namespace drogon;

namespace api
{
  namespace v1
  {
    class Capabilities : public drogon::HttpController<Capabilities, false>
    {
    public:
      Capabilities(void);
      ~Capabilities(void);

    public:
      METHOD_LIST_BEGIN
        METHOD_ADD(Capabilities::all, "/getapilist", Get);
      METHOD_LIST_END
      
      void all(
        const HttpRequestPtr& req, 
        std::function<void(const HttpResponsePtr&)>&& callback);
    };//class Capabilities
  }//namespace v1
}//namespace api

#endif//SERVICE_HTTP_CONTROLLER_CAPABILITIES_H