#include "error_code.h"
#include "test_dvs_host_client.h"

TestDvsHostClient::TestDvsHostClient(FileLog& log) 
    : LibXmqHostClient(), fileLog{log}
{}

TestDvsHostClient::~TestDvsHostClient()
{}

int TestDvsHostClient::start(
    const std::string name, 
    const std::string ip, 
    const unsigned short port/* = 0*/)
{
    int ret{LibXmqHostClient::registerXmqHostClient(name, ip, port)};

    if (Error_Code_Success == ret)
    {
        fileLog.write(SeverityLevel::SEVERITY_LEVEL_INFO, "Start xmq host client successed.");
    }
    else
    {
        fileLog.write(
            SeverityLevel::SEVERITY_LEVEL_ERROR, 
            "Start xmq host client failed with name = [ %s ] ip = [ %s ] port = [ %d ], result = [ %d ].", 
            name.c_str(), ip.c_str(), port, ret);
    }
    
    return ret;
}

int TestDvsHostClient::stop()
{
    unregisterXmqHostClient();    
    return Error_Code_Success;
}

void TestDvsHostClient::fetchXmqHostClientOnlineStatusNotification(bool online)
{
    fileLog.write(SeverityLevel::SEVERITY_LEVEL_INFO, "Fetch test dvs host client online status = [ %d ].", online);
}

void TestDvsHostClient::fetchXmqHostServiceCapabilitiesNotification(const std::vector<std::string> services)
{
    const int number{static_cast<int>(services.size())};
    fileLog.write(SeverityLevel::SEVERITY_LEVEL_INFO, "Fetch xmq host service capabilities size = [ %d ].", number);

    for(int i = 0; i != number; ++i)
    {
        fileLog.write(SeverityLevel::SEVERITY_LEVEL_INFO, " ****** Service name = [ %s ].", services[i].c_str());

        if (!services[i].compare("dvs_host_service"))
        {
            const std::string url{
                "dvs://dvs_host_service?from=test_dvs_host_client&command=query"};
            int ret{send(url)};

            if (Error_Code_Success == ret)
            {
                fileLog.write(SeverityLevel::SEVERITY_LEVEL_INFO, "Send query device information to dvs host service successed.");
            }
            else
            {
                fileLog.write(
                    SeverityLevel::SEVERITY_LEVEL_ERROR, 
                    "Send query device information to dvs host service failed, result = [ %d ].", 
                    ret);
            }
        }
    }
}

void TestDvsHostClient::fetchXmqHostClientReceivedDataNotification(const std::string data)
{
    fileLog.write(SeverityLevel::SEVERITY_LEVEL_INFO, "Fetch test dvs host client received data = [ %s ].", data.c_str());

    const std::string url{
        "dvs://dvs_host_service?from=test_dvs_host_client&command=add&ip=192.168.2.225&port=8000&user=admin&passwd=Vrc123456"};
    int ret{send(url)};

    if (Error_Code_Success == ret)
    {
        fileLog.write(SeverityLevel::SEVERITY_LEVEL_INFO, "Send add device information to dvs host service successed.");
    }
    else
    {
        fileLog.write(
            SeverityLevel::SEVERITY_LEVEL_ERROR, 
            "Send add device information to dvs host service failed, result = [ %d ].", 
            ret);
    }
}
