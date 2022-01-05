#include "animation.h"
#include <iostream>
#include "debug.h"

namespace Animation
{
  typedef Frame_ID_enum Transition;

  FS::FS(Frame_ID_enum fID) { this->frame_ID= fID; }

  void FS::set_name(const char* s) { this->name= s; }
  const char* FS::get_name() { return name; }

  void FS::add_transition(Frame_ID_enum event, FS * transitionState)
  {
    if (event_map.find(event) == event_map.end()) {
      Debug::log_from(Debug::animation,name," -> ",transitionState->get_name()," on ",event); 
      this->event_map[event]= transitionState;
    }
    else {
      Debug::log_from(Debug::animation,name," already has transition for event ",event);
    }
  }

  FS* FS::change(Frame_ID_enum event, Shader *shader)
  {
    FS * result;
    if (event_map.find(event) != event_map.end())
    {
      glUseProgram(shader->handle);
      shader->set("frameID", event_map[event]->frame_ID);
      glUseProgram(0);
      Debug::log_from(Debug::animation,name," -> ",event_map[event]->get_name());
      result = event_map[event];
    }
    else
      result = this;
    return result;
  }
  
  // class Sequence {
  //   int* fc;
  // public:
  //   Sequence(int* frameCounter) {}
  //   void operator()() {};
  // };

  void FSM::set_shader(Shader *s) { shader= s; }

  bool FSM::operator==(Frame_ID_enum fID)
  {
    return current_state->frame_ID == fID;
  }

  Frame_ID_enum FSM::get_fid()
  {
    return current_state->frame_ID;
  }

  FSM& FSM::create_state(const char *s, Frame_ID_enum fID, vector<Transition> transitions)
  {
    if (states.find(fID) == states.end()) { // if state with frame id fID doesn't yet exist
      states[fID]= new FS(fID);                // create one
      if (current_state == nullptr)            // if fsa current state is uninitialized
        current_state= states[fID];               // set it to the newly created state
    }
    states[fID]->set_name(s);               // set states name
    for (Transition t : transitions)        // for each event-transition pair
    {
      this->add_transition(fID, t);            // Create transition event
    }
    return *this;                           // fluent
  }

  void FSM::add_transition(Frame_ID_enum fID, Transition t)
  {
    if (states.find(fID) == states.end())      // if state with name s doesn't exist
      states[fID]= new FS(fID);                   // create it
    if (states.find(t) == states.end())        // if successor state doesn't exist
      states[t]= new FS(t);                       // create it
    states[fID]->add_transition(t, states[t]); // call (access unordered_map) state::add_transition
  }

  void FSM::input(Frame_ID_enum e)
  {
    if (current_state != nullptr)                    // if current state is initialized
      current_state= current_state->change(e,shader);  // attempt to change state (state decides whether it does or not)
  }
}