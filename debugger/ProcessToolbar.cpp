#include "ProcessToolbar.h"
#include "Target.h"
#include "Error.h"
#include "QAction"
#include <array>
#include <iostream>

ProcessToolbar::ProcessToolbar(const Process::Pointer &process)
    : _process(process)
  {
  auto kill = addAction("Kill");
  connect(kill, &QAction::triggered, [this]()
    {
    checkError(_process->kill());
    _process = nullptr;
    });

  _stop = addAction("");
  syncState(_process ? _process->currentState() : Process::State::Invalid);
  _stop->setCheckable(true);
  connect(_stop, SIGNAL(triggered()), this, SLOT(togglePause()));

  connect(&_timer, &QTimer::timeout, [this]
    {
    auto newState = _process ? _process->currentState() : Process::State::Invalid;
    if (_processState != newState)
      {
      syncState(newState);
      }

    _process->processEvents();

    std::array<char, 256> output;
    while (_process->getOutput(Process::OutputType::Output, output) > 0)
      {
      std::cout << output.data();
      }

    while (_process->getOutput(Process::OutputType::Error, output) > 0)
      {
      std::cout << output.data();
      }
    });
  _timer.start(100);
  }

void ProcessToolbar::syncState(Process::State type)
  {
  emit statusUpdate(
    QString("Process %1 [%2 %3]")
      .arg(_process->target()->path().data())
      .arg(_process ? _process->processID() : 0)
      .arg(Process::getStateString(type).data())
    );

  _processState = type;

  _stop->setText(type == Process::State::Stopped ? "Continue" : "Break");
  _stop->setChecked(type == Process::State::Stopped);
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
