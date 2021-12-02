#include <iostream>
#include "liblog/log.h"
using namespace module::file::log;
#include "error_code.h"
#include "utils/commandline/commandline.h"
using namespace framework::utils::parser;
#include "xmq_host_service.h"

int main(int argc, char* argv[])
{
    FileLog fileLog;
    fileLog.createNew(argv[0]);
    CommandLineParser parser;
    parser.setOption("local_port", "");
    parser.setOption("pub_port", "");
    parser.setOption("app_name", "");

    if (Error_Code_Success == parser.parse(argc, argv))
    {
        const std::string local_port{parser.getParameter("local_port")};
        const std::string pub_port{parser.getParameter("pub_port")};
        const std::string app_name{parser.getParameter("app_name")};

        fileLog.write(
            SeverityLevel::SEVERITY_LEVEL_INFO, 
            "Parse command line parameters with local_port = [ %s ], pub_port = [ %s ], app_name = [ %s ].",
            local_port.c_str(), pub_port.c_str(), app_name.c_str());

        if (!local_port.empty() && !pub_port.empty() && !app_name.empty())
        {
            XmqHostService xhs{app_name, fileLog};
            xhs.start(atoi(local_port.c_str()), atoi(pub_port.c_str()));
            getchar();
            xhs.stop();
        }
    }
    else
    {
        fileLog.write(SeverityLevel::SEVERITY_LEVEL_ERROR, "Parse command line failed.");
    }

    fileLog.destroy();
    return 0;
}