#include "curve_item_widget.hpp"
#include "global_data.hpp"
#include "utils.hpp"

#include <QtCore>
#include <QMainWindow>
#include <QTableWidget>
#include <QLineEdit>

CurveItemWidget::CurveItemWidget(QString module, QString title, QWidget *parent) :
    BaseItemWidget(module, title, parent)
{
    updateValueType();
    initValueWidget();
    retranslateUi();
}

void CurveItemWidget::initValueWidget()
{
    register_window = new RegisterTableWindow(CONTENT_CURVE);

    curve = new ManualCurve;
    curve->setRegValueType(this->reg_value_type);
    register_window->setWidget(curve);
    register_window->hide();

    push_button = new QPushButton;
    value_layout->addWidget(push_button);
    connect(push_button, &QPushButton::clicked,
            this, &CurveItemWidget::handleShowWindow);

    connect(curve, &ManualCurve::sigValueChanged,
            this, &CurveItemWidget::handleUiChanged);
    connect(curve, &ManualCurve::sigPageWriteData,
            this, &CurveItemWidget::handleWritePageData);
}

void CurveItemWidget::handleShowWindow()
{
    if (register_window->isMinimized()) {
        register_window->showNormal();
    }
    register_window->show();
    register_window->activateWindow();
}

void CurveItemWidget::handleWritePageData()
{
    emit sigWritePageData();
}

void CurveItemWidget::refreshData(const SecKeyType&)
{
}

void CurveItemWidget::setParam(QVariant params)
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
        } else {
            curve->setRange(m["min"].toInt(), m["max"].toInt());
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
}

QVariant CurveItemWidget::getData() const
{
    return curve->getData();
}

void CurveItemWidget::setRegSectionKeyList(QList<SecKeyType> &&reg_section_key_list)
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
}

void CurveItemWidget::addRegSectionKey(QString section, QString key)
{
    BaseItemWidget::addRegSectionKey(section, key);

    if (!reg_section_key_list.isEmpty()) {
        register_window->setDesp(reg_section_key_list.first().first + "/" + title_label->text());
    }

    curve->addRegSectionKey(section, key);
    updateValueType();
}

void CurveItemWidget::setOpType(QString op_type)
{
    BaseItemWidget::setOpType(op_type);
    curve->setOpType(op_type);
}

void CurveItemWidget::setRegValueType(QString value_type)
{
    IObserver::setRegValueType(value_type);
    curve->setRegValueType(value_type);
}

void CurveItemWidget::setValueMappingVec(QVector<QVariantList> &&value_mapping_vec)
{

}

void CurveItemWidget::setData(QVariant data, bool sync_to_global)
{
    if (sync_to_global == false) {
        curve->blockSignals(true);
    }

    curve->setData(data);

    if (sync_to_global == false) {
        curve->blockSignals(false);
    }
}

void CurveItemWidget::retranslateUi()
{
    push_button->setText(tr("Edit this Curve"));
}

void CurveItemWidget::updateValueType()
{
    this->value_type = TYPE_ARRAY;
}

void CurveItemWidget::setChmod(int mod)
{
    BaseItemWidget::setChmod(mod);
}

void CurveItemWidget::handleUpdateTheme(bool dark_theme_enabled)
{
    BaseItemWidget::handleUpdateTheme(dark_theme_enabled);
    curve->handleUpdateTheme(dark_theme_enabled);
}

CurveItemWidget::~CurveItemWidget()
{
    delete register_window;
}
