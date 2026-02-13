#ifndef CALIBRATION_IMAGE_LIST_WIDGET_H
#define CALIBRATION_IMAGE_LIST_WIDGET_H

#include "utils.hpp"
#include "calibration_utils.hpp"
#include "calibration_pv_window.hpp"

#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QTableWidget>

class CalibrationImageListWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CalibrationImageListWidget(QWidget *parent = nullptr);
    void addTableItem(RawImageItem *item);
    QList<RawImageItem *> getRawImageList();
    void clearOtherItemDisplay(RawImageItem *item);
signals:
    void sigRawImageItemChecked(int index);

private:
    void initUI();
    void retranslateUi();
    void showPVWindow(RawImageItem *item);

private:
     QVBoxLayout *main_layout;
     QTableWidget *image_table_widget;
     QList<RawImageItem *> raw_image_list;
     CalibrationPVWindow *pv_window{nullptr};
};

#endif // CALIBRATION_IMAGE_LIST_WIDGET_H
