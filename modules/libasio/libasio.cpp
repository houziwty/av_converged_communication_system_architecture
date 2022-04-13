#include "boost/bind/bind.hpp"
using namespace boost::placeholders;
#include "boost/checked_delete.hpp"
#include "error_code.h"
#include "map/unordered_map.h"
#include "asio/service.h"
#include "asio/acceptor.h"
#include "asio/connector.h"
#include "tcp/tcp_session.h"
#include "udp/udp_session.h"
#include "libasio.h"
using namespace module::network::asio;

using AcceptorPtr = boost::shared_ptr<Acceptor>;
using ConnectorPtr = boost::shared_ptr<Connector>;
using SessionPtr = boost::shared_ptr<Session>;
using SessionPtrs = UnorderedMap<const uint32_t, SessionPtr>;

static SessionPtrs sessions;
static Service ios;

Libasio::Libasio()
{
	ios.run();
}

Libasio::~Libasio()
{
	sessions.clear();
	ios.stop();
}

int Libasio::addConf(const ASIOModeConf& conf)
{
	int ret{0 < ios.size() ? Error_Code_Success : Error_Code_Operate_Failure};

	if (Error_Code_Success == ret)
	{
		if (ASIOProtoType::ASIO_PROTO_TYPE_TCP == conf.proto)
		{
			if (ASIOModeType::ASIO_MODE_TYPE_LISTEN == conf.tcp.mode)
			{
				boost::shared_ptr<Acceptor> acceptor{
					boost::make_shared<Acceptor>(
						ios,
						[&](boost::asio::ip::tcp::socket* so, const int32_t e)
						{
							uint32_t sid{
								afterFetchIOAcceptedEventNotification(
									so->remote_endpoint().address().to_string().c_str(), 
									so->remote_endpoint().port(), 
									e)};

							if (0 < sid)
							{
								int status{Error_Code_Success};
								SessionPtr sp{boost::make_shared<TcpSession>(so, sid)};
								if (sp)
								{
									status = sp->createNew(
										boost::bind(&Libasio::afterPolledIOSendDataNotification, this, _1, _2, _3),
										boost::bind(&Libasio::afterPolledIOReadDataNotification, this, _1, _2, _3, _4));
								}

								if (!sp)
								{
									so->close();
									boost::checked_delete(so);
								}
								else if (Error_Code_Success == status)
								{
									sp->receive();
									sessions.add(sid, sp);
								}
							}
							else
							{
								if (so)
								{
									so->close();
									boost::checked_delete(so);
								}
							}
						},
						conf.port)};

				if (acceptor)
				{
					for (int i = 0; i != ios.size(); ++i)
					{
						acceptor->listen();
					}
				}
			}
			else if (ASIOModeType::ASIO_MODE_TYPE_CONNECT == conf.tcp.mode)
			{
				ConnectorPtr connector{
					boost::make_shared<Connector>(
						ios, 
						[&](boost::asio::ip::tcp::socket* so, const int32_t e)
						{
							const uint32_t sid{afterFetchIOConnectedEventNotification(e)};

							if (0 < sid)
							{
								int status{Error_Code_Success};
								SessionPtr sp{boost::make_shared<TcpSession>(so, sid)};
								if (sp)
								{
									status = sp->createNew(
										boost::bind(&Libasio::afterPolledIOSendDataNotification, this, _1, _2, _3),
										boost::bind(&Libasio::afterPolledIOReadDataNotification, this, _1, _2, _3, _4));
								}

								if (!sp)
								{
									so->close();
									boost::checked_delete(so);
								}
								else if (Error_Code_Success == status)
								{
									sp->receive();
									sessions.add(sid, sp);
								}
							}
						})};

				if (connector)
				{
					connector->connect(conf.tcp.ip, conf.port);
				}
			}
			else
			{
				ret = Error_Code_Method_Not_Support;
			}
		}
		else if (ASIOProtoType::ASIO_PROTO_TYPE_UDP == conf.proto)
		{
			boost::asio::ip::udp::socket* so{
				new(std::nothrow) boost::asio::ip::udp::socket{
					*ios.ctx(), 
					boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), conf.port)}};

			if (so)
			{
				int status{Error_Code_Success};
				SessionPtr sp{boost::make_shared<UdpSession>(so, conf.udp.sid)};
				if (sp)
				{
					status = sp->createNew(
						boost::bind(&Libasio::afterPolledIOSendDataNotification, this, _1, _2, _3),
						boost::bind(&Libasio::afterPolledIOReadDataNotification, this, _1, _2, _3, _4));
				}

				if (!sp)
				{
					so->close();
					boost::checked_delete(so);
				}
				else if (Error_Code_Success == status)
				{
					sp->receive();
					sessions.add(conf.udp.sid, sp);
				}
			}
			else
			{
				ret = Error_Code_Bad_New_Socket;
			}
		}
		else
		{
			ret = Error_Code_Method_Not_Support;
		}
	}

	return ret;
}

int Libasio::removeConf(const uint32_t id/* = 0*/)
{
	int ret{0 < id ? Error_Code_Success : Error_Code_Operate_Failure};

	if (Error_Code_Success == ret)
	{
		SessionPtr sp{sessions.at(id)};

		if (sp)
		{
			ret = sp->destroy();
			sessions.remove(id);
		}
		else
		{
			ret = Error_Code_Object_Not_Exist;
		}
	}

	return ret;
}

int Libasio::send(
	const uint32_t id/* = 0*/, 
	const void* data/* = nullptr*/, 
	const uint64_t bytes/* = 0*/, 
	const char* ip/* = nullptr*/, 
	const uint16_t port/* = 0*/)
{
	int ret{0 < id && data && 0 < bytes ? Error_Code_Success : Error_Code_Invalid_Param};

	if(Error_Code_Success == ret)
	{
		SessionPtr sp{sessions.at(id)};

		if (sp)
		{
			ret = sp->send(data, bytes, ip, port);
		}
		else
		{
			ret = Error_Code_Object_Not_Exist;
		}
	}

	return ret;
}
