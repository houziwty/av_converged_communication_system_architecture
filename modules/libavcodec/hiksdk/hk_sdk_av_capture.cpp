#ifndef WINDOWS
#include "sdk/hikvision/include/linux/HCNetSDK.h"
#else
#include "sdk/hikvision/include/win32/HCNetSDK.h"
#endif//WINDOWS
#include "error_code.h"
#include "hk_sdk_av_capture.h"
using namespace module::media::av;

int HKSdkAVCapture::counter = 0;

HKSdkAVCapture::HKSdkAVCapture() : AVProcessor(), user{-1}, playId{Error_Code_Bad_New_Object}
{}

HKSdkAVCapture::~HKSdkAVCapture()
{
    destroy();
}

int HKSdkAVCapture::createNew(void* param/* = nullptr*/)
{
    if(0 == counter++)
    {
        NET_DVR_Init();
    }

    LoginInfo* info{reinterpret_cast<LoginInfo*>(param)};
    NET_DVR_DEVICEINFO_V30 struDeviceInfo{ 0 };
    user = NET_DVR_Login_V30(
        (char*)info.ip.c_str(), info.port, (char*)info.username.c_str(), (char*)info.passwd.c_str(), &struDeviceInfo);

    if(-1 < user)
    {
        NET_DVR_PREVIEWINFO struPlayInfo = { 0 };
        struPlayInfo.lChannel = struDeviceInfo.byStartChan;
        struPlayInfo.byProtoType = 1;
        struPlayInfo.bBlocked = 1;
        playid = NET_DVR_RealPlay_V40(user, &struPlayInfo, &HKSdkAVCapture::captureRealplayDataCallback, this);
    }

    return playid;
}

int HKSdkAVCapture::destroy()
{
    if(-1 < playId)
    {
        NET_DVR_StopRealPlay(playId);
    }

    if(-1 < user)
    {
        NET_DVR_Logout(user);
    }

    if(0 == --counter)
    {
        NET_DVR_Cleanup();
    }

    return Error_Code_Success;
}

void HKSdkAVCapture::captureRealplayDataCallback(
    int playid, unsigned long long type, char* data, unsigned long long bytes, void* ctx)
{

}
