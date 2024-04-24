#include "dataDef.h"
#include <list>
#include <string>
#include <utility>

constexpr unsigned int CARRY_NUM = 0x0A;

unsigned int stringToInt(const char *, char);

inline unsigned int stringToInt(const std::string &str) {
  return stringToInt(str.c_str(), '\0');
}

inline unsigned int stringToInt(const char *str) {
  return stringToInt(str, '\0');
}

const char *findCharNextTo(const char *, char);

std::pair<unsigned int, std::string> extractIdAndKey(const char *);

std::list<std::string> splitStringBy(const char *str, char spliter);

UserMessageInfo extraUserInfo(char *buffer);