#include "boost/compute/detail/sha1.hpp"
#include "boost/checked_delete.hpp"
#include "error_code.h"
#include "memory/xmem.h"
using namespace framework::utils::memory;
#include "sha1/encryptor.h"
using namespace framework::encrypt::sha1;

Sha1Encryptor::Sha1Encryptor() : out{nullptr}
{}

Sha1Encryptor::~Sha1Encryptor()
{
	boost::checked_array_delete(out);
}

const char* Sha1Encryptor::encrypt(const char* data /* = nullptr */)
{
	//输出重置
	boost::checked_array_delete(out);
	out = nullptr;

	if (data)
	{
		const std::string sha1String{
			boost::compute::detail::sha1(
				std::string(data) + "258EAFA5-E914-47DA-95CA-C5AB0DC85B11")};
		out = reinterpret_cast<char*>(XMem().alloc(sha1String.c_str(), sha1String.length()));
	}

	return out;
}
