#ifndef COMBOBOX_CALIBRATION_ITEM_WIDGET_H
#define COMBOBOX_CALIBRATION_ITEM_WIDGET_H

#include "base_calibration_item_widget.hpp"
#include <QWidget>
#include <QComboBox>

#define CUSTOMIZE_ITEM "Customize..."
#define LINEAR_ITEM "Linear"

class ComboboxCaliItemWidget : public BaseCalibrationItemWidget
{
    Q_OBJECT

public:
    explicit ComboboxCaliItemWidget(QString title, QWidget *parent = nullptr);

    void setData(QVariant data) override;
    QVariant getData() override;
    void addItem(QString str);
    void addItem(QString str, QVariant v);
    void setCurrentIndex(int idx);
    int currentIndex();
    QComboBox *getCombobox() {return value_combobox;}

protected:
    virtual void retranslateUi() override;

private:
    void setEnabled(bool enabled);
    void initValueWidget();

signals:
    void sigCustomizeSelected();

private slots:
    void slotComboxSelected(int index);

private:
    QComboBox *value_combobox;
};

#endif // COMBOBOX_CALIBRATION_ITEM_WIDGET_H
