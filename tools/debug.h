#ifndef DEBUG_H
#define DEBUG_H

#include <iostream>

namespace bx{

#define BLOX_DEBUG

enum DebugLevel{
  BLOX_ACTIVITY,
  BLOX_ERROR
};

void DebugLog(DebugLevel lvl, std::string event, std::string message);

}

#endif
