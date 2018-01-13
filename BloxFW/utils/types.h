#ifndef TYPES_H
#define TYPES_H

#include "labeled_container.h"
#include <vector>

namespace bx
{


class Block;
class Box;

class Message
{
public:
    Block * publisher;
};

typedef uint32_t ReceiptID;
typedef uint32_t MessageID;
typedef uint32_t BlockID;
typedef uint32_t BoxID;
typedef uint32_t SubscriptionID;

typedef std::function<void(Message &)> MessageFunction;


struct Subscription{
  Block * subscriber;
  MessageFunction callback;

  SubscriptionID subID;
  MessageID msgID;
  Box * box;
};

struct SubscriptionReceipt{
  MessageID msgID;
  SubscriptionID subID;
  Box * box;
}; // TODO put recipte inside subscipritons

typedef _Element<SubscriptionID,Subscription> SubscriptionElem;
typedef _Element<ReceiptID, SubscriptionReceipt> ReceiptElem;
typedef _Item<MessageID, container<SubscriptionID,Subscription>*> MessageItem;
typedef _Item<BoxID, Box*> BoxItem;
typedef _Item<BlockID, Block*> BlockItem;

}
#endif
