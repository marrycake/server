#include <string>

struct DeviceInfo {
  unsigned int id{0};
  std::string authorizeKey;

  DeviceInfo() = default;
  DeviceInfo(unsigned int id, std::string &authorizeKey)
      : id(id), authorizeKey(authorizeKey) {}
};