#include "DBhelper.h"
#include "../common.h"
#include "SQLiteCpp/Exception.h"
#include "SQLiteCpp/Statement.h"
#include "SQLiteCpp/Transaction.h"
#include <iostream>
#include <memory>
#include <string>
#include <vector>

DBhelper::DBhelper() : db(nullptr) {}

bool DBhelper::open() {
  try {
    db = std::make_unique<SQLite::Database>(
        DB_LOCATION, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
  } catch (SQLite::Exception &e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return false;
  }
  return true;
}

void DBhelper::close() { db.reset(); }

bool DBhelper::createUserInfoTable(const std::string &tableName) {
  try {
    SQLite::Transaction transaction(*db);
    std::string sqlString = "CREATE TABLE IF NOT EXISTS " + tableName +
                            " (userId INTEGER PRIMARY KEY, username "
                            "TEXT, password TEXT)";
    auto execStatement = SQLite::Statement(*db, sqlString);
    execStatement.exec();

    transaction.commit();
  } catch (SQLite::Exception &e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return false;
  }
  return true;
}

bool DBhelper::createDeviceTable(const std::string &tableName) {
  try {
    SQLite::Transaction transaction(*db);
    std::string sqlString = "CREATE TABLE IF NOT EXISTS " + tableName +
                            " (deviceId TEXT, userId TEXT)";
    auto execStatement = SQLite::Statement(*db, sqlString);
    execStatement.exec();
    transaction.commit();
  } catch (SQLite::Exception &e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return false;
  }
  return true;
}

bool DBhelper::destroyTable(const std::string &tableName) {
  try {

    SQLite::Transaction transaction(*db);
    auto execStatement = SQLite::Statement(*db, "DROP TABLE IF EXISTS ?");
    execStatement.bind(1, tableName);
    execStatement.exec();

    transaction.commit();
  } catch (SQLite::Exception &e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return false;
  }
  return true;
}

bool DBhelper::insertData(const std::string &tableName, const UserInfo &info) {
  try {
    SQLite::Transaction transaction(*db);
    std::string sqlString =
        "INSERT INTO " + tableName + " (username, password) VALUES (?, ?)";
    auto execStatement = SQLite::Statement(*db, sqlString);
    execStatement.bind(1, info.userName);
    execStatement.bind(2, info.password);
    execStatement.exec();

    transaction.commit();
  } catch (SQLite::Exception &e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return false;
  }
  return true;
}

bool DBhelper::insertDeviceData(const std::string &tableName,
                                const std::string &userId,
                                const std::string &deviceId) {
  try {
    SQLite::Transaction transaction(*db);
    std::string sqlString =
        "INSERT INTO " + tableName + " (userId, deviceId) VALUES (?, ?)";
    auto execStatement = SQLite::Statement(*db, sqlString);
    execStatement.bind(1, userId);
    execStatement.bind(2, deviceId);
    execStatement.exec();

    transaction.commit();
  } catch (SQLite::Exception &e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return false;
  }
  return true;
}

bool DBhelper::deleteDataByUserName(const std::string &tableName,
                                    const std::string &userName) {
  try {
    SQLite::Transaction transaction(*db);
    std::string sqlString = "DELETE FROM " + tableName + " WHERE username = ?";
    auto execStatement = SQLite::Statement(*db, sqlString);
    execStatement.bind(1, userName);
    execStatement.exec();

    transaction.commit();
  } catch (SQLite::Exception &e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return false;
  }

  return true;
}

bool DBhelper::updatePasswordByUserName(const std::string &tableName,
                                        const std::string &userName,
                                        const std::string &passwd) {
  try {
    SQLite::Transaction transaction(*db);
    auto sqlString =
        "UPDATE " + tableName + " SET password = ? WHERE username = ?";
    auto execStatement = SQLite::Statement(*db, sqlString);
    execStatement.bind(1, passwd);
    execStatement.bind(2, userName);
    execStatement.exec();

    transaction.commit();
  } catch (SQLite::Exception &e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return false;
  }

  return true;
}

std::string DBhelper::queryPasswdByUserName(const std::string &tableName,
                                            const std::string &userName) {
  try {
    auto sqlString =
        "SELECT password FROM " + tableName + " WHERE username = ?";
    auto execStatement = SQLite::Statement(*db, sqlString);
    execStatement.bind(1, userName);

    while (execStatement.executeStep()) {
      return execStatement.getColumn(0);
    }
  } catch (SQLite::Exception &e) {
    std::cerr << "Error: " << e.what() << std::endl;
  }
  return "";
}

std::vector<std::string>
DBhelper::queryDeviceByUserName(const std::string &userName) {
  std::vector<std::string> result;
  try {
    auto sqlString =
        "SELECT deviceId FROM user_info INNER JOIN device_info ON "
        "user_info.userId = device_info.userId AND user_info.username = ?";
    auto execStatement = SQLite::Statement(*db, sqlString);
    execStatement.bind(1, userName);
    while (execStatement.executeStep()) {
      result.push_back(execStatement.getColumn(0));
    }
  } catch (SQLite::Exception &e) {
    std::cerr << "Error: " << e.what() << std::endl;
  }
  return result;
}

std::string DBhelper::queryIdByUserName(const std::string &userName) {
  try {
    auto sqlString = "SELECT userId FROM  user_info  WHERE username = ?";
    auto execStatement = SQLite::Statement(*db, sqlString);
    execStatement.bind(1, userName);

    while (execStatement.executeStep()) {
      return execStatement.getColumn(0);
    }
  } catch (SQLite::Exception &e) {
    std::cerr << "Error: " << e.what() << std::endl;
  }
  return "";
}

bool DBhelper::hasUserName(const std::string &tableName,
                           const std::string &userName) {
  try {
    auto sqlString = "SELECT * FROM " + tableName + " WHERE username = ?";
    auto execStatement = SQLite::Statement(*db, sqlString);
    execStatement.bind(1, userName);
    while (execStatement.executeStep()) {
      return true;
    }
  } catch (SQLite::Exception &e) {
    std::cerr << "Error: " << e.what() << std::endl;
  }
  return false;
}

void DBhelper::queryBindedDevicesById(const std::string &userId,
                                      std::vector<std::string> &results) {
  try {
    auto sqlString = "SELECT deviceId FROM device_info WHERE username = ?";
    auto execStatement = SQLite::Statement(*db, sqlString);
    execStatement.bind(1, userId);
    while (execStatement.executeStep()) {
      results.push_back(execStatement.getColumn(0));
    }
  } catch (SQLite::Exception &e) {
    std::cerr << "Error: " << e.what() << std::endl;
  }
}

void DBhelper::queryBindedDevicesByUserName(const std::string &userName,
                                            std::vector<std::string> &results) {
  try {
    auto sqlString =
        "SELECT deviceId FROM device_info INNER JOIN user_info ON "
        "user_info.userId = device_info.userId AND user_info.username=?";
    auto execStatement = SQLite::Statement(*db, sqlString);
    execStatement.bind(1, userName);
    while (execStatement.executeStep()) {
      results.push_back(execStatement.getColumn(0));
    }
  } catch (SQLite::Exception &e) {
    std::cerr << "Error: " << e.what() << std::endl;
  }
}