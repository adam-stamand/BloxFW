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

typedef uint32_t ReceiptID;
typedef uint32_t MessageID;
typedef uint32_t ComponentID;
typedef uint32_t ContainerID;
typedef uint32_t SubscriptionID;

typedef std::function<void(Message &)> MessageFunction;


struct Subscription{
  Component * subscriber;
  MessageFunction callback;

  SubscriptionID subID;
  MessageID msgID;
  Container * cont;
};

struct SubscriptionReceipt{
  MessageID msgID;
  SubscriptionID subID;
  Container * cont;
}; // TODO put recipte inside subscipritons

typedef _Element<SubscriptionID,Subscription> SubscriptionElem;
typedef _Element<ReceiptID, SubscriptionReceipt> ReceiptElem;
typedef _Item<MessageID, box<SubscriptionID,Subscription>*> MessageItem;
typedef _Item<ContainerID, Container*> ContainerItem;
typedef _Item<ComponentID, Component*> ComponentItem;

}
#endif
