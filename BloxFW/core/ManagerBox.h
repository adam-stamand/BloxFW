#ifndef MANAGERBOX_H
#define MANAGERBOX_H

#include "Box.h"
#include "Block.h"
#include "../utils/labeled_container.h"
#include "../utils/types.h"
#include <assert.h>
#include "../tools/Debug.h"

namespace bx{



class ManagerBox : public Box
{
public:
  ManagerBox(std::string boxName);
  ~ManagerBox();

  // Get any box or block manager by ManagerBox
  template <typename T>
  Box * GetManagedBox(T boxIdentifier);
  template <typename T1, typename T2>
  Block * GetManagedBlock(T1 blockIdentifier, T2 boxIdentifier);


  friend class Block;
  friend class Box;




  // Iterators for contained boxes and blocks
  std::map<std::string, BoxID>::const_iterator managedBoxes_begin() { return managedBoxes.begin(); }
  std::map<std::string, BoxID>::const_iterator managedBoxes_end() { return managedBoxes.end(); }



private:


  // Messaging Interface // TODO Protext these as well
  template <typename T1, typename T2>
  int Subscribe(Subscription &sub, T1 subIdentifier, T2 boxIdentifier);
  template <typename T>
  int Subscribe(Subscription &sub, T subIdentifier){return Subscribe(sub, subIdentifier, this->GetGlobalID());}


  int Unsubscribe(SubscriptionReceipt &rect);

  template <typename T1, typename T2>
  int Publish(Message & msg, T1 subIdentifier, T2 boxIdentifier);
  template <typename T>
  int Publish(Message  & msg, T subIdentifier){return Publish(msg, subIdentifier, this->GetGlobalID());}

  int RegisterBox(Box * box);
  int DeregisterBox(Box * box);
  // Modify Boxes
  labeled_container<BoxID, Box*> managedBoxes;
};


// Modify Boxes
template <typename T>
Box * ManagerBox::GetManagedBox(T boxIdentifier){
  BoxItem item;
  int rv = this->managedBoxes.get(item, boxIdentifier);
  if (rv != 0){
    #ifdef BLOX_DEBUG
    DebugLog(BLOX_ERROR, "ManagerBox Failed Get Box", this->Print());
    #endif
    return NULL;
  }
  return item.data;
}


// Modify Boxes
template <typename T1, typename T2>
Block * ManagerBox::GetManagedBlock(T1 blockIdentifier, T2 boxIdentifier){
  Box *box = GetManagedBox(boxIdentifier);
  if (box == NULL){
    return box;
  }
  return box->GetBlock(blockIdentifier);
}


template <typename T1, typename T2>
int ManagerBox::Subscribe(Subscription &sub, T1 subIdentifier, T2 boxIdentifier){
  Box * box = GetManagedBox(boxIdentifier);
  if (box == NULL){
    return -1;
  }
  sub.box = box;
  return box->AddSubscription(sub, subIdentifier);
}




template <typename T1, typename T2>
int ManagerBox::Publish(Message & msg, T1 subIdentifier, T2 boxIdentifier){
  Box * box = GetManagedBox(boxIdentifier);
  if (box == NULL){
    return -1;
  }
  return box->PublishMessage(msg, subIdentifier);
}


}
#endif
