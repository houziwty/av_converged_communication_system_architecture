#include "memory/xstr.h"
using namespace framework::utils::memory;
#include "time/xtime.h"
using namespace framework::utils::time;
#include "../../server.h"
#include "device.h"
using namespace api::v1;

Device::Device(Server& s) : server{s}
{}

Device::~Device()
{}

void Device::add(
    const HttpRequestPtr& req, 
    std::function<void(const HttpResponsePtr&)>&& callback)
{
    const char* content{req->bodyData()};
    const ContentType contentType{req->getContentType()};
    Json::Value rep;
    
    if (ContentType::CT_APPLICATION_JSON == contentType && content)
    {
        std::string out;
        server.addDvs(content, out);
        Json::Reader reader;
        rep["result"] = (reader.parse(out, rep) ? "Success" : "Failed");
        rep.removeMember("command");
        rep.removeMember("timestamp");
    }
    else
    {
        rep["result"] = "Failure";
        rep["msg"] = "Need application/json";
    }

    if (callback)
    {
        callback(HttpResponse::newHttpJsonResponse(rep));
    }
}

void Device::remove(
    const HttpRequestPtr& req, 
    std::function<void(const HttpResponsePtr&)>&& callback)
{
    const char* content{req->bodyData()};
    const ContentType contentType{req->getContentType()};
    Json::Value rep;
    
    if (ContentType::CT_APPLICATION_JSON == contentType && content)
    {
        std::string out;
        server.removeDvs(content, out);
        Json::Reader reader;
        rep["result"] = (reader.parse(out, rep) ? "Success" : "Failed");
        rep.removeMember("command");
        rep.removeMember("timestamp");
    }
    else
    {
        rep["result"] = "Failure";
        rep["msg"] = "Need application/json";
    }

    if (callback)
    {
        callback(HttpResponse::newHttpJsonResponse(rep));
    }
}

void Device::all(
    const HttpRequestPtr& req, 
    std::function<void(const HttpResponsePtr&)>&& callback)
{
    std::string out;
    server.getDvslist(out);

    Json::Value rep;
    Json::Reader reader;
    rep["result"] = (reader.parse(out, rep) ? "Success" : "Failed");
    rep.removeMember("command");
    rep.removeMember("timestamp");

    if (callback)
    {
        callback(HttpResponse::newHttpJsonResponse(rep));
    }
}
