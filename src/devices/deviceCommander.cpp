#include "deviceCommander.h"
#include "deviceBase.h"
#include <cstring>
#include <exception>
#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

DeviceCommander::DeviceCommander(unsigned int id, std::string &authorizeKey,
                                 int bindedSocket)
    : DeviceBase(id, authorizeKey), bindedSocket(bindedSocket) {}

DeviceCommander::~DeviceCommander() {
  if (bindedSocket != -1) {
    deviceClose();
  }
}

N_Status DeviceCommander::send(const std::string &request) const {
  if (write(bindedSocket, request.c_str(), request.length()) == -1) {
    return NETWORKERROR;
  }
  return SUCCESS;
}

N_Status DeviceCommander::send(const std::string &request,
                               std::string &response) const {
  if (write(bindedSocket, request.c_str(), request.length()) == -1) {
    return NETWORKERROR;
  }
  char buffer[1024];
  ssize_t bytesRead = read(bindedSocket, buffer, sizeof(buffer) - 1);
  if (bytesRead == -1) {
    return NETWORKERROR;
  }
  if (bytesRead == 0) {
    return NETWORKERROR;
  }

  buffer[bytesRead] = '\0';
  response.assign(buffer);
  std::cout << "Server response: " << buffer << std::endl;

  return SUCCESS;
}

void DeviceCommander::deviceClose() {
  send("quit");
  int closed = ::close(bindedSocket);
  if (closed == -1) {
    std::cerr << "close socket error: " << std::strerror(errno)
              << "(errno: " << errno << ")\n";
    std::terminate();
  }
  bindedSocket = -1;
}