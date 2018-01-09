#ifndef BOX_H
#define BOX_H

#include <vector>
#include <list>

namespace bx{

template <typename ID, typename DATA>
  struct _Element{
    ID id;
    DATA data;
    _Element(ID id) : id(id){}
    _Element(DATA data) : data(data){}
  };


template <typename ID, typename DATA>
class box
{
public:
  box(){}
  ~box(){}

  typedef _Element<ID,DATA> Element;

  int add(Element &item);
  int remove(Element &item);
  int get(Element &item);

  size_t  size();
  void print(); // for debug TODO pass in function pointer that prints single element

private:
  std::vector<DATA> vec;
  std::list<ID> vacancies;
};






template <typename ID, typename DATA>
int box<ID,DATA>::add(Element &item){

  if (vacancies.size() > 0){ // Fill in vacancy if one exists
    item.id = vacancies.front();
    vec.at(item.id) = item.data;
    vacancies.pop_front();
  }else{
    item.id = vec.size();
    vec.push_back(item.data);
  }
  return 0;
}

template <typename ID, typename DATA>
int box<ID,DATA>::get(Element &item){
  item.data = vec.at(item.id);
  return 0;
}

template <typename ID, typename DATA>
int box<ID,DATA>::remove(Element &item){
  vacancies.push_back(item.id);
  return 0;
}

template <typename ID, typename DATA>
size_t box<ID,DATA>::size(){
  return vec.size();
}

template <typename ID, typename DATA>
void box<ID,DATA>::print(){
  printf("Box size = %d\n", vec.size());
  for (unsigned int i = 0; i < vec.size(); i++){
    printf("Element %d valid: %d\n", i, vec.at(i).valid);
  }
}


} //end namespace

#endif
