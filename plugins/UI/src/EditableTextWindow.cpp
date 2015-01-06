#include "EditableTextWindow.h"
#include "QMouseEvent"
#include "QDebug"

namespace UI
{

EditableTextWindow::EditableTextWindow()
  {
  document()->setDefaultStyleSheet("a { color: black; text-decoration: none; }");
  }

EditableTextWindow::~EditableTextWindow()
  {
  }

void EditableTextWindow::setContents(const QString &html)
  {
  setHtml(html);
  }

void EditableTextWindow::mouseDoubleClickEvent(QMouseEvent *e)
  {
  auto anchor = anchorAt(e->pos());

  if (anchor.length())
    {
    _clicked(anchor);
    }
  }

}
