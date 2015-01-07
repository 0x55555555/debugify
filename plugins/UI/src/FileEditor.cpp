  #include "FileEditor.h"
#include "QFile"
#include "QFileInfo"
#include "QPainter"
#include "QTextBlock"
#include "QVBoxLayout"

namespace UI
{

CodeEditor::CodeEditor(QWidget *parent) : QPlainTextEdit(parent)
  {
  lineNumberArea = new LineNumberArea(this);

  connect(this, SIGNAL(blockCountChanged(int)), this, SLOT(updateLineNumberAreaWidth(int)));
  connect(this, SIGNAL(updateRequest(QRect,int)), this, SLOT(updateLineNumberArea(QRect,int)));
  connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(highlightCurrentLine()));

  updateLineNumberAreaWidth(0);
  highlightCurrentLine();
  }

int CodeEditor::lineNumberAreaWidth()
  {
  int digits = 1;
  int max = qMax(1, blockCount());
  while (max >= 10)
    {
    max /= 10;
    ++digits;
    }

  int space = 10 + fontMetrics().width(QLatin1Char('9')) * digits;

  return space;
  }

void CodeEditor::marginMouseReleaseEvent(QMouseEvent *e)
  {
  auto cursor = cursorForPosition(QPoint(0, e->pos().y()));
  emit marginClicked(cursor.blockNumber());
  }

void CodeEditor::updateLineNumberAreaWidth(int)
  {
  setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
  }

void CodeEditor::updateLineNumberArea(const QRect &rect, int dy)
  {
  if (dy)
    {
    lineNumberArea->scroll(0, dy);
    }
  else
    {
    lineNumberArea->update(0, rect.y(), lineNumberArea->width(), rect.height());
    }

  if (rect.contains(viewport()->rect()))
    {
    updateLineNumberAreaWidth(0);
    }
  }

void CodeEditor::resizeEvent(QResizeEvent *e)
  {
  QPlainTextEdit::resizeEvent(e);

  QRect cr = contentsRect();
  lineNumberArea->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
  }

void CodeEditor::highlightCurrentLine()
  {
  QList<QTextEdit::ExtraSelection> extraSelections;

  if (!isReadOnly()) {
    QTextEdit::ExtraSelection selection;

    QColor lineColor = QColor(Qt::yellow).lighter(160);

    selection.format.setBackground(lineColor);
    selection.format.setProperty(QTextFormat::FullWidthSelection, true);
    selection.cursor = textCursor();
    selection.cursor.clearSelection();
    extraSelections.append(selection);
    }

  setExtraSelections(extraSelections);
  }

void CodeEditor::lineNumberAreaPaintEvent(QPaintEvent *event)
  {
  QPainter painter(lineNumberArea);
  painter.fillRect(event->rect(), Qt::lightGray);
  QTextBlock block = firstVisibleBlock();

  int blockNumber = block.blockNumber();
  int top = (int) blockBoundingGeometry(block).translated(contentOffset()).top();
  int bottom = top + (int) blockBoundingRect(block).height();

  while (block.isValid() && top <= event->rect().bottom())
    {
    if (block.isVisible() && bottom >= event->rect().top())
      {
      QString number = QString::number(blockNumber + 1);
      painter.setPen(Qt::black);
      painter.drawText(0, top, lineNumberArea->width()-5, fontMetrics().height(),
                       Qt::AlignRight, number);
      }

    block = block.next();
    top = bottom;
    bottom = top + (int) blockBoundingRect(block).height();
    ++blockNumber;
    }
  }

FileEditor::FileEditor(const QString &path)
    : _path(path)
  {
  auto layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  _editor = new CodeEditor(this);
  layout->addWidget(_editor);

  QFont font("Courier New");
  font.setStyleHint(QFont::Monospace);
  setFont(font);

  QFile f(path);
  if (f.open(QFile::ReadOnly))
    {
    _editor->setPlainText(f.readAll());
    }

  if (!QFileInfo(f).isWritable())
    {
    _editor->setReadOnly(true);
    }

  connect(_editor, SIGNAL(marginClicked(int)), this, SLOT(marginClicked(int)));
  }

void FileEditor::focusOnLine(size_t line)
  {
  QTextCursor cursor(_editor->document()->findBlockByLineNumber(line - 1));
  cursor.select(QTextCursor::LineUnderCursor);
  _editor->setTextCursor(cursor);
  }

QString FileEditor::makeKey(const QString &filename)
  {
  return filename;
  }

QString FileEditor::path() const
  {
  return _path;
  }

QString FileEditor::key() const
  {
  return _path;
  }

QString FileEditor::title() const
  {
  QFileInfo info(_path);
  return info.fileName();
  }

void FileEditor::marginClicked(int i)
  {
  _marginClicked(i);
  }

}
