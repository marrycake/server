#ifndef DBHELPER_H
#define DBHELPER_H
#include "SQLiteCpp/Database.h"
#include "userInfo.h"
#include <memory>
#include <string>
#include <vector>

class DBhelper {
public:
  DBhelper();
  bool open();
  void close();
  bool createUserInfoTable(const std::string &);
  bool createDeviceTable(const std::string &);
  bool destroyTable(const std::string &);
  bool insertData(const std::string &tableName, const UserInfo &info);
  bool insertDeviceData(const std::string &tableName, const std::string &userId,
                        const std::string &deviceId);
  bool deleteDataByUserName(const std::string &tableName,
                            const std::string &userName);
  bool updatePasswordByUserName(const std::string &, const std::string &,
                                const std::string &);
  std::string queryPasswdByUserName(const std::string &tableName,
                                    const std::string &userName);
  std::vector<std::string> queryDeviceByUserName(const std::string &);
  std::string queryIdByUserName(const std::string &userName);

  bool hasUserName(const std::string &tableName, const std::string &userName);
  void queryBindedDevicesById(const std::string &userId,
                              std::vector<std::string> &results);
  void queryBindedDevicesByUserName(const std::string &userName,
                                    std::vector<std::string> &results);

private:
  std::unique_ptr<SQLite::Database> db;
};

#endif // DBHELPER_H