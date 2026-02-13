#include "hist_item_widget.hpp"
#include "global_data.hpp"
#include "utils.hpp"

#include <QtCore>
#include <QMainWindow>
#include <QTableWidget>
#include <QLineEdit>

HistItemWidget::HistItemWidget(QString module, QString title, HIST_CHART_TYPE type, QWidget *parent) :
    BaseItemWidget(module, title, parent),
    type(type)
{
    layout->removeWidget(icon_widget);
    layout->removeWidget(title_label);
    layout->removeWidget(value_widget);
    updateValueType();
    initValueWidget();
    retranslateUi();
}

void HistItemWidget::initValueWidget()
{
    curve = new HistChartView;
    layout->addWidget(curve);
    curve->setRegValueType(this->reg_value_type);
    // Todo: set range dynamically. not on init.
    QString soc = GlobalData::getInstance()->getCvipqToolSoc();
    switch(type) {
    case LE_HIST:
        curve->initHist();
        curve->setTitle("LE Histogram");
        break;
    case SE_HIST:
        curve->initHist();
        curve->setTitle("SE Histogram");
        break;
    case GLOBAL_TONE_CURVE:
        if ((soc == CV181X_SOC) || (soc == CV180X_SOC))
            curve->initGlobalToneCurve(768, 65535);
        else
            curve->initGlobalToneCurve(768, 4095);

        curve->setTitle("Global Tone Curve");
        curve->setFixedAxisY(true);
        break;
    case DARK_TONE_CURVE:
        if ((soc == CV181X_SOC) || (soc == CV180X_SOC))
            curve->initDarkToneCurve(256, 65535);
        else
            curve->initDarkToneCurve(256, 4095);

        curve->setTitle("Dark Tone Curve");
        curve->setFixedAxisY(true);
        break;
    case BRIGHT_TONE_CURVE:
        if ((soc == CV181X_SOC) || (soc == CV180X_SOC))
            curve->initBrightToneCurve(512, 65535);
        else
            curve->initBrightToneCurve(512, 4095);

        curve->setTitle("Bright Tone Curve");
        curve->setFixedAxisY(true);
        break;
    }

    /*connect(curve, &HistChartView::sigValueChanged,
            this, &HistItemWidget::handleUiChanged);*/
}

void HistItemWidget::handleWritePageData()
{
    //emit sigWritePageData();
}

void HistItemWidget::refreshData(const SecKeyType&)
{
}

void HistItemWidget::setParam(QVariant params)
{
    QMap<QString, QVariant> m = params.toMap();
    if (m.count() == 0) {
        return;
    }
}

QVariant HistItemWidget::getData() const
{
    QVariantList list;
    QList<qreal> data = curve->getData(0);
    for(int i=0; i < data.size(); i++) {
        list.append(QVariant(data[i]));
    }
    return list;
}

void HistItemWidget::setRegSectionKeyList(QList<SecKeyType> &&reg_section_key_list)
{
    QList<SecKeyType> reg_sec_key_list_copy = reg_section_key_list;
    BaseItemWidget::setRegSectionKeyList(std::move(reg_section_key_list));

    curve->setRegSectionKeyList(std::move(reg_section_key_list));
    updateValueType();
}

void HistItemWidget::addRegSectionKey(QString section, QString key)
{
    BaseItemWidget::addRegSectionKey(section, key);
    curve->addRegSectionKey(section, key);
    updateValueType();
}

void HistItemWidget::setOpType(QString op_type)
{
    BaseItemWidget::setOpType(op_type);
    curve->setOpType(op_type);
}

void HistItemWidget::setRegValueType(QString value_type)
{
    IObserver::setRegValueType(value_type);
    curve->setRegValueType(value_type);
}

void HistItemWidget::setValueMappingVec(QVector<QVariantList> &&value_mapping_vec)
{

}

void HistItemWidget::setData(QVariant data, bool sync_to_global)
{
    if (sync_to_global == false) {
        curve->blockSignals(true);
    }

    curve->setData(data);

    if (sync_to_global == false) {
        curve->blockSignals(false);
    }
}

void HistItemWidget::retranslateUi()
{
}

void HistItemWidget::updateValueType()
{
    this->value_type = TYPE_ARRAY;
}

void HistItemWidget::setChmod(int mod)
{
    BaseItemWidget::setChmod(mod);
}

void HistItemWidget::setTitle(QString title)
{
    curve->setTitle(title);
}

void HistItemWidget::hideSeries()
{
    curve->hideSeries();
}

void HistItemWidget::showSeries()
{
    curve->showSeries();
}

HistChartView * HistItemWidget::getHisChartView()
{
    return curve;
}

void HistItemWidget::handleUpdateTheme(bool dark_theme_enabled)
{
    curve->handleUpdateTheme(dark_theme_enabled);
}
