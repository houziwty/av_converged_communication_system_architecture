#include "error_code.h"
#include "commandline/commandline.h"
using namespace framework::utils::parser;
#include "server.h"

int main(int argc, char* argv[])
{
    FileLog log;
    log.createNew("", false);
    CommandLineParser parser;
    parser.setOption("local_port", "");

    if (Error_Code_Success == parser.parse(argc, argv))
    {
        const std::string local_port{parser.getParameter("local_port")};
        
        Server svr(log);
        svr.run(atoi(local_port.c_str()));
        getchar();
        svr.stop();
    }

    return 0;
}