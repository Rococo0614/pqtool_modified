#ifndef CONNECT_DEVICE_WINDOW_H
#define CONNECT_DEVICE_WINDOW_H

#include "global_data.hpp"

#include <QMainWindow>
#include <QGridLayout>
#include <QLineEdit>
#include <QComboBox>
#include <QCheckBox>
#include <QLabel>
#include <QPushButton>
#include <QDir>

class QSpinBox;

const int CONNECT_DEVICE_WINDOW_WIDTH = 450;
const int CONNECT_DEVICE_WINDOW_HEIGHT = 360;

const QString DEFAULT_DEVICE_IP_ADDRESS = "192.168.1.3";
const QString DEFAULT_DEVICE_PORT = "5566";

static QVector<QString> connect_type_vec = {CONNECT_TYPE_NETWORK, CONNECT_TYPE_UART};

class ConnectDeviceWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ConnectDeviceWindow(QWidget *parent = nullptr);
    ~ConnectDeviceWindow();
    void initUI();
    void updateUI();
    void changeEvent(QEvent *event);
    void handleUpdateTemplate(QString file_full_name);

signals:
    void sigConnect();
    void sigIPAddressOrUartPortChanged();
    void sigGetTemplateFile();

private slots:
    void handleConnectDevice();
    void handleChangeConnectType(QString connect_type);
    void CvipqToolSocConfirm(QString text);
    void handleGetTemplateFile();
    void handleSelectAutoJsonFile();

private:
    void retranslateUi();
    void saveConnectInfoTofile(bool current_setting);
    ToolJsonState getToolJsonFromBoard();

private:
    QWidget *main_widget;
    QGridLayout *main_layout;

    QLabel *template_label;
    QLabel *connect_type_label;
    QLabel *ip_address_label;
    QLabel *port_label;
    QLabel *uart_port_label;
    QLabel *baud_rate_label;
    QLabel *auto_apply_label;
    QLabel *auto_apply_path_label;
    QLabel *auto_apply_interval_label;

    QStringList cv182x_template_name_list;
    QStringList cv183x_template_name_list;
    QStringList cv182x_template_folder_list;
    QStringList cv183x_template_folder_list;
    QComboBox *template_combo_box;
    QComboBox *connect_type_box;
    QComboBox *uart_port_box;
    QComboBox *baud_rate_box;
    QLineEdit *ip_address_line_edit;
    QLineEdit *port_line_edit;
    QCheckBox *auto_apply_checkbox;
    QLineEdit *auto_apply_path_line_edit;
    QPushButton *auto_apply_browse_btn;
    QSpinBox *auto_apply_interval_spin;
    QCheckBox *remember_checkbox;
    QPushButton *connect_btn;
    QPushButton *get_template_btn;
    QStringList strFileNameList;
};

#endif // CONNECT_DEVICE_WINDOW_H
