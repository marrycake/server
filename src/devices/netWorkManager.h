#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include "deviceCommander.h"
#include "macros.h"
#include <atomic>
#include <memory>
#include <netinet/in.h>
#include <string>
#include <thread>
#include <unordered_map>

constexpr unsigned int MAX_DEVICE_NUM = 100;

class NetWorkManager {

public:
  NO_COPYABLE(NetWorkManager)
  NO_MOVEABLE(NetWorkManager)
  using ServerConfig = struct sockaddr_in;
  explicit NetWorkManager(ServerConfig &);
  ~NetWorkManager();
  bool establish();
  bool terminate();
  N_Status send(unsigned int id, const std::string &message) const;
  N_Status send(unsigned int id, const std::string &message,
                std::string &response) const;
  N_Status send(const std::string &id, const std::string &message) const;
  N_Status send(const std::string &id, const std::string &message,
                std::string &response) const;
  bool getDeviceById(unsigned int id, DeviceCommander *device) const;

  void closeDevice(const std::string &id);

private:
  std::unordered_map<unsigned int, std::unique_ptr<DeviceCommander>> recordMap;
  ServerConfig serverConfig;
  std::thread listenThread;
  std::atomic_bool isEstablished;
  int listenfd;
};

#endif // NETWORKMANAGER_H
