#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <sys/socket.h>
#include <unistd.h>

int main() {
  // 创建UDP套接字
  int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
  if (sockfd < 0) {
    std::cerr << "Error creating socket\n";
    return 1;
  }

  // 绑定本地地址和端口
  struct sockaddr_in serverAddr;
  std::memset(&serverAddr, 0, sizeof(serverAddr));
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_addr.s_addr = htonl(INADDR_ANY); // 绑定所有网卡
  serverAddr.sin_port = htons(8000);              // 监听端口8080
  if (bind(sockfd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
    std::cerr << "Error binding socket\n";
    return 1;
  }

  // 目标地址和端口
  struct sockaddr_in destAddr;
  std::memset(&destAddr, 0, sizeof(destAddr));
  destAddr.sin_family = AF_INET;
  destAddr.sin_addr.s_addr = inet_addr("0.0.0.0"); // 目标IP地址
  destAddr.sin_port = htons(9000);                 // 目标端口号

  // 循环接收UDP数据并转发
  char buffer[65535];
  while (true) {
    socklen_t addrLen = sizeof(serverAddr);
    int bytesReceived = recvfrom(sockfd, buffer, sizeof(buffer), 0,
                                 (struct sockaddr *)&serverAddr, &addrLen);
    if (bytesReceived < 0) {
      std::cerr << "Error receiving data\n";
      break;
    }

    // 转发数据到目标地址
    addrLen = sizeof(destAddr);
    int bytesSent = sendto(sockfd, buffer, bytesReceived, 0,
                           (struct sockaddr *)&destAddr, addrLen);
    if (bytesSent < 0) {
      std::cerr << "Error sending data\n";
      break;
    }
  }

  // 关闭套接字
  close(sockfd);

  return 0;
}
