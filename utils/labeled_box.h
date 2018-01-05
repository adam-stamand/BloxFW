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

  INDEX add(ITEM &item, std::string itemName);
  ITEM remove(std::string itemName);
  ITEM remove(INDEX itemIndex);
  INDEX at(ITEM &item, std::string itemName);
  INDEX at(ITEM &item, INDEX itemIndex);
  size_t size(){return box.size()};

private:
  std::map<std::string, INDEX> itemNames;
  box<INDEX,ITEM> items;
};











template <typename INDEX, typename ITEM>
INDEX labeled_box<INDEX,ITEM>::at(ITEM &item, std::string itemName){
  auto iter = itemNames.find(itemName);
  if (iter != itemNames.end()){
    item = items.at(iter->second);
    return iter->first;
  }
  return 0;
}


template <typename INDEX, typename ITEM>
INDEX labeled_box<INDEX,ITEM>::at(ITEM &item, INDEX itemIndex){
  item = items.at(itemIndex); // consider doing a check to and return 0 if not valid
  return itemIndex;
}


template <typename INDEX, typename ITEM>
INDEX labeled_box<INDEX,ITEM>::add(ITEM &item, std::string itemName){
  auto iter = itemNames.find(itemName);
  if (iter == itemNames.end()){ //name hasn't been used yet
    INDEX index = items.add(item);
    itemNames.insert(std::pair<std::string,INDEX>(name, index));
    return index;
  }
  // name already in use
  return 0;
}


template <typename INDEX, typename ITEM>
ITEM labeled_box<INDEX,ITEM>::remove(INDEX itemIndex){
  for (auto iter = itemNames.begin(); iter != itemNames.end(); iter++){
    if (iter->second == itemIndex){
      itemNames.erase(iter);
      return items.remove(itemIndex);
    }
  }
  assert(0); //temp
  return 0;
  // TODO handle if no item existss
}



template <typename INDEX, typename ITEM>
ITEM labeled_box<INDEX,ITEM>::remove(std::string itemName){
  INDEX index;
  auto iter = itemNames.find(itemName);
  if (iter != itemNames.end()){
    index =  iter->second;
    itemNames.erase(iter);
  } else{
    assert(0);
    // TODO handle if no item existss
  }

  return items.remove(index);
}















}
#endif
