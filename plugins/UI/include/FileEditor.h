#pragma once
#include "UiGlobal.h"
#include "Editor.h"
#include "Module.h"
#include "QtWidgets/QPlainTextEdit"

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

protected:
  void resizeEvent(QResizeEvent *event);

private slots:
  void updateLineNumberAreaWidth(int newBlockCount);
  void highlightCurrentLine();
  void updateLineNumberArea(const QRect &, int);

private:
  QWidget *lineNumberArea;
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

protected:
  void paintEvent(QPaintEvent *event)
    {
    codeEditor->lineNumberAreaPaintEvent(event);
    }

private:
  CodeEditor *codeEditor;
  };

class FileEditor : public Editor
  {
public:
  FileEditor(const QString &filename);

  static QString makeKey(const QString &filename);

  QString path() const X_OVERRIDE;
  QString key() const X_OVERRIDE;
  QString title() const X_OVERRIDE;

private:
  QString _path;
  CodeEditor *_editor;
  };

}
