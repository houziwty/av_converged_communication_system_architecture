//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@icloud.com
//		Date : 2021-11-15
//		Description : 异步IO上下文
//
//		History: 2021-11-15 由王科威创建
//

#ifndef MODULE_NETWORK_AIO_CTX_H
#define MODULE_NETWORK_AIO_CTX_H

#include "boost/asio.hpp"
using io_ctxs = std::vector<boost::asio::io_context>;
using io_works = std::vector<boost::asio::io_service::work>;

namespace module
{
	namespace network
	{
		namespace aio
		{
			class Ctx
			{
			public:
				Ctx(void);
				~Ctx(void);

			public:
				//初始化
				//@Return : 错误码
				int init(void);

				//释放
				//@Return : 错误码
				int uninit(void);

				//获取空闲IO
				//@Return : 空闲IO
				void* io(void);

			private:
				ICtx* ctx;
			};//class Ctx
		}//namespace aio
	}//namespace network
}//namespace module

#endif//MODULE_NETWORK_AIO_CTX_H
