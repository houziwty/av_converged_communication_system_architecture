#include <string>
#include <sstream>
#include "boost/archive/iterators/base64_from_binary.hpp"
#include "boost/archive/iterators/binary_from_base64.hpp"
#include "boost/archive/iterators/transform_width.hpp"
#include "boost/checked_delete.hpp"
#include "error_code.h"
#include "memory/xmem.h"
using namespace framework::utils::memory;
#include "base64/encoder.h"
using namespace framework::codec::base64;

Base64Encoder::Base64Encoder() : out{nullptr}
{}

Base64Encoder::~Base64Encoder()
{
	boost::checked_array_delete(out);
}

const char* Base64Encoder::encode(const char* data /* = nullptr */)
{
	//输出重置
	boost::checked_array_delete(out);
	out = nullptr;

	if (data)
	{
		const std::string i{data};
		std::stringstream o;
		typedef boost::archive::iterators::base64_from_binary<
			boost::archive::iterators::transform_width<std::string::const_iterator, 6, 8>> Base64EncodeIterator;
		std::copy(Base64EncodeIterator(i.begin()), Base64EncodeIterator(i.end()), std::ostream_iterator<char>(o));
		std::size_t equal_count = (3 - i.length() % 3) % 3;
		for (size_t i = 0; i < equal_count; i++)
		{
			o.put('=');
		}
		o.put('\0');

		const std::string base64String{o.str()};
		out = reinterpret_cast<char*>(XMem().alloc(base64String.c_str(), base64String.length()));
	}

	return out;
}
