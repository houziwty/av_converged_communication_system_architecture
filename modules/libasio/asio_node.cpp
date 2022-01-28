#include "boost/bind/bind.hpp"
using namespace boost::placeholders;
#include "boost/make_shared.hpp"
#include "error_code.h"
#include "utils/map/unordered_map.h"
#include "asio/service.h"
#include "asio/acceptor.h"
#include "session/session.h"
#include "asio_node.h"
using namespace module::network::asio;

using AcceptorPtr = boost::shared_ptr<Acceptor>;
using AcceptorPtrs = UnorderedMap<const int32_t, AcceptorPtr>;
using SessionPtr = boost::shared_ptr<Session>;
using SessionPtrs = UnorderedMap<const int32_t, SessionPtr>;

static AcceptorPtrs acceptors;
static SessionPtrs sessions;
static Service ios;

ASIONode::ASIONode()
{}

ASIONode::~ASIONode()
{}

int ASIONode::run()
{
	return ios.run();
}

int ASIONode::stop()
{
	int ret{ios.stop()};
	return ret;
}

int ASIONode::addConf(const ASIOModeConf& conf)
{
	int ret{0 < ios.size() && 0 < conf.id ? Error_Code_Success : Error_Code_Operate_Failure};

	if (Error_Code_Success == ret)
	{
		if (ASIOProtoType::ASIO_PROTO_TYPE_TCP == conf.proto)
		{
			if (ASIOModeType::ASIO_MODE_TYPE_LISTEN == conf.mode)
			{
				boost::shared_ptr<Acceptor> acceptor{
					boost::make_shared<Acceptor>(
						*ios.ctx(),
						[&](boost::asio::ip::tcp::socket* so, const int e)
						{
							int32_t sid{
								afterFetchAcceptedEventNotification(
									so->remote_endpoint().address().to_string().c_str(), 
									so->remote_endpoint().port(), 
									e)};

							if (!e)
							{
								acceptor->listen(*service.ctx());
							}

							SessionPtr sess{boost::make_shared<Session>(so, conf.id)};
							if (sess && session->createNew())
							{
								sess->receive();
								sessions.add(conf.id, sess);
							}
						},
						conf.port)};

				if (acceptor)
				{
					for (int i = 0; i != ios.size(); ++i)
					{
						acceptor->listen(*ios.ctx());
					}

					acceptors.add(conf.id, acceptor);
				}
			}
			else if (ASIOModeType::ASIO_MODE_TYPE_CONNECT == conf.mode)
			{
				/* code */
			}
			else
			{
				ret = Error_Code_Method_Not_Support;
			}
		}
		else if (ASIOProtoType::ASIO_PROTO_TYPE_UDP == conf.proto)
		{
		}
		else
		{
			ret = Error_Code_Method_Not_Support;
		}
	}

	return ret;
}

int ASIONode::removeConf(const uint32_t id/* = 0*/)
{
	int ret{ctx && 0 < id ? Error_Code_Success : Error_Code_Operate_Failure};

	if (Error_Code_Success == ret)
	{
		XMQRolePtr role{roles.at(id)};

		if (role)
		{
			ret = role->stop();
			roles.remove(id);
		}
	}

	return ret;
}

int XMQNode::send(
	const uint32_t id/* = 0*/, 
	const char* name/* = nullptr*/, 
	const void* data/* = nullptr*/, 
	const uint64_t bytes/* = 0*/)
{
	int ret{ctx ? Error_Code_Success : Error_Code_Operate_Failure};

	if(Error_Code_Success == ret)
	{
		XMQRolePtr role{roles.at(id)};

		if (role)
		{
			ret = role->send(name, data, bytes);
		}
	}

	return ret;
}
