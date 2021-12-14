#include "liblog/log.h"
using namespace module::file::log;
#include "error_code.h"
#include "utils/commandline/commandline.h"
using namespace framework::utils::parser;
#include "test_dvs_host_client.h"

static FileLog gFileLog;

int main(int argc, char* argv[])
{
    gFileLog.createNew(argv[0]);
    CommandLineParser parser;
    parser.setOption("xmq_addr", "");
    parser.setOption("xmq_port", "");
    parser.setOption("app_name", "");

    if (Error_Code_Success == parser.parse(argc, argv))
    {
        const std::string xmq_addr{parser.getParameter("xmq_addr")};
        const std::string xmq_port{parser.getParameter("xmq_port")};
        const std::string app_name{parser.getParameter("app_name")};

        gFileLog.write(
            SeverityLevel::SEVERITY_LEVEL_INFO, 
            "Parse command line parameters with xmq_addr = [ %s ], xmq_port = [ %s ], app_name = [ %s ].",
            xmq_addr.c_str(), xmq_port.c_str(), app_name.c_str());

        if (!xmq_addr.empty() && !xmq_port.empty() && !app_name.empty())
        {
            TestDvsHostClient test{gFileLog};
            test.start(app_name, xmq_addr, atoi(xmq_port.c_str()));
            getchar();
            test.stop();
        }
    }
    else
    {
        gFileLog.write(SeverityLevel::SEVERITY_LEVEL_ERROR, "Parse command line failed.");
    }

    gFileLog.destroy();
    return 0;
}