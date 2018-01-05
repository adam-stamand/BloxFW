#include "debug.h"
#include <iostream>

using namespace bx;


void DebugLog(DebugLevel lvl, std::string message){
  switch(lvl){
    case BLOX_ACTIVITY:
      std::cout << "ACTIVITY ";
      break;

    case BLOX_ERROR:
      std::cout << "ERROR";
      break;
  }

  std::cout << message << std::endl;
}
