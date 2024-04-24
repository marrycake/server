#ifndef MACROS_H
#define MACROS_H

#define NO_COPYABLE(className)                                                 \
  className(const className &) = delete;                                       \
  className &operator=(const className &) = delete;

#define NO_MOVEABLE(className)                                                 \
  className(className &&) = delete;                                            \
  className &operator=(className &&) = delete;

#endif // MACROS_H