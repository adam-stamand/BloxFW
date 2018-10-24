#include <string>
#include "Debug.h"


using namespace bx;


void bx::DebugLog(DebugLevel lvl, std::string event, std::string message){
  switch(lvl){
    case BLOX_ACTIVITY:
      std::cout << "ACTIVITY:\t";
      break;

    case BLOX_ERROR:
      std::cout << "ERROR:\t";
      errors++;
      break;
      
    default:
      break;
  }

 std::cout << "--" + event + "--\t" + message << std::endl;
}

uint32_t bx::errors = 0;
