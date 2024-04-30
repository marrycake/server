#ifndef USERNETWORKMANAGER_H
#define USERNETWORKMANAGER_H

#include <atomic>
#include <gsl/pointers>
#include <list>
#include <map>
#include <memory>
#include <mutex>
#include <netinet/in.h>
#include <string>
#include <thread>
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
  bool getAddrPorts(unsigned bindedDeviceId,
                    std::list<std::pair<std::string, uint16_t>> &result) const;

private:
  static std::unique_ptr<UserNetworkManager> userNetworkManager;
  static std::mutex mutex;

  std::map<unsigned int,
           std::unique_ptr<std::map<std::string, struct sockaddr_in>>>
      recordMap;

  struct sockaddr_in serverConfig;
  std::thread listenThread;
  std::atomic_bool isEstablished;
  int listenfd;
};

#endif // USERNETWORKMANAGER_H