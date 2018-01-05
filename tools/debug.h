#ifndef DEBUG_H
#define DEBUG_H


#include <sstream>

namespace bx{

#define BLOX_DEBUG

enum DebugLevel{
  BLOX_ACTIVITY,
  BLOX_ERROR
};

void DebugLog(DebugLevel lvl, std::string message);

}

#endif
