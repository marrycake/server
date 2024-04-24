#include "userNetworkManager.h"
#include "userSocketWrapper.h"
#include "utils.h"
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

  for (auto &userSocketWrapper : *iter->second) {
    if (userSocketWrapper->getUserId() == userId) {
      //   userSocketWrapper->waitToThreadTerminate();
      (*iter->second).remove(userSocketWrapper);
      return;
    }
  }
}

bool UserNetworkManager::establish() {
  int connfd = 0;
  char buffer[4096];
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

  if (listen(listenfd, MAX_USER_NUM) == -1) {
    std::cout << "listen socket error: " << strerror(errno)
              << "(errno: " << errno << ")\n";
    return false;
  }

  isEstablished = true;

  listenThread = std::thread([&, this]() {
    std::cout << "user network thread start"
              << "\n";
    while (isEstablished) {
      connfd = accept(listenfd, (struct sockaddr *)nullptr, nullptr);
      if (connfd == -1) {
        printf("accept socket error: %s(errno: %d)", strerror(errno), errno);
        continue;
      }
      n = recv(connfd, buffer, MAXLINE, 0);
      buffer[n] = '\0';
      auto userInfo = extraUserInfo(buffer);
      auto item = recordMap.find(userInfo.deviceId);
      if (item == recordMap.end()) {
        auto userSocketList =
            std::make_unique<std::list<std::unique_ptr<UserSocketWrapper>>>();
        userSocketList->push_back(std::make_unique<UserSocketWrapper>(
            connfd, userInfo.userId, userInfo.deviceId));
        recordMap.insert({userInfo.deviceId, std::move(userSocketList)});

      } else {
        item->second->push_back(std::make_unique<UserSocketWrapper>(
            connfd, userInfo.userId, userInfo.deviceId));
      }

      write(connfd, REGISTER_SUCCESSFUL.c_str(),
            REGISTER_SUCCESSFUL.length() - 1);
    }
    std::cout << "user network thread end"
              << "\n";
  });

  return true;
}

bool UserNetworkManager::terminate() {
  isEstablished = false;
  ::close(listenfd);
  listenThread.join();
  return true;
}

void UserNetworkManager::writeToSockets(unsigned int bindedDeviceId,
                                        char *buffer, unsigned int len) {
  auto iter = recordMap.find(bindedDeviceId);
  if (iter == recordMap.end()) {
    return;
  }

  for (auto &userSocketWrapper : *iter->second) {
    userSocketWrapper->sendToClient(buffer, len);
  }
}
