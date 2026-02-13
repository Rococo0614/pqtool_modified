#ifndef UNIVERSAL_CURVE_HPP
#define UNIVERSAL_CURVE_HPP

#include <memory>
#include <QWidget>
#include "i_observer.hpp"
#include "iso_chart_view.hpp"
#include <QVBoxLayout>
#include <QLineEdit>
#include <QComboBox>

class UniversalCurve : public QWidget, public IObserver
{
    Q_OBJECT
public:
    UniversalCurve();
    virtual void refreshData(const SecKeyType &reg_section_key) override;
    void setCurveType(CURVE_TYPE curve_type);
    void init_chart();
    void init_editPanel();
    void handleUpdateTheme(bool dark_theme_enabled);
    void setRange(QJsonValue min, QJsonValue max);
    void setLabelNames(QStringList name_list) {this->name_list = name_list; }
    void setComboBoxItems(QStringList item_list) { this->item_list = item_list; }

private:
    QVariant validateValue(QVariant value, int index);

signals:
    void sigValueChanged(QVariant value);
    void sigPageWriteData();
    void sigUpdateChart();

public slots:
    void handleImportTable(QString file_path);
    void handleExportTable(QString file_path);
    void cellEdited(QLineEdit *edit, int index);
    void handleUpdateChart();
    void handleItemChanged(int index);
    void handlePointsUpdate(QList<QPointF> ps);
    void handlePageWriteData();
    void handleValueChanged(QVariant);

private:
    QVariantList min_list = {0};
    QVariantList max_list = {4096};
    CURVE_TYPE curve_type{CURVE_TYPE::NORMAL};
    ISOChartView *iso_chart;
    int interp_num{2};
    int item_index{0};
    QStringList name_list;
    QStringList item_list;
    QList<QLineEdit *> edit_list;
    QVBoxLayout *main_layout;
};

#endif // UNIVERSAL_CURVE_HPP
