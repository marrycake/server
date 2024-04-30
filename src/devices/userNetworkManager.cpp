#include "userNetworkManager.h"
#include "userSocketWrapper.h"
#include "utils.h"
#include <arpa/inet.h>
#include <cstring>
#include <gsl/pointers>
#include <iostream>
#include <memory>
#include <netinet/in.h>
#include <string>
#include <sys/socket.h>
#include <unistd.h>
#include <utility>

std::unique_ptr<UserNetworkManager> UserNetworkManager::userNetworkManager =
    nullptr;
std::mutex UserNetworkManager::mutex;

const std::string REGISTER_SUCCESSFUL = "register successful";

UserNetworkManager::UserNetworkManager() : isEstablished(false), listenfd(-1) {}

UserNetworkManager::~UserNetworkManager() {
  if (isEstablished) {
    terminate();
  }
}

void UserNetworkManager::deleteUserSocket(unsigned int bindedDeviceId,
                                          const std::string &userId) {
  auto iter = recordMap.find(bindedDeviceId);
  if (iter == recordMap.end()) {
    return;
  }
  auto *addrMap = iter->second.get();
  auto userIter = addrMap->find(userId);
  if (userIter == addrMap->end()) {
    return;
  }
  addrMap->erase(userIter);
}

bool UserNetworkManager::establish() {
  int connfd = 0;
  char buffer[4096];
  int n = 0;
  listenfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);
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

  isEstablished = true;

  listenThread = std::thread([&, this]() {
    std::cout << "user network thread start"
              << "\n";
    struct sockaddr_in clientAddr;
    socklen_t clientAddrLen = sizeof(clientAddr);
    while (isEstablished) {

      auto bytesReceived =
          recvfrom(listenfd, buffer, sizeof(buffer), 0,
                   (struct sockaddr *)&clientAddr, &clientAddrLen);
      if (bytesReceived < 0) {
        return;
      }
      buffer[bytesReceived] = '\0';
      auto userInfo = extraUserInfo(buffer);
      auto iter = recordMap.find(userInfo.deviceId);
      if (iter == recordMap.end()) {
        auto ipaddrMap =
            std::make_unique<std::map<std::string, struct sockaddr_in>>();
        ipaddrMap->insert(std::make_pair(userInfo.userId, clientAddr));
        recordMap.insert(
            std::make_pair(userInfo.deviceId, std::move(ipaddrMap)));
      } else {
        iter->second->insert(std::make_pair(userInfo.userId, clientAddr));
      }
      std::string successInfo = "successful";
      sendto(listenfd, successInfo.c_str(), successInfo.length(), 0,
             (struct sockaddr *)&clientAddr, clientAddrLen);
      std::cout << "Received from " << inet_ntoa(clientAddr.sin_addr) << ":"
                << ntohs(clientAddr.sin_port) << ":" << std::string(buffer)
                << '\n';
    }
  });

  return true;
}

bool UserNetworkManager::terminate() {
  isEstablished = false;
  ::close(listenfd);
  listenThread.join();
  return true;
}

bool UserNetworkManager::getAddrPorts(
    unsigned bindedDeviceId,
    std::list<std::pair<std::string, uint16_t>> &result) const {
  auto iter = recordMap.find(bindedDeviceId);
  if (iter == recordMap.end()) {
    return false;
  }

  for (auto [_, value] : *iter->second) {
    result.emplace_back(inet_ntoa(value.sin_addr), ntohs(value.sin_port));
  }

  return true;
}
