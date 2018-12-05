#ifndef Manager_H
#define Manager_H

#include <BloxFW/core/Box.h>
#include <BloxFW/core/Block.h>
#include <BloxFW/utils/container.h>
#include <BloxFW/utils/types.h>
#include <assert.h>
#include <BloxFW/tools/Debug.h>

namespace bx{



class Manager : public Box
{
public:
  Manager(std::string boxName);
  ~Manager();


  // Get any box or block manager by Manager
  Box * GetManagedBox(std::string boxIdentifier);
  Block * GetManagedBlock(std::string blockIdentifier, std::string boxIdentifier);

  // Iterators for contained boxes and blocks
  std::map<std::string, Box*>::const_iterator managedBoxes_begin() { return managedBoxes.begin(); }
  std::map<std::string, Box*>::const_iterator managedBoxes_end() { return managedBoxes.end(); }
  friend class Block;
  friend class Box;
private:


  // Messaging Interface // TODO Protext these as well
  int Subscribe(Subscription &sub, std::string subIdentifier, std::string boxIdentifier);
  int Subscribe(Subscription &sub, std::string subIdentifier){return Subscribe(sub, subIdentifier, this->GetName());}


  int Unsubscribe(const SubscriptionReceipt &rect);


  int Publish(Message & msg, std::string subIdentifier, std::string boxIdentifier);
  int Publish(Message  & msg, std::string subIdentifier){return Publish(msg, subIdentifier, this->GetName());}

  int RegisterBox(Box * box);
  int DeregisterBox(Box * box);
  // Modify Boxes
  container<std::string, Box*> managedBoxes;
};



}
#endif
