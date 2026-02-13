#ifndef UNIVERSAL_TABLE_HPP
#define UNIVERSAL_TABLE_HPP

#include <memory>
#include <QTableWidget>
#include "i_observer.hpp"

class UniversalTable : public QTableWidget, public IObserver
{
    Q_OBJECT
public:
    virtual void refreshData(const SecKeyType &reg_section_key) override;

    void initLineEdits();
    QVariant getData(QString path);
    void setData(QString path, QVariant data);
    void setRange(QJsonValue min, QJsonValue max);
    void setValueType(QString type);

private:
    void cellEdited(QLineEdit *lineedit, int index);
    QVariant validateValue(QVariant value, int row, bool &is_invalid);

signals:
    void sigValueChanged();
    void sigPageWriteData();

public slots:
    void handleImportTable(QString file_path);
    void handleExportTable(QString file_path);
    void hideSumColumn(bool colum_state);

private:
    QVector<QVariantList> value_mapping_vec;
    QMetaType::Type value_type{QMetaType::Int};
    QVariantList min_list = {0};
    QVariantList max_list = {0};
    QList<int> num_list {100, 200, 400, 800, 1600, 3200, 6400, 12800, 25600, 51200, 102400, 204800, 409600, 819200, 1638400, 3276800};
    bool is_ccm_sum_member = false;
};

#endif // UNIVERSAL_TABLE_HPP
