#ifndef TYPES_H
#define TYPES_H

namespace bx
{


class Component;

typedef uint32_t MessageID;
typedef uint32_t ComponentID;
typedef uint32_t ContainerID;
typedef uint32_t SubscriptionID;

struct Message{
  Component * publisher;
  void * params;
};
typedef std::function<void(Message const &)> MessageFunction;


struct Subscription{
  Component * subscriber;
  MessageFunction callback;
};

}
#endif
