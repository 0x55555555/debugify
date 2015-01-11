  #include "FileEditor.h"
#include "QFile"
#include "QFileInfo"
#include "QPainter"
#include "QTextBlock"
#include "QVBoxLayout"

namespace UI
{

CodeEditor::CodeEditor(QWidget *parent)
    : QPlainTextEdit(parent),
      _currentLine(-1)
  {
  lineNumberArea = new LineNumberArea(this);

  connect(this, SIGNAL(blockCountChanged(int)), this, SLOT(updateLineNumberAreaWidth(int)));
  connect(this, SIGNAL(updateRequest(QRect,int)), this, SLOT(updateLineNumberArea(QRect,int)));

  updateLineNumberAreaWidth(0);
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

void CodeEditor::addBreakpoint(int line)
  {
  _breakpoints.insert(line);
  }

void CodeEditor::removeBreakpoint(int line)
  {
  auto it = _breakpoints.find(line);
  if (it != _breakpoints.end())
    {
    _breakpoints.erase(it);
    }
  }

void CodeEditor::addActiveLine(int line)
  {
  _activelines.insert(line);
  highlightLines();
  }

void CodeEditor::removeActiveLine(int line)
  {
  auto it = _activelines.find(line);
  if (it != _activelines.end())
    {
    _activelines.erase(it);
    highlightLines();
    }
  }

void CodeEditor::setCurrentLine(int line)
  {
  _currentLine = line;
  highlightLines();
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

void CodeEditor::highlightLines()
  {
  QList<QTextEdit::ExtraSelection> extraSelections;

  auto highlightLine = [&](auto line, const auto &col)
    {
    QTextEdit::ExtraSelection selection;

    selection.format.setBackground(col);
    selection.format.setProperty(QTextFormat::FullWidthSelection, true);
    selection.cursor = QTextCursor(document()->findBlockByLineNumber(line));
    selection.cursor.clearSelection();
    extraSelections.append(selection);
      
    qDebug() << selection.cursor.blockNumber();
    };

  if (_currentLine != -1)
    {

    QColor lineColour = QColor(Qt::yellow).lighter(160);

    highlightLine(_currentLine, lineColour);
    }

  QColor activeColour = QColor(Qt::darkGray).lighter(160);
  xForeach(auto l, _activelines)
    {
    highlightLine(l, activeColour);
    }

  setExtraSelections(extraSelections);
  }

void CodeEditor::lineNumberAreaPaintEvent(QPaintEvent *event)
  {
  QPainter painter(lineNumberArea);
  painter.fillRect(event->rect(), Qt::lightGray);
  QTextBlock block = firstVisibleBlock();

  int blockNumber = block.blockNumber();
  int lineCount = 0;
  int top = (int) blockBoundingGeometry(block).translated(contentOffset()).top();
  int bottom = top + (int) blockBoundingRect(block).height();
  auto breakpoint = _breakpoints.lower_bound(blockNumber);

  const int width = lineNumberArea->width() - 5;
  const int fontHeight = fontMetrics().height();

  while (block.isValid() && top <= event->rect().bottom())
    {
    if (block.isVisible() && bottom >= event->rect().top())
      {
      QString number = QString::number(blockNumber + 1);
      painter.setPen(Qt::black);
      painter.drawText(0, top, width, fontHeight, Qt::AlignRight, number);
      ++lineCount;
      }

    int breakpointCount = 0;
    while (*breakpoint == blockNumber && breakpoint != _breakpoints.end())
      {
      int offset = std::min(breakpointCount * 3, width - fontHeight);

      painter.setBrush(Qt::red);
      painter.drawEllipse(2 + offset, top, fontHeight, fontHeight);

      ++breakpointCount;
      ++breakpoint;
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


  _editor->addBreakpoint(5);
  _editor->addBreakpoint(10);
  _editor->addBreakpoint(10);
  _editor->addBreakpoint(13);
  _editor->addBreakpoint(13);
  _editor->addBreakpoint(13);
  _editor->addBreakpoint(100);
  _editor->addBreakpoint(105);
  _editor->addBreakpoint(110);
  _editor->addBreakpoint(1000);
  _editor->addBreakpoint(1002);

  _editor->addActiveLine(6);
  _editor->addActiveLine(7);

  _editor->addActiveLine(9);

  _editor->setCurrentLine(5);
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

void FileEditor::addMarker(MarkerType m, size_t line)
  {
  if (m == Breakpoint)
    {
    _editor->addBreakpoint(line);
    }
  else if (m == ActiveLine)
    {
    _editor->addActiveLine(line);
    }
  else if (m == CurrentLine)
    {
    _editor->setCurrentLine(line);
    }
  }

void FileEditor::removeMarker(MarkerType m, size_t line)
  {
  if (m == Breakpoint)
    {
    _editor->removeBreakpoint(line);
    }
  else if (m == ActiveLine)
    {
    _editor->removeActiveLine(line);
    }
  else if (m == CurrentLine)
    {
    _editor->setCurrentLine(-1);
    }
  }

void FileEditor::marginClicked(int i)
  {
  _marginClicked(i);
  }

}
