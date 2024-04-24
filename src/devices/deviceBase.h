#ifndef DEVICEBASE_H
#define DEVICEBASE_H

#include "deviceInfo.h"
#include <string>

class DeviceBase {
public:
  DeviceBase();
  virtual ~DeviceBase();
  DeviceBase(unsigned int id, std::string &authorizeKey);
  DeviceBase(const DeviceBase &) = default;
  DeviceBase &operator=(const DeviceBase &) = default;
  DeviceBase(DeviceBase &&) = default;
  DeviceBase &operator=(DeviceBase &&) = default;

  [[nodiscard]] inline unsigned int getKey() const { return info->id; }
  [[nodiscard]] inline const std::string &getAuthorizeKey() const {
    return info->authorizeKey;
  }

private:
  DeviceInfo *info;
};

#endif // DEVICEBASE_H