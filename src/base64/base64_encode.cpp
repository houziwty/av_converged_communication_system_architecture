#include <string>
#include <sstream>
#include "boost/archive/iterators/base64_from_binary.hpp"
#include "boost/archive/iterators/binary_from_base64.hpp"
#include "boost/archive/iterators/transform_width.hpp"
#include "boost/checked_delete.hpp"
#include "error_code.h"
#include "memory/xmem.h"
using namespace framework::utils::memory;
#include "base64/base64_encode.h"
using namespace framework::encrypt::codec;

Base64Encode::Base64Encode() : base64{nullptr}
{}

Base64Encode::~Base64Encode()
{
	boost::checked_array_delete(base64);
}

const char* Base64Encode::encode(const char* data /* = nullptr */)
{
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
		const std::size_t base64Length{base64String.length()};
		base64 = reinterpret_cast<char*>(XMem().alloc(base64String.c_str(), base64Length));
	}

	return base64;
}
