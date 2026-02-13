#ifndef ISO_TABLE_HPP
#define ISO_TABLE_HPP

#include <memory>
#include <QTableWidget>
#include "i_observer.hpp"

enum class VALUE_TYPE
{
    INT,
    DOUBLE,
};

class IsoTable : public QTableWidget, public IObserver
{
    Q_OBJECT
public:
    virtual void refreshData(const SecKeyType &reg_section_key) override;

    void initLineEdits();
    QVariant getData();
    void setData(QVariant data);
    void setValueMappingVec(QVector<QVariantList> &&value_mapping_vec);
    void setRange(int min, int max);
    void setRangeList(const QVariantList &min, const QVariantList &max);
    void setHeaderType(QString type);
    void setValueType(QString type);

private:
    void cellEdited(QLineEdit *lineedit, int index);
    QVariant getValue(const SecKeyType &reg_section_key, int iso);
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

public slots:
    void handleImportTable(QString file_path);
    void handleExportTable(QString file_path);

private:
    HORIZONTAL_HEADER_TYPE horizontal_header_type{HORIZONTAL_HEADER_TYPE::ISO};
    VALUE_TYPE value_type{VALUE_TYPE::INT};
    QVector<QVariantList> value_mapping_vec;
    QVariantList value_min_list = {0};
    QVariantList value_max_list = {4096};
    QString header_name {"ISO"};
    QList<int> num_list {100, 200, 400, 800, 1600, 3200, 6400, 12800, 25600, 51200, 102400, 204800, 409600, 819200, 1638400, 3276800};
};

#endif // ISO_TABLE_HPP
