#include "Terminal.h"
#include "QPlainTextEdit"
#include "QVBoxLayout"

namespace UI
{

Terminal::Terminal(QWidget *parent)
    : Console(parent),
      _historyPos(0)
  {
  }

Terminal::~Terminal()
  {
  }

void Terminal::onInput(int chr, const QString &str)
  {
  if (chr == Qt::Key_Return)
    {
    _history << currentSubmit();
    _historyPos = _history.size();
    }

  Console::onInput(chr, str);
  }

void Terminal::moveVertical(int dir)
  {
  _historyPos = Eks::clamp(_historyPos + dir, 0, _history.size() + 1);
  if (_historyPos < _history.size())
    {
    setSubmit(_history[_historyPos]);
    }
  else
    {
    clearSubmit();
    }
  }

}

