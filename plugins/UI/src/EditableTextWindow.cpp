#include "EditableTextWindow.h"
#include "QMouseEvent"
#include "QDebug"

namespace UI
{

EditableTextWindow::EditableTextWindow()
  {
  QFont font("Courier New");
  font.setStyleHint(QFont::Monospace);
  setFont(font);

  document()->setDocumentMargin(0.0);

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
