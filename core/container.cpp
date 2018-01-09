#include "container.h"
#include "manager.h"

using namespace bx;



Container * Container::GetParent(){
  return this->parent;
}


std::string Container::GetName() {
  return this->name;
}


Manager * Container::GetManager(){
  return this->manager;
}

//void Container::SetInit(bool state){
//  this->initialized = state;
//}

std::string Container::Print(){
  return "Container: " + this->GetName() + "/" + std::to_string(this->GetID())   + \
  "; Parent Container: " + manager->GetContainer(this->GetParentID())->GetName() + \
  "/" + std::to_string(this->GetParentID()) + "; ";
}


//void Container::SetID(ContainerID contID) {
//  this->id = contID;
//}


void Container::SetParent(Container * cont) {
  assert(cont != NULL);
  this->parent = cont;
}


void Container::SetManager(Manager * manager) {
  assert(manager != NULL);
  this->manager = manager;
}


void Container::Init(){
  for (unsigned int i = 0; i < components.size(); i++){
    if (!components.valid(i)){
      continue;
    }
    Component * comp = components.at(i);
    InitComponent(comp);
  }

  for (unsigned int i = 0; i < containers.size(); i++){
    if (!containers.valid(i)){
      continue;
    }
    Container * cont = containers.at(i);
    InitContainer(cont);
  }


}


void Container::InitContainer(Container * cont){
  //this->manager->RegisterContainer(cont, this->GetID());
  cont->SetManager(this->manager);
  cont->SetParent(this);
  cont->initialized = true;
}

// Modify Entities
void Container::AddContainer(Container * cont){
  assert(cont != NULL);
  containers.add(cont, cont->GetName());
  // contID should not be used; use global id in manager
  //if (!this->initialized){
    //return 0;
  //}
  //ContainerID contID = this->manager->RegisterContainer(cont, this->GetID());
  InitContainer(cont);
  #ifdef BLOX_DEBUG
  DebugLog(BLOX_ACTIVITY, "Container Initialized", cont->Print());
  #endif
}


Container * Container::GetContainer(ContainerID contID){
  return this->containers.at(contID);
}

Container * Container::GetContainer(std::string contName){
  return this->containers.at(contName);
}


void Container::Uninit(){

  for (unsigned int i = 0; i < components.size(); i++){
    if (!components.valid(i)){
      continue;
    }
    Component * comp = components.at(i);
    comp->SetParentID(this->parentID);
  }

  for (unsigned int i = 0; i < containers.size(); i++){
    if (!containers.valid(i)){
      continue;
    }
    Container * cont = containers.at(i);
    cont->SetParentID(this->parentID);
  }

  #ifdef BLOX_DEBUG
  DebugLog(BLOX_ACTIVITY, "Container Removed", this->Print());
  #endif

  this->SetID(0);
  this->SetManager(NULL);
  this->SetParentID(0);

}



Container * Container::RemoveContainer(std::string contName){
  Container * cont = containers.remove(contName);
  if (cont == NULL){
    #ifdef BLOX_DEBUG
    DebugLog(BLOX_ERROR, "Failed Removal: Container doesn't exist", cont->Print());
    #endif
    return cont;
  }
  for (unsigned int i = 1; i < components.size(); i++){
    Component * comp;
    components.at(comp, i);
    if (comp != NULL){
      comp->SetParentID(this->parentID);
    }
  }

  for (unsigned int i = 1; i < containers.size(); i++){
    Container * cont;
    containers.at(cont, i);
    if (cont != NULL){
      cont->SetParentID(this->parentID);
    }
  }

  cont->SetID(0);
  cont->SetManager(NULL);
  cont->SetParentID(0);

  #ifdef BLOX_DEBUG
  DebugLog(BLOX_ACTIVITY, "Container Removed", cont->Print());
  #endif

  return cont;
}






void Container::InitComponent(Component * comp){
  comp->SetParent(this);
  comp->SetManager(this->manager);
  //comp->SetInit(true);

  #ifdef BLOX_DEBUG
  DebugLog(BLOX_ACTIVITY, "Component Initialized", comp->Print());
  #endif
  comp->Init();
}


// Modify Components within Entity
void Container::AddComponents(std::vector<Component*> comps){
  for (unsigned int i = 0; i < comps.size(); i++){
    Component * comp = comps.at(i);
    assert(comp != NULL);
    ComponentID compID = components.add(comp, comp->GetName());
    //comp->SetID(compID);
    if (this->initialized){
      InitComponent(comp);
    }
  }
}

Component * Container::GetComponent(ComponentID compID){
  return this->components.at(compID);
}

Component * Container::GetComponent(std::string compName){
  return this->components.at(compName);
}


Component * Container::RemoveComponent(ComponentID compID){
  Component * comp =  components.remove(compID);

  #ifdef BLOX_DEBUG
  DebugLog(BLOX_ACTIVITY, "Container Removed", comp->Print());
  #endif

  comp->manager = NULL;
  comp->SetInit(false);
  comp->SetParentID(0);
  comp->SetID(0);

  return comp;
}


Component * Container::RemoveComponent(std::string compName){
  Component * comp =  components.remove(compName);

  #ifdef BLOX_DEBUG
  DebugLog(BLOX_ACTIVITY, "Container Removed", comp->Print());
  #endif

  comp->manager = NULL;
  comp->SetInit(false);
  comp->SetParentID(0);
  comp->SetID(0);

  return comp;
}
