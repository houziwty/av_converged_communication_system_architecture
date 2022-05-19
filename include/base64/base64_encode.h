//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2021-11-15
//		Description : Base64加密算法
//
//		History: 2021-11-15 由王科威创建
//

#ifndef FRAMEWORK_ENCRYPT_CODEC_BASE64_ENCODE_H
#define FRAMEWORK_ENCRYPT_CODEC_BASE64_ENCODE_H

namespace framework
{
	namespace encrypt
	{
		namespace codec
		{
			class Base64Encode
			{
			public:
				Base64Encode(void);
				~Base64Encode(void);

			public:
				//Base64编码
				//@data [in] : 原始数据
				//@Return : 编码数据
				const char* encode(const char* data = nullptr);

			private:
				char* base64;
			};//class Base64Encode
		}//namespace codec
	}//namespace encrypt
}//namespace framework

#endif//FRAMEWORK_ENCRYPT_CODEC_BASE64_ENCODE_H
