#include "TypeEditor.h"
#include "ui_TypeEditor.h"
#include "TypeManager.h"
#include "TypeMember.h"
#include "FunctionMember.h"
#include "QLayout"
#include "QPushButton"
#include "QScrollArea"
#include <map>

namespace UI
{

TypeEditor::TypeEditor(TypeManager *types, const QString &path)
    : _type(types->findType(path)),
      _types(types),
      ui(new Ui::TypeEditor),
      _functions(nullptr)
  {
  if (!_type)
    {
    throw NoSuchTypeException();
    }

  ui->setupUi(this);

  if (_type->definitions.size())
    {
    repopulate();

    connect(ui->simplifyTypes, SIGNAL(clicked()), this, SLOT(repopulate()));
    connect(ui->members, SIGNAL(activated(QModelIndex)), this, SLOT(selectIndex(QModelIndex)));

    connect(ui->baseClasses, SIGNAL(activated(QModelIndex)), this, SLOT(selectIndex(QModelIndex)));
    }
  else
    {
    ui->typeAttributes->setVisible(false);
    ui->dataPane->setVisible(false);
    }

  populatorLocator(ui->locator, _type);
  }

QString TypeEditor::makeKey(const QString &path)
  {
  return path;
  }

QString TypeEditor::path() const
  {
  return _type->path;
  }

QString TypeEditor::key() const
  {
  return _type->path;
  }

QString TypeEditor::title() const
  {
  QFontMetrics metrics(font());
  return metrics.elidedText(_type->basename + _type->specialisation, Qt::ElideRight, 150);
  }

void TypeEditor::selectIndex(const QModelIndex &idx)
  {
  QString path = idx.data(PathRole).toString();
  emit selectType(path);
  }

void TypeEditor::repopulate()
  {
  auto firstDef = _type->definitions.front();

  populateAttributes(firstDef);
  populateTemplateArguments(firstDef);
  populateFunctions(firstDef);
  populateBaseClasses(firstDef);
  populateMembers(firstDef);
  }

void TypeEditor::populatorLocator(QWidget *widg, const std::shared_ptr<CachedType> &object)
  {
  if (object->parent)
    {
    populatorLocator(widg, object->parent);
    }

  widg->layout()->addWidget(makeTypeLink(object));
  }

void TypeEditor::populateAttributes(const Type &type)
  {
  ui->sizeBytes->setText(QString("%1 bytes").arg(type.size()));
  }

void TypeEditor::populateTemplateArguments(const Type &type)
  {
  ui->templateArguments->clear();
  ui->templateArgumentsGroup->setVisible(type.templateArgumentCount() > 0);

  for (size_t i = 0; i < type.templateArgumentCount(); ++i)
    {
    auto member = type.templateArgumentType(i);

    addTypeItem(ui->templateArguments, nullptr, member);
    }
  }

void TypeEditor::populateFunctions(const Type &type)
  {
  ui->functionsGroup->setVisible(type.functionCount() > 0);

  if (_functions)
    {
    delete _functions;
    _functions = nullptr;
    }

  _functions = new QScrollArea(ui->functionsGroup);
  ui->functionsGroup->layout()->addWidget(_functions);

  auto widg = new QWidget;
  auto functionList = new QVBoxLayout(widg);

  for (size_t i = 0; i < type.functionCount(); ++i)
    {
    auto fn = type.function(i);

    auto layout = new QHBoxLayout;

    layout->addWidget(makeTypeLink(fn.returnType()));
    layout->addWidget(new QLabel(QString(fn.name().data()) + "("));

    for (size_t j = 0; j < fn.argumentCount(); ++j)
      {
      layout->addWidget(makeTypeLink(fn.argument(j)));
      }

    layout->addWidget(new QLabel(")"));

    functionList->addLayout(layout);
    }

  _functions->setWidget(widg);
  }

void TypeEditor::populateMembers(const Type &type)
  {
  ui->membersGroup->setVisible(type.memberCount() > 0);
  ui->members->clear();

  for (size_t i = 0; i < type.memberCount(); ++i)
    {
    auto member = type.member(i);

    addTypeItem(ui->members, member.name().data(), member.type());
    }
  }

void TypeEditor::populateBaseClasses(const Type &type)
  {
  ui->baseClassesGroup->setVisible(type.directBaseClassCount() > 0 || type.virtualBaseClassCount() > 0);
  ui->baseClasses->clear();

  struct Item
    {
    QString given;
    Type type;
    };

  std::map<size_t, Item> items;

  for (size_t i = 0; i < type.directBaseClassCount(); ++i)
    {
    auto base = type.directBaseClass(i);

    items[base.byteOffset()] = { base.type().name().data(), base.type() };
    }

  for (size_t i = 0; i < type.virtualBaseClassCount(); ++i)
    {
    auto base = type.virtualBaseClass(i);

    items[base.byteOffset()] = { base.type().name().data(), base.type() };
    }

  xForeach(auto &item, items)
    {
    addTypeItem(ui->baseClasses, nullptr, item.second.type);
    }
  }

QString TypeEditor::getTypeName(const Type &type)
  {
  auto canon = type.canonicalType();

  if (canon.isTypedef())
    {
    canon = canon.typedefType();
    }

  while (canon.isReference())
    {
    canon = canon.dereferencedType();
    }

  while (canon.isPointer())
    {
    canon = canon.pointeeType();
    }

  return canon.name().data();
  }

void TypeEditor::addTypeItem(QListWidget *list, const char *name, const Type &type)
  {
  auto resolveTypes = ui->simplifyTypes->checkState() == Qt::Checked;

  auto resolved = getTypeName(type);
  QString typeName = resolveTypes ? resolved : type.name().data();

  QString text;
  if (name)
    {
    text = QString("%1 : %2").arg(name).arg(typeName);
    }
  else
    {
    text = typeName;
    }

  auto listItem = new QListWidgetItem(text);
  listItem->setData(PathRole, resolved);

  list->addItem(listItem);
  }

QWidget *TypeEditor::makeTypeLink(const Type &type)
  {
  auto resolveTypes = ui->simplifyTypes->checkState() == Qt::Checked;

  auto resolved = getTypeName(type);
  auto found = _types->findType(resolved);

  if (!found)
    {
    return new QLabel(resolveTypes ? resolved : type.name().data());
    }

  return makeTypeLink(found);
  }

QWidget *TypeEditor::makeTypeLink(const std::shared_ptr<CachedType> &object)
  {
  auto button = new QPushButton(object->basename + object->specialisation);
  if (object == _type)
    {
    button->setEnabled(false);
    }
  connect(button, &QPushButton::clicked, [this, object]()
    {
    emit selectType(object->path);
    });

  return button;
  }

}
