#include <string>
#include <utility>

struct UserInfo {
  std::string userName;
  std::string password;

  UserInfo(std::string userName, std::string password)
      : userName(std::move(userName)), password(std::move(password)) {}
};