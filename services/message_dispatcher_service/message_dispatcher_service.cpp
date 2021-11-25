#include "message_dispatcher_service.h"

MessageDispatcher::MessageDispatcher() 
	: SwitcherPub()
{}

MessageDispatcher::~MessageDispatcher()
{}

void MessageDispatcher::afterSwitcherPolledDataHandler(const std::string uid, const std::string data)
{
}
