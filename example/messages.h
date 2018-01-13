#ifndef MESSAGES_H
#define MESSAGES_H


#include "../utils/types.h"

class TestMessage : public bx::Message
{
public:
  int test = 0;
};

class PositionMessage : public bx::Message
{
public:
  float x = 0;
  float y = 0;
};


class VelocityMessage : public bx::Message
{
public:
  float x = 0;
  float y = 0;
};



class VectorMessage : public bx::Message
{
public:
  float magnitude = 0;
  float angle = 0;
};




#endif
