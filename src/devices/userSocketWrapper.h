#ifndef USERSOCKETWRAPPER_H
#define USERSOCKETWRAPPER_H

#include <atomic>
#include <string>
#include <thread>
const int BUFFER_MAXSIZE = 4096;

class UserSocketWrapper {
public:
  explicit UserSocketWrapper(int bindedSocket, const std::string &userId,
                             unsigned int deviceId);
  ~UserSocketWrapper();

  bool sendToClient(char *buffer, unsigned int len) const;
  std::string &getUserId() { return userId; }
  void waitToThreadTerminate() { mThread.join(); }

private:
  int extraCommand(char *buffer);
  void terminate();

private:
  char mBuffer[BUFFER_MAXSIZE];
  int bindedSocket;
  std::string userId;
  unsigned int deviceId;
  std::thread mThread;
  std::atomic<bool> connecting;
};

#endif // USERSOCKETWRAPPER_H