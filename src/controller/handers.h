#include <httplib.h>

extern void loginHander(const httplib::Request &req, httplib::Response &res);
extern void registerHander(const httplib::Request &req, httplib::Response &res);
extern void commandForwarder(const httplib::Request &req,
                             httplib::Response &res);
extern void changePasswd(const httplib::Request &req, httplib::Response &res);
extern void bindDevice(const httplib::Request &req, httplib::Response &res);
extern void getBindedDevice(const httplib::Request &req,
                            httplib::Response &res);
extern void successfulInfo(std::string message, httplib::Response &res);