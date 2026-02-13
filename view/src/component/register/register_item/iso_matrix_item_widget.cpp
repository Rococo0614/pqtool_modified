#include "iso_matrix_item_widget.hpp"
#include "global_data.hpp"
#include "utils.hpp"

#include <QtCore>
#include <QMainWindow>
#include <QTableWidget>
#include <QLineEdit>

IsoMatrixItemWidget::IsoMatrixItemWidget(QString module, QString title, QWidget *parent) :
    BaseItemWidget(module, title, parent)
{
    updateValueType();
    initValueWidget();
    retranslateUi();
}

void IsoMatrixItemWidget::initValueWidget()
{
    register_window = new RegisterTableWindow;
    iso_table = new IsoTable();
    iso_table->setRegValueType(this->reg_value_type);
    register_window->setWidget(iso_table);
    register_window->hide();

    connect(iso_table, &IsoTable::sigValueChanged,
            this, &IsoMatrixItemWidget::handleUiChanged);
    connect(iso_table, &IsoTable::sigPageWriteData,
            this, &IsoMatrixItemWidget::handleWritePageData);

    push_button = new QPushButton;

    value_layout->addWidget(push_button);
    connect(push_button, &QPushButton::clicked,
            this, &IsoMatrixItemWidget::handleShowWindow);
}

void IsoMatrixItemWidget::handleShowWindow()
{
    if (register_window->isMinimized()) {
        register_window->showNormal();
    }
    register_window->show();
    register_window->activateWindow();
}

void IsoMatrixItemWidget::handleWritePageData()
{
    emit sigWritePageData();
}

void IsoMatrixItemWidget::refreshData(const SecKeyType&)
{
}

void IsoMatrixItemWidget::setParam(QVariant params)
{
    QMap<QString, QVariant> m = params.toMap();
    if (m.count() == 0) {
        return;
    }

    if (m.contains("min") && m.contains("max")) {
        QVariantList min = m["min"].toList();
        QVariantList max = m["max"].toList();
        if (!(min.isEmpty() || max.isEmpty())) {
            iso_table->setRangeList(min, max);
        } else {
            iso_table->setRange(m["min"].toInt(), m["max"].toInt());
        }
    }

    if (m.contains("row")) {
        vertical_headers = m["row"].toStringList();

        if (iso_table != nullptr) {
            QStringList v_labels = vertical_headers;
            iso_table->setRowCount(v_labels.count());
            iso_table->setVerticalHeaderLabels(v_labels);
        }
    }

    if (m.contains("header_type")) {
        QString type = m["header_type"].toString();
        if (iso_table != nullptr) {
            iso_table->setHeaderType(type);
        }
    }

    if (m.contains("value_type")) {
        QString type = m["value_type"].toString();
        if (iso_table != nullptr) {
            iso_table->setValueType(type);
        }
    }
}

QVariant IsoMatrixItemWidget::getData() const
{
    return iso_table->getData();
}

void IsoMatrixItemWidget::setRegSectionKeyList(QList<SecKeyType> &&reg_section_key_list)
{
    BaseItemWidget::setRegSectionKeyList(std::move(reg_section_key_list));

    if (!reg_section_key_list.isEmpty()) {
        register_window->setDesp(reg_section_key_list.first().first + "/" + title_label->text());
    }

    iso_table->setRegSectionKeyList(std::move(reg_section_key_list));
    iso_table->initLineEdits();
    updateValueType();
}

void IsoMatrixItemWidget::addRegSectionKey(QString section, QString key)
{
    BaseItemWidget::addRegSectionKey(section, key);

    if (!reg_section_key_list.isEmpty()) {
        register_window->setDesp(reg_section_key_list.first().first + "/" + title_label->text());
    }

    iso_table->addRegSectionKey(section, key);
    iso_table->initLineEdits();
    updateValueType();
}

void IsoMatrixItemWidget::setOpType(QString op_type)
{
    BaseItemWidget::setOpType(op_type);

    iso_table->setOpType(op_type);
}

void IsoMatrixItemWidget::setRegValueType(QString value_type)
{
    IObserver::setRegValueType(value_type);

    iso_table->setRegValueType(value_type);
}

void IsoMatrixItemWidget::setValueMappingVec(QVector<QVariantList> &&value_mapping_vec)
{
    QVector<QVariantList> v = value_mapping_vec;
    BaseItemWidget::setValueMappingVec(std::move(v));

    iso_table->setValueMappingVec(std::move(value_mapping_vec));
}

void IsoMatrixItemWidget::setData(QVariant data, bool sync_to_global)
{
    if (sync_to_global == false) {
        iso_table->blockSignals(true);
    }

    iso_table->setData(data);

    if (sync_to_global == false) {
        iso_table->blockSignals(false);
    }
}

void IsoMatrixItemWidget::retranslateUi()
{
    push_button->setText(tr("Edit this Matrix"));
}

void IsoMatrixItemWidget::updateValueType()
{
    if (reg_section_key_list.size() > 1) {
        this->value_type = TYPE_ARRAY_2D;
    } else {
        this->value_type = TYPE_ARRAY;
    }
}

void IsoMatrixItemWidget::setChmod(int mod)
{
    BaseItemWidget::setChmod(mod);
    iso_table->setChmod(mod);
}

IsoMatrixItemWidget::~IsoMatrixItemWidget()
{
    delete register_window;
}