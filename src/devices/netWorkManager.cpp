#include "netWorkManager.h"
#include "../common.h"
#include "deviceCommander.h"
#include "utils.h"
#include <cerrno>
#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define MAXLINE 4096

NetWorkManager::NetWorkManager(ServerConfig &serverConfig)
    : serverConfig(serverConfig), isEstablished(false), listenfd(-1) {}

NetWorkManager::~NetWorkManager() {
  if (isEstablished) {
    terminate();
  }
}

bool NetWorkManager::establish() {
  int connfd = 0;
  char buff[4096];
  int n = 0;

  listenfd = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
  if (listenfd == -1) {
    std::cout << "create socket error: " << strerror(errno)
              << "(errno: " << errno << ")" << '\n';
    return false;
  }

  if (bind(listenfd, (struct sockaddr *)&serverConfig, sizeof(serverConfig)) ==
      -1) {
    std::cout << "bind socket error: " << strerror(errno) << "(errno: " << errno
              << ")\n";
    return false;
  }

  if (listen(listenfd, MAX_DEVICE_NUM) == -1) {
    std::cout << "listen socket error: " << strerror(errno)
              << "(errno: " << errno << ")\n";
    return false;
  }

  isEstablished = true;

  listenThread = std::thread([&, this]() {
    std::cout << "device network thread start"
              << "\n";
    while (isEstablished) {
      connfd = accept(listenfd, (struct sockaddr *)nullptr, nullptr);
      if (connfd == -1) {
        printf("accept socket error: %s(errno: %d)", strerror(errno), errno);
        continue;
      }
      n = recv(connfd, buff, MAXLINE, 0);
      buff[n] = '\0';

      auto [id, key] = extractIdAndKey(buff);
      std::cout << "received id: " << id << " key: " << key << '\n';
      recordMap.insert(std::make_pair(
          id, std::make_unique<DeviceCommander>(id, key, connfd)));
    }
    std::cout << "device network thread end"
              << "\n";
  });

  return true;
}

bool NetWorkManager::terminate() {
  isEstablished = false;
  ::close(listenfd);
  listenThread.join();
  return true;
}

N_Status NetWorkManager::send(unsigned int id,
                              const std::string &message) const {
  int count = static_cast<int>(recordMap.count(id));
  if (count == 0) {
    return UNKNOWID;
  }

  return recordMap.at(id)->send(message);
}

N_Status NetWorkManager::send(unsigned int id, const std::string &message,
                              std::string &response) const {

  int count = static_cast<int>(recordMap.count(id));
  if (count == 0) {
    return UNKNOWID;
  }

  return recordMap.at(id)->send(message, response);
}

N_Status NetWorkManager::send(const std::string &id,
                              const std::string &message) const {

  return send(stringToInt(id), message);
}

N_Status NetWorkManager::send(const std::string &id, const std::string &message,
                              std::string &response) const {
  return send(stringToInt(id), message, response);
}

bool NetWorkManager::getDeviceById(unsigned int id,
                                   DeviceCommander *device) const {

  if (static_cast<bool>(recordMap.count(id))) {
    device = recordMap.at(id).get();
    return true;
  }

  return false;
}

void NetWorkManager::closeDevice(const std::string &id) {
  int deviceId = stringToInt(id);
  recordMap.at(deviceId)->deviceClose();
  recordMap.erase(deviceId);
}