#include "error_code.h"
#include "utils/commandline/commandline.h"
using namespace framework::utils::parser;
#include "dvs_host_service.h"

static FileLog gFileLog;

int main(int argc, char* argv[])
{
    gFileLog.createNew(argv[0]);
    CommandLineParser parser;
    parser.setOption("xmq_addr", "");
    parser.setOption("xmq_port", "");
    parser.setOption("xms_port", "");
    parser.setOption("app_name", "");

    if (Error_Code_Success == parser.parse(argc, argv))
    {
        const std::string xmq_addr{parser.getParameter("xmq_addr")};
        const std::string xmq_port{parser.getParameter("xmq_port")};
        const std::string xms_port{parser.getParameter("xms_port")};
        const std::string app_name{parser.getParameter("app_name")};

        gFileLog.write(
            SeverityLevel::SEVERITY_LEVEL_INFO, 
            "Parse command line parameters with xmq_addr = [ %s ], xmq_port = [ %s ], xms_port = [ %s ], app_name = [ %s ].",
            xmq_addr.c_str(), xmq_port.c_str(), xms_port.c_str(), app_name.c_str());

        if (!xmq_addr.empty() && !xmq_port.empty() && !xms_port.empty() && !app_name.empty())
        {
            DvsHostService dhs{gFileLog};
            dhs.start(app_name, xmq_addr, atoi(xmq_port.c_str()));
            dhs.startXMS(static_cast<unsigned short>(atoi(xms_port.c_str())));
            getchar();
            dhs.stopXMS();
            dhs.stop();
        }
    }
    else
    {
        gFileLog.write(SeverityLevel::SEVERITY_LEVEL_ERROR, "Parse command line failed.");
    }

    gFileLog.destroy();
    return 0;
}