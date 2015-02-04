#pragma once
#include "QtWidgets/QDialog"
#include "XCore.h"
#include "Utilities/XNotifier.h"
#include "Containers/XStringSimple.h"

namespace UI
{

/// \expose unmanaged
X_DECLARE_NOTIFIER(ChangedNotifier, std::function<void (QString)>);

/// \expose managed
class Dialog : public QDialog
  {
  Q_OBJECT

public:
  class Wrapper;

  Dialog(const QString &uicLocation);
  ~Dialog();

  void setAvailableValues(const QString &str, const QVariant &val);
  void setValue(const QString &str, const QVariant &val);
  QVariant value(const QString &str);

  ChangedNotifier *changed() { return &_changed; }
  ChangedNotifier *clicked() { return &_clicked; }

  /// \expose
  enum Result
    {
    Accepted,
    Rejected
    };
  Result result() const;

private:
  std::shared_ptr<Wrapper> makeWrapper(QObject *w);

  QHash<QString, std::shared_ptr<Wrapper>> _elements;

  ChangedNotifier _changed;
  ChangedNotifier _clicked;
  };

}

