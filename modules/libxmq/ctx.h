//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@hotmail.com
//		Date :							2021-11-15
//		Description :					XMQ上下文类
//
//		History:						Author										Date											Description
//										王科威										 2021-11-15										 创建
//

#ifndef NETWORK_XMQ_CTX_H
#define NETWORK_XMQ_CTX_H

namespace network
{
    namespace xmq
    {
		typedef void ctx_t;

		class Ctx
		{
		public:
			Ctx(void);
			~Ctx(void);

		public:
			//创建
			//@Return : 上下文实例
			ctx_t* createNew(void);

			//销毁
			//@c : 上下文实例
			int destroy(ctx_t* c = nullptr);
		};//class Ctx
    }//namespace xmq
}//namespace network

#endif//NETWORK_XMQ_CTX_H
