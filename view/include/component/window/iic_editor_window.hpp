#ifndef IIC_EDITOR_WINDOW_HPP
#define IIC_EDITOR_WINDOW_HPP

#include "network_client_helper.hpp"
#include "register_edit.hpp"
#include <QMainWindow>
#include <QGridLayout>
#include <QLabel>
#include <QComboBox>
#include <QPixmap>
#include <QGroupBox>
#include <QCheckBox>


class IICEditorWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit IICEditorWindow(QWidget *parent = nullptr);
    void initMenu();
    void initUI();
    void initEvent();

private:
    QGroupBox *createDeviceInfoGroupbox();
    void disableComponents();
    void enableComponents();
    void handleLoadData();
    void handleSaveData();
    void handleShow();
    void handleRead();
    void handleReadStatus(int id);
    void handleWrite();
    void handleWriteStatus(int id);
    void clearWidget();
    void handleTimeOut();

private:
    QWidget *main_widget;
    QVBoxLayout *main_layout;

    QGroupBox *device_info_group;
    QLineEdit *id_edit;
    QLineEdit *addr_edit;
    QComboBox *abytes_box;
    QComboBox *dbytes_box;
    QPushButton *load_btn;
    QPushButton *save_btn;

    QLineEdit *start_edit;
    QLineEdit *length_edit;
    QPushButton *show_btn;
    QPushButton *read_btn;
    QPushButton *write_btn;

    QWidget *show_widget;
    QVBoxLayout *show_layout;
    QList<RegisterEdit *> reg_list;

    QByteArray _data;

    QTimer *timer;
};

#endif
