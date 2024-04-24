#ifndef USERNETWORKMANAGER_H
#define USERNETWORKMANAGER_H

#include <atomic>
#include <gsl/pointers>
#include <list>
#include <memory>
#include <mutex>
#include <netinet/in.h>
#include <string>
#include <thread>
#include <unordered_map>
#include <utility>

class UserSocketWrapper;

const int MAX_USER_NUM = 1000;
const int MAXLINE = 4096;

class UserNetworkManager {
public:
  static UserNetworkManager *getInstance() {
    std::lock_guard<std::mutex> lock(mutex);
    if (userNetworkManager == nullptr) {
      userNetworkManager = std::make_unique<UserNetworkManager>();
    }
    return userNetworkManager.get();
  }
  UserNetworkManager();
  ~UserNetworkManager();
  UserNetworkManager(const UserNetworkManager &) = delete;
  UserNetworkManager &operator=(const UserNetworkManager &) = delete;
  UserNetworkManager(UserNetworkManager &&) = delete;
  UserNetworkManager &operator=(const UserNetworkManager &&) = delete;

  void setServerConfig(struct sockaddr_in &serverConfig) {
    this->serverConfig = serverConfig;
  }
  bool establish();
  bool terminate();

  void deleteUserSocket(unsigned int bindedDeviceId, const std::string &userId);
  void writeToSockets(unsigned int bindedDeviceId, char *buffer,
                      unsigned int len);

private:
  static std::unique_ptr<UserNetworkManager> userNetworkManager;
  static std::mutex mutex;

  std::unordered_map<
      unsigned int,
      std::unique_ptr<std::list<std::unique_ptr<UserSocketWrapper>>>>
      recordMap;

  struct sockaddr_in serverConfig;
  std::thread listenThread;
  std::atomic_bool isEstablished;
  int listenfd;
};

#endif // USERNETWORKMANAGER_H