#include "boost/checked_delete.hpp"
#include "error_code.h"
#include "utils/memory/xmem.h"
using namespace framework::utils::memory;
#include "network/buffer/buffer_parser.h"
using namespace framework::network::buffer;

BufferParser::BufferParser(
	ParsedOneFrameDataCallback callback, 
	const uint64_t bytes/* = 3 * 1024 * 1024*/)
	: parsedOneFrameDataCallback{ callback }, bufSize{ bytes }, pos{ 0 }, buffer{nullptr}
{
	buffer = new(std::nothrow) uint8_t[bufSize];
}

BufferParser::~BufferParser()
{
	boost::checked_array_delete(buffer);
}

int BufferParser::append(
	const uint8_t* data/* = nullptr*/, 
	const uint64_t bytes/* = 0*/)
{
	int ret{data && 0 < bytes ? Error_Code_Success : Error_Code_Invalid_Param};

	if (Error_Code_Success == ret)
	{
		//从数据缓存第一个字节开始解析
		//首先检查数据头是否是帧头标志
		//不要一开始就拷贝数据
		if (0 == pos)
		{
			const uint32_t* flag{ (const uint32_t*)data };
			if (frame_header_flag != *flag)
			{
				return Error_Code_Invalid_Flag;
			}
		}

		//如果不是从数据缓存第一个字节开始解析
		//检测游标是否超过缓存大小
		//如果超界就重置游标到缓存第一个字节以保证后续数据正常添加到缓存
		if (pos >= bufSize)
		{
			pos = 0;
			return Error_Code_Out_Of_Range;
		}

		//检测本次添加到数据是全部还是部分放入缓存
		int buffer_left_bytes{ 0 }, data_left_bytes{ 0 };
		if (bytes + pos > bufSize)
		{
			buffer_left_bytes = bufSize - pos;
			data_left_bytes = bytes - buffer_left_bytes;
			XMem().copy(data, buffer_left_bytes, buffer + pos, buffer_left_bytes);
			pos += buffer_left_bytes;
		}
		else
		{
			XMem().copy(data, bytes, buffer + pos, bytes);
			pos += bytes;
		}

		ret = parse();

		//如果本次添加数据是部分放入缓存就将剩余部分追加入缓存
		if (!ret && 0 < buffer_left_bytes && 0 < data_left_bytes)
		{
			XMem().copy(data + buffer_left_bytes, data_left_bytes, buffer + pos, data_left_bytes);
			pos += data_left_bytes;
		}
	}

	return ret;
}

int BufferParser::parse()
{
	int current_pos{ 0 }, ret{ 0 };

	//解析当前段内所有的帧数据
	while (current_pos < pos)
	{
		//剩余数据不够帧头大小就只将剩余数据移动到缓存第一个字节
		if (pos - current_pos <= frame_header_size)
		{
			pos -= current_pos;
			return XMem().move(buffer + current_pos, buffer, pos);
		}

		//检查数据头是否是帧头标志
		//如果不是帧标志就表明数据有错
		//重置游标到缓存第一个字节以保证后续数据正常添加到缓存
		const uint32_t* flag{ (const uint32_t*)(buffer + current_pos) };
		if (frame_header_flag != *flag)
		{
			pos = 0;
			return Error_Code_Invalid_Flag;
		}

		const uint32_t* data_type{ (const uint32_t*)(buffer + current_pos + 4) };
		const uint32_t* stream_type{ (const uint32_t*)(buffer + current_pos + 8) };
		const uint32_t* frame_type{ (const uint32_t*)(buffer + current_pos + 12) };
		const uint32_t* frame_size{ (const uint32_t*)(buffer + current_pos + 16) };
		const uint64_t* sequence{ (const uint64_t*)(buffer + current_pos + 20) };
		const uint64_t* timestamp{ (const uint64_t*)(buffer + current_pos + 28) };

		//检查剩余数据是否不足帧数据大小
		//如果数据不足则将剩余数据移动到缓存第一个字节
		if (pos - current_pos - frame_header_size < *frame_size)
		{
			pos -= current_pos;
			XMem().move(buffer + current_pos, buffer, pos);
			return Error_Code_Success;
		}

		if (parsedOneFrameDataCallback)
		{
			parsedOneFrameDataCallback(
				*data_type, *stream_type, *frame_type, *frame_size, *sequence, *timestamp, buffer + current_pos + frame_header_size);
		}
		current_pos += (frame_header_size + *frame_size);
	}

	pos -= current_pos;
	return Error_Code_Success;
}
