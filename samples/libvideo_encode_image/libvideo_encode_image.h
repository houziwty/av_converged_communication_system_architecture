//
//		Copyright :						@2021, 车路通, All Rights Reserved
//
//		Author :						  王科威
//		E-mail :						   wangkw531@hotmail.com
//		Date :							   2021-11-05
//		Description :					海康DVS设备视频PS流编码JPEG库
//                                            1.该库通过读取配置文件获取登录设备信息；
//                                            2.自动获取设备和通道信息；
//                                            3.输出解码后的YUV数据和JPEG图片；
//
//		History:						Author									Date									    Description
//										    王科威									 2021-11-05								 创建
//

#ifndef LIB_VIDEO_ENCODE_IMAGE_H
#define LIB_VIDEO_ENCODE_IMAGE_H

#include <string>

#define CLT_LIB_API extern "C"

typedef struct tagOutputFrameData_t
{
    void* bgr24;
    int bgr24bytes;
    void* jpeg;
    int jpegbytes;
    int width;
    int height;
    int sequence;
    double timestamp;
}OutputFrameData;

//启动
//@Return : 错误码
//                0 - 成功
//                -1 - 获取配置信息失败
CLT_LIB_API int CLT_LIB_VideoEncodeImage_Start(void);

//停止
//@Return : 错误码
//          0 - 成功
//          -1 - 获取配置信息失败
CLT_LIB_API int CLT_LIB_VideoEncodeImage_Stop(void);

//添加设备
//@username [in] : 用户名
//@passwd [in] : 密码
//@ip [in] : 设备IP
//@port [in] : 端口号
//@Return : 设备标识，>0
CLT_LIB_API int CLT_LIB_VideoEncodeImage_AddDevice(
    const std::string username, 
    const std::string passwd, 
    const std::string ip, 
    const unsigned short port);

//删除设备
//@did [in] : 设备标识，由CLT_LIB_VideoEncodeImage_AddDevice返回
//@Return : 错误码
//          0 - 成功
//          -1 - 失败
CLT_LIB_API int CLT_LIB_VideoEncodeImage_RemoveDevice(const int did = -1);

//获取一帧数据
//@did [in] : 设备标识，由CLT_LIB_VideoEncodeImage_AddDevice返回
//@fd [out] : 数据
//@Return : 错误码
//          0 - 成功
//          -1 - 失败
CLT_LIB_API int CLT_LIB_VideoEncodeImage_GetFrame(
    const int did, 
    OutputFrameData*& ofd);

#endif//LIB_VIDEO_ENCODE_IMAGE_H