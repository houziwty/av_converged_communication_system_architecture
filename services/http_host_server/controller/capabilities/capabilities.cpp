#include "capabilities.h"
using namespace api::v1;

Capabilities::Capabilities()
{}

Capabilities::~Capabilities()
{}

void Capabilities::all(
    const HttpRequestPtr& req, 
    std::function<void(const HttpResponsePtr&)>&& callback)
{
    Json::Value rep;
    rep["获取服务器api列表"] = "/api/v1/capabilities/getapilist";
    rep["获取视频设备列表"] = "/api/v1/device/getdvslist";
    rep["添加视频设备"] = "/api/v1/device/add";
    rep["删除视频设备列表"] = "/api/v1/device/remove";

    if (callback)
    {
        callback(HttpResponse::newHttpJsonResponse(rep));
    }
}
