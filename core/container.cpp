#include "container.h"

using namespace bx;


ContainerID Container::GetID(){
  return this->id;
}


ContainerID Container::GetParentID(){
  return this->parentID;
}


std::string Container::GetName() {
  return this->name;
}


Manager * Container::GetManager(){
  return this->manager;
}


std::string Container::Print(){
  return "to be implemented";
}


void Container::SetID(ContainerID id) {
  this->id = id;
}


void Container::SetParentID(ContainerID id) {
  this->contID = parentiD;
}


void Container::SetManager(Manager * manager) {
  this->manager = manager;
}


// Modify Entities
ContainerID Container::AddContainer(Container * cont){
  return containers.add(cont, cont->GetName());
}


Container * Container::RemoveContainer(ContainerID id){
  return containers.RemoveItem(id);
}


Container * Container::RemoveContainer(std::string contName){
  return containers.RemoveItem(contName);
}


// Modify Components within Entity
void Container::AddComponents(std::vector<Component*> comps){
  for (unsigned int i = 0; i < comps.size(); i++){
    Component * comp = comps.at(i);
    comp->SetContainerID(this->GetID());
    comp->SetManager(this->manager);
    components.AddItem(comp, comp->GetName());
  }
}


Component * Container::RemoveComponent(ComponentID compID){
  return components.RemoveItem(compID);
}


Component * Container::RemoveComponent(std::string compName){
  return components.RemoveItem(compName);
}
