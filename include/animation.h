#pragma once
#include "shader.h"
#include <vector>
#include <unordered_map>

namespace Animation
{
  using std::vector;
  using std::unordered_map;

  typedef std::pair<const char*, const char*> Transition;

  // Acts as an input acceptor
  class State
  {
    const char* name;
    unordered_map<const char*, State*> event_map;
  public:
    int frame_ID; // must match the frame index in shader sampler_2D_array

    State(const char *);
    const char* get_name();

    void add_transition(const char *, State *);
    State* change(const char *, Shader*);
  };

  // class Sequence {
  //   int* fc;
  // public:
  //   Sequence(int* frameCounter) {}
  //   void operator()() {};
  // };

  class Automaton
  {
    State *current_state= nullptr;
    unordered_map<const char*, State*> states;
    Shader *shader;
  
  public:

    void set_shader(Shader *);

    bool operator==(const char *);

    Automaton* create_state(const char *, int, vector<Transition>);
    void add_transition(const char *, Transition);
    void input_event(const char *);
  };
}