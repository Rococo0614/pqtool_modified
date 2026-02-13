#include "ccmatrix_item_widget.hpp"
#include "global_data.hpp"
#include "register_chart_table.hpp"
#include <QtCore>
#include <QMainWindow>
#include <QTableWidget>
#include <QLineEdit>

CCMatrixItemWidget::CCMatrixItemWidget(QString module, QString title, LineChartView *chart, QWidget *parent) :
    BaseItemWidget(module, title, parent),
    chart(chart)
{
    this->value_type = TYPE_ARRAY;
    this->reg_value_type = TYPE_ARRAY;
    initValueWidget();
    retranslateUi();
    is_chart_value_double = false;
    validator_min = 0.0;
    validator_max = 0.0;
}

void CCMatrixItemWidget::initValueWidget()
{
    foldable_register_window = new FoldableRegisterTableWindow;
    if (chart != nullptr) {
        chart_reg_table = new RegisterChartTable(chart);
        if (is_chart_value_double) {
            chart_reg_table->setDoubleValidator(validator_min, validator_max);
        }
        foldable_register_window->setWidget(chart_reg_table);

        connect(chart_reg_table, &RegisterChartTable::sigValueChanged,
                this, &CCMatrixItemWidget::handleUiChanged);
    } else {
        color_correction_table = new CcmTable();
        color_correction_table->setRegValueType(this->reg_value_type);
        foldable_register_window->setWidget(color_correction_table);

        connect(color_correction_table, &CcmTable::sigValueChanged,
                this, &CCMatrixItemWidget::handleUiChanged);
        connect(color_correction_table, &CcmTable::sigPageWriteData,
                this, &CCMatrixItemWidget::handleWritePageData);
    }
    foldable_register_window->hide();

    push_button = new QPushButton;

    value_layout->addWidget(push_button);
    connect(push_button, &QPushButton::clicked,
            this, &CCMatrixItemWidget::handleShowWindow);
}

QVariant CCMatrixItemWidget::getData() const
{
    QVariantList data;
    QVariantList list;

    if (chart != nullptr) {

    } else {
        list = color_correction_table->getData().value<QVariantList>();
        for (int row=0; row<list.size(); row++)
        {
            QVariantList col_list = list.at(row).value<QVariantList>();
            for (int col=0; col<col_list.size(); col++) {
                data.append(col_list.at(col).value<int>());
            }
        }
    }

    return data;
}

void CCMatrixItemWidget::setData(QVariant data, bool sync_to_global)
{
    if (chart != nullptr) {
        if (sync_to_global == false) {
            chart_reg_table->blockSignals(true);
        }

        ///TODO: set data

        if (sync_to_global == false) {
            chart_reg_table->blockSignals(false);
        }
    } else {
        if (sync_to_global == false) {
            color_correction_table->blockSignals(true);
        }

        color_correction_table->setData(data);

        if (sync_to_global == false) {
            color_correction_table->blockSignals(false);
        }
    }
}

void CCMatrixItemWidget::setRegSectionKeyList(QList<SecKeyType> &&reg_section_key_list)
{
    BaseItemWidget::setRegSectionKeyList(std::move(reg_section_key_list));

    if (!reg_section_key_list.isEmpty()) {
        foldable_register_window->setDesp(reg_section_key_list.first().first + "/" + title_label->text());
    }

    if (chart != nullptr) {
        chart_reg_table->setRegSectionKeyList(std::move(reg_section_key_list));
    } else {
        color_correction_table->setRegSectionKeyList(std::move(reg_section_key_list));
        color_correction_table->initLineEdits();
    }
}

void CCMatrixItemWidget::addRegSectionKey(QString section, QString key)
{
    BaseItemWidget::addRegSectionKey(section, key);

    if (!reg_section_key_list.isEmpty()) {
        foldable_register_window->setDesp(reg_section_key_list.first().first + "/" + title_label->text());
    }

    if (chart != nullptr) {
        chart_reg_table->addRegSectionKey(section, key);
    } else {
        color_correction_table->addRegSectionKey(section, key);
        color_correction_table->initLineEdits();
    }
}

void CCMatrixItemWidget::setRegValueType(QString value_type)
{
    this->reg_value_type = value_type;
    if (chart != nullptr) {
        chart_reg_table->setRegValueType(value_type);
    } else {
        color_correction_table->setRegValueType(value_type);
    }
}

void CCMatrixItemWidget::setOpType(QString op_type)
{
    BaseItemWidget::setOpType(op_type);

    if (chart != nullptr) {
        chart_reg_table->setOpType(op_type);
    } else {
        color_correction_table->setOpType(op_type);
    }
}

void CCMatrixItemWidget::setChmod(int mod)
{
    BaseItemWidget::setChmod(mod);
    color_correction_table->setChmod(mod);
}

void CCMatrixItemWidget::setParam(QVariant params)
{
    QVariantMap m = params.toMap();
    if (m.count() == 0) {
        return;
    }

    if (m.contains("col") || m.contains("row")) {
        horizontal_headers = m["col"].toStringList();
        vertical_headers = m["row"].toStringList();

        if (color_correction_table != nullptr) {
            QStringList v_labels = vertical_headers;
            QStringList h_labels = horizontal_headers;

            color_correction_table->setRowCount(v_labels.count());
            color_correction_table->setColumnCount(h_labels.count());
            color_correction_table->setVerticalHeaderLabels(v_labels);
            color_correction_table->setHorizontalHeaderLabels(h_labels);
        }
    }

    if (m.contains("fold_col"))
    {
        color_correction_table->setFoldableColumn(m["fold_col"].value<int>());
    }

    if (m.contains("min") || m.contains("max")) {
        if (color_correction_table != nullptr) {
            color_correction_table->setRange(m["min"].toInt(), m["max"].toInt());
        }
    }
}

void CCMatrixItemWidget::handleShowWindow()
{
    foldable_register_window->show();
}

QStringList CCMatrixItemWidget::getMatrixHorizontalHeaders()
{
    return horizontal_headers;
}

QStringList CCMatrixItemWidget::getMatrixVerticalHeaders()
{
    return vertical_headers;
}

void CCMatrixItemWidget::refreshData(const SecKeyType&)
{
}

void CCMatrixItemWidget::setDoubleValidator(double min, double max)
{
    is_chart_value_double = true;
    validator_min = min;
    validator_max = max;
}

void CCMatrixItemWidget::handleWritePageData()
{
    emit sigWritePageData();
}

void CCMatrixItemWidget::retranslateUi()
{
    push_button->setText(tr("Edit this Matrix"));
}
