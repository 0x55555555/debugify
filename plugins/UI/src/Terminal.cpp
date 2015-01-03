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
        }
      //case Qt::Key_Left:
      //case Qt::Key_Right:
      case Qt::Key_Up:
      case Qt::Key_Down:
        break;
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
    : QWidget(parent)
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
    _submit.clear();
    }
  else
    {
    _submit += str;
    }
  }

void Terminal::deleteBack()
  {
  if (_submit.length())
    {
    toEnd();
    _edit->textCursor().clearSelection();
    _edit->textCursor().deletePreviousChar();
    _submit.resize(_submit.size()-1);
    }
  }

void Terminal::toEnd()
  {
  auto c = _edit->textCursor();
  c.movePosition(QTextCursor::End, QTextCursor::MoveAnchor);
  _edit->setTextCursor(c);
  }
}

