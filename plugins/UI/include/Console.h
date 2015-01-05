#pragma once
#include <QWidget>
#include <QPlainTextEdit>
#include "Notifier.h"
#include "Containers/XStringSimple.h"
#include "Containers/XStringBuffer.h"

class QPlainTextEdit;

namespace UI
{

/// \expose unmanaged
DECLARE_NOTIFIER(InputNotifier, std::function<void (Eks::String)>);

class ConsoleWidget : public QPlainTextEdit
  {
  Q_OBJECT
public:
  ConsoleWidget(QWidget *w);

  void keyPressEvent(QKeyEvent *e) X_OVERRIDE;

signals:
  void keyPress(int chr, QString c);
  void deleteBack();
  void moveVertical(int);
  void moveHorizontal(int);
  };

/// \expose
class Console : public QWidget
  {
  Q_OBJECT

public:
  /// \noexpose
  explicit Console(QWidget *parent = 0);
  ~Console();

  void append(QString str);
  void clear();

  InputNotifier *input() { return &_onInput; }


  const QString &currentSubmit() const { return _submit; }
  void setSubmit(const QString &str);
  void clearSubmit();

public slots:
  void toEnd();
  virtual void moveHorizontal(int);
  virtual void moveVertical(int);
  void deleteBack();

protected slots:
  virtual void onInput(int chr, const QString &str);

private:
  ConsoleWidget *_edit;
  InputNotifier _onInput;
  QString _submit;
  int _pos;
  };

}
