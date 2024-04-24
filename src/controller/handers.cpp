#include "handers.h"
#include "../common.h"
#include "DBhelper.h"
#include "json.hpp"
#include "netWorkManagerController.h"
#include "udpForwarderManager.h"
#include "userManager.h"
#include "utils.h"
#include <httplib.h>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

using json = nlohmann::json;

void loginHander(const httplib::Request &req, httplib::Response &res) {
  auto userName = req.get_param_value("userName");
  auto passwd = req.get_param_value("passwd");
  json responseJson;
  if (UserManager::loginIsValid(userName, passwd)) {
    responseJson["status"] = "success";
    responseJson["message"] = "Login successful";
    res.set_content(responseJson.dump(), "application/json");
    res.status = httplib::OK_200; // OK status code
  } else {
    responseJson["status"] = "error";
    responseJson["message"] = "Invalid credentials";
    res.set_content(responseJson.dump(), "application/json");
    res.status = httplib::Forbidden_403; // Unauthorized status code
  }
}

void registerHander(const httplib::Request &req, httplib::Response &res) {
  auto userName = req.get_param_value("userName");
  auto passwd = req.get_param_value("passwd");
  UserInfo user(userName, passwd);
  json responseJson;
  if (UserManager::userRegister(user) == RegStatu::USERNAMEHAVEEXIST) {
    responseJson["status"] = "error";
    responseJson["message"] = "User name have existed";
    res.set_content(responseJson.dump(), "application/json");
    res.status = httplib::Forbidden_403; // Unauthorized status code
  } else {
    responseJson["status"] = "success";
    responseJson["message"] = "Register success";
    res.set_content(responseJson.dump(), "application/json");
    res.status = httplib::OK_200; // Unauthorized status code
  }
}

void changePasswd(const httplib::Request &req, httplib::Response &res) {
  auto userName = req.get_param_value("userName");
  auto passwd = req.get_param_value("passwd");
  json responseJson;
  if (UserManager::changePasswd(userName, passwd)) {
    responseJson["status"] = "success";
    responseJson["message"] = "Change password success";
    res.set_content(responseJson.dump(), "application/json");
    res.status = httplib::OK_200; // Unauthorized status code
  } else {
    responseJson["status"] = "error";
    responseJson["message"] = "UserName does not exist";
    res.set_content(responseJson.dump(), "application/json");
    res.status = httplib::Forbidden_403; // Unauthorized status code
  }
}

void commandForwarder(const httplib::Request &req, httplib::Response &res) {
  auto deviceId = req.get_param_value("deviceId");
  auto command = req.get_param_value("command");
  auto need_response = req.get_param_value("need_response");

  json responseJson;

  auto *nManger = NetWorkManagerController::getGlobalInstance();

  if (command == CLOSE_DEVICE_COMMAND) {
    nManger->closeDevice(deviceId);
    responseJson["status"] = "success";
    responseJson["message"] = "";
    res.set_content(responseJson.dump(), "application/json");
    res.status = httplib::OK_200; // Unauthorized status code
  } else if (command == START_MONITOR_COMMAND) {
    auto port = UdpForwarderManager::getInstance()
                    ->registerUdpForwarder(stringToInt(deviceId))
                    ->getUdpListenPort();
    std::cout << "udp port: " << port << "\n";
    command = command + ";" + std::to_string(port);
    nManger->send(deviceId, command);
    successfulInfo("no response", res);
  } else if (command == STOP_MONITOR_COMMAND) {
    UdpForwarderManager::getInstance()->eraseUdpForwarderById(
        stringToInt(deviceId));
    nManger->send(deviceId, command);
    successfulInfo("no response", res);
  } else {
    if (need_response == "y") {
      std::string response;
      nManger->send(deviceId, command, response);
      successfulInfo(std::move(response), res);
      res.status = httplib::OK_200; // Unauthorized status code
    } else {
      nManger->send(deviceId, command);
      successfulInfo("no response", res);
    }
  }
}

void bindDevice(const httplib::Request &req, httplib::Response &res) {
  auto userName = req.get_param_value("userName");
  auto deviceId = req.get_param_value("deviceId");
  auto authorizeKey = req.get_param_value("authorizeKey");

  auto *nManager = NetWorkManagerController::getGlobalInstance();
  DeviceCommander *device = nullptr;

  json responseJson;
  if (nManager->getDeviceById(stringToInt(deviceId), device)) {
    if (authorizeKey == device->getAuthorizeKey()) {
      DBhelper db;
      db.open();
      auto userId = db.queryIdByUserName(userName);
      db.insertDeviceData(DEVICE_INFO_TABLE_NAME, userId, deviceId);

      responseJson["status"] = "success";
      responseJson["message"] = "Device binded successfully";
      res.set_content(responseJson.dump(), "application/json");
      res.status = httplib::OK_200; // Unauthorized status code
    } else {
      responseJson["status"] = "error";
      responseJson["message"] = "Authorizekey uncorrect";
      res.set_content(responseJson.dump(), "application/json");
      res.status = httplib::OK_200; // Unauthorized status code
    }
  } else {
    responseJson["status"] = "error";
    responseJson["message"] = "Cannot find device";
    res.set_content(responseJson.dump(), "application/json");
    res.status = httplib::OK_200; // Unauthorized status code
  }
}

void getBindedDevice(const httplib::Request &req, httplib::Response &res) {
  auto userName = req.get_param_value("userName");

  std::vector<std::string> devices;

  DBhelper db;
  db.open();
  db.queryBindedDevicesByUserName(userName, devices);

  json responseJson;
  responseJson["status"] = "success";
  responseJson["devices"] = devices;

  res.set_content(responseJson.dump(), "application/json");
  res.status = httplib::OK_200; // Unauthorized status code
}

extern void successfulInfo(std::string message, httplib::Response &res) {
  json responseJson;
  responseJson["status"] = "success";
  responseJson["message"] = message;
  res.set_content(responseJson.dump(), "application/json");
  res.status = httplib::OK_200; // Unauthorized status code
}