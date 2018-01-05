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
  int at(ITEM &item, std::string itemName);
  int at(ITEM &item, INDEX itemIndex);
  size_t size(){return box.size()};

private:
  std::map<std::string, INDEX> itemNames;
  box<INDEX,ITEM> items;
};














template <typename INDEX, typename ITEM>
int labeled_box<INDEX,ITEM>::at(ITEM &item, std::string itemName){
  auto iter = itemNames.find(itemName);
  if (iter != itemNames.end()){
    item = items.at(iter->second);
    return 0;
  }
  return -1;
}


template <typename INDEX, typename ITEM>
int labeled_box<INDEX,ITEM>::at(ITEM &item, INDEX itemIndex){
  item = items.at(itemIndex);
  return 0;
}


template <typename INDEX, typename ITEM>
INDEX labeled_box<INDEX,ITEM>::add(ITEM &item, std::string name){
  INDEX index = items.add(item);
  itemNames.insert(std::pair<std::string,INDEX>(name, index));
  return index;
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
