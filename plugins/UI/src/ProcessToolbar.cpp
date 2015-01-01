#include "ProcessToolbar.h"
#include "Target.h"
#include "Error.h"
#include "QAction"
#include <array>
#include <iostream>

namespace UI
{

ProcessToolbar::ProcessToolbar()
  {
  auto kill = addAction("Kill");
  connect(kill, &QAction::triggered, [this]()
    {
    checkError(_process->kill());
    _process = nullptr;
    });

  _stop = addAction("");
  _stop->setCheckable(true);
  connect(_stop, SIGNAL(triggered()), this, SLOT(togglePause()));

  connect(&_timer, &QTimer::timeout, [this]
    {
    auto newState = _process ? _process->currentState() : Process::State::Invalid;
    syncState(newState);

    if (!_process)
      {
      return;
      }

    _process->processEvents();

    auto forwardOutput = [this](auto type)
      {
      std::array<char, 256> output;
      while (size_t read = _process->getOutput(type, output.data(), output.size()-1))
        {
        xAssert(read < (output.size()-1));
        output[read] = '\0';
        std::cout << output.data();
        }
      };

    forwardOutput(Process::OutputType::Output);
    forwardOutput(Process::OutputType::Error);
    });
  _timer.start(100);
  }

void ProcessToolbar::setProcess(const Process::Pointer &process)
  {
  _process = process;
  syncState(_process ? _process->currentState() : Process::State::Invalid);
  }

void ProcessToolbar::syncState(Process::State type)
  {
  if (_processState == type)
    {
    return;
    }

  if (_process)
    {
    emit statusUpdate(
      QString("Process %1 [%2 %3]")
        .arg(_process->target()->path().data())
        .arg(_process ? _process->processID() : 0)
        .arg(Process::getStateString(type).data())
      );
    }

  _processState = type;

  _stop->setText(type == Process::State::Stopped ? "Continue" : "Break");
  _stop->setChecked(type == Process::State::Stopped);

  emit stateChanged(type);
  }

void ProcessToolbar::checkError(const Error &err)
  {
  if (err.hasError())
    {
    emit error(err.error().data());
    }
  }

void ProcessToolbar::togglePause()
  {
  if (_processState == Process::State::Stopped)
    {
    checkError(_process->continueExecution());
    }
  else
    {
    checkError(_process->pauseExecution());
    }
  }

}
