#include "session/session.h"
using namespace framework::network::session;

Session::Session(const uint32_t id/* = 0*/) : sessionid{id}
{}

Session::~Session()
{}
