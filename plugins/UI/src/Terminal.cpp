#include "Terminal.h"
#include "QPlainTextEdit"
#include "QVBoxLayout"

namespace UI
{

TerminalWidget::TerminalWidget(QWidget *w) : QPlainTextEdit(w)
  {
  }

void TerminalWidget::keyPressEvent(QKeyEvent *e)
{
    switch (e->key())
      {
      case Qt::Key_Backspace:
        {
        emit deleteBack();
        break;
        }
      case Qt::Key_Left:
        {
        emit move(-1);
        QPlainTextEdit::keyPressEvent(e);
        break;
        }
      case Qt::Key_Right:
        {
        emit move(1);
        QPlainTextEdit::keyPressEvent(e);
        break;
        }
      case Qt::Key_Up:
        {
        emit moveHistory(-1);
        break;
        }
      case Qt::Key_Down:
        {
        emit moveHistory(1);
        break;
        }
      default:
        {
        QPlainTextEdit::keyPressEvent(e);
        emit keyPress(e->key(), e->text());
        }
    }
}

Terminal *g_lastTerminal = nullptr;

void onQMessage(QtMsgType, const QMessageLogContext &, const QString &str)
  {
  g_lastTerminal->append(str);
  }

Terminal::Terminal(QWidget *parent)
    : QWidget(parent),
      _pos(0),
      _historyPos(0)
  {
  auto layout = new QVBoxLayout(this);
  layout->setContentsMargins(4, 4, 4, 4);

  _edit = new TerminalWidget(this);
  layout->addWidget(_edit);

  QFont font("Courier New");
  font.setStyleHint(QFont::Monospace);
  _edit->setFont(font);

  connect(_edit, SIGNAL(keyPress(int, QString)), this, SLOT(onInput(int, QString)));
  connect(_edit, SIGNAL(deleteBack()), this, SLOT(deleteBack()));
  connect(_edit, SIGNAL(move(int)), this, SLOT(move(int)));
  connect(_edit, SIGNAL(moveHistory(int)), this, SLOT(moveHistory(int)));

  xAssert(!g_lastTerminal);
  g_lastTerminal = this;
  _oldHandler = qInstallMessageHandler(onQMessage);
  }

Terminal::~Terminal()
  {
  if (g_lastTerminal == this)
    {
    g_lastTerminal = nullptr;
    qInstallMessageHandler(_oldHandler);
    }
  }

void Terminal::append(QString str)
  {
  bool erasing = false;
  for(int i = 0; i < str.size(); ++i)
    {
    bool erase = false;
    QChar c = str[i];
    if (c == 27)
      {
      erasing = true;
      erase = true;
      }
    else if (erasing && c == 'm')
      {
      erasing = false;
      erase = true;
      }
    else if(erasing)
      {
      erase = true;
      }

    if (erase)
      {
      str.remove(i, 1);
      --i;
      }
    }

  _edit->insertPlainText(str);

  toEnd();
  }

void Terminal::onInput(int chr, QString str)
  {
  if (chr == Qt::Key_Return)
    {
    _onInput(_submit.toUtf8().data());

    _history << _submit;
    _historyPos = _history.size();
    _submit.clear();
    }
  else
    {
    _submit.insert(_pos, str);
    _pos += str.length();
    }
  }

void Terminal::deleteBack()
  {
  if (_submit.length())
    {
    _edit->textCursor().deletePreviousChar();
    _submit.remove(_pos-1, 1);
    _pos -= 1;
    }
  }

void Terminal::move(int dir)
  {
  _pos = Eks::clamp(_pos + dir, 0, _submit.length());
  }

void Terminal::moveHistory(int dir)
  {
  _historyPos = Eks::clamp(_historyPos + dir, 0, _history.size() + 1);
  if (_historyPos < _history.size())
    {
    setCommand(_history[_historyPos]);
    }
  else
    {
    clearCommand();
    }
  }

void Terminal::toEnd()
  {
  auto c = _edit->textCursor();
  c.movePosition(QTextCursor::End, QTextCursor::MoveAnchor);
  _edit->setTextCursor(c);
  _pos = _submit.length();
  }

void Terminal::clearCommand()
  {
  toEnd();

  while(_submit.length())
    {
    deleteBack();
    }
  }

void Terminal::setCommand(const QString &str)
  {
  clearCommand();
  _submit = str;
  _pos = _submit.length();
  append(str);
  }
}

