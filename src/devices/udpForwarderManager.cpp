#include "udpForwarderManager.h"
#include "udpForwarder.h"
#include <memory>

std::unique_ptr<UdpForwarderManager> UdpForwarderManager::instance = nullptr;
std::mutex UdpForwarderManager::mutex;

UdpForwarder *
UdpForwarderManager::registerUdpForwarder(unsigned int bindedDeviceId) {
  recordMap[bindedDeviceId] = std::make_unique<UdpForwarder>(bindedDeviceId);
  auto *ptr = recordMap[bindedDeviceId].get();
  ptr->establish();
  return ptr;
}
void UdpForwarderManager::eraseUdpForwarderById(unsigned int bindedDeviceId) {
  recordMap.find(bindedDeviceId)->second = nullptr;
  recordMap.erase(bindedDeviceId);
}