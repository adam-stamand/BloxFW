#ifndef BOX_H
#define BOX_H

#include <vector>
#include <stdint.h>
#include <map>
#include <assert.h>

#include "Block.h"
#include "../utils/types.h"
#include "../utils/labeled_container.h"
#include "../tools/Debug.h"

namespace bx{

class Box
{
public:
  // Name Must be provided upon instantiation
  Box(std::string name): name(name){}
  virtual ~Box();

  // Getters
  BoxID GetID();
  std::string GetName();
  Box * GetBox();
  ManagerBox *   GetManagerBox();
  std::string Print(); // for debug

protected:

  // Add/Get/Remove Blocks
  int AddBlocks(std::vector<Block*> blocks); //TODO look into return value
  template <typename T>
  Block * GetBlock(T blockIdentifier);
  template <typename T>
  int RemoveBlock(T blockIdentifier);

  // Add/Get/Remove Boxes
  int AddBox(Box * box);
  template <typename T>
  Box * GetBox(T boxIdentifier);
  template <typename T>
  int RemoveBox(T boxIdentifier);

  // Iterators for contained boxes and blocks
  std::map<std::string, BlockID>::const_iterator block_begin(){return blocks.begin();}
  std::map<std::string, BlockID>::const_iterator block_end(){return blocks.end();}
  std::map<std::string, BoxID>::const_iterator box_begin(){return boxes.begin();}
  std::map<std::string, BoxID>::const_iterator box_end(){return boxes.end();}




  friend class ManagerBox;
  friend class Block;

private:
  BoxID GetLocalID(); //TODO look into how to handle ID's
  BoxID GetGlobalID(); //TODO look into how to handle ID's
  int RemoveFromManager();
  int AddToManager(ManagerBox * manager);
  void SetBox(Box * box);
  void SetLocalID(BoxID boxID);
  void SetGlobalID(BoxID boxID);

  // Publishing and Subscription System for Intra Entity communication
  int AddSubscription(Subscription &sub, std::string subName);
  int AddSubscription(Subscription &sub, SubscriptionID subID);
  int RemoveSubscription(SubscriptionReceipt &rect);

  template <typename T>
  int PublishMessage(Message & msg, T subIdentifier);

  // Private Members
  ManagerBox * manager = NULL;
  Box * box = NULL;
  const std::string name;
  BoxID globalID;
  BoxID localID;
  labeled_container<MessageID,container<SubscriptionID,Subscription>*> subscriptions;
  labeled_container<BoxID,Box*> boxes;
  labeled_container<BlockID,Block*> blocks;
};



template <typename T>
Box * Box::GetBox(T boxIdentifier){
  BoxItem item;
  int rv = this->boxes.get(item, boxIdentifier);
  if (rv != 0){
    return NULL;
  }
  return item.data;
}

template <typename T>
int Box::RemoveBox(T boxIdentifier){
  Box * box = GetBox(boxIdentifier);
  if (box == NULL){
    return -1;
  }
  if (this->manager != NULL){ // TODO consider added flag
    box->RemoveFromManager(); //TODO consider rv
  }
  box->SetBox(NULL);

  #ifdef BLOX_DEBUG
  DebugLog(BLOX_ACTIVITY, "Box Removed", box->Print());
  #endif

  BoxItem item;
  return this->boxes.remove(item, boxIdentifier);
}

template <typename T>
Block * Box::GetBlock(T blockIdentifier){
  BlockItem item;
  int rv = this->blocks.get(item, blockIdentifier);
  if (rv != 0){
    DebugLog(BLOX_ERROR, "Failed Get Block", this->Print());
    return NULL;
  }
  return item.data;
}


template <typename T>
int Box::RemoveBlock(T blockIdentifier){
  Block * block = GetBlock(blockIdentifier);
  if (block == NULL){
    return -1;
  }

  if (this->manager != NULL){
    block->RemoveFromManager();
  }

  block->SetBox(NULL);
  #ifdef BLOX_DEBUG
  DebugLog(BLOX_ACTIVITY, "Block Removed", block->Print());
  #endif

  BlockItem item;
  return this->blocks.remove(item, blockIdentifier);
}



template <typename T>
int Box::PublishMessage(Message & msg, T subIdentifier){
  MessageItem item;
  int rv = subscriptions.get(item, subIdentifier);
  if (rv != 0){
    #ifdef BLOX_DEBUG
    DebugLog(BLOX_ERROR, "Publish Failed; No Subscriptions", Print());
    #endif
    return rv;
  }

  container<SubscriptionID,Subscription>* subs = item.data;

  for (auto iter = subs->begin(); iter!=subs->end(); iter++){
    SubscriptionElem elem;
    elem.id = iter->second;
    subs->get(elem);
    elem.data.callback(msg);
  }
  return 0;
}



}


#endif
