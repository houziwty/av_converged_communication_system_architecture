//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2022-05-20
//		Description : base64编码器
//
//		History: 2022-05-20 由王科威创建
//

#ifndef FRAMEWORK_CODEC_BASE64_BASE64_ENCODE_H
#define FRAMEWORK_CODEC_BASE64_BASE64_ENCODE_H

namespace framework
{
	namespace codec
	{
		namespace base64
		{
			class Base64Encoder
			{
			public:
				Base64Encoder(void);
				~Base64Encoder(void);

			public:
				//base64编码
				//@data [in] : 原始数据
				//@Return : 编码数据
				const char* encode(const char* data = nullptr);

			private:
				char* out;
			};//class Base64Encoder
		}//namespace base64
	}//namespace codec
}//namespace framework

#endif//FRAMEWORK_CODEC_BASE64_BASE64_ENCODE_H
