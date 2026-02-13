#include "gamma_chart_item_widget.hpp"
#include "global_data.hpp"
#include "utils.hpp"
#include <QMessageBox>

GammaChartItemWidget::GammaChartItemWidget(QString module, QString title, QWidget *parent) :
    BaseItemWidget(module, title, parent),
    sample_interval(1),
    display_interval(1)
{
    this->value_type = TYPE_ARRAY;
    layout->removeWidget(icon_widget);
    layout->removeWidget(title_label);
    layout->removeWidget(value_widget);

    initValueWidget();
    retranslateUi();
}

CubicCurveChartView *GammaChartItemWidget::getLineChart()
{
    return this->line_chart_view;
}

void GammaChartItemWidget::initValueWidget()
{
    line_chart_view = new CubicCurveChartView(this);
    layout->addWidget(line_chart_view);
    connect(line_chart_view, &CubicCurveChartView::sigMouseMove, this, &GammaChartItemWidget::updateMousePos);
    connect(line_chart_view, &CubicCurveChartView::sigPressedScatterPt, this, &GammaChartItemWidget::slotPressedScatterPt);
    connect(line_chart_view, &CubicCurveChartView::sigPageWriteData, this, &GammaChartItemWidget::handlePageWriteData);
    connect(line_chart_view, &CubicCurveChartView::sigValueChanged, this, &GammaChartItemWidget::handleValueChanged);
}

QVariant GammaChartItemWidget::getData() const
{
    QVariantList data;
    if (line_chart_view == nullptr) return data;

    QList<qreal> series_data = line_chart_view->getInterpolationDataList(0);
    for (auto point_data : series_data) {
        data.append(point_data);
    }

    return data;
}

void GammaChartItemWidget::refreshData(const SecKeyType &reg_section_key)
{

}

void GammaChartItemWidget::setData(QVariant data, bool sync_to_global)
{
    if (sync_to_global == false) {
        line_chart_view->blockSignals(true);
    }

    line_chart_view->setData(data);

    if (sync_to_global == false) {
        line_chart_view->blockSignals(false);
    }
}

void GammaChartItemWidget::setRegSectionKeyList(QList<SecKeyType> &&reg_section_key_list)
{
    QList<SecKeyType> reg_section_key_list_copy = reg_section_key_list;
    BaseItemWidget::setRegSectionKeyList(std::move(reg_section_key_list));
    line_chart_view->setRegSectionKeyList(std::move(reg_section_key_list_copy));
}

void GammaChartItemWidget::addRegSectionKey(QString section, QString key)
{
    BaseItemWidget::addRegSectionKey(section, key);
    line_chart_view->addRegSectionKey(section, key);
}

void GammaChartItemWidget::setPoints(int series_idx, QList<QPointF> points, bool update_global_data)
{
    line_chart_view->setPoints(points, update_global_data);
}

void GammaChartItemWidget::retranslateUi()
{
}

void GammaChartItemWidget::saveJsonFile(QString filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly))
    {
        return;
    }

    QJsonObject data;
    for (int i = 0; i < reg_section_key_list.size(); i++) {
        SecKeyType reg_section_key = reg_section_key_list[i];
        QString value_array = line_chart_view->getInterpolationDataString(i);
        data.insert(reg_section_key.second, value_array);
    }

    utils::saveJson(filePath, data);
}

void GammaChartItemWidget::saveTxtFile(QString filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly))
    {
        return;
    }

    QString data = "";
    for (int i = 0; i < reg_section_key_list.size(); i++) {
        SecKeyType reg_section_key = reg_section_key_list[i];
        QString value_array = line_chart_view->getInterpolationDataString(i, "\n");
        data += value_array;
    }

    QTextStream stream(&file);
    stream << data;
}

void GammaChartItemWidget::loadTxtFile(QString file_path)
{
    QFile inputFile(file_path);
    QString data = "";

    if (inputFile.open(QIODevice::ReadOnly))
    {
       QTextStream in(&inputFile);
       while (!in.atEnd())
       {
          QString line = in.readLine();
          data += line + ",";
       }
       inputFile.close();
    }

    if (data.isEmpty()) {
        return;
    }

    for (int i = 0; i < reg_section_key_list.size(); i++) {
        QList<QVariant> all_points = utils::stringToList<QVariant>(data);
        QList<QPointF> points = line_chart_view->samplePointFromList(all_points);
        if (points.size() > 256) points.removeLast();
        setPoints(i, points);
        setControlPointsNum(getControlPointsNum());
    }
}

void GammaChartItemWidget::setSampleInterval(int num)
{
    sample_interval = num;
}

void GammaChartItemWidget::setDisplayInterval(int num)
{
    display_interval = num;
}

void GammaChartItemWidget::loadJsonFile(QString filePath)
{
    QJsonObject data;
    QString error_msg;
    if (utils::loadJson(filePath, data, error_msg) != 0) {
        QMessageBox::critical(this, "Error", error_msg, QMessageBox::Ok);
        return;
    }

    for (int i = 0; i < reg_section_key_list.size(); i++) {
        SecKeyType reg_section_key = reg_section_key_list[i];
        if (!data.contains(reg_section_key.second)) {
            return;
        }
    }

    for (int i = 0; i < reg_section_key_list.size(); i++) {
        SecKeyType reg_section_key = reg_section_key_list[i];
        QList<QVariant> all_points = utils::stringToList<QVariant>(data[reg_section_key.second].toString());
        QList<QPointF> points = line_chart_view->samplePointFromList(all_points);
        setPoints(i, points);
        setControlPointsNum(getControlPointsNum());
    }
}

void GammaChartItemWidget::setChmod(int mod)
{
    BaseItemWidget::setChmod(mod);
    //TODO set chart readonly
}

void GammaChartItemWidget::updateMousePos(double x, double y)
{
    emit sigUpdateMousePos(x, y);
}

void GammaChartItemWidget::slotPressedScatterPt(QPointF p)
{
    emit sigPressedScatterPt(p);
}

void GammaChartItemWidget::setControlPointsNum(int num)
{
    line_chart_view->setControlPointsNum(num);
}

int GammaChartItemWidget::getControlPointsNum()
{
    return line_chart_view->getControlPointsNum();
}

void GammaChartItemWidget::setUserDefineMode(bool mode)
{
    line_chart_view->setUserDefineMode(mode);
}

void GammaChartItemWidget::addControlPt(double x, double y)
{
    line_chart_view->addControlPt(x, y);
}

double GammaChartItemWidget::getY(double x)
{
    return line_chart_view->getY(x);
}

void GammaChartItemWidget::setUserDefineCtrlPt(bool en)
{
    line_chart_view->setUserDefineCtrlPt(en);
}

void GammaChartItemWidget::handlePageWriteData()
{
    emit sigWritePageData();
}

void GammaChartItemWidget::handleValueChanged()
{
    handleUiChanged();
    emit sigValueChanged();
}

void GammaChartItemWidget::handleUpdateTheme(bool dark_theme_enabled)
{
    line_chart_view->handleUpdateTheme(dark_theme_enabled);
}
