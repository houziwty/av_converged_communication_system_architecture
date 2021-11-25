#ifndef WINDOWS
#include "sdk/hikvision/include/linux/HCNetSDK.h"
#else
#include "sdk/hikvision/include/win32/HCNetSDK.h"
#endif//WINDOWS
#include "error_code.h"
#include "avcap/sdk/hk_dvs_capture.h"
using namespace framework::media::av;

HKSdkCapture::HKSdkCapture() : user{-1}
{}

HKSdkCapture::~HKSdkCapture()
{}

int HKSdkCapture::createNew()
{
    NET_DVR_Init();

    return Error_Code_Success;
}

int HKSdkCapture::destroy()
{
    NET_DVR_Cleanup();

    return Error_Code_Success;
}
