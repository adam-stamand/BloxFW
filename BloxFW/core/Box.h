#ifndef BOX_H
#define BOX_H

#include <vector>
#include <stdint.h>
#include <map>
#include <assert.h>

#include <BloxFW/core/Block.h>
#include <BloxFW/utils/types.h>
#include <BloxFW/utils/container.h>
#include <BloxFW/tools/Debug.h>

namespace bx{

class Box
{
public:
  // Name Must be provided upon instantiation
  Box(std::string name): name(name){}
  virtual ~Box();


  // Getters
  std::string GetName();
  std::string GetBoxName();
  Box * GetBox();
  Manager *   GetManager();
  std::string Print(); // for debug

  // Add/Get/Remove Blocks
  int AddBlocks(std::vector<Block*> blocks); //TODO look into return value
  Block * GetBlock(std::string blockIdentifier);
  int RemoveBlock(std::string blockIdentifier);

  // Add/Get/Remove Boxes
  int AddBox(Box * box);
  Box * GetBox(std::string boxIdentifier);
  int RemoveBox(std::string boxIdentifier);

  // Iterators for contained boxes and blocks
  std::map<std::string, Block*>::const_iterator block_begin(){return blocks.begin();}
  std::map<std::string, Block*>::const_iterator block_end(){return blocks.end();}
  std::map<std::string, Box*>::const_iterator box_begin(){return boxes.begin();}
  std::map<std::string, Box*>::const_iterator box_end(){return boxes.end();}

  friend class Manager;
  friend class Block;

private:

  int AddToManager(Manager * manager);
  void AddToBox(Box * box);
  int RemoveFromManager();
  int RemoveFromBox();

  // Publishing and Subscription System
  int AddSubscription(Subscription &sub, std::string subName);
  int RemoveSubscription(const SubscriptionReceipt &rect);
  int PublishMessage(Message & msg, std::string subIdentifier);

  // Private Members
  Manager * manager = NULL;
  Box * box = NULL;
  const std::string name;
  container<std::string,container<SubscriptionID,Subscription>*> subscriptions;
  container<std::string,Box*> boxes;
  container<std::string,Block*> blocks;
};



}


#endif
