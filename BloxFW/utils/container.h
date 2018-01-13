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
  typename std::map<ID,ID>::const_iterator end(){return occupants.end();}
  typename std::map<ID,ID>::const_iterator begin(){return occupants.begin();}


  size_t  size();

private:
  std::vector<DATA> vec;
  std::map<ID, ID> occupants; //TODO fix this; don't use map
  std::list<ID> vacancies;
};






template <typename ID, typename DATA>
int container<ID,DATA>::add(Element &item){

  if (vacancies.size() > 0){ // Fill in vacancy if one exists
    item.id = vacancies.front();
    vec.at(item.id) = item.data;
    vacancies.pop_front();
  }else{
    item.id = vec.size();
    vec.push_back(item.data);
  }
  occupants.insert(std::pair<ID,ID>(item.id, item.id));
  return 0;
}

template <typename ID, typename DATA>
int container<ID,DATA>::get(Element &item){
  item.data = vec.at(item.id);
  return 0;
}

template <typename ID, typename DATA>
int container<ID,DATA>::remove(Element &item){
  vacancies.push_back(item.id);

  auto iter = occupants.find(item.id);
  if (iter == occupants.end()){
    return -1;
  }
  item.data = vec.at(item.id);
  occupants.erase(iter);
  return 0;
}

template <typename ID, typename DATA>
size_t container<ID,DATA>::size(){
  return occupants.size();
}



} //end namespace

#endif
