#include "utils.h"
#include <string>

unsigned int stringToInt(const char *str, char end) {
  unsigned int result = 0;
  while (*str != end) {
    result = result * CARRY_NUM + (*str - '0');
    ++str;
  }
  return result;
}

const char *findCharNextTo(const char *str, char split) {

  if (split == '\0') {
    return nullptr;
  }
  while (*str != split && *str != '\0') {
    ++str;
  }
  if (*str == '\0') { // not found split
    return nullptr;
  }
  return ++str; // return next char
}

std::pair<unsigned int, std::string> extractIdAndKey(const char *str) {
  unsigned int id = stringToInt(str, ';');
  const char *key = findCharNextTo(str, ';');
  return std::make_pair(id, std::string(key));
}

std::list<std::string> splitStringBy(const char *str, char spliter) {
  std::list<std::string> mList;
  const char *currentHead;
  int currentSize;
  while (*str != '\0') {
    currentSize = 0;
    currentHead = str;
    while (*str != '\0' && *str != spliter) {
      ++str;
      ++currentSize;
    }
    mList.emplace_back(currentHead, currentSize);
    if (*str == '\0') {
      break;
    }
    ++str;
  }
  return mList;
}

UserMessageInfo extraUserInfo(char *buffer) {
  auto params = splitStringBy(buffer, ';');
  auto userId = params.front();
  auto bindedDeviceId = stringToInt(params.back());
  return {userId, bindedDeviceId};
}
