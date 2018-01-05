#ifndef BOX_H
#define BOX_H

#include <vector>

namespace bx{




template <typename INDEX, typename ELEMENT>
class box
{
public:
  box(){}
  ~box(){}

  INDEX   add(ELEMENT &element);
  ELEMENT remove(INDEX index);
  ELEMENT at(INDEX index);
  size_t  size();

private:
  size_t m_size = 0;
  std::vector<ELEMENT> vec;
  std::vector<INDEX> vacancies; //TODO change to list or some other data structure
};



















template <typename INDEX, typename ELEMENT>
INDEX box<INDEX,ELEMENT>::add(ELEMENT &element){
  INDEX index;
  if (vacancies.size() > 0){ // Fill in vacancy if one exists
    index = vacancies.at(0);
    vacancies.erase(vacancies.begin());
    vec.at(index) = element;
  }else{ // else push to back
    index = vec.size();
    vec.push_back(element);
  }
  m_size++;
  return index;
}

template <typename INDEX, typename ELEMENT>
ELEMENT box<INDEX,ELEMENT>::at(INDEX index){
  return vec.at(index);
}

template <typename INDEX, typename ELEMENT>
ELEMENT box<INDEX,ELEMENT>::remove(INDEX index){
  vacancies.push_back(index);
  m_size--;
  return vec.at(index);
}

template <typename INDEX, typename ELEMENT>
size_t box<INDEX,ELEMENT>::size(){
  return m_size;
}

} //end namespace

#endif
