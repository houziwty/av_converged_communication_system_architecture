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

#ifndef FRAMEWORK_CODEC_BASE64_ENCODE_H
#define FRAMEWORK_CODEC_BASE64_ENCODE_H

namespace framework
{
	namespace codec
	{
		class Base64Encode
		{
		public:
			Base64Encode(void);
			~Base64Encode(void);

		public:
			int encode(
				const char* data = nullptr,
				const int bytes = 0);
		};//class Base64Encode
	}//namespace codec
}//namespace framework

#endif//FRAMEWORK_CODEC_BASE64_ENCODE_H
