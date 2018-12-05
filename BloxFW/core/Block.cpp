#include <BloxFW/core/Block.h>
#include <BloxFW/core/Manager.h>
#include <stdio.h>
#include <iostream>


using namespace bx;

Block::~Block(){
  if (this->GetBox() != NULL){
    this->GetBox()->RemoveBlock(GetName()); //TODO consider adding component ID for this TODO read rv
  }
  #ifdef BLOX_DEBUG
  puts("block deleted");
  //DebugLog(BLOX_ACTIVITY, "Block Deleted", Print()); //TODO long prints in desctructor cause seg fault
  #endif
} // Allow destructor to be overriden


void Block::AddToBox(Box * box){
  this->box = box;
}

std::string Block::GetBoxName(){
  assert(this->box != NULL); //TODO replace assert with BLOX_ASSERT
  return this->box->GetName();
}

Box * Block::GetBox(){
  return this->box;
}


Manager * Block::GetManager (){
  return this->manager;
}


std::string Block::GetName(){
  return this->name;
}


std::string Block::Print(){
  std::string box_str, man_str;
  if (this->GetBox() != NULL){
    box_str = this->GetBox()->GetName();
  }

  if (this->GetManager() != NULL){
    man_str = this->GetManager()->GetName();
  }

    return "Name: " + this->GetName() + "; Parent Box: " + box_str + "; Manager: " + man_str;
}


void Block::AddToManager(Manager * manager){
  assert(manager != NULL);
  this->manager = manager;
  this->Init();
}


void Block::RemoveFromManager(){
  for (auto iter = receipts.begin(); iter != receipts.end(); iter++){
    manager->Unsubscribe(iter->second);
  }
  this->manager = NULL;
}



int Block::SubscribeHelper(Subscription &sub, std::string msgIdentifier, std::string boxIdentifier){
  static uint32_t receipt_counter = 0; //TODO figure out how to label receitps
  int rv = manager->Subscribe(sub, msgIdentifier, boxIdentifier);
  if (rv != 0){
    return -1;
  }
  SubscriptionReceipt rect;
  rect.subID = sub.subID;
  rect.msgName = msgIdentifier;
  rect.box = sub.box;

  ReceiptElem elem;
  elem.data = rect;
  elem.id = receipt_counter++;
  receipts.add(elem); // reciept copied here; TODO consider allocating above to prevent copy
  return elem.id;
}


int Block::UnsubscribeMessage(ReceiptID rectID){
  assert(this->manager != NULL); //TODO do receipts ever get deleted?
  ReceiptElem elem;
  elem.id = rectID;
  receipts.get(elem);
  return manager->Unsubscribe(elem.data);
}


int Block::PublishMessage(Message &msg, std::string msgIdentifier, std::string boxIdentifier){
  if (this->manager == NULL){
    #ifdef BLOX_DEBUG
    DebugLog(BLOX_ERROR, "Publish Failed; No Manager", GetName());
    #endif
    return -1;
  }
  msg.publisher = this;
  return manager->Publish(msg, msgIdentifier, boxIdentifier);
}
