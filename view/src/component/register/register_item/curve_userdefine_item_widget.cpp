#include "curve_userdefine_item_widget.hpp"
#include "global_data.hpp"
#include "utils.hpp"

#include <QtCore>
#include <QMainWindow>
#include <QTableWidget>
#include <QLineEdit>

CurveUserDefineItemWidget::CurveUserDefineItemWidget(QString module, QString title, QWidget *parent) :
    BaseItemWidget(module, title, parent)
{
    updateValueType();
    initValueWidget();
    retranslateUi();
}

void CurveUserDefineItemWidget::initValueWidget()
{
    register_window = new RegisterTableWindow(CONTENT_CURVE);
    register_window->setMinimumSize(1280, 720);

    curve = new CurveUserDefine;
    curve->setRegValueType(this->reg_value_type);
    register_window->setWidget(curve);
    register_window->hide();

    connect(curve, &CurveUserDefine::sigValueChanged,
            this, &CurveUserDefineItemWidget::handleUiChanged);
    connect(curve, &CurveUserDefine::sigPageWriteData,
            this, &CurveUserDefineItemWidget::handleWritePageData);

    push_button = new QPushButton;

    value_layout->addWidget(push_button);
    connect(push_button, &QPushButton::clicked,
            this, &CurveUserDefineItemWidget::handleShowWindow);
}

void CurveUserDefineItemWidget::handleShowWindow()
{
    if (register_window->isMinimized()) {
        register_window->showNormal();
    }
    register_window->show();
    register_window->activateWindow();
}

void CurveUserDefineItemWidget::handleWritePageData()
{
    emit sigWritePageData();
}

void CurveUserDefineItemWidget::refreshData(const SecKeyType&)
{
}

void CurveUserDefineItemWidget::setParam(QVariant params)
{
    QMap<QString, QVariant> m = params.toMap();
    if (m.count() == 0) {
        return;
    }

    if (m.contains("min") && m.contains("max")) {
        curve->setRange(m["min"].toInt(), m["max"].toInt());
    }
}

QVariant CurveUserDefineItemWidget::getData() const
{
    return curve->getData();
}

void CurveUserDefineItemWidget::setRegSectionKeyList(QList<SecKeyType> &&reg_section_key_list)
{
    QList<SecKeyType> reg_sec_key_list_copy = reg_section_key_list;
    BaseItemWidget::setRegSectionKeyList(std::move(reg_section_key_list));

    if (!reg_section_key_list.isEmpty()) {
        register_window->setDesp(reg_section_key_list.first().first + "/" + title_label->text());
    }

    curve->setRegSectionKeyList(std::move(reg_section_key_list));
    updateValueType();
    curve->initUI();
}

void CurveUserDefineItemWidget::addRegSectionKey(QString section, QString key)
{
    BaseItemWidget::addRegSectionKey(section, key);

    if (!reg_section_key_list.isEmpty()) {
        register_window->setDesp(reg_section_key_list.first().first + "/" + title_label->text());
    }

    curve->addRegSectionKey(section, key);
    updateValueType();
}

void CurveUserDefineItemWidget::setOpType(QString op_type)
{
    BaseItemWidget::setOpType(op_type);
    curve->setOpType(op_type);
}

void CurveUserDefineItemWidget::setRegValueType(QString value_type)
{
    IObserver::setRegValueType(value_type);
    curve->setRegValueType(value_type);
}

void CurveUserDefineItemWidget::setValueMappingVec(QVector<QVariantList> &&value_mapping_vec)
{

}

void CurveUserDefineItemWidget::setData(QVariant data, bool sync_to_global)
{
    if (sync_to_global == false) {
        curve->blockSignals(true);
    }

    curve->setData(data);

    if (sync_to_global == false) {
        curve->blockSignals(false);
    }
}

void CurveUserDefineItemWidget::retranslateUi()
{
    push_button->setText(tr("Edit this Curve"));
}

void CurveUserDefineItemWidget::updateValueType()
{
    this->value_type = TYPE_ARRAY;
}

void CurveUserDefineItemWidget::setChmod(int mod)
{
    BaseItemWidget::setChmod(mod);
}

void CurveUserDefineItemWidget::handleUpdateTheme(bool dark_theme_enabled)
{
    BaseItemWidget::handleUpdateTheme(dark_theme_enabled);
    curve->handleUpdateTheme(dark_theme_enabled);
}

CurveUserDefineItemWidget::~CurveUserDefineItemWidget()
{
    delete register_window;
}
