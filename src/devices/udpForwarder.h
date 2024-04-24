#ifndef UDPFORWARDER_H
#define UDPFORWARDER_H

#include <arpa/inet.h>
#include <atomic>
#include <sys/socket.h>
#include <thread>

class UdpForwarder {
public:
  explicit UdpForwarder(unsigned int bindedDeviceId);
  ~UdpForwarder();
  bool establish();
  bool terminate();

  [[nodiscard]] unsigned int getUdpListenPort() const;

private:
  struct sockaddr_in serverAddr;
  unsigned int bindedDeviceId;
  int sockfd;

  std::thread forwarderThread;
  std::atomic_bool isEstablished;
};

#endif // UDPFORWARDER_H