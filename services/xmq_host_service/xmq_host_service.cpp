#include "boost/bind/bind.hpp"
using namespace boost::placeholders;
#include "liblog/log.h"
using namespace module::file::log;
#include "error_code.h"
#include "utils/url/url.h"
using namespace framework::utils::url;
#include "utils/thread/thread.h"
#include "utils/thread/thread_pool.h"
using namespace framework::utils::thread;
#include "utils/time/xtime.h"
using namespace framework::utils::time;
#include "xmq_host_service.h"

XmqHostService::XmqHostService(const std::string name, FileLog& log) 
    : SwitcherPubModel(), serviceName{name}, fileLog{log}, stopped{false}, thread{nullptr}
{}

XmqHostService::~XmqHostService()
{}

int XmqHostService::start(
	const unsigned short switcherPort /* = 0 */, 
	const unsigned short publisherPort /* = 0 */, 
	const int hwm /* = 10 */)
{
    int ret{SwitcherPubModel::start(switcherPort, publisherPort, hwm)};

    if (Error_Code_Success == ret)
    {
        thread = ThreadPool().get_mutable_instance().createNew(
            boost::bind(&XmqHostService::checkRegisterExpiredOfServiceThread, this));
    }
    
    return ret;
}

int XmqHostService::stop()
{
    int ret{SwitcherPubModel::stop()};

    if (Error_Code_Success == ret && false == stopped)
    {
        stopped = true;
        Thread().join(thread);
        ThreadPool().get_mutable_instance().destroy(thread);
    }
    
    return ret;
}

void XmqHostService::afterSwitcherPolledDataHandler(const std::string appid, const std::string data)
{
    Url requestUrl;

    if(Error_Code_Success == requestUrl.parse(data))
    {
        if (0 == requestUrl.getProtocol().compare("register"))
        {
            const std::vector<ParamItem> items{requestUrl.getParameters()};
            for(int i = 0; i != items.size(); ++i)
            {
                if(0 == items[i].key.compare("tick"))
                {
                    registeredServices.replace(appid, std::stoull(items[i].value));
                    fileLog.write(
                        SeverityLevel::SEVERITY_LEVEL_INFO, "Add register app_name = [ %s ] with data = [ %s ].",  appid.c_str(), data.c_str());
                }
            }
        }
        else if (0 == requestUrl.getProtocol().compare("query"))
        {

        }
        else
        {
            fileLog.write(
                SeverityLevel::SEVERITY_LEVEL_WARNING, "Parsed unknown data app_name = [ %s ] with data = [ %s ].",  appid.c_str(), data.c_str());
        }
    }
    else
    {
        fileLog.write(
            SeverityLevel::SEVERITY_LEVEL_ERROR, "Parsed data from = [ %s ] failed with data = [ %s ].",  appid.c_str(), data.c_str());
    }
}

void XmqHostService::checkRegisterExpiredOfServiceThread()
{
	while (!stopped)
	{
        XTime xt;
        const unsigned long long startTickCount{xt.tickcount()};
        const std::vector<std::string> keies{registeredServices.keies()};

        for (int i = 0; i != keies.size(); ++i)
        {
            const unsigned long long tick{registeredServices.at(keies[i])};
            const unsigned long long diff{startTickCount - tick};

            if (diff > 90000)
            {
                registeredServices.remove(keies[i]);
                fileLog.write(
                    SeverityLevel::SEVERITY_LEVEL_WARNING, "Remove app_name = [ %s ] with expired tick = [ %ld ].",  keies[i].c_str(), diff);
            }
        }
        
		xt.sleep(1000);
	}
}
