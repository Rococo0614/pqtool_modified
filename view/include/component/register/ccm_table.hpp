#ifndef CCM_TABLE_H
#define CCM_TABLE_H

#include <QTableWidget>
#include "i_observer.hpp"

class CcmTable : public QTableWidget, public IObserver
{
    Q_OBJECT
public:
    ~CcmTable() override;

    virtual void refreshData(const SecKeyType &reg_section_key) override;

    void initLineEdits();
    QVariant getData();
    void setData(QVariant data);
    void setRange(int min, int max);
    void setFoldableColumn(int col);

private:
    void cellEdited(QLineEdit *lineedit, int index);
    void initCCMLineEdits();
    void initArrayValueLineEdits();
    QVariant validateValue(QVariant value);

signals:
    void sigValueChanged(QVariant value);
    void sigPageWriteData();

public slots:
    void handleToggleEvent(bool table_fold_state);
    void handleImportTable(QString file_path);
    void handleExportTable(QString file_path);

private:
    int value_min{-8192};
    int value_max{8191};
    int foldable_column {3};
};

#endif // CCM_TABLE_H
