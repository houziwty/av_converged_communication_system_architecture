#include "videostream.h"
#include <memory.h>

int videoStream::run()
{
    return  ASIONode::run();
}

int videoStream::stop()
{
    return  ASIONode::stop();
}


bool videoStream::videoInit(char *configData)
{

}

int videoStream::send(const uint32_t id, const void *data, const uint64_t bytes, const char *ip, const uint16_t port)
{
    return  ASIONode::send(id,data,bytes,ip,port);
}


uint32_t videoStream::afterFetchAcceptedEventNotification(const char* ip ,const uint16_t port, const int32_t e )
{
    //        LOG(INFO) << "AiMoude Init Error";
}

void videoStream::afterPolledReadDataNotification(const uint32_t id, const void *data, const uint64_t bytes, const int32_t e)
{
    cout << "Recv: "<< id<<endl;

    if(e!=0)
    {
        cout << "streamData Error"<<endl;
        return;
    }
    static bool first{ true };
    if (first)
    {
        AVModeConf conf{ 41, AVModeType::AV_MODE_TYPE_GRAB_BRG24, nullptr};
        conf.callback = std::bind(&videoStream::avframeDataCallback,this,std::placeholders::_1);
        AVNode::addConf(conf);
        first = false;
    }
    AVPkt avpkt;
    avpkt.input(data, bytes);
    AVNode::input(41, &avpkt);
}

void videoStream::afterPolledSendDataNotification(const uint32_t id, const uint64_t bytes, const int32_t e)
{

}
uint32_t videoStream::afterFetchConnectedEventNotification(const int32_t e )
{
    cout << "Connection : "<<e<<endl;

    if(e==0){
        streamId ++;
        streamIdList.push(streamId);
        return streamId;
    }
    else{
        return 0;
    }
}
bool videoStream::openStream()
{
    const std::string url{ "realplay://1?command=1&channel=1&stream=0" };
    const uint64_t bytes{ 32 + url.length() };
    char* data{ new char[bytes] };
    *((uint32_t*)data) = 0xFF050301;
    *((uint32_t*)(data + 4)) = (uint32_t)AVMainType::AV_MAIN_TYPE_NONE;
    *((uint32_t*)(data + 8)) = (uint32_t)AVSubType::AV_SUB_TYPE_NONE;
    *((uint32_t*)(data + 12)) = url.length();
    *((uint32_t*)(data + 16)) = 0;
    *((uint64_t*)(data + 24)) = 0;

    memcpy(data + 32, url.c_str(), url.length());
    if(streamIdList.empty())
    {
        cout << "Not  streamId"<<endl;
        return false;
    }
    auto Id = streamIdList.top();
    cout << "OpenId:"<<Id<<endl;
    auto ret = send(Id, data, bytes);

    printf("sendRet: %d\n",ret);
    streamIdList.pop();

    return true;

}
void videoStream::avframeDataCallback(const void *videoData)
{
    cout<< "videoData:"<<endl;
}
