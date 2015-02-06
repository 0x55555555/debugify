#include "MessageBox.h"
#include "QMessageBox"

namespace UI
{

int MessageBox::question(QString title, QString text, int buttonsInt)
  {
  return QMessageBox::question(0, title, text, (QMessageBox::StandardButtons)buttonsInt);
  }

}
