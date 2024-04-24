#include "common.h"
#include "controller/handers.h"
#include "devices/netWorkManager.h"
#include "devices/netWorkManagerController.h"
#include "devices/udpForwarder.h"
#include "devices/userNetworkManager.h"
#include <csignal>
#include <cstdlib>
#include <exception>
#include <httplib.h>
#include <netinet/in.h>
#include <sys/socket.h>

int main(int /*argc*/, char const * /*argv*/[]) {

  static_cast<void>(std::signal(SIGINT, [](int) {
    std::cout << "SIGINT received, exit"
              << "\n";
    std::terminate();
  }));

  NetWorkManager::ServerConfig sConfig;
  sConfig.sin_family = AF_INET;
  sConfig.sin_port = htons(DEVICE_SERVER_LISTEN_PORT);
  sConfig.sin_addr.s_addr = htonl(INADDR_ANY);

  auto *nManager = new NetWorkManager(sConfig);

  NetWorkManagerController::setInstance(nManager);

  if (!nManager->establish()) {
    std::terminate();
  }

  sConfig.sin_port = htons(USER_NETWORK_LISTEN_PORT);
  auto *userNetworkManager = UserNetworkManager::getInstance();
  userNetworkManager->setServerConfig(sConfig);
  if (!userNetworkManager->establish()) {
    std::terminate();
  }

  //   struct sockaddr_in serverAddr;
  //   std::memset(&serverAddr, 0, sizeof(serverAddr));
  //   serverAddr.sin_family = AF_INET;
  //   serverAddr.sin_addr.s_addr = htonl(INADDR_ANY); // 绑定所有网卡
  //   serverAddr.sin_port = htons(8000);

  //   struct sockaddr_in destAddr;
  //   std::memset(&destAddr, 0, sizeof(destAddr));
  //   destAddr.sin_family = AF_INET;
  //   destAddr.sin_addr.s_addr = htonl(INADDR_ANY); // 目标IP地址
  //   destAddr.sin_port = htons(9000);

  //   UdpForwarder udpForwarder(serverAddr, destAddr);
  //   if (!udpForwarder.establish()) {
  //     return EXIT_FAILURE;
  //   }

  httplib::Server clientServer;

  clientServer.Get(LOGIN_ROUTE, loginHander);
  clientServer.Post(LOGIN_ROUTE, loginHander);

  clientServer.Get(REGISTER_ROUTE, registerHander);
  clientServer.Post(REGISTER_ROUTE, registerHander);

  clientServer.Get(CHANGE_PASSWORD_ROUTE, changePasswd);
  clientServer.Post(CHANGE_PASSWORD_ROUTE, changePasswd);

  clientServer.Get(COMMAND_FORWARD_ROUTE, commandForwarder);
  clientServer.Post(COMMAND_FORWARD_ROUTE, commandForwarder);

  clientServer.Get(BIND_DEVICE_ROUTE, bindDevice);
  clientServer.Post(BIND_DEVICE_ROUTE, bindDevice);

  clientServer.Get(GET_BINDED_DEVICE_ROUTE, getBindedDevice);
  clientServer.Post(GET_BINDED_DEVICE_ROUTE, getBindedDevice);

  clientServer.listen(LISTEN_ADDR, CLIENT_SERVER_LISTEN_PORT);

  return 0;
}