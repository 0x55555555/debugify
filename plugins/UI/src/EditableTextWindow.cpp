#include "EditableTextWindow.h"
#include "QMouseEvent"
#include "QDebug"

namespace UI
{
EditableTextEdit::EditableTextEdit(EditableTextWindow *window)
    : _window(window)
  {
  xAssert(_window);
  QFont font("Courier New");
  font.setStyleHint(QFont::Monospace);
  setFont(font);

  document()->setDocumentMargin(0.0);
  document()->setDefaultStyleSheet("a { color: black; text-decoration: none; }");
  }

void EditableTextEdit::mouseDoubleClickEvent(QMouseEvent *ev)
  {
  auto anchor = anchorAt(ev->pos());

  (*_window->clicked())(anchor, ev->pos().x(), ev->pos().y());
  }

void EditableTextEdit::contextMenuEvent(QContextMenuEvent *ev)
  {
  auto anchor = anchorAt(ev->pos());

  (*_window->contextMenu())(anchor, ev->pos().x(), ev->pos().y());
  }

EditableTextWindow::EditableTextWindow(bool toolbar)
    : Dockable(toolbar),
      _edit(new EditableTextEdit(this))
  {
  setWidget(_edit);
  }

EditableTextWindow::~EditableTextWindow()
  {
  }

void EditableTextWindow::setContents(const QString &html)
  {
  _edit->setHtml(html);
  }

}
