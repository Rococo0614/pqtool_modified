#include "op_type_combobox_widget.hpp"

#include "combobox_item_widget.hpp"
#include "global_data.hpp"

OpTypeComboBoxWidget::OpTypeComboBoxWidget(QString module, QString title, QWidget *parent) :
    BaseItemWidget(module, title, parent)
{
    initValueWidget();

    valueToIndexMap = new QMap<int, int>;
    for (auto config_item : config_map.toStdMap()) {
        addValue(config_item.second, static_cast<int>(config_item.first));
    }

    // set default to manual
    int index = this->combobox->findData(static_cast<int>(OP_TYPE_IDX::MANUAL));
    if (index != -1) {
        this->combobox->setCurrentIndex(index);
    }
}

void OpTypeComboBoxWidget::setConfigSection(QString section)
{
    this->config_section = section;
}

QString OpTypeComboBoxWidget::convertOpTypeName(OP_TYPE_IDX idx)
{
    QString op_type;
    switch (idx) {
        case OP_TYPE_IDX::AUTO:
            op_type = OP_TYPE_AUTO;
            break;
        case OP_TYPE_IDX::MANUAL:
            op_type = OP_TYPE_MANUAL;
            break ;
        default:
            op_type = OP_TYPE_MANUAL;
            break;
    }

    return op_type;
}

void OpTypeComboBoxWidget::setData(QVariant v, bool sync_to_global)
{
    if (sync_to_global == false) {
        combobox->blockSignals(true);
    }

    combobox->setCurrentIndex(v.toInt());
    if (sync_to_global == false) {
        combobox->blockSignals(false);
    }
}

QVariant OpTypeComboBoxWidget::getData() const
{
    return combobox->currentData();
}

/*QJsonObject OpTypeComboBoxWidget::getJsonData() const
{
    return QJsonObject();
}*/

void OpTypeComboBoxWidget::addValue(QString text, int value)
{
    combobox->addItem(text, value);

    int index = valueToIndexMap->count();
    valueToIndexMap->insert(value, index);
}

void OpTypeComboBoxWidget::setValue(int value)
{
    int index = valueToIndexMap->value(value);
    combobox->setCurrentIndex(index);
}

void OpTypeComboBoxWidget::initValueWidget()
{
    combobox = new QComboBox;

    value_layout->addWidget(combobox);
    connect(combobox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &OpTypeComboBoxWidget::valueChanged);
    connect(this, &OpTypeComboBoxWidget::sigValueChanged,
            this, &OpTypeComboBoxWidget::handleUiChanged);
}

void OpTypeComboBoxWidget::valueChanged(int index)
{
    if (this->config_section.isEmpty()) {
        return;
    }

    int value = combobox->itemData(index).toInt();
    ///TODO: load config file
    switch (value) {
        case static_cast<int>(OP_TYPE_IDX::AUTO):
            break;
        case static_cast<int>(OP_TYPE_IDX::MANUAL):
            break;
    }
    syncValueChanged(value);
}

void OpTypeComboBoxWidget::setChmod(int mod)
{
    BaseItemWidget::setChmod(mod);

    bool disabled = (mod == MOD_RO);
    combobox->setDisabled(disabled);
}
