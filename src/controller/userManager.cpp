#include "userManager.h"
#include "../common.h"
#include "DBhelper.h"

bool UserManager::loginIsValid(std::string &userName, std::string &passwd) {
  DBhelper db;
  db.open();

  db.createUserInfoTable(USER_INFO_TABLE_NAME);

  auto pw = db.queryPasswdByUserName(USER_INFO_TABLE_NAME, userName);
  return !pw.empty() && pw == passwd;
}

RegStatu UserManager::userRegister(UserInfo &userInfo) {
  DBhelper db;
  db.open();
  db.createUserInfoTable(USER_INFO_TABLE_NAME);
  if (db.hasUserName(USER_INFO_TABLE_NAME, userInfo.userName)) {
    return RegStatu::USERNAMEHAVEEXIST;
  }

  db.insertData(USER_INFO_TABLE_NAME, userInfo);
  return RegStatu::REGISTESUCCESS;
}

bool UserManager::changePasswd(std::string &userName, std::string &passwd) {
  DBhelper db;
  db.open();
  db.createUserInfoTable(USER_INFO_TABLE_NAME);
  return db.updatePasswordByUserName(USER_INFO_TABLE_NAME, userName, passwd);
}

void UserManager::getBindedDevices(std::string &userName,
                                   std::vector<std::string> results) {
  DBhelper db;
  db.open();
  db.createUserInfoTable(USER_INFO_TABLE_NAME);
  db.queryBindedDevicesByUserName(userName, results);
}
