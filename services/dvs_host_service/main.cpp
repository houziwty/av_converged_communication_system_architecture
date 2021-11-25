#include <iostream>
#include "error_code.h"
#include "utils/commandline/commandline.h"
using namespace framework::utils::parser;
#include "dvs_host_service.h"

int main(int argc, const char** argv)
{
   CommandLineParser parser;
   parser.setOption("xmq,x", "");
   parser.setOption("port,p", "");
   parser.setOption("xmsport", "");
   parser.setOption("name,n", "dvs_host_service");

   if (Error_Code_Success == parser.parse())
   {
       const std::string xmq_addr{parser.getParameter("xmq")};
       const std::string xmq_port{parser.getParameter("port")};
       const std::string xms_port{parser.getParameter("xmsport")};
       const std::string app_name{parser.getParameter("name")};

       std::cout << "Parse command line parameters [ xmq_addr = " << xmq_addr 
                    << ", xmq_port = " << xmq_port 
                    << ", xms_port = " << xms_port 
                    << ", app_name = " << app_name 
                    << " ]." << std::endl;

        if (!xmq_addr.empty() && !xmq_port.empty() && !xms_port.empty() && !app_name.empty())
        {
            DvsHostService dhs{
                static_cast<unsigned short>(atoi(xms_port.c_str()))};
            int ret{dhs.start(app_name, xmq_addr, atoi(xmq_port.c_str()))};
            std::cout << "Start service result = " << ret << "." << std::endl;
            getchar();
            dhs.stop();
        }
   }
   else
   {
       std::cout << "Parse command line failed." << std::endl;
   }

    return 0;
}