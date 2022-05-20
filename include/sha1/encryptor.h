//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2022-05-20
//		Description : base64加密器
//
//		History: 2022-05-20 由王科威创建
//

#ifndef FRAMEWORK_ENCRYPT_SHA1_SHA1_ENCRYPTOR_H
#define FRAMEWORK_ENCRYPT_SHA1_SHA1_ENCRYPTOR_H

namespace framework
{
	namespace encrypt
	{
		namespace sha1
		{
			class Sha1Encryptor
			{
			public:
				Sha1Encryptor(void);
				~Sha1Encryptor(void);

			public:
				//SHA1加密
				//@data [in] : 原始数据
				//@Return : 加密数据
				const char* encrypt(const char* data = nullptr);

			private:
				char* out;
			};//class Sha1Encryptor
		}//namespace sha1
	}//namespace encrypt
}//namespace framework

#endif//FRAMEWORK_ENCRYPT_SHA1_SHA1_ENCRYPTOR_H
