#include <string>
#include <sstream>
#include "boost/archive/iterators/base64_from_binary.hpp"
#include "boost/archive/iterators/binary_from_base64.hpp"
#include "boost/archive/iterators/transform_width.hpp"
//#include "error_code.h"
#include "codec/base64_encode.h"
using namespace framework::codec;

Base64Encode::Base64Encode()
{}

Base64Encode::~Base64Encode()
{}

int Base64Encode::encode(
	const char* data /* = nullptr */, 
	const int bytes /* = 0 */)
{
	int ret{ data && 0 < bytes ? 0 : -1 };

	if (!ret)
	{
		typedef boost::archive::iterators::base64_from_binary<
			boost::archive::iterators::transform_width<std::string::const_iterator, 6, 8>> Base64EncodeIterator;
		std::string inputData((const char*)data, bytes);
		std::stringstream outputData;
		copy(Base64EncodeIterator(inputData.begin()), Base64EncodeIterator(inputData.end()), std::ostream_iterator<char>(outputData));
		size_t equal_count = (3 - inputData.length() % 3) % 3;
		for (size_t i = 0; i < equal_count; i++)
		{
			outputData.put('=');
		}
		outputData.put('\0');

		const std::string s{ outputData.str() };

		FILE* fd{nullptr};
		if(!fd)
		{
			fd = fopen("/mnt/build/test.jpg", "wb+");
		}
		fwrite(data, bytes, 1, fd);
		fclose(fd);
	}

	return ret;
}
