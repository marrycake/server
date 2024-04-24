#ifndef COMMANDMAPPER_H
#define COMMANDMAPPER_H

#include "macros.h"
#include <atomic>
#include <list>
#include <memory>
#include <mutex>
#include <semaphore>
#include <string>
#include <thread>
#include <utility>

struct TagDeleteUserSocketParams {
  std::string userId;
  unsigned int bindedDeviceId;

  TagDeleteUserSocketParams(std::string userId, unsigned int bindedDeviceId)
      : userId(std::move(userId)), bindedDeviceId(bindedDeviceId) {}
};
using DeleteUserSocketParams = struct TagDeleteUserSocketParams;
using pDeleteUserSocketParams = struct TagDeleteUserSocketParams *;

struct TagCommandInfo {
  unsigned int command;
  std::shared_ptr<void> params;

  TagCommandInfo(unsigned int command, const std::shared_ptr<void> &params)
      : command(command), params(params) {}
};
using CommandInfo = struct TagCommandInfo;
using pCommandInfo = struct TagCommandInfo *;

const int COMMAND_SIZE = 10;
enum {
  DELETE_USER_SOCKET,
  TERMINATE_MAPPER,
};
class CommandMapper {
public:
  static CommandMapper *getInstance() {
    std::lock_guard<std::mutex> lock(mutex);
    if (instance == nullptr) {
      instance = std::make_unique<CommandMapper>();
    }
    return instance.get();
  }
  explicit CommandMapper();
  ~CommandMapper();
  NO_COPYABLE(CommandMapper)
  NO_MOVEABLE(CommandMapper)
  void sendCommand(int command, std::shared_ptr<void> params);

private:
  CommandInfo getCommand();

  static std::unique_ptr<CommandMapper> instance;
  static std::mutex mutex;
  std::list<CommandInfo> commandList;
  std::mutex listMutex;

  std::counting_semaphore<0> countSemaphore;

  std::thread localThread;
  std::atomic<bool> threadRunning;
};

#endif // COMMANDMAPPER_H