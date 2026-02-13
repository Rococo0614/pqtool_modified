#ifndef CALIBRATION_CT_LIST_WIDGET_H
#define CALIBRATION_CT_LIST_WIDGET_H

#include "utils.hpp"
#include "calibration_utils.hpp"

#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QTableWidget>

class CTItem : public QObject
{
    Q_OBJECT

public:
    CTItem(QString name) {
        this->file_name = name;
    }

    virtual ~CTItem() {}

    CTItem(QString name, int group, int ct) {
        this->file_name = name;
        this->group = group;
        this->ct = ct;
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

    int getCT() {
        return this->ct;
    }

    void setCT(int ct) {
        this->ct = ct;
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

    void set_wb_r_gain(unsigned short wb_r_gain)
    {
        this->wb_r_gain = wb_r_gain;
    }

    void set_wb_g_gain(unsigned short wb_g_gain)
    {
        this->wb_g_gain = wb_g_gain;
    }

    void set_wb_b_gain(unsigned short wb_b_gain)
    {
        this->wb_b_gain = wb_b_gain;
    }

    unsigned short get_wb_r_gain()
    {
        return this->wb_r_gain;
    }

    unsigned short get_wb_g_gain()
    {
        return this->wb_g_gain;
    }

    unsigned short get_wb_b_gain()
    {
        return this->wb_b_gain;
    }

    void setCCM(int ccm[], int len)
    {
        for (int i = 0; i < len && i < 9; i++) {
            this->ccm[i] = ccm[i];
        }
    }

    int* getCCM()
    {
        return ccm;
    }

   void setLSC(int mlsc_r_gain[], int mlsc_g_gain[], int mlsc_b_gain[],
               int rlsc_r_gain[], int rlsc_g_gain[], int rlsc_b_gain[], int rlsc_ir_gain[],
               int center_x, int center_y, int norm)
   {
       for (int i = 0; i < MLSC_TABLE_SIZE; i++) {
           this->mlsc_r_gain[i] = mlsc_r_gain[i];
           this->mlsc_g_gain[i] = mlsc_g_gain[i];
           this->mlsc_b_gain[i] = mlsc_b_gain[i];
       }

       for (int i = 0; i < RLSC_TABLE_SIZE; i++) {
           this->rlsc_r_gain[i] = rlsc_r_gain[i];
           this->rlsc_g_gain[i] = rlsc_g_gain[i];
           this->rlsc_b_gain[i] = rlsc_b_gain[i];
           this->rlsc_ir_gain[i] = rlsc_ir_gain[i];
       }

       this->center_x = center_x;
       this->center_y = center_y;
       this->norm = norm;
   }

   void getLSC(int mlsc_r_gain[], int mlsc_g_gain[], int mlsc_b_gain[],
               int rlsc_r_gain[], int rlsc_g_gain[], int rlsc_b_gain[], int rlsc_ir_gain[],
               int *center_x, int *center_y, int *norm)
   {
       for (int i = 0; i < MLSC_TABLE_SIZE; i++) {
           mlsc_r_gain[i] = this->mlsc_r_gain[i];
           mlsc_g_gain[i] = this->mlsc_g_gain[i];
           mlsc_b_gain[i] = this->mlsc_b_gain[i];
           
       }

       for (int i = 0; i < RLSC_TABLE_SIZE; i++) {
           rlsc_r_gain[i] = this->rlsc_r_gain[i];
           rlsc_g_gain[i] = this->rlsc_g_gain[i];
           rlsc_b_gain[i] = this->rlsc_b_gain[i];
           rlsc_ir_gain[i] = this->rlsc_ir_gain[i];
       }

       *center_x = this->center_x;
       *center_y = this->center_y;
       *norm = this->norm;
   }

signals:
    void displayStateChanged(Qt::CheckState display);

private:
    int group;
    QString file_name;
    int ct{1000};
    Qt::CheckState display;
    QPixmap input_pixmap;
    QPixmap output_pixmap;

    unsigned short wb_r_gain{0};
    unsigned short wb_g_gain{0};
    unsigned short wb_b_gain{0};
    int ccm[9];
    // LSC
    int center_x = 0, center_y = 0, norm = 0;
    unique_ptr<int []> mlsc_r_gain{new int [MLSC_TABLE_SIZE]{0}};
    unique_ptr<int []> mlsc_g_gain{new int [MLSC_TABLE_SIZE]{0}};
    unique_ptr<int []> mlsc_b_gain{new int [MLSC_TABLE_SIZE]{0}};
    unique_ptr<int []> rlsc_r_gain{new int [RLSC_TABLE_SIZE]{0}};
    unique_ptr<int []> rlsc_g_gain{new int [RLSC_TABLE_SIZE]{0}};
    unique_ptr<int []> rlsc_b_gain{new int [RLSC_TABLE_SIZE]{0}};
    unique_ptr<int []> rlsc_ir_gain{new int [RLSC_TABLE_SIZE]{0}};
};

class CalibrationCTListWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CalibrationCTListWidget(QWidget *parent = nullptr);
    bool addTableItem(CTItem *item);
    QList<CTItem *> getCTList();
    QList<CTItem *> getDisplayCTList();
    int getNextGroup();

private:
    void initUI();
    void retranslateUi();
    void clearOtherItemDisplay(CTItem *item);

public:
    static const int MAX_CT_ITEM_NUM = 14;

signals:
    void sigSetInputPixmap(QPixmap *pixmap);
    void sigSetOutputPixmap(QPixmap *pixmap);

private:
    QVBoxLayout *main_layout;
    QTableWidget *ct_table_widget;
    QList<CTItem *> ct_item_list;
};

#endif // CALIBRATION_CT_LIST_WIDGET_H
