#pragma once
#include "Console.h"

namespace UI
{

/// \expose
class Terminal : public Console
  {
  Q_OBJECT

public:
  /// \noexpose
  explicit Terminal(bool toolbar);
  ~Terminal();

  void onInput(int chr, const QString &str) X_OVERRIDE;
  void moveVertical(int) X_OVERRIDE;

private:
  void clearCommand();
  void setCommand(const QString &str);

  int _historyPos;
  QStringList _history;
  };

}
