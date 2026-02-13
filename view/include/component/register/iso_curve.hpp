#ifndef ISO_CURVE_HPP
#define ISO_CURVE_HPP

#include <memory>
#include <QWidget>
#include "i_observer.hpp"
#include "iso_chart_view.hpp"
#include <QVBoxLayout>
#include <QLineEdit>
#include <QComboBox>

class IsoCurve : public QWidget, public IObserver
{
    Q_OBJECT
public:
    IsoCurve();
    virtual void refreshData(const SecKeyType &reg_section_key) override;
    QVariant getData();
    void setData(QVariant data);
    void setRange(int min, int max);
    void setRangeList(const QVariantList &min, const QVariantList &max);
    void setEditLabels(QVariantList list);
    void setInterpNum(int interp_num);
    void setHeaderType(QString type);
    void setCurveType(CURVE_TYPE curve_type);
    void init_chart();
    void init_editPanel();
    void handleUpdateTheme(bool dark_theme_enabled);

private:
    QVariant getValue(const SecKeyType &reg_section_key, int iso);
    QVariant setValue(const SecKeyType &reg_section_key, int iso, QVariant value);
    // get ui item value by value mapping
    QVariant getValueByValueMapping(QVariantList &&values) const;
    // get reg value by reg's idx through value mapping table (for mapping to multiple regs)
    QVariant getRegValueByValueMapping(QVariant value, SecKeyType &&reg_section_key) const;
    // get reg values by value mapping table (for value is array)
    QVariantList getRegValueListByValueMapping(QVariant value) const;
    QVariant validateValue(QVariant value);
    QVariant validateValue(QVariant value, int row);

signals:
    void sigValueChanged(QVariant value);
    void sigPageWriteData();
    void sigUpdateChart();

public slots:
    void handleImportTable(QString file_path);
    void handleExportTable(QString file_path);
    void cellEdited(QLineEdit *edit, int idx);
    void handleUpdateChart();
    void handleIsoChanged(int iso_index);
    void handlePointsUpdate(QList<QPointF> ps);
    void handlePageWriteData();
    void handleValueChanged(QVariant);

private:
    QVariantList value_min_list = {0};
    QVariantList value_max_list = {4096};
    CURVE_TYPE curve_type{CURVE_TYPE::NORMAL};
    ISOChartView *iso_chart;
    int interp_num{4};
    QList<QStringList> str_list;
    QList<QLineEdit *> edit_list;
    QVBoxLayout *main_layout;
    int cur_iso{0};
    QComboBox *iso_box;
    HORIZONTAL_HEADER_TYPE horizontal_header_type{HORIZONTAL_HEADER_TYPE::ISO};
    QString header_name {"ISO"};
    QList<int> num_list {100, 200, 400, 800, 1600, 3200, 6400, 12800, 25600, 51200, 102400, 204800, 409600, 819200, 1638400, 3276800};
};

#endif // ISO_CURVE_HPP
