#include "TargetToolbar.h"
#include "Error.h"
#include "QtWidgets/QAction"

TargetToolbar::TargetToolbar(const Target::Pointer &target)
    : _target(target)
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
    emit processStarted(_target->Launch(Eks::Vector<Eks::StringRef>(), Eks::Vector<Eks::StringRef>(), err));
    checkError(err);
    });
  }


void TargetToolbar::checkError(const Error &err)
  {
  if (err.hasError())
    {
    emit error(err.error().data());
    }
  }
