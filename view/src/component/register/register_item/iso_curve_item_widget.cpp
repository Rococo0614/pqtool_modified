#include "iso_curve_item_widget.hpp"
#include "global_data.hpp"
#include "utils.hpp"

#include <QtCore>
#include <QMainWindow>
#include <QTableWidget>
#include <QLineEdit>

IsoCurveItemWidget::IsoCurveItemWidget(QString module, QString title, QWidget *parent) :
    BaseItemWidget(module, title, parent)
{
    updateValueType();
    initValueWidget();
    retranslateUi();
}

void IsoCurveItemWidget::initValueWidget()
{
    register_window = new RegisterTableWindow(CONTENT_CURVE);

    curve = new IsoCurve;
    curve->setRegValueType(this->reg_value_type);
    register_window->addIsoWidget(curve);
    register_window->hide();

    iso_table = new IsoTable();
    iso_table->setRegValueType(this->reg_value_type);
    register_window->setSwitchButton(true);
    iso_table->hide();
    register_window->setWidget(iso_table);

    connect(iso_table, &IsoTable::sigValueChanged,
            this, &IsoCurveItemWidget::handleUiChanged);
    connect(iso_table, &IsoTable::sigPageWriteData,
            this, &IsoCurveItemWidget::handleWritePageData);

    connect(curve, &IsoCurve::sigValueChanged,
            this, &IsoCurveItemWidget::handleUiChanged);
    connect(curve, &IsoCurve::sigPageWriteData, this, &IsoCurveItemWidget::handleWritePageData);

    push_button = new QPushButton;

    value_layout->addWidget(push_button);
    connect(push_button, &QPushButton::clicked,
            this, &IsoCurveItemWidget::handleShowWindow);
}

void IsoCurveItemWidget::handleShowWindow()
{
    if (register_window->isMinimized()) {
        register_window->showNormal();
    }
    register_window->show();
    register_window->activateWindow();
}

void IsoCurveItemWidget::handleWritePageData()
{
    emit sigWritePageData();
}

void IsoCurveItemWidget::refreshData(const SecKeyType&)
{
}

void IsoCurveItemWidget::setParam(QVariant params)
{
    QMap<QString, QVariant> m = params.toMap();
    if (m.count() == 0) {
        return;
    }

    if (m.contains("min") && m.contains("max")) {
        QVariantList min = m["min"].toList();
        QVariantList max = m["max"].toList();
        if (!(min.isEmpty() || max.isEmpty())) {
            curve->setRangeList(min, max);
            iso_table->setRangeList(min, max);
        } else {
            curve->setRange(m["min"].toInt(), m["max"].toInt());
            iso_table->setRange(m["min"].toInt(), m["max"].toInt());
        }
    }

    if (m.contains("interop_num")) {
        int num = m["interop_num"].toInt();
        curve->setInterpNum(num);
    }

    if (m.contains("label")) {
        edit_labels = m["label"].toList();
        if (curve != nullptr) {
            curve->setEditLabels(edit_labels);
        }
    }

    if (m.contains("header_type")) {
        QString type = m["header_type"].toString();
        if (curve != nullptr) {
            curve->setHeaderType(type);
        }
        if (iso_table != nullptr) {
            iso_table->setHeaderType(type);
        }
    }

    if (m.contains("curve_type")) {
        CURVE_TYPE type = (CURVE_TYPE)m["curve_type"].toInt();
        curve->setCurveType(type);
    }
}

QVariant IsoCurveItemWidget::getData() const
{
    return iso_table->getData();
}

void IsoCurveItemWidget::setRegSectionKeyList(QList<SecKeyType> &&reg_section_key_list)
{
    QList<SecKeyType> reg_sec_key_list_copy = reg_section_key_list;
    BaseItemWidget::setRegSectionKeyList(std::move(reg_section_key_list));

    if (!reg_section_key_list.isEmpty()) {
        register_window->setDesp(reg_section_key_list.first().first + "/" + title_label->text());
    }

    curve->setRegSectionKeyList(std::move(reg_section_key_list));
    updateValueType();
    curve->init_chart();
    curve->init_editPanel();

    iso_table->setRegSectionKeyList(std::move(reg_sec_key_list_copy));
    iso_table->initLineEdits();

}

void IsoCurveItemWidget::addRegSectionKey(QString section, QString key)
{
    BaseItemWidget::addRegSectionKey(section, key);

    if (!reg_section_key_list.isEmpty()) {
        register_window->setDesp(reg_section_key_list.first().first + "/" + title_label->text());
    }

    curve->addRegSectionKey(section, key);
    iso_table->addRegSectionKey(section, key);


    updateValueType();
}

void IsoCurveItemWidget::setOpType(QString op_type)
{
    BaseItemWidget::setOpType(op_type);
    curve->setOpType(op_type);
    iso_table->setOpType(op_type);

}

void IsoCurveItemWidget::setRegValueType(QString value_type)
{
    IObserver::setRegValueType(value_type);
    curve->setRegValueType(value_type);
    iso_table->setRegValueType(value_type);
}

void IsoCurveItemWidget::setValueMappingVec(QVector<QVariantList> &&value_mapping_vec)
{

}

void IsoCurveItemWidget::setData(QVariant data, bool sync_to_global)
{
    if (sync_to_global == false) {
        curve->blockSignals(true);
    }

    //curve->setData(data);
    iso_table->setData(data);

    if (sync_to_global == false) {
        curve->blockSignals(false);
    }
}

void IsoCurveItemWidget::retranslateUi()
{
    push_button->setText(tr("Edit this Curve"));
}

void IsoCurveItemWidget::updateValueType()
{
    this->value_type = TYPE_ARRAY_2D;
}

void IsoCurveItemWidget::setChmod(int mod)
{
    BaseItemWidget::setChmod(mod);
}

void IsoCurveItemWidget::handleUpdateTheme(bool dark_theme_enabled)
{
    BaseItemWidget::handleUpdateTheme(dark_theme_enabled);
    curve->handleUpdateTheme(dark_theme_enabled);
}

IsoCurveItemWidget::~IsoCurveItemWidget()
{
    delete register_window;
}
