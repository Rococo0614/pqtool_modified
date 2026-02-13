#ifndef LINE_CHART_ITEM_WIDGET_HPP
#define LINE_CHART_ITEM_WIDGET_HPP

#include "base_item_widget.hpp"
#include "cubic_curve_chart_view.hpp"

class GammaChartItemWidget : public BaseItemWidget
{
    Q_OBJECT

public:
    explicit GammaChartItemWidget(QString module, QString title, QWidget *parent = nullptr);

    virtual void setData(QVariant data, bool sync_to_global = false) override;
    virtual QVariant getData() const override;
    virtual void setRegSectionKeyList(QList<SecKeyType> &&reg_section_key_list) override;
    virtual void addRegSectionKey(QString section, QString key) override;
    void setPoints(int series_idx, QList<QPointF> points, bool update_global_data = true);
    CubicCurveChartView *getLineChart();
    void saveJsonFile(QString filePath);
    void saveTxtFile(QString filePath);
    void loadJsonFile(QString filePath);
    void loadTxtFile(QString file_path);
    void setSampleInterval(int num);
    void setDisplayInterval(int num);
    virtual void setChmod(int mod) override;
    void setControlPointsNum(int num);
    int getControlPointsNum();
    virtual void refreshData(const SecKeyType &reg_section_key) override;
    void setUserDefineMode(bool mode);
    void addControlPt(double x, double y);
    double getY(double x);
    void setUserDefineCtrlPt(bool en);
    virtual void handleUpdateTheme(bool dark_theme_enabled) override;

private slots:
    void updateMousePos(double x, double y);
    void slotPressedScatterPt(QPointF p);
    void handlePageWriteData();
    void handleValueChanged();

signals:
    void sigUpdateMousePos(double x, double y);
    void sigPressedScatterPt(QPointF p);
    void sigValueChanged();

protected:
    virtual void retranslateUi() override;

private:
    void initValueWidget();

private:
    CubicCurveChartView *line_chart_view{nullptr};
    int sample_interval;
    int display_interval;
};

#endif // LINE_CHART_ITEM_WIDGET_HPP
