#include "TargetToolbar.h"
#include "Error.h"
#include "QtWidgets/QAction"

TargetToolbar::TargetToolbar()
  {
  setObjectName("Target Controls");

  auto act = addAction("Run");
  connect(act, &QAction::triggered, [this]()
    {
    if (!_target)
      {
      error("Invalid target");
      }

    Error err;
    emit processStarted(_target->launch(Eks::Vector<Eks::StringRef>(), Eks::Vector<Eks::StringRef>(), err));
    checkError(err);
    });
  }

void TargetToolbar::setTarget(const Target::Pointer &target)
  {
  _target = target;
  }

void TargetToolbar::checkError(const Error &err)
  {
  if (err.hasError())
    {
    emit error(err.error().data());
    }
  }
