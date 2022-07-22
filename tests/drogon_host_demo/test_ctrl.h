/// The TestCtrl.h file

#pragma once

#include <drogon/HttpController.h>

using namespace drogon;

namespace api
{
namespace v1
{
class User : public drogon::HttpController<User, false>
{
  public:
    METHOD_LIST_BEGIN
        //use METHOD_ADD to add your custom processing function here;
        METHOD_ADD(User::login,"/adduser?userId={1}&passwd={2}",Post);
        METHOD_ADD(User::getInfo,"/{1}/info?token={2}",Get);
    METHOD_LIST_END
    //your declaration of processing function maybe like this:
    void login(const HttpRequestPtr &req,
                std::function<void (const HttpResponsePtr &)> &&callback,
                std::string &&userId,
                const std::string &password);
    void getInfo(const HttpRequestPtr &req,
                    std::function<void (const HttpResponsePtr &)> &&callback,
                    std::string userId,
                    const std::string &token) const;
  public:
    User()
    {
        LOG_DEBUG << "User constructor!";
    }
};
} // namespace v1
} // namespace api