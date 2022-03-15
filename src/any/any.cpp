#include "boost/checked_delete.hpp"
#include "error_code.h"
#include "memory/xmem.h"
using namespace framework::utils::memory;
#include "any/any.h"
using namespace framework::utils::data;

Any::Any() : _data{nullptr}, _bytes{0}
{}

Any::~Any()
{
    boost::checked_array_delete((uint8_t*)_data);
}

int Any::input(const void* data/* = nullptr*/, const uint64_t bytes/* = 0*/)
{
    _data = XMem().alloc(data, bytes);
    if (_data)
    {
        _bytes = bytes;
    }
    
    return _data ? Error_Code_Success : Error_Code_Bad_New_Memory;
}