#include "commandMapper.h"
#include "userNetworkManager.h"
#include <memory>
#include <mutex>

std::unique_ptr<CommandMapper> CommandMapper::instance = nullptr;
std::mutex CommandMapper::mutex;

CommandMapper::CommandMapper() : countSemaphore(0), threadRunning(true) {
  localThread = std::thread([&]() {
    while (threadRunning) {
      auto commandInfo = getCommand();
      switch (commandInfo.command) {
      case DELETE_USER_SOCKET: {
        auto *p =
            reinterpret_cast<pDeleteUserSocketParams>(commandInfo.params.get());
        UserNetworkManager::getInstance()->deleteUserSocket(p->bindedDeviceId,
                                                            p->userId);
        break;
      }
      case TERMINATE_MAPPER: {
        threadRunning = false;
        break;
      }
      default:
        break;
      }
    }
  });
}

CommandMapper::~CommandMapper() {
  sendCommand(TERMINATE_MAPPER, nullptr);
  localThread.join();
}

void CommandMapper::sendCommand(int command, std::shared_ptr<void> params) {
  std::lock_guard<std::mutex> lock(listMutex);
  commandList.emplace_back(command, params);
  countSemaphore.release();
}

CommandInfo CommandMapper::getCommand() {
  countSemaphore.acquire();
  std::lock_guard<std::mutex> lock(listMutex);
  auto value = commandList.front();
  commandList.pop_front();
  return value;
}
