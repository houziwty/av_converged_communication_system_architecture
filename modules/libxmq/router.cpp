#include "boost/format.hpp"
#include "zmq.h"
#include "socket_base.hpp"
#include "Error.h"
#include "Define.h"
#include "Network/ZMQ/Msg_.h"
#include "Network/ZMQ/Core/Router_.h"

namespace framework
{
    namespace network
    {
        namespace zeromq
        {
			Router::Router()
			{}
			Router::~Router()
			{}

			int Router::bind(
				void* s /* = nullptr */, 
				const unsigned short port /* = 0 */)
			{
				int e{ 
					s && gMinPortNumber < port && gMaxPortNumber > port ? eSuccess : eInvalidParameter };

				if (eSuccess == e)
				{
					zmq::socket_base_t* so{ reinterpret_cast<zmq::socket_base_t*>(s) };
					e = so->bind((boost::format("tcp://*:%d") % port).str().c_str()) ? eBadBind : eSuccess;
				}

				return e;
			}

			int Router::send(
				void* s /* = nullptr */, 
				Msg* msg /* = nullptr */)
			{
				return msg ? msg->sendData(s) : eInvalidParameter;
			}
        }//namespace zeromq
    }//namespace network
}//namespace framework

