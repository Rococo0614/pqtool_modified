#include "universal_matrix_item_widget.hpp"
#include "global_data.hpp"
#include "utils.hpp"

#include <QtCore>
#include <QMainWindow>
#include <QTableWidget>
#include <QLineEdit>

UniversalMatrixItemWidget::UniversalMatrixItemWidget(QString module, QString title, QWidget *parent) :
    BaseItemWidget(module, title, parent)
{
    this->module_name = module;
    this->title = title;
    initValueWidget();
    retranslateUi();
}

UniversalMatrixItemWidget::UniversalMatrixItemWidget(QString module, QString title, int, QWidget *parent) :
    BaseItemWidget(module, title, parent)
{}

void UniversalMatrixItemWidget::initValueWidget()
{
    register_window = new RegisterTableWindow(module_name);
    univ_table = new UniversalTable();
    register_window->setWidget(univ_table);
    register_window->hide();
    if ((this->module_name == "CCM") && (utils::isMatchCCMSumTitle(this->title))) {
        register_window->showCCMSumButton(true);
    }

    connect(univ_table, &UniversalTable::sigValueChanged,
            this, &UniversalMatrixItemWidget::handleUiChanged);
    connect(univ_table, &UniversalTable::sigValueChanged,
            this, &UniversalMatrixItemWidget::handleTableValueChanged);
    connect(univ_table, &UniversalTable::sigPageWriteData,
                this, &UniversalMatrixItemWidget::handleWritePageData);
    connect(register_window, &RegisterTableWindow::sigHideSumColumn, univ_table, &UniversalTable::hideSumColumn);

    push_button = new QPushButton;

    value_layout->addWidget(push_button);
    connect(push_button, &QPushButton::clicked,
            this, &UniversalMatrixItemWidget::handleShowWindow);
}

void UniversalMatrixItemWidget::handleShowWindow()
{
    if (register_window->isMinimized()) {
        register_window->showNormal();
    }
    register_window->show();
    register_window->activateWindow();
}

void UniversalMatrixItemWidget::handleTableValueChanged()
{
    QVariant data = getData();
    emit sigTableValueChanged(data);
    emit sigValueChanged(data);
}

void UniversalMatrixItemWidget::refreshData(const SecKeyType&)
{
}

void UniversalMatrixItemWidget::setParam(QVariant param)
{
    QJsonObject param_obj = param.toJsonObject();

    if (param_obj.contains("min") && param_obj.contains("max")) {
        QJsonValue min_val = param_obj["min"];
        QJsonValue max_val = param_obj["max"];
        univ_table->setRange(min_val, max_val);
    }

    if (param_obj.contains("count")) {
        int row = 1, column = 1;
        QJsonValue count_val = param_obj["count"];
        if (count_val.isDouble()) {
            column = count_val.toInt();
        } else if (count_val.isArray()){
            row = count_val.toArray()[0].toInt();
            column = count_val.toArray()[1].toInt();
        }

        univ_table->setRowCount(row);
        univ_table->setColumnCount(column);
    }

    if (param_obj.contains("title_h")) {
        QStringList title_h_str_list;
        QVariantList title_h_var_list = param_obj["title_h"].toArray().toVariantList();
        for (int i = 0; i < title_h_var_list.count(); i++) {
            title_h_str_list.append(title_h_var_list[i].toString());
        }

        if (title_h_str_list[0].contains(NAME_ISO)) {
            auto_type = NAME_ISO;
        } else if (title_h_str_list[0].contains(NAME_LV)) {
            auto_type = NAME_LV;
        }

        univ_table->setHorizontalHeaderLabels(title_h_str_list);
    }

    if (param_obj.contains("title_v")) {
        QStringList title_v_str_list;
        QVariantList title_v_var_list = param_obj["title_v"].toArray().toVariantList();
        for (int i = 0; i < title_v_var_list.count(); i++) {
            title_v_str_list.append(title_v_var_list[i].toString());
        }

        univ_table->setVerticalHeaderLabels(title_v_str_list);
    }

    if (param_obj.contains("value_type")) {
        QString type = param_obj["value_type"].toString();
        univ_table->setValueType(type);
    }
}

void UniversalMatrixItemWidget::handleWritePageData()
{
    emit sigWritePageData();
}

QVariant UniversalMatrixItemWidget::getData() const
{
    return univ_table->getData("");
}

void UniversalMatrixItemWidget::setRegSectionKeyList(QList<SecKeyType> &&reg_section_key_list)
{
    qDebug("UniversalMatrixItemWidget::setRegSectionKeyList");
    BaseItemWidget::setRegSectionKeyList(std::move(reg_section_key_list));

    if (!reg_section_key_list.isEmpty()) {
        register_window->setDesp(reg_section_key_list.first().first + "/" + title_label->text());
    }

    univ_table->setRegSectionKeyList(std::move(reg_section_key_list));
    univ_table->initLineEdits();
}

void UniversalMatrixItemWidget::addRegSectionKey(QString section, QString key)
{
    qDebug("UniversalMatrixItemWidget::addRegSectionKey");
    BaseItemWidget::addRegSectionKey(section, key);

    if (!reg_section_key_list.isEmpty()) {
        register_window->setDesp(reg_section_key_list.first().first + "/" + title_label->text());
    }

    univ_table->addRegSectionKey(section, key);
    univ_table->initLineEdits();
}

void UniversalMatrixItemWidget::setOpType(QString op_type)
{
    BaseItemWidget::setOpType(op_type);

    univ_table->setOpType(op_type);

    if (!reg_section_key_list.isEmpty()) {
        register_window->setDesp(reg_section_key_list.first().first + "/" + title_label->text());
    }
}

void UniversalMatrixItemWidget::setData(QVariant data, bool sync_to_global)
{
    if (sync_to_global == false) {
        univ_table->blockSignals(true);
    }

    univ_table->setData("", data);

    if (sync_to_global == false) {
        univ_table->blockSignals(false);
    }
}

void UniversalMatrixItemWidget::retranslateUi()
{
    if (chmod == MOD_RO)
        push_button->setText(tr("View this Matrix"));
    else
        push_button->setText(tr("Edit this Matrix"));
}

void UniversalMatrixItemWidget::setChmod(int mod)
{
    BaseItemWidget::setChmod(mod);

    if (register_window)
    {
        register_window->setReadOnly(mod == MOD_RO);
    }
    univ_table->setChmod(mod);
    retranslateUi();
}

void UniversalMatrixItemWidget::handleLoadManuData(QVariant data)
{
    int col = univ_table->columnCount();
    int row = univ_table->rowCount();

    if (table_index >= col) {
        return;
    }
    if (row > 1 && data.toList().count() != row) {
        return;
    }

    QVariantList data_list;
    QVariantList data_table_list = getData().toList();

    if (row == 1) {
        data_list.append(data);
    } else {
        data_list += data.toList();
    }

    for (int i = 0; i < row; i++) {
        data_table_list[i * col + table_index] = data_list[i];
    }
    setData(data_table_list);
    emit sigValueChanged(data);
}

QVariant UniversalMatrixItemWidget::getDataByIndex(int index)
{
    int col = univ_table->columnCount();
    int row = univ_table->rowCount();

    if (index >= col) {
        return QVariant();
    }

    QVariantList data_list;
    QVariantList data_table_list = getData().toList();

    for (int i = 0; i < row; i++) {
        data_list.append(data_table_list[i * col + index]);
    }

    if (row == 1) {
        return data_list[0];
    }

    return data_list;
}

UniversalMatrixItemWidget::~UniversalMatrixItemWidget()
{
    if (register_window) {
        delete register_window;
        register_window = nullptr;
    }
}
