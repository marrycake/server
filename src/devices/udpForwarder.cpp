#include "udpForwarder.h"
#include "userNetworkManager.h"
#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

UdpForwarder::UdpForwarder(unsigned int bindedDeviceId)
    : serverAddr(), bindedDeviceId(bindedDeviceId), sockfd(-1),
      isEstablished(false) {}

UdpForwarder::~UdpForwarder() {
  if (isEstablished) {
    terminate();
  }
}

bool UdpForwarder::establish() {

  // 初始化服务器地址
  memset(&serverAddr, 0, sizeof(serverAddr));
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
  serverAddr.sin_port = htons(0); // 设置端口为0，表示使用随机端口

  socklen_t len = sizeof(serverAddr);

  sockfd = socket(AF_INET, SOCK_DGRAM, 0);
  if (sockfd < 0) {
    return false;
  }

  if (bind(sockfd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
    return false;
  }

  if (getsockname(sockfd, (struct sockaddr *)&serverAddr, &len) < 0) {
    return false;
  }

  isEstablished = true;

  forwarderThread = std::thread([&, this]() {
    char buffer[65535];
    while (isEstablished) {
      struct sockaddr_in clientAddr;
      socklen_t clientAddrLen = sizeof(clientAddr);
      auto bytesReceived =
          recvfrom(sockfd, buffer, sizeof(buffer), 0,
                   (struct sockaddr *)&clientAddr, &clientAddrLen);
      if (bytesReceived < 0) {
        return;
      }

      std::cout << "Received from " << inet_ntoa(clientAddr.sin_addr) << ":"
                << ntohs(clientAddr.sin_port) << ":" << bytesReceived << " bits"
                << '\n';
    }
  });
  return true;
}

bool UdpForwarder::terminate() {
  isEstablished = false;
  ::close(sockfd);
  forwarderThread.join();
  return true;
}

unsigned int UdpForwarder::getUdpListenPort() const {
  return ntohs(serverAddr.sin_port);
}