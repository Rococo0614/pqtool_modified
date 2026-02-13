#ifndef AF_STATISTIC_WINDOW_HPP
#define AF_STATISTIC_WINDOW_HPP

#include <QWidget>
#include <QMainWindow>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTableWidget>
#include <QMap>
#include <QCloseEvent>

const QString KEY_ATTR_AF_ENABLE = "Enable";
const QString KEY_ARRT_H_WND = "Hwnd";
const QString KEY_ARRT_V_WND = "Vwnd";
const QString KEY_ARRT_CROP_ENABLE = "Crop.Enable";
const QString KEY_ARRT_CROP_X = "Crop.X";
const QString KEY_ARRT_CROP_Y = "Crop.Y";
const QString KEY_ARRT_CROP_W = "Crop.W";
const QString KEY_ARRT_CROP_H = "Crop.H";
const QString KEY_ATTR_AF_ZONE_V1 = "AF Zone V1";
const QString KEY_ATTR_AF_ZONE_H1 = "AF Zone H1";
const QString KEY_ATTR_AF_ZONE_H2 = "AF Zone H2";
const QString KEY_ATTR_AF_ZONE_HLCNT = "AF Zone HlCnt";

class AFStatWindow: public QMainWindow
{
    Q_OBJECT

public:
    explicit AFStatWindow(QWidget *parent = nullptr);

public slots:
    void updateStatistic(QVariant );
    void updateStatistic(QMap<QString, int> &map);
    void exportStatistic();

signals:
    void sigWinClose();
    void sigChangeFocusItem(int idx);

protected:
    virtual void closeEvent(QCloseEvent *event) override;

private:
    void initUI();
    void initEvent();
    void initMenu();
    void initLeftArea();
    void initRightArea();

private:
    QHBoxLayout *main_layout;
    QWidget *main_widget;

    QTableWidget *stat_table;
};

#endif //AF_STATISTIC_WINDOW_HPP
