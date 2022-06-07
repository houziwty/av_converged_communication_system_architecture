#include <string>
#include "error_code.h"
#include "memory/xmem.h"
using namespace framework::utils::memory;
#include "dispatcher.h"

Dispatcher::Dispatcher() : Libxmq()
{}

Dispatcher::~Dispatcher()
{}

int Dispatcher::run(const uint16_t port/* = 0*/)
{
    int ret{0 < port && 65535 > port ? Error_Code_Success : Error_Code_Invalid_Param};

    if (Error_Code_Success == ret)
    {
        const std::string name{XMQHostID};
        modeconf.id = 0xA1;
        modeconf.port = port;
        modeconf.type = XMQModuleType::XMQ_MODULE_TYPE_DISPATCHER;
        XMem().copy(name.c_str(), name.length(), modeconf.name, 128);
        modeconf.opt.hwm = 10;
        ret = addNode(modeconf);
    }

    return ret;
}

int Dispatcher::stop()
{
    int ret{0 < modeconf.id ? Error_Code_Success : Error_Code_Invalid_Param};

    if (Error_Code_Success == ret)
    {
        ret = removeNode(modeconf.id);
    }

    return ret;
}
