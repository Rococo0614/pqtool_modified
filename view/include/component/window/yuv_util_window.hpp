#ifndef YUV_UTIL_WINDOW_HPP
#define YUV_UTIL_WINDOW_HPP

#include <QMainWindow>
#include <QGridLayout>
#include <QLabel>
#include <QComboBox>
#include <QPixmap>
#include <QTableView>
#include <QTableWidget>
#include <QStandardItem>
#include <QPushButton>
#include <QCheckBox>
#include "yuv_image_list_widget.hpp"

class YuvUtilWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit YuvUtilWindow(QWidget *parent = nullptr);
    void initMenu();
    void initUI();
    void initEvent();
    void refresh();
    void retranslateUi();

protected:
    void keyReleaseEvent(QKeyEvent *event) override;

private:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void handleOpenYuvFile();
    void slotSend();
    void slotFlush();
    void slotCancel();

private:
    QWidget *main_widget;
    QGridLayout *main_layout;
    YuvImageListWidget *image_list_view;
    QPushButton *add_yuv_btn;
    QPushButton *dis_select_btn;
    QPushButton *move_up_btn;
    QPushButton *move_down_btn;
    QPushButton *del_btn;
    QPushButton *del_all_btn;
    QComboBox *send_target;
    QCheckBox *stitch_flag;
    QCheckBox *snap_mode;
    QCheckBox *loop;
    QPushButton *send_btn;
    QPushButton *flush_btn;
    QPushButton *cancel_btn;
    bool disel_toggle{false};
};

#endif // YUV_UTIL_WINDOW_HPP
