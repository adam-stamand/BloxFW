#ifndef LABELED_BOX_H
#define LABELED_BOX_H

#include "box.h"
#include <assert.h>
#include <map>

namespace bx{

template <typename ID, typename DATA>
struct _Item{
  ID id;
  std::string name;
  DATA data;
  _Item(DATA data, std::string name):
  name(name), data(data){};
  _Item(){};
};

template <typename ID, typename DATA>
class labeled_box
{

public:

  typedef _Element<ID,DATA> Element;
  typedef _Item<ID,DATA> Item;




  int     add(Item &item);
  template <typename T>
  int     remove(Item &item, T identifier);
  template <typename T>
  int     get(Item &item, T identifier);

  typename std::map<std::string, ID>::iterator begin(){return labelMap.begin();}
  typename std::map<std::string, ID>::iterator end(){return labelMap.end();}
  size_t  size(){return labelMap.size();}

private:
  int searchIdentifier(typename std::map<std::string, ID>::iterator &iter, ID itemID);
  int searchIdentifier(typename  std::map<std::string, ID>::iterator &iter, std::string itemName);
  std::map<std::string, ID> labelMap;
  box<ID,DATA> items;
};




template <typename ID, typename DATA>
template <typename T>
int labeled_box<ID,DATA>::get(Item &item, T identifier){
  auto iter = labelMap.begin();
  int rv = searchIdentifier(iter, identifier);
  if (rv != 0){
    return rv;
  }

  Element elem(iter->second);
  items.get(elem);

  item.name = iter->first;
  item.id = iter->second;
  item.data = elem.data;
  return 0;
}


template <typename ID, typename DATA>
int labeled_box<ID,DATA>::add(Item &item){
  auto iter = labelMap.find(item.name); // Prevent duplicates
  if (iter != labelMap.end()){
    return -1;
  };

  Element elem(item.data);
  items.add(elem);
  labelMap.insert(std::pair<std::string,ID>(item.name, elem.id));

  item.id = iter->second;
  return 0;
}


template <typename ID, typename DATA>
template <typename T>
int labeled_box<ID,DATA>::remove(Item &item, T identifier){
  // Find the item by index
  auto iter = labelMap.begin();
  int rv = searchIdentifier(iter, identifier);
  if (rv != 0){
    return rv;
  }

  Element elem(iter->second);
  items.remove(elem);
  labelMap.erase(iter);

  item.data = elem.data;
  item.name = iter->first;
  item.id = iter->second;
  return 0;
}



template <typename ID, typename DATA>
int labeled_box<ID,DATA>::searchIdentifier(typename std::map<std::string, ID>::iterator &iter, std::string itemName){
  iter = labelMap.find(itemName);
  if (iter == labelMap.end()){
    return -1;
  };
  return 0;
}


template <typename ID, typename DATA>
int labeled_box<ID,DATA>::searchIdentifier(typename std::map<std::string, ID>::iterator &iter, ID itemID){
  for (iter = labelMap.begin(); iter != labelMap.end(); iter++){
    if (iter->second == itemID){
      break;
    }
  }
  if (iter == labelMap.end()){
    return -1;
  };
  return 0;
}


}
#endif
