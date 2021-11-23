//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2021-11-18
//		Description : 消息分发服务类
//
//		History:
//					1. 2021-11-18 由王科威创建
//

#ifndef MESSAGE_DISTRIBUTE_SERVICE_H
#define MESSAGE_DISTRIBUTE_SERVICE_H

#include "network/mq/switcher_pub.h"
using namespace framework::network::mq;

class MessageDistribute final : public SwitcherPub
{
public:
	MessageDistribute(void);
	~MessageDistribute(void);

protected:
	//交换模型数据接收回调函数
	//@uid [out] : 用户ID标识
	//@data [out] : 数据
	void afterSwitcherPolledDataHandler(const std::string uid, const std::string data) override;
};//class MessageDistribute

#endif//MESSAGE_DISTRIBUTE_SERVICE_H
