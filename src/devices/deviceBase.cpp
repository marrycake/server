#include "deviceBase.h"
#include <string>

DeviceBase::DeviceBase() : info(nullptr) {}

DeviceBase::DeviceBase(unsigned int id, std::string &authorizeKey)
    : info(new DeviceInfo(id, authorizeKey)) {}

DeviceBase::~DeviceBase() { delete info; }
