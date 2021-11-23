#include "MessageDistribute.h"

MessageDistribute::MessageDistribute() 
	: SwitcherPub()
{}

MessageDistribute::~MessageDistribute()
{}

void MessageDistribute::afterSwitcherPolledDataHandler(const std::string uid, const std::string data)
{
}
