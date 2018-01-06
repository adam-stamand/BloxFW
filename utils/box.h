#ifndef BOX_H
#define BOX_H

#include <vector>
#include <list>

namespace bx{




template <typename INDEX, typename ELEMENT>
class box
{
public:
  box(){}
  ~box(){}

  struct BoxElement{
    ELEMENT item;
    bool valid;
    BoxElement(ELEMENT item, bool valid) :
      valid(valid),
      item(item)
      {}
  };

  INDEX   add(ELEMENT &element);
  ELEMENT remove(INDEX index);
  ELEMENT at(INDEX index);
  bool valid(INDEX index);
  size_t  size();
  void print(); // for debug TODO pass in function pointer that prints single element

private:
  std::vector<BoxElement> vec;
  std::list<INDEX> vacancies;
};






template <typename INDEX, typename ELEMENT>
INDEX box<INDEX,ELEMENT>::add(ELEMENT &element){
  INDEX index;
  BoxElement boxElem(element, true);
  if (vacancies.size() > 0){ // Fill in vacancy if one exists
    index = vacancies.front();
    vec.at(index) = boxElem;
    vacancies.pop_front();
  }else{
    index = vec.size();
    vec.at(index) = boxElem;
  }
  return index;
}

template <typename INDEX, typename ELEMENT>
ELEMENT box<INDEX,ELEMENT>::at(INDEX index){
  BoxElement elem = vec.at(index);
  assert(elem.valid == true);
  return elem.item;
}

template <typename INDEX, typename ELEMENT>
ELEMENT box<INDEX,ELEMENT>::remove(INDEX index){
  BoxElement elem = vec.at(index);
  assert(elem.valid == true);
  vacancies.push_back(index);
  elem.valid = false;
  return elem.item;
}

template <typename INDEX, typename ELEMENT>
bool box<INDEX,ELEMENT>::valid(INDEX index){
  if (index >= vec.size() || index < 0){
    return false;
  }
  return vec.at(index).valid;
}

template <typename INDEX, typename ELEMENT>
size_t box<INDEX,ELEMENT>::size(){
  return vec.size();
}

template <typename INDEX, typename ELEMENT>
void box<INDEX,ELEMENT>::print(){
  printf("Box size = %d\n", vec.size());
  for (unsigned int i = 0; i < vec.size(); i++){
    printf("Element %d valid: %d\n", i, vec.at(i).valid);
  }
}


} //end namespace

#endif
