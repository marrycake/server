#ifndef UDPFORWARDERMANAGER_H
#define UDPFORWARDERMANAGER_H

#include "udpForwarder.h"
#include <map>
#include <memory>
#include <mutex>
class UdpForwarderManager {
public:
  static UdpForwarderManager *getInstance() {
    std::lock_guard<std::mutex> lock(mutex);
    if (instance == nullptr) {
      instance = std::make_unique<UdpForwarderManager>();
    }
    return instance.get();
  }

  UdpForwarder *registerUdpForwarder(unsigned int bindedDeviceId);
  void eraseUdpForwarderById(unsigned int bindedDeviceId);

private:
  std::map<unsigned int, std::unique_ptr<UdpForwarder>> recordMap;
  static std::mutex mutex;
  static std::unique_ptr<UdpForwarderManager> instance;
};

#endif // UDPFORWARDERMANAGER_H