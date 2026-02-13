#include "connect_device_window.hpp"
#include "network_client_helper.hpp"
#include "utils.hpp"
#include "global_data.hpp"

#include <QLabel>
#include <QPushButton>
#include <QMessageBox>
#include <QShortcut>
#include <QFileDialog>
#include <QSpinBox>

ConnectDeviceWindow::ConnectDeviceWindow(QWidget *parent) :
    QMainWindow(parent)
{
    setWindowFlags(Qt::WindowCloseButtonHint);
    this->setFixedSize(CONNECT_DEVICE_WINDOW_WIDTH, CONNECT_DEVICE_WINDOW_HEIGHT);

    initUI();
    retranslateUi();
}

ConnectDeviceWindow::~ConnectDeviceWindow() {}

void ConnectDeviceWindow::initUI()
{
    int i;
    main_widget = new QWidget;
    main_layout = new QGridLayout;

    template_label = new QLabel();
    connect_type_label = new QLabel();
    ip_address_label = new QLabel();
    port_label = new QLabel();
    uart_port_label = new QLabel();
    baud_rate_label = new QLabel();
    auto_apply_label = new QLabel();
    auto_apply_path_label = new QLabel();
    auto_apply_interval_label = new QLabel();

    template_combo_box = new QComboBox;
    QDir templates_dir(TEMPLATES_FOLDER);
    QFileInfoList files_list = templates_dir.entryInfoList({"*.json"}, QDir::Files);
    for (i = 0; i < files_list.count(); i++) {
        strFileNameList.append(files_list.at(i).fileName());
    }
    for (i = 0; i < strFileNameList.size(); i++) {
        template_combo_box->addItem(strFileNameList.at(i));
    }

    connect_type_box = new QComboBox;
    for (auto connect_type : connect_type_vec) {
        connect_type_box->addItem(tr(connect_type.toStdString().c_str()));
    }
    connect(connect_type_box, &QComboBox::currentTextChanged, this, &ConnectDeviceWindow::handleChangeConnectType);

    ip_address_line_edit = new QLineEdit;

    port_line_edit = new QLineEdit;
    // port validator
    QIntValidator *port_validator = new QIntValidator(0, 10000, this);
    port_line_edit->setValidator(port_validator);

    uart_port_box = new QComboBox();
    QList<QSerialPortInfo> uart_available_port = QSerialPortInfo::availablePorts();
    if (uart_available_port.isEmpty())
    {
        uart_port_box->addItem("None");
    } else {
        foreach(const QSerialPortInfo &port_info, uart_available_port)
        {
            uart_port_box->addItem(port_info.portName());
        }
    }

    baud_rate_box = new QComboBox();
    baud_rate_box->setFixedWidth(250);
    for (auto baud_rate : baud_rate_list) {
        baud_rate_box->addItem(tr(baud_rate.toStdString().c_str()));
    }

    auto_apply_checkbox = new QCheckBox();
    auto_apply_path_line_edit = new QLineEdit();
    auto_apply_browse_btn = new QPushButton();
    connect(auto_apply_browse_btn, &QPushButton::clicked, this, &ConnectDeviceWindow::handleSelectAutoJsonFile);
    auto_apply_interval_spin = new QSpinBox();
    auto_apply_interval_spin->setRange(100, 3600 * 1000);
    auto_apply_interval_spin->setSingleStep(500);
    auto_apply_interval_spin->setSuffix(" ms");

    remember_checkbox = new QCheckBox();

    connect_btn = new QPushButton();
    connect(connect_btn, &QPushButton::clicked, this, &ConnectDeviceWindow::handleConnectDevice);

    get_template_btn = new QPushButton();
    connect(get_template_btn, &QPushButton::clicked, this, &ConnectDeviceWindow::handleGetTemplateFile);

    QWidget *auto_apply_path_widget = new QWidget();
    QHBoxLayout *auto_apply_path_layout = new QHBoxLayout();
    auto_apply_path_layout->setContentsMargins(0, 0, 0, 0);
    auto_apply_path_layout->addWidget(auto_apply_path_line_edit);
    auto_apply_path_layout->addWidget(auto_apply_browse_btn);
    auto_apply_path_widget->setLayout(auto_apply_path_layout);

    main_layout->addWidget(template_label, 1, 0);
    main_layout->addWidget(connect_type_label, 2, 0);
    main_layout->addWidget(ip_address_label, 3, 0);
    main_layout->addWidget(port_label, 4, 0);
    main_layout->addWidget(uart_port_label, 5, 0);
    main_layout->addWidget(baud_rate_label, 6, 0);
    main_layout->addWidget(auto_apply_label, 7, 0);
    main_layout->addWidget(auto_apply_path_label, 8, 0);
    main_layout->addWidget(auto_apply_interval_label, 9, 0);
    main_layout->addWidget(template_combo_box, 1, 1);
    main_layout->addWidget(connect_type_box, 2, 1);
    main_layout->addWidget(ip_address_line_edit, 3, 1);
    main_layout->addWidget(port_line_edit, 4, 1);
    main_layout->addWidget(uart_port_box, 5, 1);
    main_layout->addWidget(baud_rate_box, 6, 1);
    main_layout->addWidget(auto_apply_checkbox, 7, 1);
    main_layout->addWidget(auto_apply_path_widget, 8, 1);
    main_layout->addWidget(auto_apply_interval_spin, 9, 1);
    main_layout->addWidget(remember_checkbox, 10, 0);
    main_layout->addWidget(connect_btn, 11, 0, 1, 2);
    main_layout->addWidget(get_template_btn, 12, 0, 1, 2);

    setCentralWidget(main_widget);
    centralWidget()->setLayout(main_layout);

    updateUI();
}

void ConnectDeviceWindow::changeEvent(QEvent *event)
{
    switch(event->type()) {
    case QEvent::LanguageChange:
        retranslateUi();
        break;
    default:
        break;
    }
    QMainWindow::changeEvent(event);
}

void ConnectDeviceWindow::updateUI()
{
    shared_ptr<GlobalData> global_data = GlobalData::getInstance();
    int item_index = 0;


    QString template_name = global_data->getSettings(DEVICE_SECTION, KEY_TEMPLATE, "").toString();
    item_index = template_combo_box->findText(template_name);

    if (-1 != item_index) {
        template_combo_box->setCurrentIndex(item_index);
    }

    QString connect_type = global_data->getSettings(DEVICE_SECTION, KEY_CONNECT_TYPE, CONNECT_TYPE_NETWORK).toString();
    item_index = connect_type_box->findText(connect_type);
    if (-1 != item_index) {
        connect_type_box->setCurrentIndex(item_index);
    }

    QString ip_address = global_data->getSettings(DEVICE_SECTION, KEY_IP_ADDRESS, DEFAULT_DEVICE_IP_ADDRESS).toString();
    ip_address_line_edit->setText(tr(ip_address.toStdString().c_str()));

    QString port = global_data->getSettings(DEVICE_SECTION, KEY_PORT, DEFAULT_DEVICE_PORT).toString();
    port_line_edit->setText(tr(port.toStdString().c_str()));

    QString uart_port = global_data->getSettings(DEVICE_SECTION, KEY_UART_PORT, UART_PORT_COM1).toString();
    item_index = uart_port_box->findText(uart_port);
    if (-1 != item_index) {
        uart_port_box->setCurrentIndex(item_index);
    }

    QString baud_rate = global_data->getSettings(DEVICE_SECTION, KEY_BAUD_RATE, BAUD_RATE_115200).toString();
    item_index = baud_rate_box->findText(baud_rate);
    if (-1 != item_index) {
        baud_rate_box->setCurrentIndex(item_index);
    }

    bool auto_apply_enabled = global_data->getSettings(SETTINGS_SECTION, KEY_AUTO_APPLY_JSON, false).toBool();
    auto_apply_checkbox->setChecked(auto_apply_enabled);
    QString auto_apply_path = global_data->getSettings(SETTINGS_SECTION, KEY_AUTO_APPLY_JSON_PATH, "").toString();
    auto_apply_path_line_edit->setText(auto_apply_path);
    int auto_apply_interval = global_data->getSettings(SETTINGS_SECTION, KEY_AUTO_APPLY_INTERVAL_MS, 1000).toInt();
    auto_apply_interval_spin->setValue(auto_apply_interval);

    bool remember_setting = global_data->getSettings(DEVICE_SECTION, KEY_REMEMBER_SETTINGS, true).toBool();
    remember_checkbox->setChecked(remember_setting);

    handleChangeConnectType(connect_type);
}

void ConnectDeviceWindow::saveConnectInfoTofile(bool current_setting)
{
    shared_ptr<GlobalData> global_data = GlobalData::getInstance();

    QString ip_address = global_data->getSettings(DEVICE_SECTION, KEY_IP_ADDRESS, DEFAULT_DEVICE_IP_ADDRESS).toString();
    QString port = global_data->getSettings(DEVICE_SECTION, KEY_PORT, DEFAULT_DEVICE_PORT).toString();
    QString template_info = global_data->getSettings(DEVICE_SECTION, KEY_TEMPLATE, template_combo_box->currentText()).toString();
    QString connect_type = global_data->getSettings(DEVICE_SECTION, KEY_CONNECT_TYPE, CONNECT_TYPE_NETWORK).toString();
    QString uart_port = global_data->getSettings(DEVICE_SECTION, KEY_UART_PORT, UART_PORT_COM1).toString();
    QString baud_rate = global_data->getSettings(DEVICE_SECTION, KEY_BAUD_RATE, BAUD_RATE_115200).toString();
    bool save_setting = global_data->getSettings(DEVICE_SECTION, KEY_REMEMBER_SETTINGS, true).toBool();

    if (current_setting) {
        if ((ip_address != ip_address_line_edit->text()) || (port != port_line_edit->text())
            || (template_info != template_combo_box->currentText()) || (connect_type != connect_type_box->currentText())
            || (uart_port != uart_port_box->currentText()) || (baud_rate != baud_rate_box->currentText())
            || (current_setting != save_setting)) {
                global_data->setSettings(DEVICE_SECTION, KEY_IP_ADDRESS, ip_address_line_edit->text());
                global_data->setSettings(DEVICE_SECTION, KEY_PORT, port_line_edit->text());
                global_data->setSettings(DEVICE_SECTION, KEY_TEMPLATE, template_combo_box->currentText());
                global_data->setSettings(DEVICE_SECTION, KEY_CONNECT_TYPE, connect_type_box->currentText());
                global_data->setSettings(DEVICE_SECTION, KEY_UART_PORT, uart_port_box->currentText());
                global_data->setSettings(DEVICE_SECTION, KEY_BAUD_RATE, baud_rate_box->currentText());
                global_data->setSettings(DEVICE_SECTION, KEY_REMEMBER_SETTINGS, current_setting);
                global_data->setSettings(SETTINGS_SECTION, KEY_AUTO_APPLY_JSON, auto_apply_checkbox->isChecked());
                global_data->setSettings(SETTINGS_SECTION, KEY_AUTO_APPLY_JSON_PATH, auto_apply_path_line_edit->text());
                global_data->setSettings(SETTINGS_SECTION, KEY_AUTO_APPLY_INTERVAL_MS, auto_apply_interval_spin->value());
                global_data->setSettings(SETTINGS_SECTION, KEY_MAX_LUT_SHOW_COUNT, 51);
                global_data->setSettings(SETTINGS_SECTION, COLOR_BLOCKS_DETECT_LOWTHR, 10);
                global_data->setSettings(SETTINGS_SECTION, COLOR_BLOCKS_DETECT_HIGHTHR, 40);
        }
    }
}

ToolJsonState ConnectDeviceWindow::getToolJsonFromBoard()
{
    ToolJsonState ret = JSON_WAIT;
    QTime stopTime;

    NetworkClientHelper::getInstance()->connectJsonrpc(CONNECTION_MODE::GET_TEMPLATE);
    stopTime = QTime::currentTime().addSecs(3);
    while (QTime::currentTime() < stopTime) {
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
        if (GlobalData::getInstance()->getBoardTooljsonState() != JSON_WAIT) {
            ret = GlobalData::getInstance()->getBoardTooljsonState();
            break;
        }
    }

    return ret;
}

void ConnectDeviceWindow::handleConnectDevice()
{
    if (template_combo_box->currentText() == NULL) {
        QMessageBox::warning(this, tr("CviPQ Tool"), tr("Template is null. Pelease select a template!"), QMessageBox::Yes);
        return;
    }

    QString connect_type =  connect_type_box->currentText();

    if (connect_type == CONNECT_TYPE_NETWORK) {
        QString ip_range = "(?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])";
        QRegExp ip_regex ("^" + ip_range
                         + "\\." + ip_range
                         + "\\." + ip_range
                         + "\\." + ip_range + "$");
        QRegExpValidator *ip_validator = new QRegExpValidator(ip_regex, this);
        int pos = 0;
        QString input = ip_address_line_edit->text();

        if (!input.isEmpty() && ip_validator->validate(input, pos) != QValidator::State::Acceptable) {
            QMessageBox::warning(this, tr("CviPQ Tool"), tr("The IP Address is not valid!"), QMessageBox::Yes);
            return;
        }
    }
    shared_ptr<GlobalData> global_data = GlobalData::getInstance();
    bool remember_settings = true == remember_checkbox->isChecked();
    global_data->setCvipqToolTemplate(template_combo_box->currentText());
    CvipqToolSocConfirm(template_combo_box->currentText());
    saveConnectInfoTofile(remember_settings);

    ToolJsonState eJsonState = getToolJsonFromBoard();
    if (eJsonState == JSON_EXIST) {
        global_data->setCvipqToolTemplate(template_combo_box->currentText());
        CvipqToolSocConfirm(template_combo_box->currentText());
    }
    GlobalData::getInstance()->setBoardTooljsonState(JSON_WAIT);

    global_data->saveSettings();
    emit sigIPAddressOrUartPortChanged();
    emit sigConnect();
    this->hide();
}

void ConnectDeviceWindow::handleChangeConnectType(QString connect_type)
{
    if (connect_type == CONNECT_TYPE_CMODEL) {
        ip_address_label->hide();
        port_label->hide();
        ip_address_line_edit->hide();
        port_line_edit->hide();
        uart_port_label->hide();
        baud_rate_label->hide();
        uart_port_box->hide();
        baud_rate_box->hide();
    } else if (connect_type == CONNECT_TYPE_NETWORK) {
        ip_address_label->show();
        port_label->show();
        ip_address_line_edit->show();
        port_line_edit->show();
        uart_port_label->hide();
        baud_rate_label->hide();
        uart_port_box->hide();
        baud_rate_box->hide();
    } else if (connect_type == CONNECT_TYPE_UART) {
        ip_address_label->hide();
        port_label->hide();
        ip_address_line_edit->hide();
        port_line_edit->hide();
        uart_port_label->show();
        baud_rate_label->show();
        uart_port_box->show();
        baud_rate_box->show();
    }
    GlobalData::getInstance()->setSettings(DEVICE_SECTION, KEY_CONNECT_TYPE, connect_type_box->currentText());
}

void ConnectDeviceWindow::CvipqToolSocConfirm(QString text)
{
    QString path = TEMPLATES_FOLDER + text;

    QJsonObject load_data;
    QString error_msg = "Template contents don't meet json standards. ";
    if (utils::loadJson(path, load_data, error_msg) != 0) {
        QMessageBox::warning(this, tr("CviPQ Tool"), error_msg, QMessageBox::Yes);
        return;
    }

    QJsonObject device = load_data["DEVICE"].toObject();
    QString fullName = device["FULL_NAME"].toString();
    QString ispBranch = device["ISP_BRANCH"].toString();

    GlobalData::getInstance()->setCvipqToolBranch(ispBranch);

    if (fullName == CV183X_SOC) {
        GlobalData::getInstance()->setCvipqToolSoc(CV183X_SOC);
    } else if ((fullName == CV182X_SOC) || (fullName == "cv182x")) {
        GlobalData::getInstance()->setCvipqToolSoc(CV182X_SOC);
    } else if ((fullName == CV181X_SOC) || (fullName == "mars") || (fullName == "cv181x")) {
        GlobalData::getInstance()->setCvipqToolSoc(CV181X_SOC);
    } else if ((fullName == CV180X_SOC) || (fullName == "phobos") || (fullName == "cv180x")) {
        GlobalData::getInstance()->setCvipqToolSoc(CV180X_SOC);
    } else if ((fullName == CV186X_SOC) || (fullName == "athena2") || (fullName == "cv186x")) {
        GlobalData::getInstance()->setCvipqToolSoc(CV186X_SOC);
    } else if ((fullName == CV184X_SOC) || (fullName == "mars3") || (fullName == "cv184x")) {
        GlobalData::getInstance()->setCvipqToolSoc(CV184X_SOC);
    }
}

void ConnectDeviceWindow::retranslateUi()
{
    setWindowTitle(tr("Connect device"));

    template_label->setText(tr("Templates:"));
    connect_type_label->setText(tr("Connect Type:"));
    ip_address_label->setText(tr("IP Address:"));
    port_label->setText(tr("Port:"));
    uart_port_label->setText(tr("Uart Port:"));
    baud_rate_label->setText(tr("Baud Rate:"));
    auto_apply_label->setText(tr("Auto Apply Json:"));
    auto_apply_path_label->setText(tr("Json Path:"));
    auto_apply_interval_label->setText(tr("Apply Interval:"));
    remember_checkbox->setText(tr("Remember Settings"));
    connect_btn->setText(tr("Connect"));
    get_template_btn->setText(tr("Get Template from Board"));
    auto_apply_browse_btn->setText(tr("Browse"));
}

void ConnectDeviceWindow::handleSelectAutoJsonFile()
{
    QString current_path = auto_apply_path_line_edit->text();
    QString file_path = QFileDialog::getOpenFileName(this, tr("Select Json File"), current_path, tr("Json files (*.json)"));
    if (!file_path.isEmpty()) {
        auto_apply_path_line_edit->setText(file_path);
    }
}

void ConnectDeviceWindow::handleGetTemplateFile()
{
    QString connect_type =  connect_type_box->currentText();

    if (connect_type == CONNECT_TYPE_NETWORK) {
        QString ip_range = "(?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])";
        QRegExp ip_regex ("^" + ip_range
                         + "\\." + ip_range
                         + "\\." + ip_range
                         + "\\." + ip_range + "$");
        QRegExpValidator *ip_validator = new QRegExpValidator(ip_regex, this);
        int pos = 0;
        QString input = ip_address_line_edit->text();

        if (!input.isEmpty() && ip_validator->validate(input, pos) != QValidator::State::Acceptable) {
            QMessageBox::warning(this, tr("CviPQ Tool"), tr("The IP Address is not valid!"), QMessageBox::Yes);
            return;
        }
    }
    shared_ptr<GlobalData> global_data = GlobalData::getInstance();
    bool remember_settings = true == remember_checkbox->isChecked();

    saveConnectInfoTofile(remember_settings);
    global_data->saveSettings();

    NetworkClientHelper::getInstance()->connectJsonrpc(CONNECTION_MODE::GET_TEMPLATE);
}

void ConnectDeviceWindow::handleUpdateTemplate(QString file_full_name)
{
    QStringList strList = file_full_name.split('/');
    QString file_name = strList.at(1);
    shared_ptr<GlobalData> global_data = GlobalData::getInstance();
    bool remember_settings = true == remember_checkbox->isChecked();

    template_combo_box->clear();
    for (int i = 0; i < strFileNameList.size(); i++) {
        if (file_name == strFileNameList.at(i)) {
            strFileNameList.removeAt(i);
            break;
        }
    }
    strFileNameList.prepend(file_name);
    for (int i = 0; i < strFileNameList.size(); i++) {
        template_combo_box->addItem(strFileNameList.at(i));
    }
    saveConnectInfoTofile(remember_settings);
    global_data->saveSettings();
}


