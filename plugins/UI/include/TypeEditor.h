#pragma once
#include "UiGlobal.h"
#include "Editor.h"
#include "Module.h"
#include "Type.h"
#include "QtWidgets/QWidget"

namespace Ui {
class TypeEditor;
}

class QScrollArea;
class QListWidget;

namespace UI
{

class TypeManager;
class CachedType;

class NoSuchTypeException : public std::runtime_error
  {
public:
  NoSuchTypeException() : std::runtime_error("No such type") { }
  };

class TypeEditor : public Editor
  {
  Q_OBJECT

public:
  TypeEditor(TypeManager *types, const QString &filename);

  static QString makeKey(const QString &filename);

  QString path() const X_OVERRIDE;
  QString key() const X_OVERRIDE;
  QString title() const X_OVERRIDE;

signals:
  void selectType(const QString &path);
  void selectFile(const QString &file, int line);

private slots:
  void selectIndex(const QModelIndex &);
  void repopulate();

private:
  void populateLocation(const Type &type);
  void populatorLocator(QWidget *widg, const std::shared_ptr<CachedType> &);
  void populateAttributes(const Type &type);
  void populateTemplateArguments(const Type &type);
  void populateMembers(const Type &type);
  void populateFunctions(const Type &type);
  void populateBaseClasses(const Type &type);

  void addTypeItem(QListWidget *list, const char *name, const Type &type);
  QWidget *makeTypeLink(const Type &type);
  QWidget *makeTypeLink(const std::shared_ptr<CachedType> &object);

  QString getTypeName(const Type &path);

  enum
    {
    PathRole = Qt::UserRole
    };

  std::shared_ptr<CachedType> _type;
  TypeManager *_types;
  Ui::TypeEditor *ui;

  QScrollArea *_functions;
  };

}
