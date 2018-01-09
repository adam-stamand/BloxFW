#ifndef LABELED_BOX_H
#define LABELED_BOX_H

#include "box.h"
#include <assert.h>
#include <map>

namespace bx{


template <typename ID, typename ITEM>
class labeled_box
{

public:
  typedef std::map<std::string, ID>::iterator _pair;
  _pair   insert(ITEM item, std::string itemName);
  _pair   erase(std::string itemName);
  _pair   erase(ID itemIndex);
  _pair   find(std::string itemName);
  _pair   find(ID itemIndex);
  bool    valid(std::string itemName);
  bool    valid(ID itemIndex);
  size_t  size(){return items.size();}

private:
  std::map<std::string, ID> itemNames;
  box<ID,ITEM> items;
};






template <typename ID, typename ITEM>
bool labeled_box<ID,ITEM>::valid(std::string itemName){
  auto iter = itemNames.find(itemName);
  if (iter != itemNames.end()){
    return false;
  }
  return items.valid(iter->second);
}

template <typename ID, typename ITEM>
bool labeled_box<ID,ITEM>::valid(ID itemIndex){
  return items.valid(itemIndex);
}

template <typename ID, typename ITEM>
ITEM labeled_box<ID,ITEM>::at(std::string itemName){
  auto iter = itemNames.find(itemName);
  assert(iter != itemNames.end());
  return items.at(iter->second);
}


template <typename ID, typename ITEM>
ITEM labeled_box<ID,ITEM>::at(ID itemIndex){
  return items.at(itemIndex);
}


template <typename ID, typename ITEM>
ID labeled_box<ID,ITEM>::add(ITEM item, std::string itemName){
  auto iter = itemNames.find(itemName); // Prevent duplicates
  assert(iter == itemNames.end());

  ID index = items.add(item);
  itemNames.insert(std::pair<std::string,ID>(itemName, index));
  return index;
}


template <typename ID, typename ITEM>
ID labeled_box<ID,ITEM>::add(ITEM item, ID itemIndex){
  assert(0); //TODO Look into
}


template <typename ID, typename ITEM>
ITEM labeled_box<ID,ITEM>::remove(ID itemIndex){
  // Find the item by index
  auto iter = itemNames.begin();
  for (iter; iter != itemNames.end(); iter++){
    if (iter->second == itemIndex){
      break;
    }
  }
  assert(iter != itemNames.end()); // Invalid index

  itemNames.erase(iter);
  return items.remove(itemIndex);
}



template <typename ID, typename ITEM>
ITEM labeled_box<ID,ITEM>::remove(std::string itemName){
  auto iter = itemNames.find(itemName);
  assert(iter != itemNames.end());
  ID index = iter->second;

  itemNames.erase(iter);
  return items.remove(index);
}















}
#endif
