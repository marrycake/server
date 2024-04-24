#ifndef USERMANAGER_H
#define USERMANAGER_H

#include <vector>

struct UserInfo;

enum RegStatu { USERNAMEHAVEEXIST, REGISTESUCCESS };

#include <string>
class UserManager {
public:
  static bool loginIsValid(std::string &userName, std::string &passwd);
  static RegStatu userRegister(UserInfo &userInfo);
  static bool changePasswd(std::string &userName, std::string &passwd);
  static void getBindedDevices(std::string &userName,
                               std::vector<std::string> results);
};

#endif // USERMANAGER_H