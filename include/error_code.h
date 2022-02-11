//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@hotmail.com
//		Date :							2021-11-15
//		Description :					错误码
//
//		History:						Author										Date											Description
//										王科威										 2021-11-15										 创建
//

#ifndef GLOBAL_ERROR_CODE_H
#define GLOBAL_ERROR_CODE_H

//通用
static const int Error_Code_Success = 0;
static const int Error_Code_Invalid_Param = -1;
static const int Error_Code_Bad_New_Object = -2;
static const int Error_Code_Bad_New_Thread = -3;
static const int Error_Code_Bad_New_Memory = -4;
static const int Error_Code_Operate_Out_Of_Range = -5;
static const int Error_Code_Operate_Not_Support = -6;
static const int Error_Code_Operate_Failure = -7;
static const int Error_Code_Object_Existed = -8;
static const int Error_Code_Object_Not_Exist = -9;
static const int Error_Code_Method_Not_Support = -10;

//网络操作
static const int Error_Code_Bad_New_Socket = -1000;
static const int Error_Code_Bad_Operate_Bind = -1001;
static const int Error_Code_Bad_Operate_Connect = -1002;
static const int Error_Code_Bad_Operate_Term = -1003;
static const int Error_Code_Bad_Operate_Send = -1004;
static const int Error_Code_Bad_Operate_Recv = -1005;
static const int Error_Code_Bad_RequestUrl = -1006;

//SDK
static const int Error_Code_Init_Sdk_Failure = -2000;
static const int Error_Code_Uninit_Sdk_Failure = -2001;
static const int Error_Code_Device_Login_Failure = -2002;
static const int Error_Code_Device_Logout_Failure = -2003;
static const int Error_Code_Stream_Open_Failure = -2004;
static const int Error_Code_Stream_Close_Failure = -2005;

//解析
static const int Error_Code_Invalid_Flag = -3000;
static const int Error_Code_Out_Of_Range = -3001;

#endif//GLOBAL_ERROR_CODE_H
