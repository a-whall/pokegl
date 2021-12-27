#include "animation.h"
#include <iostream>
#include "debug.h"

namespace Animation
{
  typedef std::pair<const char*, const char*> Transition;


  State::State(const char *name) { this->name= name; }

  const char* State::get_name() { return name; }

  void State::add_transition(const char *event, State *transitionState)
  {
    if (event_map.find(event) == event_map.end())
    {
      Debug::log_from(Debug::animation,"transition from ",name," to ",transitionState->get_name()," on event ",event); 
      event_map[event]= transitionState;
    }
    else
      Debug::log_from(Debug::animation,name," already has transition for event ",event);
  }

  State* State::change(const char *event, Shader *shader)
  {
    if (event_map.find(event) != event_map.end())
    {
      glUseProgram(shader->handle);
      shader->set("frameID", event_map[event]->frame_ID);
      glUseProgram(0);
      Debug::log_from(Debug::animation, "changing from ",name," to ",event_map[event]->get_name());
      return event_map[event];
    }
    return this;
  }
  
  // class Sequence {
  //   int* fc;
  // public:
  //   Sequence(int* frameCounter) {}
  //   void operator()() {};
  // };

  void Automaton::set_shader(Shader *s) { shader= s; }

  bool Automaton::operator==(const char *stateName)
  {
    return current_state->get_name() == stateName;
  }

  Automaton* Automaton::create_state(const char *s, int fID, vector<Transition> transitions)
  {
    if (states.find(s) == states.end())
    {                                // if state with name s doesn't yet exist
      states[s]= new State(s);          // create one
      if (current_state == nullptr)     // if current state is uninitialized
        current_state= states[s];          // set it to the new state
    }
    states[s]->frame_ID= fID;        // set states frameID
    for (Transition t : transitions) // for each event-transition pair
      this->add_transition(s, t);       // Create transition event
    return this;                     // fluent
  }

  void Automaton::add_transition(const char *s, Transition t)
  {
    if (states.find(s) == states.end())                  // IF state with name s doesn't exist
      states[s]= new State(s);                             // create it
    if (states.find(t.second) == states.end())           // IF successor state doesn't exist
      states[t.second]= new State(t.second);               // create it
    states[s]->add_transition(t.first, states[t.second]);// INVOKE states add_transition
  }

  void Automaton::input_event(const char *e)
  {
    if (current_state != nullptr)                   // if current state is initialized
      current_state= current_state->change(e, shader); // attempt to change state (state decides whether it does or not)
  }
}