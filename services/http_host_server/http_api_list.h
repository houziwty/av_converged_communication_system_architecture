//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2022-05-25
//		Description : HTTP API接口
//
//		History:
//					1. 2022-05-25 由王科威创建
//

#ifndef SERVICE_HTTP_API_LIST_H
#define SERVICE_HTTP_API_LIST_H

#include <vector>
#include <string>

static const std::vector<const std::string> httpAPIs
{
    //系统接口
    "/api/v1/getapilist",//获取API列表
    "/api/v1/login",//用户登录
    "/api/v1/logout",//用户登出
    "/api/v1/getuserinfo",//获取当前登录用户信息
    "/api/v1/modifypassword",//修改登录密码
    "/api/v1/getserverinfo",//获取平台运行信息
    "/api/v1/restart",//重启服务
    //用户管理
    "/api/v1/user/list",//获取用户列表
    "/api/v1/user/info",//获取用户信息
    "/api/v1/user/add",//添加用户
    "/api/v1/user/remove",//添加删除
    "/api/v1/user/resetpassword",//重置用户密码
    "/api/v1/user/setenable",//设置用户启用状态
    "/api/v1/user/sethasallchannels",//设置用户关联所有通道
    "/api/v1/user/sethaschannels",//设置用户关联通道
    "/api/v1/user/removechannels",//删除用户关联通道
    "/api/v1/user/channellist",//获取用户关联通道
    //设备管理
    "/api/v1/device/list",//查询DVS列表
    "/api/v1/device/info",//查询单个DVS信息
    "/api/v1/device/add",//新增DVS设备
    "/api/v1/device/remove"//删除DVS设备
};

#endif//SERVICE_HTTP_API_LIST_H