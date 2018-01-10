#ifndef TYPES_H
#define TYPES_H

#include "labeled_box.h"
#include <vector>

namespace bx
{


class Component;
class Container;

class Message
{
public:
    Component * publisher;
};

typedef uint32_t MessageID;
typedef uint32_t ComponentID;
typedef uint32_t ContainerID;
typedef uint32_t SubscriptionID;

typedef std::function<void(Message &)> MessageFunction;


struct Subscription{
  Component * subscriber;
  MessageFunction callback;
  SubscriptionID id;
  Container * subscribee;
};

typedef _Item<SubscriptionID, std::vector<Subscription>*> SubscriptionItem;
typedef _Item<ContainerID, Container*> ContainerItem;
typedef _Item<ComponentID, Component*> ComponentItem;
typedef _Item<MessageID, Message> MessageItem;


}
#endif
