#include "Console.h"
#include "QPlainTextEdit"
#include "QVBoxLayout"

namespace UI
{

ConsoleWidget::ConsoleWidget(QWidget *w) : QPlainTextEdit(w)
  {
  }

void ConsoleWidget::keyPressEvent(QKeyEvent *e)
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
        emit moveHorizontal(-1);
        QPlainTextEdit::keyPressEvent(e);
        break;
        }
      case Qt::Key_Right:
        {
        emit moveHorizontal(1);
        QPlainTextEdit::keyPressEvent(e);
        break;
        }
      case Qt::Key_Up:
        {
        emit moveVertical(-1);
        break;
        }
      case Qt::Key_Down:
        {
        emit moveVertical(1);
        break;
        }
      default:
        {
        QPlainTextEdit::keyPressEvent(e);
        emit keyPress(e->key(), e->text());
        }
    }
}

Console::Console(QWidget *parent)
    : QWidget(parent),
      _pos(0)
  {
  auto layout = new QVBoxLayout(this);
  layout->setContentsMargins(4, 4, 4, 4);

  _edit = new ConsoleWidget(this);
  layout->addWidget(_edit);

  QFont font("Courier New");
  font.setStyleHint(QFont::Monospace);
  _edit->setFont(font);

  connect(_edit, SIGNAL(keyPress(int, QString)), this, SLOT(onInput(int, QString)));
  connect(_edit, SIGNAL(deleteBack()), this, SLOT(deleteBack()));
  connect(_edit, SIGNAL(moveHorizontal(int)), this, SLOT(moveHorizontal(int)));
  connect(_edit, SIGNAL(moveVertical(int)), this, SLOT(moveVertical(int)));

  }

Console::~Console()
  {
  }

void Console::append(QString str)
  {
  // Scrub out any colour codes...
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

void Console::clear()
  {
  clearSubmit();
  _edit->clear();
  }

void Console::setSubmit(const QString &str)
  {
  clearSubmit();
  _submit = str;
  _pos = _submit.length();
  append(str);
  }

void Console::clearSubmit()
  {
  toEnd();

  while(_submit.length())
    {
    deleteBack();
    }
  }

void Console::onInput(int chr, const QString &str)
  {
  if (chr == Qt::Key_Return)
    {
    _onInput(_submit.toUtf8().data());

    _submit.clear();
    }
  else
    {
    _submit.insert(_pos, str);
    _pos += str.length();
    }
  }

void Console::deleteBack()
  {
  if (_submit.length())
    {
    _edit->textCursor().deletePreviousChar();
    _submit.remove(_pos-1, 1);
    _pos -= 1;
    }
  }

void Console::moveHorizontal(int dir)
  {
  _pos = Eks::clamp(_pos + dir, 0, _submit.length());
  }

void Console::moveVertical(int X_UNUSED(dir))
  {
  }

void Console::toEnd()
  {
  auto c = _edit->textCursor();
  c.movePosition(QTextCursor::End, QTextCursor::MoveAnchor);
  _edit->setTextCursor(c);
  _pos = _submit.length();
  }

}

