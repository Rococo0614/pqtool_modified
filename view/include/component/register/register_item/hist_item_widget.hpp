#ifndef HIST_ITEM_WIDGET_HPP
#define HIST_ITEM_WIDGET_HPP

#include "base_item_widget.hpp"
#include "hist_chart_view.hpp"

#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QJsonObject>

typedef enum {
    LE_HIST,
    SE_HIST,
    GLOBAL_TONE_CURVE,
    DARK_TONE_CURVE,
    BRIGHT_TONE_CURVE
} HIST_CHART_TYPE;

class HistItemWidget : public BaseItemWidget
{
    Q_OBJECT

public:
    explicit HistItemWidget(QString module, QString title, HIST_CHART_TYPE type = LE_HIST, QWidget *parent = nullptr);

    virtual void refreshData(const SecKeyType &reg_section_key) override;
    virtual void setParam(QVariant params) override;
    virtual void setData(QVariant data, bool sync_to_global = false) override;
    virtual QVariant getData() const override;
    virtual void setRegSectionKeyList(QList<SecKeyType> &&reg_section_key_list) override;
    virtual void addRegSectionKey(QString section, QString key) override;
    virtual void setOpType(QString op_type) override;
    virtual void setRegValueType(QString reg_value_type) override;
    virtual void setValueMappingVec(QVector<QVariantList> &&value_mapping_vec) override;
    virtual void setChmod(int mod) override;
    void setTitle(QString title);
    void hideSeries();
    void showSeries();
    virtual void handleUpdateTheme(bool dark_theme_enabled) override;

    HistChartView *getHisChartView();

protected:
    virtual void initValueWidget();
    virtual void retranslateUi() override;
    void updateValueType();

private slots:
    void handleWritePageData();

private:
    HistChartView *curve{nullptr};
    HIST_CHART_TYPE type;
};
#endif // HIST_ITEM_WIDGET_HPP
