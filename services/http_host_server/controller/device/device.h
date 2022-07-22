//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2022-07-20
//		Description : HTTP设备控制器
//
//		History:
//					1. 2022-07-20 由王科威创建
//

#ifndef SERVICE_HTTP_CONTROLLER_DEVICE_H
#define SERVICE_HTTP_CONTROLLER_DEVICE_H

#include <drogon/HttpController.h>
using namespace drogon;

class Server;

namespace api
{
  namespace v1
  {
    class Device : public drogon::HttpController<Device, false>
    {
    public:
      Device(Server& s);
      virtual ~Device(void);

    public:
      METHOD_LIST_BEGIN
        METHOD_ADD(Device::all, "/getdvslist", Get);
        METHOD_ADD(Device::add, "/add", Post);
        METHOD_ADD(Device::remove,"/remove", Post);
      METHOD_LIST_END
      
      void add(
        const HttpRequestPtr& req, 
        std::function<void(const HttpResponsePtr&)>&& callback);
      void remove(
        const HttpRequestPtr& req, 
        std::function<void(const HttpResponsePtr&)>&& callback);
      void all(
        const HttpRequestPtr& req, 
        std::function<void(const HttpResponsePtr&)>&& callback);
        
    private:
      Server& server;
    };//class Device
  }//namespace v1
}//namespace api

#endif//SERVICE_HTTP_CONTROLLER_DEVICE_H