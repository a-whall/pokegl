#pragma once
#include "shader.h"
#include <vector>
#include <unordered_map>
#include "frameid.h"

namespace Animation
{
  using std::vector;
  using std::unordered_map;

  typedef Frame_ID_enum Transition;

  // FS stands for Frame State. Collections of these objects will form a Frame State Machine. Each Frame should be uniquely identified by a Frame_ID_enum. The primary purpose of this class is to act as an input acceptor as it stores all of its successors in a hash table accessed by their unique frame ID.
  class FS
  {
    const char* name="nameless_fstate";
    unordered_map<Frame_ID_enum, FS*> event_map;
  public:
    Frame_ID_enum frame_ID; // must match the frame index in shader sampler_2D_array
    FS(Frame_ID_enum);
    void set_name(const char*);
    const char* get_name();
    void add_transition(Frame_ID_enum, FS*);
    FS* change(Frame_ID_enum, Shader*);
  };

  // class Sequence {
  //   int* fc;
  // public:
  //   Sequence(int* frameCounter) {}
  //   void operator()() {};
  // };

  // FSM stands for Frame State Machine which is based on a Finite State Machine. Thus Animation::FSM exists so that classes that extend <<Sprite>> have a data structure to model their frame states. States themselves are containers for successor states they may have, FSM is more like the interface to create states as well as encapsulating a current state and allowing to pass input to that current state.
  class FSM
  {
    FS *current_state= nullptr;
    unordered_map<Frame_ID_enum, FS*> states;
    Shader *shader;
  public:
    void set_shader(Shader *);
    bool operator==(Frame_ID_enum);
    Frame_ID_enum get_fid();
    FSM& create_state(const char *, Frame_ID_enum, vector<Transition>);
    void add_transition(Frame_ID_enum, Transition);
    void input(Frame_ID_enum);
  };
}