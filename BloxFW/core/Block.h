
#ifndef BLOCK_H
#define BLOCK_H

#include <stdint.h>
#include <string>
#include <functional>
#include <assert.h>
#include "../tools/Debug.h"
#include "../utils/types.h"



namespace bx{

class ManagerBox;
class Box;



class Block
{

public:
  // Must give Block Name upon instantiation
  Block(std::string compName) : name(compName){}
  virtual ~Block();

  // Getters
  std::string GetName();
  Box * GetBox();
  ManagerBox * GetManagerBox();
  std::string Print(); // for debug

  // Subscription and Publishing System //TODO how to return error value??
  template <class T1, class T2, class T3>
  ReceiptID SubscribeMessage(void (T1::*f)(Message &), T2 msgIdentifier, T3 boxIdentifier); //TODO shitty interface
  template <class T1, class T2>
  ReceiptID SubscribeMessage(void (T1::*f)(Message &), T2 msgIdentifier) {return SubscribeMessage(f, msgIdentifier, this->GetBoxID());}

  int UnsubscribeMessage(ReceiptID rectID);

  template <class T1, class T2>
  int PublishMessage(Message &msg, T1 msgIdentifier, T2 boxIdentifier);
  template <class T>
  int PublishMessage(Message &msg, T msgIdentifier){return PublishMessage(msg, msgIdentifier, this->GetBoxID());}





  friend class Box;



private:

  void RemoveFromManager();
  BlockID GetBoxID(); //TODO fix hack maybe?
  virtual void Init(){}; // User can define behavior when component is added to ManagerBox
  void AddToManager(ManagerBox * manager);
  void SetBox(Box * cont);


  // Message Helpers //TODO FIx this shit
  int SubscribeHelper(Subscription &sub, std::string msgIdentifier, std::string boxIdentifier);
  int SubscribeHelper(Subscription &sub, SubscriptionID msgIdentifier, std::string boxIdentifier);
  int SubscribeHelper(Subscription &sub, std::string msgIdentifier, BoxID boxIdentifier);
  int SubscribeHelper(Subscription &sub, SubscriptionID msgIdentifier, BoxID boxIdentifier);

  int PublishHelper(Message  & msg, std::string msgIdentifier, std::string boxIdentifier);
  int PublishHelper(Message  & msg, SubscriptionID msgIdentifier, std::string boxIdentifier);
  int PublishHelper(Message  & msg, std::string msgIdentifier, BoxID boxIdentifier);
  int PublishHelper(Message  & msg, SubscriptionID msgIdentifier, BoxID boxIdentifier);




  container<ReceiptID, SubscriptionReceipt> receipts;

  // Private members
  Box * box  = NULL;
  ManagerBox * manager   = NULL;
  const std::string   name;
};


template <class T1, class T2, class T3>
ReceiptID Block::SubscribeMessage(void (T1::*f)(Message &), T2 msgIdentifier, T3 boxIdentifier){
  assert(this->manager != NULL);
  assert(f != NULL);
  Subscription sub;
  MessageFunction function = std::bind(f, (T1*)(this), std::placeholders::_1);
  sub.subscriber = this;
  sub.callback = function;
  return SubscribeHelper(sub, msgIdentifier, boxIdentifier);
}



template <class T1, class T2>
int Block::PublishMessage(Message &msg, T1 msgIdentifier, T2 boxIdentifier){
  if (this->manager == NULL){
    #ifdef BLOX_DEBUG
    DebugLog(BLOX_ERROR, "Publish Failed; No Manager", GetName());
    #endif
    return -1;
  }
  msg.publisher = this;
  return PublishHelper(msg, msgIdentifier, boxIdentifier);
}


}

#endif
