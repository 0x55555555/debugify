#pragma once
#include "UiGlobal.h"
#include "Editor.h"
#include "Utilities/XNotifier.h"
#include "Module.h"
#include "QtWidgets/QPlainTextEdit"
#include <set>

namespace UI
{

class CodeEditor : public QPlainTextEdit
  {
  Q_OBJECT

public:
  CodeEditor(QWidget *parent = 0);

  QWidget *widget() { return this; }

  void lineNumberAreaPaintEvent(QPaintEvent *event);
  int lineNumberAreaWidth();

  void marginMouseReleaseEvent(QMouseEvent *e);

  void addBreakpoint(int line);
  void removeBreakpoint(int line);

  void addActiveLine(int line);
  void removeActiveLine(int line);

  void setCurrentLine(int line);

signals:
  void marginClicked(int);

protected:
  void resizeEvent(QResizeEvent *event);

private slots:
  void updateLineNumberAreaWidth(int newBlockCount);
  void highlightLines();
  void updateLineNumberArea(const QRect &, int);

private:
  QWidget *lineNumberArea;
  std::multiset<int> _breakpoints;
  int _currentLine;
  std::set<int> _activelines;
  };

class LineNumberArea : public QWidget
  {
public:
  LineNumberArea(CodeEditor *editor) : QWidget(editor)
    {
    codeEditor = editor;
    }

  QSize sizeHint() const
    {
    return QSize(codeEditor->lineNumberAreaWidth(), 0);
    }

signals:


protected:
  void paintEvent(QPaintEvent *event)
    {
    codeEditor->lineNumberAreaPaintEvent(event);
    }

  void mouseReleaseEvent(QMouseEvent *e)
    {
    codeEditor->marginMouseReleaseEvent(e);
    }

private:
  CodeEditor *codeEditor;
  };

/// \expose unmanaged
X_DECLARE_NOTIFIER(MarginClickNotifier, std::function<void (int)>);

/// \expose
class FileEditor : public Editor
  {
  Q_OBJECT
public:
  /// \noexpose
  FileEditor(const QString &filename);

  void focusOnLine(size_t line);

  static QString makeKey(const QString &filename);

  QString path() const X_OVERRIDE;
  QString key() const X_OVERRIDE;
  QString title() const X_OVERRIDE;

  MarginClickNotifier *marginClicked() { return &_marginClicked; }

  /// \expose
  enum MarkerType
    {
    Breakpoint,
    ActiveLine,
    CurrentLine
    };

  void addMarker(MarkerType m, size_t line);
  void removeMarker(MarkerType m, size_t line);

private slots:
  void marginClicked(int);

private:
  QString _path;
  CodeEditor *_editor;
  MarginClickNotifier _marginClicked;
  };

}
