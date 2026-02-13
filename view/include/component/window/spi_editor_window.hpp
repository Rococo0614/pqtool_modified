#ifndef SPI_EDITOR_WINDOW_HPP
#define SPI_EDITOR_WINDOW_HPP

#include "network_client_helper.hpp"
#include "register_edit.hpp"
#include <QMainWindow>
#include <QGridLayout>
#include <QLabel>
#include <QComboBox>
#include <QPixmap>
#include <QGroupBox>
#include <QCheckBox>


class SPIEditorWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit SPIEditorWindow(QWidget *parent = nullptr);
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
    void handleTransmit();
    void handleTransmitStatus(int id);
    void clearWidget();
    void handleTimeOut();
    void spiNumChange(int index);

private:
    QWidget *main_widget;
    QVBoxLayout *main_layout;

    QGroupBox *device_info_group;
    QComboBox *num_box;
    QComboBox *csn_box;
    QComboBox *mode_box;
    QLineEdit *speed_edit;
    QComboBox *dbytes_box;
    QComboBox *dorder_box;
    QPushButton *load_btn;
    QPushButton *save_btn;

    QLineEdit *len_tx_edit;
    QLineEdit *len_rx_edit;
    QPushButton *show_btn;
    QPushButton *clear_btn;
    QPushButton *trans_btn;

    QWidget *show_widget;
    QVBoxLayout *show_layout;
    QList<RegisterEdit *> reg_list_tx;
    QList<RegisterEdit *> reg_list_rx;

    QByteArray _data;

    QTimer *timer;
};

#endif
