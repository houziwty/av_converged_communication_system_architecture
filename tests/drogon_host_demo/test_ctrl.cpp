/// The TestCtrl.cc file

#include "test_ctrl.h"
using namespace api::v1;

void User::login(const HttpRequestPtr &req,
                std::function<void (const HttpResponsePtr &)> &&callback,
                std::string &&userId,
                const std::string &password)
{
    LOG_DEBUG<<"User "<<userId<<" login";
    //Authentication algorithm, read database, verify identity, etc...
    //...
    Json::Value ret;
    ret["result"]="ok";
    ret["token"]=drogon::utils::getUuid();
    auto resp=HttpResponse::newHttpJsonResponse(ret);
    callback(resp);
}

void User::getInfo(const HttpRequestPtr &req,
                    std::function<void (const HttpResponsePtr &)> &&callback,
                    std::string userId,
                    const std::string &token) const
{
    LOG_DEBUG<<"User "<<userId<<" get his information";

    //Verify the validity of the token, etc.
    //Read the database or cache to get user information
    Json::Value ret;
    ret["result"]="ok";
    ret["user_name"]="Jack";
    ret["user_id"]=userId;
    ret["gender"]=1;
    auto resp=HttpResponse::newHttpJsonResponse(ret);
    resp->setContentTypeCode(CT_APPLICATION_JSON);
    callback(resp);
}