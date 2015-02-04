#include "Dialog.h"
#include "QUiLoader"
#include "QVBoxLayout"
#include "QFile"
#include "QLineEdit"
#include "QAbstractButton"
#include "QSpinBox"

namespace UI
{

class Dialog::Wrapper
  {
public:
  virtual void setValue(const QVariant &) { }
  virtual QVariant value() const { return QVariant(); }
  };

template <typename T> class GenericTextWrapper : public Dialog::Wrapper
  {
public:
  GenericTextWrapper(T *t, ChangedNotifier *n)
      : _t(t)
    {
    QObject::connect(t, &T::editingFinished, [n, this]()
      {
      (*n)(_t->objectName());
      });
    }

  void setValue(const QVariant & t) override
    {
    _t->setText(t.toString());
    }

  QVariant value() const override
    {
    return _t->text();
    }

protected:
  T *_t;
  };

template <typename T, typename V> class GenericValueWrapper : public Dialog::Wrapper
  {
public:
  GenericValueWrapper(T *t, ChangedNotifier *n)
      : _t(t)
    {
    typedef void (T::*Changed)(V);

    QObject::connect(t, (Changed)&T::valueChanged, [n, this](V)
      {
      (*n)(_t->objectName());
      });
    }

  void setValue(const QVariant & t) override
    {
    _t->setValue(t.value<V>());
    }

  QVariant value() const override
    {
    return _t->value();
    }

protected:
  T *_t;
  };

template <typename T> class GenericButtonWrapper : public Dialog::Wrapper
  {
public:
  GenericButtonWrapper(T *t, ChangedNotifier *n)
    {
    QObject::connect(t, &T::clicked, [n, t]()
      {
      (*n)(t->objectName());
      });
    }
  };


Dialog::Dialog(const QString &str)
  {
  QFile file(str);
  if (!file.open(QFile::ReadOnly))
    {
    throw std::runtime_error("File doesn't exist");
    }

  QUiLoader loader;
  QWidget *w = loader.load(&file);
  if (!w)
    {
    qWarning() << loader.errorString();
    }

  auto layout = new QVBoxLayout;
  layout->setContentsMargins(0, 0, 0, 0);
  w->setParent(this);
  layout->addWidget(w);
  setLayout(layout);

  setWindowTitle(w->windowTitle());

  for (auto w : w->children())
    {
    auto wrap = makeWrapper(w);
    if (!wrap)
      {
      continue;
      }

    _elements[w->objectName()] = std::move(wrap);
    }

  if (auto d = qobject_cast<QDialog *>(w))
    {
    connect(d, SIGNAL(finished(int)), this, SLOT(done(int)));
    }
  }

Dialog::~Dialog()
  {
  }

void Dialog::setValue(const QString &str, const QVariant &val)
  {
  auto &w = _elements[str];
  if (!w)
    {
    return;
    }

  w->setValue(val);
  }

QVariant Dialog::value(const QString &str)
  {
  auto &w = _elements[str];
  if (!w)
    {
    qWarning() << "No child" << str;
    return QVariant();
    }

  return w->value();
  }

Dialog::Result Dialog::result() const
  {
  return QDialog::result() == QDialog::Accepted ? Accepted : Rejected;
  }

std::shared_ptr<Dialog::Wrapper> Dialog::makeWrapper(QObject *w)
  {
  if (auto l = qobject_cast<QLineEdit*>(w))
    {
    return std::make_shared<GenericTextWrapper<QLineEdit>>(l, &_changed);
    }
  if (auto l = qobject_cast<QSpinBox*>(w))
    {
    return std::make_shared<GenericValueWrapper<QSpinBox, int>>(l, &_changed);
    }
  if (auto l = qobject_cast<QAbstractButton*>(w))
    {
    return std::make_shared<GenericButtonWrapper<QAbstractButton>>(l, &_clicked);
    }

  return nullptr;
  }
}
