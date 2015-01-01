#pragma once
#include "Global.h"
#include "Containers/XStringSimple.h"

namespace LldbDriver
{

class Error;
class Target;

class Process
  {
  SHARED_CLASS(Process);
  PIMPL_CLASS(Process, sizeof(void*) * 8);

public:
  Process();
  ~Process();

  enum class State
    {
    Invalid = 0,
    Unloaded,
    Connected,    ///< Process is connected to remote debug services, but not launched or attached to anything yet
    Attaching,    ///< Process is currently trying to attach
    Launching,    ///< Process is in the process of launching
    Stopped,      ///< Process or thread is stopped and can be examined.
    Running,      ///< Process or thread is running and can't be examined.
    Stepping,     ///< Process or thread is in the process of stepping and can not be examined.
    Crashed,      ///< Process or thread has crashed and can be examined.
    Detached,     ///< Process has been detached and can't be examined.
    Exited,       ///< Process has exited and can't be examined.
    Suspended     ///< Process or thread is in a suspended state as far
                  ///< as the debugger is concerned while other processes
                  ///< or threads get the chance to run.
    };

  std::shared_ptr<Target> target();
  std::shared_ptr<const Target> target() const;

  size_t processID() const;
  State currentState() const;

  static Eks::String getStateString(State s);

  enum class OutputType
    {
    Output,
    Error
    };
  size_t getOutput(OutputType type, char *data, size_t inputSize);

  Error kill();
  Error pauseExecution();
  Error continueExecution();

  void processEvents();

  friend class Target;
  };

}
