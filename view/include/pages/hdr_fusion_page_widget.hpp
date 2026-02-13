#ifndef HDR_FUSION_PAGE_WIDGET_H
#define HDR_FUSION_PAGE_WIDGET_H

#include "base_page_widget.hpp"

class HDRFusionPageWidget : public BasePageWidget
{
    Q_OBJECT

public:
    explicit HDRFusionPageWidget(QWidget *parent = nullptr);

protected:
    void initLeftArea() override;
    void initCenterArea() override;

    virtual void retranslateUi() override;

private:
    QLayout *initButtons();
    void saveFile();
    void openFile();

private slots:
    void handleSelectCurve(const QString &text);

private:
    QList<QPointF> initPoints(double coefficient);

    LineChartView *cv;
    QPushButton *load_button;
    QPushButton *save_button;
    QHBoxLayout *tm_curve_layout;
    QComboBox *tm_curve_combobox;
    QList<QString> tm_curve_name_list;
};

#endif // HDR_FUSION_PAGE_WIDGET_H
