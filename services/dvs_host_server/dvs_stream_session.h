//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2022-02-11
//		Description : DVS流会话
//
//		History:
//					1. 2022-02-11 由王科威创建
//

#ifndef SERVICE_DVS_STREAM_SESSION_H
#define SERVICE_DVS_STREAM_SESSION_H

#include "xmq_node.h"
using namespace module::network::xmq;
#include "av_parser_node.h"
using namespace module::av::stream;

class DvsHostServer;

class DvsStreamSession : protected AVParserNode
{
public:
    DvsStreamSession(
        XMQNode& node, 
        const XMQModeConf& conf, 
        const uint32_t id = 0);
    ~DvsStreamSession(void);

public:
    //接收数据
    //@data [in] : 数据
    //@bytes [in] : 数据大小
    //@Return ：错误码
	int recv(
        const void* data = nullptr, 
        const uint64_t bytes = 0);

    //获取会话、设备和通道ID
    //@data [out] : 会话ID
    //@did [out] : 设备ID
    //@cid [out] : 通道ID
    //@fid [out] : 帧ID
	void getIDs(
        uint32_t& sid, 
        uint32_t& did, 
        uint32_t& cid, 
        uint64_t& fid);

private:
    //解析单帧数据回调
    //@dataType [out] : 数据类型
    //@frameData [out] : 帧数据
    void afterParsedDataNotification(
        const uint32_t id = 0, 
        const AVPkt* avpkt = nullptr);

private:
    XMQNode& xmqNode;
    const XMQModeConf& modeconf;
    const uint32_t sid;
    uint32_t did;
    uint32_t cid;
    uint64_t fid;
};//class DvsStreamSession

#endif//SERVICE_DVS_STREAM_SESSION_H