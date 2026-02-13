#ifndef REGISTER_CHART_TABLE_H
#define REGISTER_CHART_TABLE_H

#include <QTableWidget>
#include "i_observer.hpp"
#include "line_chart_view.hpp"

class RegisterChartTable : public QTableWidget, public IObserver
{
    Q_OBJECT
public:
    RegisterChartTable(LineChartView *chart);
    ~RegisterChartTable() override;

    void initLineEdits();
    virtual void refreshData(const SecKeyType &reg_section_key) override;
    void setDoubleValidator(double min, double max);

private:
    void cellEdited(QLineEdit *lineedit, int index);

signals:
    void sigValueChanged(int value);

private:
    LineChartView *chart;
};

#endif // REGISTER_CHART_TABLE_H
