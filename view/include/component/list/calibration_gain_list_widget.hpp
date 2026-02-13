#ifndef CALIBRATION_GAIN_LIST_WIDGET_H
#define CALIBRATION_GAIN_LIST_WIDGET_H

#include "utils.hpp"
#include "calibration_utils.hpp"

#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QTableWidget>

typedef struct {
    QString path;
    bool select;
    int weight;
    int temp;
    unsigned short Rgain;
    unsigned short Ggain;
    unsigned short Bgain;
} GAIN_INFO;

class GainItem : public QObject
{
    Q_OBJECT

public:
    GainItem() {}
    virtual ~GainItem() {}

    GainItem(GAIN_INFO info) {
        this->path = info.path;
        this->select = info.select;
        this-> weight = info.weight;
        this-> temp = info.temp;
        this->Rgain = info.Rgain;
        this->Ggain = info.Ggain;
        this->Bgain = info.Bgain;
    }

    QString getFilePath() {
        return this->path;
    }

    QString getFileName() {
        if (this->path.isEmpty()) {
            return "";
        }
        QFileInfo info(this->path);
        return info.fileName();
    }

    unsigned short getRgain() {
        return this->Rgain;
    }

    unsigned short getGgain() {
        return this->Ggain;
    }

    unsigned short getBgain() {
        return this->Bgain;
    }

    float getBg() {
        if (Bgain > 0) {
            return 1024.0/Bgain;
        } else {
            return 0;
        }
    }

    float getRg() {
        if (Rgain > 0) {
            return 1024.0/Rgain;
        } else {
            return 0;
        }
    }

    int getTemp() {
        return this->temp;
    }

    void setTemp(int temp) {
        this->temp = temp;
    }

    void setWeight(int weight) {
        this->weight = weight;
    }

    int getWeight() {
        return this->weight;
    }

    bool getSelected() {
        return this->select;
    }

    void setSelected(bool select) {
        this->select = select;
    }

private:
    QString path;
    bool select = false;
    int weight = 1;
    int temp = 0;
    unsigned short Rgain;
    unsigned short Ggain;
    unsigned short Bgain;
};

class CalibrationGainListWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CalibrationGainListWidget(QWidget *parent = nullptr);
    bool addTableItem(GainItem *item);
    QList<GainItem *> getGainList();
    QList<GainItem *> getSelectGainList();
    void clearTable();

signals:
    void sigDelPoint(int idx);

private:
    void initUI();
    void retranslateUi();

public:
    static const int MAX_GAIN_ITEM_NUM = 7;

private:
    QVBoxLayout *main_layout;
    QTableWidget *gain_table_widget;
    QList<GainItem *> gain_item_list;
};

#endif
