#include "combobox_item_widget.hpp"
#include "global_data.hpp"

ComboBoxItemWidget::ComboBoxItemWidget(QString module, QString title, QWidget *parent) :
    BaseItemWidget(module, title, parent)
{
    initValueWidget();

    valueToIndexMap = new QMap<int, int>;
}

void ComboBoxItemWidget::addValue(QString text, int value)
{
    combobox->addItem(text, value);

    int index = valueToIndexMap->count();
    valueToIndexMap->insert(value, index);
}

void ComboBoxItemWidget::setValue(int value)
{
    int index = valueToIndexMap->value(value);
    combobox->setCurrentIndex(index);
}

void ComboBoxItemWidget::getValue(int *value)
{
    *value = combobox->currentIndex();
}

void ComboBoxItemWidget::setParam(QVariant params)
{
    QVariantList m = params.toList();
    QSignalBlocker filter_blocker(combobox);

    for (QVariant param : m) {
        QVariantMap l = param.toMap();
        if (l.contains("label") && l.contains("value")) {
            QString text = l["label"].toString();
            int value = l["value"].toInt();

            addValue(text, value);
        }
    }
}

void ComboBoxItemWidget::initValueWidget()
{
    combobox = new QComboBox;

    value_layout->addWidget(combobox);
    connect(combobox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &ComboBoxItemWidget::valueChanged);
    connect(this, &ComboBoxItemWidget::sigValueChanged,
            this, &ComboBoxItemWidget::handleUiChanged);
}

void ComboBoxItemWidget::valueChanged(int index)
{
    int value = combobox->itemData(index).toInt();
    syncValueChanged(value);
}

QVariant ComboBoxItemWidget::getData() const
{
    if (getTitle() == OP_TYPE) {
        if (combobox->currentIndex() <= 1) {
            return combobox->currentData();
        } else {
            return QVariant(0);
        }
    }
    return combobox->currentData();
}

void ComboBoxItemWidget::setData(QVariant data, bool sync_to_global)
{
    if (data == getData() && getTitle() != OP_TYPE) {
        return;
    }

    if (sync_to_global == false) {
        combobox->blockSignals(true);
    }

    setValue(data.toInt());

    if (sync_to_global == false) {
        combobox->blockSignals(false);
    }
}

void ComboBoxItemWidget::setChmod(int mod)
{
    BaseItemWidget::setChmod(mod);

    bool disabled = (mod == MOD_RO);
    combobox->setDisabled(disabled);
}
