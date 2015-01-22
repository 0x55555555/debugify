#include "Process.h"
#include "Thread.h"
#include "ThreadImpl.h"
#include "lldb/API/SBProcess.h"
#include "lldb/API/SBListener.h"
#include "Target.h"

template <> struct std::hash<lldb::SBThread>
  {
  size_t operator()(const lldb::SBThread &v) const
    {
    return std::hash<lldb::tid_t>{}(v.GetThreadID());
    }
  };

class LldbDriver::Process::Impl
  {
public:
  Impl()
      : threads(Eks::Core::defaultAllocator())
    {
    }

  std::weak_ptr<LldbDriver::Process> myself;
  Target::Pointer target;
  lldb::SBProcess process;
  lldb::StateType processState;
  ProcessStateChangeNotifier stateChanged;
  ProcessEndedNotifier ended;
  lldb::SBListener listener;

  std::shared_ptr<LldbDriver::Thread> wrapThread(const lldb::SBThread &thr)
    {
    auto fnd = threads.find(thr);
    if (fnd != threads.end())
      {
      auto lck = fnd->second.lock();
      if (lck)
        {
        return lck;
        }
      }

    auto thread = std::make_shared<Thread>();
    thread->_impl->myself = thread;
    thread->_impl->process = myself.lock();
    thread->_impl->thread = thr;

    threads[thr] = thread;
    return thread;
    }

  Eks::UnorderedMap<lldb::SBThread, std::weak_ptr<LldbDriver::Thread>> threads;
  };
