#ifndef CALIBRATION_ISO_LIST_WIDGET_H
#define CALIBRATION_ISO_LIST_WIDGET_H

#include "utils.hpp"
#include "calibration_utils.hpp"

#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QTableWidget>

class IsoItem : public QObject
{
    Q_OBJECT

public:
    IsoItem() {
    }

    virtual ~IsoItem() {}

    int getGroup() {
        return group;
    }

    void setGroup(int group) {
        this->group = group;
    }

    int getIso() {
        return this->iso;
    }

    void setIso(int iso) {
        this->iso = iso;
    }

    int getFrames() {
        return this->frames;
    }

    void setFrames(int frames) {
        this->frames = frames;
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

    void setfCaliSlope(float fCaliSlopeB, float fCaliSlopeGb, float fCaliSlopeGr, float fCaliSlopeR, float fCaliSlopeMax) {
        this->fCaliSlopeB = fCaliSlopeB;
        this->fCaliSlopeGb = fCaliSlopeGb;
        this->fCaliSlopeGr = fCaliSlopeGr;
        this->fCaliSlopeR = fCaliSlopeR;
        this->fCaliSlopeMax = fCaliSlopeMax;
    }

    void setfCaliSlope(float fCaliSlopeB, float fCaliSlopeG, float fCaliSlopeR) {
        this->fCaliSlopeB = fCaliSlopeB;
        this->fCaliSlopeG = fCaliSlopeG;
        this->fCaliSlopeR = fCaliSlopeR;
    }

    void setfCaliIntercept(float fCaliInterceptB, float fCaliInterceptGb, float fCaliInterceptGr, float fCaliInterceptR, float fCaliInterceptMax) {
        this->fCaliInterceptB = fCaliInterceptB;
        this->fCaliInterceptGb = fCaliInterceptGb;
        this->fCaliInterceptGr = fCaliInterceptGr;
        this->fCaliInterceptR = fCaliInterceptR;
        this->fCaliInterceptMax = fCaliInterceptMax;
    }

    float getfCaliSlopeB()
    {
        return fCaliSlopeB;
    }

    float getfCaliSlopeGb()
    {
        return fCaliSlopeGb;
    }

    float getfCaliSlopeGr()
    {
        return fCaliSlopeGr;
    }

    float getfCaliSlopeG()
    {
        return fCaliSlopeG;
    }

    float getfCaliSlopeR()
    {
        return fCaliSlopeR;
    }

    float getfCaliSlopeMax()
    {
        return fCaliSlopeMax;
    }

    float getffCaliInterceptMax()
    {
        return fCaliInterceptMax;
    }

    float getfCaliInterceptB()
    {
        return fCaliInterceptB;
    }

    float getfCaliInterceptGb()
    {
        return fCaliInterceptGb;
    }

    float getfCaliInterceptGr()
    {
        return fCaliInterceptGr;
    }

    float getfCaliInterceptR()
    {
        return fCaliInterceptR;
    }

signals:
    void displayStateChanged(Qt::CheckState display);

private:
    int group;
    int iso;
    int frames;
    Qt::CheckState display;
    QPixmap input_pixmap;
    QPixmap output_pixmap;

    float fCaliSlopeB{0};
    float fCaliInterceptB{0};
    float fCaliSlopeGb{0};
    float fCaliInterceptGb{0};
    float fCaliSlopeGr{0};
    float fCaliInterceptGr{0};
    float fCaliSlopeG{0};
    float fCaliSlopeR{0};
    float fCaliInterceptR{0};
    float fCaliInterceptMax{0};
    float fCaliSlopeMax{0};
};

class CalibrationIsoListWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CalibrationIsoListWidget(QWidget *parent = nullptr);
    bool addTableItem(IsoItem *item);
    QList<IsoItem *> getIsoList();
    QList<IsoItem *> getDisplayIsoList();
    int getNextGroup();

private:
    void initUI();
    void retranslateUi();
    void clearOtherItemDisplay(IsoItem *item);
    void clearSameIsoItemDisplay(IsoItem *item);

public:
    static const int MAX_ISO_ITEM_NUM = 24;

signals:
    void sigSetInputPixmap(QPixmap *pixmap);
    void sigSetOutputPixmap(QPixmap *pixmap);

private:
    QVBoxLayout *main_layout;
    QTableWidget *iso_table_widget;
    QList<IsoItem *> iso_item_list;
};

#endif // CALIBRATION_ISO_LIST_WIDGET_H
