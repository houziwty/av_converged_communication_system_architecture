#ifndef VIDEOSTREAM_H
#define VIDEOSTREAM_H
#include <stack>
#include "asio_node.h"
#include "av_node.h"
#include "av_pkt.h"
#include <iostream>
#include <string>

using namespace module::network:: asio;
using  namespace module::av::stream;
using  namespace std;
class videoStream : public ASIONode, protected AVNode
{
public:
    videoStream() {};
    ~videoStream(){};
public:
    bool openStream();
    bool videoInit(char *configData);
public:
    int run(void) override;
    int stop(void)override;

    int send(const uint32_t id = 0,
             const void* data = nullptr,
             const uint64_t bytes = 0,
             const char* ip = nullptr,
             const uint16_t port = 0)override;

    uint32_t afterFetchAcceptedEventNotification(
            const char* ip = nullptr,
            const uint16_t port = 0,
            const int32_t e = 0)override;

    uint32_t afterFetchConnectedEventNotification(const int32_t e = 0) override;

    void afterPolledReadDataNotification(
            const uint32_t id = 0,
            const void* data = nullptr,
            const uint64_t bytes = 0,
            const int32_t e = 0)override;

    void afterPolledSendDataNotification(
            const uint32_t id = 0,
            const uint64_t bytes = 0,
            const int32_t e = 0)override;
private:
    void avframeDataCallback(const void* videoData);
private:
    int streamId = 0;
    stack<int> streamIdList;

};

#endif // VIDEOSTREAM_H
