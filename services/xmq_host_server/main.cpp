#include "boost/make_shared.hpp"
#include "error_code.h"
#include "commandline/commandline.h"
using namespace framework::utils::parser;
#include "memory/xmem.h"
using namespace framework::utils::memory;
#include "xmq_host_server.h"

int main(int argc, char* argv[])
{
    FileLog log;
    log.createNew("", false);
    CommandLineParser parser;
    parser.setOption("local_port", "");

    if (Error_Code_Success == parser.parse(argc, argv))
    {
        const std::string local_port{parser.getParameter("local_port")};
        const std::string name{XMQHostID};
        XMQModeConf conf{0};
        conf.id = 0xA1;
        conf.port = atoi(local_port.c_str());
        conf.type = XMQModeType::XMQ_MODE_TYPE_ROUTER;
        XMem().copy(name.c_str(), name.length(), conf.name, 128);

        boost::shared_ptr<XMQNode> node{
            boost::make_shared<XmqHostServer>(conf, log)};
        if (node && Error_Code_Success == node->addConf(conf))
        {
            log.write(
                SeverityLevel::SEVERITY_LEVEL_INFO,
                "Run xmq host server with port [ %u ] and name [ %s ] successfully.",
                conf.port, conf.name);
            node->run();
            getchar();
            node->stop();
            node->removeConf(conf.id);
        }
        else
        {
			log.write(
				SeverityLevel::SEVERITY_LEVEL_ERROR,
				"Run xmq host server with port [ %u ] and name [ %s ] failed, object [ %x ].",
				conf.port, conf.name, node);
        }
    }

    return 0;
}