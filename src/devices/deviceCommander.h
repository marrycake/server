#ifndef DEVICECOMMANDER_H
#define DEVICECOMMANDER_H

#include "deviceBase.h"

enum N_Status { SUCCESS = 0, UNKNOWID = -1, NETWORKERROR = -2 };
class DeviceCommander : public DeviceBase {
public:
  DeviceCommander() = default;
  DeviceCommander(unsigned int id, std::string &authorizeKey, int bindedSocket);
  ~DeviceCommander() override;
  DeviceCommander(const DeviceCommander &) = default; // Add copy constructor
  DeviceCommander &
  operator=(const DeviceCommander &) = default;  // Add copy assignment operator
  DeviceCommander(DeviceCommander &&) = default; // Add move constructor
  DeviceCommander &
  operator=(DeviceCommander &&) = default; // Add move assignment operator
  N_Status send(const std::string &request) const;
  N_Status send(const std::string &request, std::string &response) const;
  void deviceClose();

private:
  int bindedSocket;
};

#endif // DEVICECOMMANDER_H