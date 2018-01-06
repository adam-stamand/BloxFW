#ifndef LABELED_BOX_H
#define LABELED_BOX_H

#include "box.h"
#include <assert.h>
#include <map>

namespace bx{


template <typename INDEX, typename ITEM>
class labeled_box
{
public:

  INDEX add(ITEM item, std::string itemName);
  INDEX add(ITEM item, INDEX itemIndex);
  ITEM remove(std::string itemName);
  ITEM remove(INDEX itemIndex);
  ITEM  at(std::string itemName);
  ITEM  at(INDEX itemIndex);
  bool valid(std::string itemName);
  bool valid(INDEX itemIndex);
  size_t size(){return items.size();}
  std::string get_label(INDEX itemIndex);
  INDEX get_index(std::string itemName);

private:
  std::map<std::string, INDEX> itemNames;
  box<INDEX,ITEM> items;
};





template <typename INDEX, typename ITEM>
std::string labeled_box<INDEX,ITEM>::get_label(INDEX itemIndex){
  for (auto iter = itemNames.begin(); iter != itemNames.end(); iter++){
    if (iter->second == itemIndex){
      return iter->first;
    }
  }
  return "";
}

template <typename INDEX, typename ITEM>
INDEX labeled_box<INDEX,ITEM>::get_index(std::string itemName){
  auto iter = itemNames.find(itemName);
  if (iter != itemNames.end()){
    return iter->second;
  }
  return 0;
}


template <typename INDEX, typename ITEM>
bool labeled_box<INDEX,ITEM>::valid(std::string itemName){
  auto iter = itemNames.find(itemName);
  if (iter != itemNames.end()){
    return false;
  }
  return items.valid(iter->second);
}

template <typename INDEX, typename ITEM>
bool labeled_box<INDEX,ITEM>::valid(INDEX itemIndex){
  return items.valid(itemIndex);
}

template <typename INDEX, typename ITEM>
ITEM labeled_box<INDEX,ITEM>::at(std::string itemName){
  auto iter = itemNames.find(itemName);
  assert(iter != itemNames.end());
  return items.at(iter->second);
}


template <typename INDEX, typename ITEM>
ITEM labeled_box<INDEX,ITEM>::at(INDEX itemIndex){
  return items.at(itemIndex);
}


template <typename INDEX, typename ITEM>
INDEX labeled_box<INDEX,ITEM>::add(ITEM item, std::string itemName){
  auto iter = itemNames.find(itemName); // Prevent duplicates
  assert(iter == itemNames.end());

  INDEX index = items.add(item);
  itemNames.insert(std::pair<std::string,INDEX>(itemName, index));
  return index;
}


template <typename INDEX, typename ITEM>
INDEX labeled_box<INDEX,ITEM>::add(ITEM item, INDEX itemIndex){
  assert(0); //TODO Look into
}


template <typename INDEX, typename ITEM>
ITEM labeled_box<INDEX,ITEM>::remove(INDEX itemIndex){
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



template <typename INDEX, typename ITEM>
ITEM labeled_box<INDEX,ITEM>::remove(std::string itemName){
  auto iter = itemNames.find(itemName);
  assert(iter != itemNames.end());
  INDEX index = iter->second;

  itemNames.erase(iter);
  return items.remove(index);
}















}
#endif
