#ifndef CONTAINER_H
#define CONTAINER_H

#include <vector>


template <typename INDEX, typename ELEMENT>
class container
{
public:
  container(){}
  ~container(){}

  INDEX push_back(ELEMENT element);
  void remove(INDEX index);
  INDEX size();
  ELEMENT at(INDEX index);
private:
  INDEX cap = 0;

  std::vector<ELEMENT> vec;
  std::vector<INDEX> vacancies; //TODO change to list or some other data structure
};


template <typename INDEX, typename ELEMENT>
INDEX container<INDEX,ELEMENT>::push_back(ELEMENT element){
  if (vacancies.size()){
    INDEX index = vacancies.at(0);
    vacancies.erase(vacancies.begin());
    vec.at(index) = element;
    return index;
  }else{
    INDEX index = cap++;
    vec.push_back(element);
    return index;
  }
}

template <typename INDEX, typename ELEMENT>
ELEMENT container<INDEX,ELEMENT>::at(INDEX index){
  return vec.at(index);
}

template <typename INDEX, typename ELEMENT>
void container<INDEX,ELEMENT>::remove(INDEX index){
  vec.at(index) = 0;
  vacancies.push_back(index);
}

template <typename INDEX, typename ELEMENT>
INDEX  container<INDEX,ELEMENT>::size(){
  return cap;
}


#endif
