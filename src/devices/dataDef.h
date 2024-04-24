#ifndef DATADEF_H
#define DATADEF_H

#include <string>

using UserMessageInfo = struct TagUserInfo {
  std::string userId;
  unsigned int deviceId;

  TagUserInfo(std::string userId, unsigned int deviceId)
      : userId(std::move(userId)), deviceId(deviceId) {}
};

#endif // DATADEF_H