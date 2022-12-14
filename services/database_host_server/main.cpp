#include "boost/pointer_cast.hpp"
#include "boost/make_shared.hpp"
#include "error_code.h"
#include "commandline/commandline.h"
using namespace framework::utils::parser;
#include "memory/xstr.h"
using namespace framework::utils::memory;
#include "database_host_server.h"

int main(int argc, char* argv[])
{
  CommandLineParser parser;
  parser.setOption("xmq_addr", "");
  parser.setOption("xmq_port", "");
  parser.setOption("db_addr", "");
  parser.setOption("db_port", "");
  parser.setOption("db_auth", "");
  parser.setOption("db_name", "");

  if (Error_Code_Success == parser.parse(argc, argv))
  {
      const std::string xmq_addr{parser.getParameter("xmq_addr")};
      const std::string xmq_port{parser.getParameter("xmq_port")};
      const std::string db_addr{parser.getParameter("db_addr")};
      const std::string db_port{parser.getParameter("db_port")};
      const std::string db_auth{parser.getParameter("db_auth")};
      const std::string db_name{parser.getParameter("db_name")};
      const std::string name{DatabaseHostID};

      XMQModeConf conf{0};
      conf.id = 0xB1;
      conf.port = atoi(xmq_port.c_str());
      conf.type = XMQModeType::XMQ_MODE_TYPE_DEALER;
      XStr xstr;
      xstr.copy(name.c_str(), name.length(), conf.name, 128);
      xstr.copy(xmq_addr.c_str(), xmq_addr.length(), conf.ip, 32);

      boost::shared_ptr<XMQNode> node{
          boost::make_shared<DatabaseHostServer>(conf)};
      if(node && Error_Code_Success == node->addConf(conf))
      {
        boost::shared_ptr<DatabaseHostServer> downcast{
          boost::dynamic_pointer_cast<DatabaseHostServer>(node)};
        DatabaseType type{DatabaseType::DATABASE_TYPE_NONE};

        if (!db_name.compare("redis"))
        {
          type = DatabaseType::DATABASE_TYPE_REDIS;
        }
        else
        {
          return -1;
        }

        downcast->run(
          type, db_addr.c_str(), db_auth.c_str(), (uint16_t)atoi(db_port.c_str()));
        getchar();
        node->removeConf(conf.id);
        node->stop();
      }
  }

  return 0;
}