#ifndef NETWORKMANAGERCONTROLLER_H
#define NETWORKMANAGERCONTROLLER_H

#include "netWorkManager.h"
#include <memory>

class NetWorkManagerController {
public:
  static NetWorkManager *getGlobalInstance();
  static void setInstance(NetWorkManager *instance);

private:
  static std::unique_ptr<NetWorkManager> nManager;
};

#endif // NETWORKMANAGERCONTROLLER_H