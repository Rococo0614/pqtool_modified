#include "matrix_item_widget.hpp"
#include "global_data.hpp"
#include "register_chart_table.hpp"
#include <QtCore>
#include <QMainWindow>
#include <QTableWidget>
#include <QLineEdit>

MatrixItemWidget::MatrixItemWidget(QString module, QString title, LineChartView *chart, QWidget *parent) :
    BaseItemWidget(module, title, parent),
    chart(chart)
{
    this->value_type = TYPE_ARRAY;
    initValueWidget();
    retranslateUi();
    is_chart_value_double = false;
    validator_min = 0.0;
    validator_max = 0.0;
}

void MatrixItemWidget::initValueWidget()
{
    register_window = new RegisterTableWindow;
    if (chart != nullptr) {
        chart_reg_table = new RegisterChartTable(chart);
        if (is_chart_value_double) {
            chart_reg_table->setDoubleValidator(validator_min, validator_max);
        }
        register_window->setWidget(chart_reg_table);

        connect(chart_reg_table, &RegisterChartTable::sigValueChanged,
                this, &MatrixItemWidget::handleUiChanged);
    } else {
        reg_table = new RegisterTable();

        reg_table->setRegValueType(this->reg_value_type);
        register_window->setWidget(reg_table);

        connect(reg_table, &RegisterTable::sigValueChanged,
                this, &MatrixItemWidget::handleUiChanged);
        connect(reg_table, &RegisterTable::sigPageWriteData,
                this, &MatrixItemWidget::handleWritePageData);
    }
    register_window->hide();

    push_button = new QPushButton;

    value_layout->addWidget(push_button);
    connect(push_button, &QPushButton::clicked,
            this, &MatrixItemWidget::handleShowWindow);
}

QVariant MatrixItemWidget::getData() const
{
    QVariant data;
    if (chart != nullptr) {
    } else {
        data = reg_table->getData();
    }

    return data;
}

void MatrixItemWidget::setData(QVariant data, bool sync_to_global)
{
    if (chart != nullptr) {
        if (sync_to_global == false) {
            chart_reg_table->blockSignals(true);
        }

        reg_table->setData(data);

        if (sync_to_global == false) {
            chart_reg_table->blockSignals(false);
        }
    } else {
        if (sync_to_global == false) {
            reg_table->blockSignals(true);
        }

        reg_table->setData(data);

        if (sync_to_global == false) {
            reg_table->blockSignals(false);
        }
    }
}

void MatrixItemWidget::setRegSectionKeyList(QList<SecKeyType> &&reg_section_key_list)
{
    BaseItemWidget::setRegSectionKeyList(std::move(reg_section_key_list));

    if (!reg_section_key_list.isEmpty()) {
        register_window->setDesp(reg_section_key_list.first().first + "/" + title_label->text());
    }

    if (chart != nullptr) {
        chart_reg_table->setRegSectionKeyList(std::move(reg_section_key_list));
    } else {
        reg_table->setRegSectionKeyList(std::move(reg_section_key_list));
        reg_table->initLineEdits();
    }
}

void MatrixItemWidget::addRegSectionKey(QString section, QString key)
{
    BaseItemWidget::addRegSectionKey(section, key);

    if (!reg_section_key_list.isEmpty()) {
        register_window->setDesp(reg_section_key_list.first().first + "/" + title_label->text());
    }

    if (chart != nullptr) {
        chart_reg_table->addRegSectionKey(section, key);
    } else {
        reg_table->addRegSectionKey(section, key);
        reg_table->initLineEdits();
    }
}

void MatrixItemWidget::setRegValueType(QString value_type)
{
    this->reg_value_type = value_type;
    if (chart != nullptr) {
        chart_reg_table->setRegValueType(value_type);
    } else {
        reg_table->setRegValueType(value_type);
    }
}

void MatrixItemWidget::setOpType(QString op_type)
{
    BaseItemWidget::setOpType(op_type);

    if (chart != nullptr) {
        chart_reg_table->setOpType(op_type);
    } else {
        reg_table->setOpType(op_type);
    }
}

void MatrixItemWidget::setParam(QVariant params)
{
    QVariantMap m = params.toMap();
    if (m.count() == 0) {
        return;
    }

    if (m.contains("col") || m.contains("row")) {
        horizontal_headers = m["col"].toStringList();
        vertical_headers = m["row"].toStringList();

        if (reg_table != nullptr) {
            QStringList v_labels = vertical_headers;
            QStringList h_labels = horizontal_headers;

            reg_table->setRowCount(v_labels.count());
            reg_table->setColumnCount(h_labels.count());
            reg_table->setVerticalHeaderLabels(v_labels);
            reg_table->setHorizontalHeaderLabels(h_labels);
        }
    }

    if (m.contains("min") || m.contains("max")) {
        if (reg_table != nullptr) {
            QVariantList min = m["min"].toList();
            QVariantList max = m["max"].toList();
            if (!min.isEmpty() && !max.isEmpty())
            {
                reg_table->setRangeList(min, max);
            }else
            {
                reg_table->setRange(m["min"].toLongLong(), m["max"].toLongLong());
            }
        }
    }
}

void MatrixItemWidget::handleShowWindow()
{
    if (register_window->isMinimized()) {
        register_window->showNormal();
    }
    register_window->show();
    register_window->activateWindow();
}

QStringList MatrixItemWidget::getMatrixHorizontalHeaders()
{
    return horizontal_headers;
}

QStringList MatrixItemWidget::getMatrixVerticalHeaders()
{
    return vertical_headers;
}

void MatrixItemWidget::refreshData(const SecKeyType&)
{
}

void MatrixItemWidget::setDoubleValidator(double min, double max)
{
    is_chart_value_double = true;
    validator_min = min;
    validator_max = max;
}

void MatrixItemWidget::handleWritePageData()
{
    emit sigWritePageData();
}

void MatrixItemWidget::retranslateUi()
{
    if (chmod == MOD_RO)
    {
        push_button->setText(tr("View this Matrix"));
    }
    else {
        push_button->setText(tr("Edit this Matrix"));
    }
}

void MatrixItemWidget::setChmod(int mod)
{
    BaseItemWidget::setChmod(mod);

    if (register_window)
    {
        register_window->setReadOnly(mod == MOD_RO);
    }

    if (reg_table)
    {
        reg_table->setChmod(mod);
    }

    if (chart_reg_table)
    {
        chart_reg_table->setChmod(mod);
    }

    retranslateUi();
}

MatrixItemWidget::~MatrixItemWidget()
{
    delete register_window;
}