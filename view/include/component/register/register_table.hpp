#ifndef REGISTER_TABLE_H
#define REGISTER_TABLE_H

#include <QTableWidget>
#include "i_observer.hpp"

class RegisterTable : public QTableWidget, public IObserver
{
    Q_OBJECT

public:
    ~RegisterTable() override;

    virtual void refreshData(const SecKeyType &reg_section_key) override;

    void initLineEdits();
    QVariant getData();
    void setData(QVariant data);
    void setRange(long long min, long long max);
    void setRangeList(const QVariantList &min, const QVariantList &max);

private:
    void cellEdited(QLineEdit *lineedit, int index);
    void initSingleValueLineEdits();
    void initArrayValueLineEdits();
    QVariant validateValue(QVariant value);
    QVariant validateValue(QVariant value, int col_idx);

signals:
    void sigValueChanged(QVariant value);
    void sigPageWriteData();

public slots:
    void handleImportTable(QString file_path);
    void handleExportTable(QString file_path);

private:
    QVariantList value_min_list = {0};
    QVariantList value_max_list = {4096};
};

#endif // REGISTER_TABLE_H
