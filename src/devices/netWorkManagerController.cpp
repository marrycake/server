#include "netWorkManagerController.h"
#include "netWorkManager.h"

std::unique_ptr<NetWorkManager> NetWorkManagerController::nManager = nullptr;

NetWorkManager *NetWorkManagerController::getGlobalInstance() {
  return nManager.get();
}

void NetWorkManagerController::setInstance(NetWorkManager *instance) {
  nManager.reset(instance);
}
