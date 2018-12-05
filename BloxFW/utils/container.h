#ifndef CONTAINER_H
#define CONTAINER_H

#include <vector>
#include <list>
#include <map>

namespace bx{

template <typename ID, typename DATA>
  struct _Element{
    ID id;
    DATA data;
    _Element(DATA data) : data(data){}
    _Element(){}
  };


template <typename ID, typename DATA>
class container
{
public:
  container(){}

  typedef _Element<ID,DATA> Element;

  int add(Element &item);
  int remove(Element &item);
  int get(Element &item);
  typename std::map<ID,DATA>::const_iterator end(){return map.end();}
  typename std::map<ID,DATA>::const_iterator begin(){return map.begin();}
  size_t  size();

private:
  std::map<ID, DATA> map;
};






template <typename ID, typename DATA>
int container<ID,DATA>::add(Element &item){
  map.insert(std::pair<ID,DATA>(item.id, item.data));
  return 0;
}

template <typename ID, typename DATA>
int container<ID,DATA>::get(Element &item){
  auto iter = map.find(item.id);
  if (iter == map.end()){
    return -1;
  }
  item.data = iter->second;
  return 0;
}

template <typename ID, typename DATA>
int container<ID,DATA>::remove(Element &item){
  auto iter = map.find(item.id);
  if (iter == map.end()){
    return -1;
  }
  item.data = iter->second;
  map.erase(iter);
  return 0;
}

template <typename ID, typename DATA>
size_t container<ID,DATA>::size(){
  return map.size();
}



} //end namespace

#endif
