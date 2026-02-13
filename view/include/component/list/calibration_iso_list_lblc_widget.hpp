#ifndef CALIBRATION_ISO_LIST_LBLC_WIDGET_H
#define CALIBRATION_ISO_LIST_LBLC_WIDGET_H

#include "utils.hpp"
#include "calibration_utils.hpp"

#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QTableWidget>

class ISOItem : public QObject
{
    Q_OBJECT

public:
    ISOItem(QString name) {
        this->file_name = name;
    }

    virtual ~ISOItem() {}

    ISOItem(QString name, int group, int iso) {
        this->file_name = name;
        this->group = group;
        this->iso = iso;
    }

    QString getName() {
        return this->file_name;
    }

    int getGroup() {
        return group;
    }

    void setGroup(int group) {
        this->group = group;
    }

    int getISO() {
        return this->iso;
    }

    void setISO(int iso) {
        this->iso = iso;
    }

    QPixmap *getInputPixmap() {
        return &this->input_pixmap;
    }

    void setInputPixmap(QPixmap pixmap) {
        this->input_pixmap = pixmap;
    }

    QPixmap *getOutputPixmap() {
        return &this->output_pixmap;
    }

    void setOutputPixmap(QPixmap pixmap) {
        this->output_pixmap = pixmap;
    }

    Qt::CheckState getDisplay() {
       return this->display;
    }

    void setDisplay(Qt::CheckState state) {
        if (state != this->display) {
            this->display = state;
            emit displayStateChanged(state);
        }
    }

   void setLBLC(float lblc_rr_gain, float lblc_gr_gain, float lblc_gb_gain, float lblc_bb_gain,
               int lblc_rr_offset[], int lblc_gr_offset[], int lblc_gb_offset[], int lblc_bb_offset[], int block_num)
   {
       for (int i = 0; i < block_num; i++) {
           this->lblc_rr_offset[i] = lblc_rr_offset[i];
           this->lblc_gr_offset[i] = lblc_gr_offset[i];
           this->lblc_gb_offset[i] = lblc_gb_offset[i];
           this->lblc_bb_offset[i] = lblc_bb_offset[i];
       }

       this->lblc_rr_gain = lblc_rr_gain;
       this->lblc_gr_gain = lblc_gr_gain;
       this->lblc_gb_gain = lblc_gb_gain;
       this->lblc_bb_gain = lblc_bb_gain;
       this->block_num = block_num;
   }

   void getLBLC(float &lblc_rr_gain, float &lblc_gr_gain, float &lblc_gb_gain, float &lblc_bb_gain,
               int lblc_rr_offset[], int lblc_gr_offset[], int lblc_gb_offset[], int lblc_bb_offset[])
   {
       for (int i = 0; i < this->block_num; i++) {
           lblc_rr_offset[i] = this->lblc_rr_offset[i];
           lblc_gr_offset[i] = this->lblc_gr_offset[i];
           lblc_gb_offset[i] = this->lblc_gb_offset[i];
           lblc_bb_offset[i] = this->lblc_bb_offset[i];
       }

       lblc_rr_gain = this->lblc_rr_gain;
       lblc_gr_gain = this->lblc_gr_gain;
       lblc_gb_gain = this->lblc_gb_gain;
       lblc_bb_gain = this->lblc_bb_gain;
   }

signals:
    void displayStateChanged(Qt::CheckState display);

private:
    int group;
    QString file_name;
    int iso{100};
    Qt::CheckState display;
    QPixmap input_pixmap;
    QPixmap output_pixmap;

    // LBLC
    int block_num = 0;
    float lblc_rr_gain = 0.0;
    float lblc_gr_gain = 0.0;
    float lblc_gb_gain = 0.0;
    float lblc_bb_gain = 0.0;
    unique_ptr<int []> lblc_rr_offset{new int [LBLC_BLOCK_NUMS]{0}};
    unique_ptr<int []> lblc_gr_offset{new int [LBLC_BLOCK_NUMS]{0}};
    unique_ptr<int []> lblc_gb_offset{new int [LBLC_BLOCK_NUMS]{0}};
    unique_ptr<int []> lblc_bb_offset{new int [LBLC_BLOCK_NUMS]{0}};
};

class CalibrationISOLBlcListWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CalibrationISOLBlcListWidget(QWidget *parent = nullptr);
    bool addTableItem(ISOItem *item);
    QList<ISOItem *> getISOList();
    QList<ISOItem *> getDisplayISOList();
    int getNextGroup();

private:
    void initUI();
    void retranslateUi();
    void clearOtherItemDisplay(ISOItem *item);

public:
    static const int MAX_LBLC_ISO_ITEM_NUM = 7;

signals:
    void sigSetInputPixmap(QPixmap *pixmap);
    void sigSetOutputPixmap(QPixmap *pixmap);

private:
    QVBoxLayout *main_layout;
    QTableWidget *iso_table_widget;
    QList<ISOItem *> iso_item_list;
};

#endif // CALIBRATION_CT_LIST_WIDGET_H
