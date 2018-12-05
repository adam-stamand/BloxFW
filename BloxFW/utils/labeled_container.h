#ifndef LABELED_CONT_H
#define LABELED_CONT_H

#include <BloxFW/utils/container.h>
#include <assert.h>
#include <map>

namespace bx{

template <typename ID, typename DATA>
struct _Item{
  //ID id;
  std::string name;
  DATA data;
  _Item(DATA data):data(data){};
  _Item(){};
};

template <typename ID, typename DATA>
class labeled_container
{

public:

  typedef _Item<ID,DATA> Item;


  int     add(Item &item, std::string name);
  int     remove(Item &item, std::string name);
  int     get(Item &item, std::string name);

  typename std::map<std::string, DATA>::const_iterator begin(){return labelMap.begin();}
  typename std::map<std::string, DATA>::const_iterator end(){return labelMap.end();}
  size_t  size(){return labelMap.size();}

private:
  std::map<std::string, DATA> labelMap;
};



template <typename ID, typename DATA>
int labeled_container<ID,DATA>::get(Item &item, std::string name){
  auto iter = labelMap.find(name); // Prevent duplicates
  if (iter = labelMap.end()){
    return -1;
  }

  item.data = iter->second;
  item.name = iter->first;

  return 0;
}

template <typename ID, typename DATA>
int labeled_container<ID,DATA>::add(Item &item, std::string name){
  auto iter = labelMap.find(name); // Prevent duplicates
  if (iter != labelMap.end()){
    return -1;
  };

  item.data = iter->second;
  item.name = iter->first;
  labelMap.insert(std::pair<std::string,DATA>(item.name, item.data));
  return 0;
}


template <typename ID, typename DATA>
int labeled_container<ID,DATA>::remove(Item &item, std::string name){
  // Find the item by index
  auto iter = labelMap.find(name); // Prevent duplicates
  if (iter != labelMap.end()){
    return -1;
  };


  labelMap.erase(iter);

  item.data = iter->second;
  item.name = iter->first;
  return 0;
}




}
#endif
