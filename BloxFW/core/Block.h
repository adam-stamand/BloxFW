
#ifndef BLOCK_H
#define BLOCK_H

#include <stdint.h>
#include <string>
#include <functional>
#include <assert.h>
#include <BloxFW/tools/Debug.h>
#include <BloxFW/utils/types.h>



namespace bx{

class Manager;
class Box;



class Block
{

public:
  // Must give Block Name upon instantiation
  Block(std::string compName) : name(compName){}
  virtual ~Block();



  // Getters
  std::string GetName();
  std::string GetBoxName();
  Box * GetBox();
  Manager * GetManager(); 
  std::string Print(); // for debug

  // Subscription and Publishing System //TODO how to return error value??
  template <class T1>
  ReceiptID SubscribeMessage(void (T1::*f)(Message &), std::string msgIdentifier, std::string boxIdentifier); //TODO shitty interface
  template <class T1>
  ReceiptID SubscribeMessage(void (T1::*f)(Message &), std::string msgIdentifier) {return SubscribeMessage(f, msgIdentifier, this->GetBoxName());}

  int UnsubscribeMessage(ReceiptID rectID);
  int PublishMessage(Message &msg, std::string msgIdentifier, std::string boxIdentifier);
  int PublishMessage(Message &msg, std::string msgIdentifier){return PublishMessage(msg, msgIdentifier, this->GetBoxName());}

  friend class Box;
    friend class Manager;

private:

  virtual void Init(){}; // User can define behavior when component is added to Manager
  void AddToBox(Box * cont); //rename to AddToBox
  void AddToManager(Manager * manager);
  void RemoveFromBox();
  void RemoveFromManager();// TODO add RemoveFromBox()

  // Message Helpers
  int SubscribeHelper(Subscription &sub, std::string msgIdentifier, std::string boxIdentifier);
  container<ReceiptID, SubscriptionReceipt> receipts;

  // Private members
  Box * box  = NULL;
  Manager * manager   = NULL;
  const std::string   name;
};


template <class T1>
ReceiptID Block::SubscribeMessage(void (T1::*f)(Message &), std::string msgIdentifier, std::string boxIdentifier){
  assert(this->manager != NULL);
  assert(f != NULL);
  Subscription sub; // copied into container later on TODO consider allocating here to prevent unecessary copying
  MessageFunction function = std::bind(f, (T1*)(this), std::placeholders::_1);
  sub.subscriber = this;
  sub.callback = function;
  return SubscribeHelper(sub, msgIdentifier, boxIdentifier);
}


}

#endif
