#include "boost/make_shared.hpp"
#include "error_code.h"
#include "commandline/commandline.h"
using namespace framework::utils::parser;
#include "memory/xstr.h"
using namespace framework::utils::memory;
#include "dvs_host_server.h"

int main(int argc, char* argv[])
{
    CommandLineParser parser;
    parser.setOption("xmq_addr", "");
    parser.setOption("xmq_port", "");

    if (Error_Code_Success == parser.parse(argc, argv))
    {
        const std::string xmq_addr{parser.getParameter("xmq_addr")};
        const std::string xmq_port{parser.getParameter("xmq_port")};
        const std::string name{DVSHostID};
        XMQModeConf conf{0};
        conf.id = 0xB1;
        conf.port = atoi(xmq_port.c_str());
        conf.type = XMQModeType::XMQ_MODE_TYPE_DEALER;
        XStr xstr;
		xstr.copy(name.c_str(), name.length(), conf.name, 128);
		xstr.copy(xmq_addr.c_str(), xmq_addr.length(), conf.ip, 32);

        boost::shared_ptr<XMQNode> node{
            boost::make_shared<DvsHostServer>(conf)};
        if(node && Error_Code_Success == node->addConf(conf))
        {
            node->run();
            getchar();
			node->removeConf(conf.id);
            node->stop();
        }
    }

    return 0;
}