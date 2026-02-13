#include "register_lineedit.hpp"
#include "global_data.hpp"

RegisterLineEdit::RegisterLineEdit(QWidget *parent) :
    BaseItemWidget("", parent)
{
    initValueWidget();
}

void RegisterLineEdit::initValueWidget()
{
    layout->removeWidget(title_label);
    layout->removeWidget(value_widget);
    value_lineedit = new QLineEdit;
    value_lineedit->setFrame(false);
    value_lineedit->installEventFilter(this);
    layout->addWidget(value_lineedit);

    connect(value_lineedit, &QLineEdit::textChanged,
            [=] (QString text) {
                syncValueChanged(text);
        }
    );
}

void RegisterLineEdit::setData(QVariant data, bool sync_to_global)
{
    if (sync_to_global == false) {
        value_lineedit->blockSignals(true);
    }

    value_lineedit->setText(data.toString());

    if (sync_to_global == false) {
        value_lineedit->blockSignals(false);
    }
}

void RegisterLineEdit::setBitmask(QString bitmask)
{
    this->bitmask = bitmask;
}

QString RegisterLineEdit::getBitmask()
{
    return this->bitmask;
}

bool RegisterLineEdit::eventFilter(QObject* object, QEvent* event)
{
    if(object == value_lineedit && event->type() == QEvent::FocusIn) {
        emit sigFocusIn();
        return false;
    }
    return false;
}

void RegisterLineEdit::setChmod(int mod)
{
    BaseItemWidget::setChmod(mod);

    bool readonly = (mod == MOD_RO);
    value_lineedit->setReadOnly(readonly);
}
