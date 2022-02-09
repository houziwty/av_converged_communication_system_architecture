#include "device.h"
using namespace module::device::dvs;

Device::Device(const DVSModeConf& conf, PolledRealplayDataCallback callback) 
    : modeconf{conf}, polledRealplayDataCallback{callback}
{}

Device::~Device()
{}
