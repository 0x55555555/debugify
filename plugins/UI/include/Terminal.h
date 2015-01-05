#pragma once
#include <QWidget>
#include <QPlainTextEdit>
#include <iostream>
#include "Notifier.h"
#include "Containers/XStringSimple.h"
#include "Containers/XStringBuffer.h"

class QPlainTextEdit;

namespace UI
{

/*struct StreamRedirect : public Eks::String::Buffer
  {
  StreamRedirect(std::ostream *stream)
      : _stream(stream),
        _old(_stream->rdbuf(this))
    {
    }

  ~StreamRedirect()
    {
    _stream->rdbuf(_old);
    }

private:
  Eks::String _stream;
  std::streambuf *_old;
};*/

/// \expose unmanaged
DECLARE_NOTIFIER(InputNotifier, std::function<void (Eks::String)>);

class TerminalWidget : public QPlainTextEdit
  {
  Q_OBJECT
public:
  TerminalWidget(QWidget *w);

  void keyPressEvent(QKeyEvent *e) X_OVERRIDE;

signals:
  void keyPress(int chr, QString c);
  void deleteBack();
  void move(int);
  void moveHistory(int);
  };

/// \expose
class Terminal : public QWidget
  {
  Q_OBJECT

public:
  /// \noexpose
  explicit Terminal(QWidget *parent = 0);
  ~Terminal();

  /// \expose
  void append(QString str);

  /// \expose
  InputNotifier *input() { return &_onInput; }

private slots:
  void onInput(int chr, QString str);
  void deleteBack();
  void move(int);
  void moveHistory(int);

private:
  void toEnd();
  void clearCommand();
  void setCommand(const QString &str);

  TerminalWidget *_edit;
  InputNotifier _onInput;
  QString _submit;
  int _pos;
  int _historyPos;
  QStringList _history;
  QtMessageHandler _oldHandler;
  };

}
