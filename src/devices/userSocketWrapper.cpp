#include "userSocketWrapper.h"
#include "commandMapper.h"
#include <iostream>
#include <sys/socket.h>
#include <unistd.h>

UserSocketWrapper::UserSocketWrapper(int bindedSocket,
                                     const std::string &userId,
                                     unsigned int deviceId)
    : bindedSocket(bindedSocket), userId(userId), deviceId(deviceId),
      connecting(true) {
  mThread = std::thread([&, this]() {
    int n = 0;
    while (connecting) {
      n = recv(this->bindedSocket, mBuffer, BUFFER_MAXSIZE, 0);
      if (n > 0) {
        mBuffer[n] = '\n';
        int commandId = extraCommand(mBuffer);
        CommandMapper::getInstance()->sendCommand(commandId, nullptr);
      } else {
        if (connecting) {
          terminate();
        }
      }
    }

    std::shared_ptr<void> params = std::shared_ptr<void>(
        new DeleteUserSocketParams(this->userId, this->deviceId));
    CommandMapper::getInstance()->sendCommand(DELETE_USER_SOCKET, params);
    std::cout << "user Socket wrapper thread exit"
              << "\n";
  });
}

UserSocketWrapper::~UserSocketWrapper() {
  if (connecting) {
    terminate();
  }
  mThread.join();
}

bool UserSocketWrapper::sendToClient(char *buffer, unsigned int len) const {
  int n = send(bindedSocket, buffer, len, 0);
  return n > 0;
}

void UserSocketWrapper::terminate() {
  connecting = false;
  ::close(bindedSocket);
}

int UserSocketWrapper::extraCommand(char *buffer) { return 0; }
