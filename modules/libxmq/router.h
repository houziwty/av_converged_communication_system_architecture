//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@hotmail.com
//		Date :							2021-11-15
//		Description :					XMQ路由模型类
//
//		History:						Author										Date											Description
//										王科威										 2021-11-15										 创建
//

#ifndef NETWORK_XMQ_ROUTER_H
#define NETWORK_XMQ_ROUTER_H

namespace network
{
    namespace xmq
    {
		namespace zeromq
		{
			class Msg;

			class Router
			{
			public:
				Router(void);
				virtual ~Router(void);

			public:
				//�󶨼����˿ں�
				//@s : Socketʵ��
				//@port : �����˿ں�
				//@Return : ������
				virtual int bind(
					void* s = nullptr, 
					const unsigned short port = 0);

				//������Ϣ
				//@s : Socketʵ��
				//@msg : ��Ϣ����
				//@Return : ������
				virtual int send(
					void* s = nullptr,
					Msg* msg = nullptr);
			};//class Router
		}//namespace zeromq
    }//namespace network
}//namespace framework

#endif//NETWORK_XMQ_ROUTER_H
