#include "device.h"
using namespace module::device::dvs;

Device::Device(const DVSModeConf& conf, PolledDataCallback callback) 
    : modeconf{conf}, polledDataCallback{callback}
{}

Device::~Device()
{
    stop();
}
