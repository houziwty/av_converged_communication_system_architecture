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

#include "boost/shared_ptr.hpp"
#include "log.h"
using namespace module::file::log;

using BufferParserPtr = boost::shared_ptr<BufferParser>;

class DvsStreamSession
{
public:
    //id [in] : 会话ID
    DvsStreamSession(FileLog& log, const uint32_t id = 0);
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
	void getIDs(uint32_t& sid, uint32_t& did, uint32_t& cid);

private:
    //解析单帧数据回调
    //@dataType [out] : 数据类型
    //@streamType [out] : 流类型
    //@frameType [out] : 帧类型
    //@frameBytes [out] : 帧大小
    //@frameSeq [out] : 帧序号
    //@frameTs [out] : 帧时间戳
    //@frameData [out] : 帧数据
    void afterParsedOneFrameNotification(
        const uint32_t dataType = 0, 
        const uint32_t streamType = 0, 
        const uint32_t frameType = 0, 
        const uint32_t frameBytes = 0, 
        const uint64_t frameSeq = 0, 
        const uint64_t frameTs = 0, 
        const void* frameData = nullptr);

private:
    FileLog& fileLog;
    const uint32_t sid;
    uint32_t did;
    uint32_t cid;
};//class DvsStreamSession

#endif//SERVICE_DVS_STREAM_SESSION_H