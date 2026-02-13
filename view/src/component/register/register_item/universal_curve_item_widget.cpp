#include "universal_curve_item_widget.hpp"
#include "global_data.hpp"
#include "utils.hpp"

#include <QtCore>
#include <QMainWindow>
#include <QTableWidget>
#include <QLineEdit>

UniversalCurveItemWidget::UniversalCurveItemWidget(QString module, QString title, QWidget *parent) :
    BaseItemWidget(module, title, parent)
{
    this->module_name = module;
    initValueWidget();
    retranslateUi();
}

void UniversalCurveItemWidget::initValueWidget()
{
    register_window = new RegisterTableWindow(CONTENT_CURVE, module_name);
    univ_table = new UniversalTable();
    univ_curve = new UniversalCurve();
    univ_table->hide();
    register_window->setWidget(univ_table);
    register_window->addIsoWidget(univ_curve);
    register_window->setSwitchButton(univ_curve);
    register_window->hide();

    connect(univ_table, &UniversalTable::sigValueChanged,
            this, &UniversalCurveItemWidget::handleUiChanged);
    connect(univ_curve, &UniversalCurve::sigValueChanged,
            this, &UniversalCurveItemWidget::handleUiChanged);

    connect(univ_curve, &UniversalCurve::sigPageWriteData,
                this, &UniversalCurveItemWidget::handleWritePageData);

    push_button = new QPushButton;

    value_layout->addWidget(push_button);
    connect(push_button, &QPushButton::clicked,
            this, &UniversalCurveItemWidget::handleShowWindow);
}

void UniversalCurveItemWidget::handleShowWindow()
{
    if (register_window->isMinimized()) {
        register_window->showNormal();
    }
    register_window->show();
    register_window->activateWindow();
}

void UniversalCurveItemWidget::refreshData(const SecKeyType&)
{
}

void UniversalCurveItemWidget::setParam(QVariant param)
{
    QJsonObject param_obj = param.toJsonObject();

    if (param_obj.contains("min") && param_obj.contains("max")) {
        QJsonValue min_val = param_obj["min"];
        QJsonValue max_val = param_obj["max"];
        univ_table->setRange(min_val, max_val);
        univ_curve->setRange(min_val, max_val);
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

        univ_table->setHorizontalHeaderLabels(title_h_str_list);
        univ_curve->setComboBoxItems(title_h_str_list);
    }

    if (param_obj.contains("title_v")) {
        QStringList title_v_str_list;
        QVariantList title_v_var_list = param_obj["title_v"].toArray().toVariantList();
        for (int i = 0; i < title_v_var_list.count(); i++) {
            title_v_str_list.append(title_v_var_list[i].toString());
        }

        univ_table->setVerticalHeaderLabels(title_v_str_list);
        univ_curve->setLabelNames(title_v_str_list);
    }

    if (param_obj.contains("value_type")) {
        QString type = param_obj["value_type"].toString();
        univ_table->setValueType(type);
    }
}

void UniversalCurveItemWidget::handleWritePageData()
{
    emit sigWritePageData();
}

QVariant UniversalCurveItemWidget::getDataJsonrpc(QString path) const
{
    return univ_table->getData(path);
}

void UniversalCurveItemWidget::setRegSectionKeyList(QList<SecKeyType> &&reg_section_key_list)
{
    QList<SecKeyType> reg_sec_key_list_copy = reg_section_key_list;
    BaseItemWidget::setRegSectionKeyList(std::move(reg_section_key_list));

    if (!reg_section_key_list.isEmpty()) {
        register_window->setDesp(reg_section_key_list.first().first + "/" + title_label->text());
    }

    univ_table->setRegSectionKeyList(std::move(reg_section_key_list));
    univ_table->initLineEdits();
    univ_curve->setRegSectionKeyList(std::move(reg_sec_key_list_copy));
    univ_curve->init_chart();
    univ_curve->init_editPanel();

}

void UniversalCurveItemWidget::addRegSectionKey(QString section, QString key)
{
    BaseItemWidget::addRegSectionKey(section, key);

    if (!reg_section_key_list.isEmpty()) {
        register_window->setDesp(reg_section_key_list.first().first + "/" + title_label->text());
    }

    univ_table->addRegSectionKey(section, key);
    univ_table->initLineEdits();
}

void UniversalCurveItemWidget::setOpType(QString op_type)
{
    BaseItemWidget::setOpType(op_type);

    univ_table->setOpType(op_type);
    // univ_curve->setOpType(op_type);
}

void UniversalCurveItemWidget::setDataJsonrpc(QString path, QVariant data, bool sync_to_global)
{
    if (sync_to_global == false) {
        univ_table->blockSignals(true);
    }

    univ_table->setData(path, data);

    if (sync_to_global == false) {
        univ_table->blockSignals(false);
    }
}

void UniversalCurveItemWidget::retranslateUi()
{
    if (chmod == MOD_RO)
        push_button->setText(tr("View this Curve"));
    else
        push_button->setText(tr("Edit this Curve"));
}

void UniversalCurveItemWidget::setChmod(int mod)
{
    BaseItemWidget::setChmod(mod);
    univ_table->setChmod(mod);
    retranslateUi();
}

UniversalCurveItemWidget::~UniversalCurveItemWidget()
{
    delete register_window;
}
