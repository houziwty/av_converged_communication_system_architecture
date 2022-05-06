#include "server.h"
#include "session.h"

Session::Session(Server& svr, const uint32_t sid/* = 0*/) 
    : server{svr}, sessionid{sid}, sequence{0}
{}

Session::~Session()
{}
