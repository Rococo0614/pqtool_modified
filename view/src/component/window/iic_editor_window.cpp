#include "iic_editor_window.hpp"
#include "utils.hpp"
#include "value_validator.hpp"
#include "global_data.hpp"
#include "main_window.hpp"
#include <windows.h>

#include <QLabel>
#include <QCloseEvent>
#include <QFileDialog>
#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include <QMessageBox>
#include <QShortcut>
#include <QPainter>

const int MIN_WINDOW_WIDTH = 960;
const int MIN_WINDOW_HEIGHT = 720;

IICEditorWindow::IICEditorWindow(QWidget *parent) :
    QMainWindow(parent)
{
    setWindowTitle(tr("I2C Editor"));
    setWindowFlags(Qt::WindowMaximizeButtonHint|Qt::WindowCloseButtonHint);

    timer = new QTimer(this);
    initMenu();
    initUI();
    initEvent();
}

void IICEditorWindow::initMenu()
{
}

void IICEditorWindow::initUI()
{
    main_widget = new QWidget();
    main_widget->setMinimumSize(QSize(MIN_WINDOW_WIDTH, MIN_WINDOW_HEIGHT));
    main_layout = new QVBoxLayout(main_widget);

    device_info_group = createDeviceInfoGroupbox();

    QGroupBox * rw_group = new QGroupBox();
    QHBoxLayout *rw_layout = new QHBoxLayout(rw_group);
    rw_group->setMaximumSize(960, 80);
    QLabel *start_label = new QLabel("Start: 0x", this);
    QLabel *length_label = new QLabel("Length: 0x", this);
    QRegExp regx("[A-Fa-f0-9]{1,}");
    QValidator *validator = new QRegExpValidator(regx);
    start_edit = new QLineEdit(this);
    start_edit->setValidator(validator);
    connect(start_edit, &QLineEdit::textChanged, this, &IICEditorWindow::clearWidget);
    length_edit = new QLineEdit(this);
    length_edit->setValidator(validator);
    connect(length_edit, &QLineEdit::textChanged, this, &IICEditorWindow::clearWidget);
    show_btn = new QPushButton("Show", this);
    read_btn = new QPushButton("Read", this);
    read_btn->setEnabled(false);
    write_btn = new QPushButton("Write", this);
    write_btn->setEnabled(false);
    rw_layout->addWidget(start_label,0);
    rw_layout->addWidget(start_edit,0);
    rw_layout->addWidget(length_label,0);
    rw_layout->addWidget(length_edit,0);
    rw_layout->addWidget(show_btn,0);
    rw_layout->addStretch(1);
    rw_layout->addWidget(read_btn,0);
    rw_layout->addWidget(write_btn,0);

    connect(show_btn, &QPushButton::clicked, this, &IICEditorWindow::handleShow);
    connect(read_btn, &QPushButton::clicked, this, &IICEditorWindow::handleRead);
    connect(write_btn, &QPushButton::clicked, this, &IICEditorWindow::handleWrite);

    show_widget = new QWidget();
    show_layout = new QVBoxLayout(show_widget);
    show_layout->setAlignment(Qt::AlignLeft | Qt::AlignTop);

    QScrollArea *scrol = new QScrollArea(this);
    scrol->setMinimumSize(960, 500);
    scrol->setWidgetResizable(true);
    scrol->setWidget(show_widget);
    scrol->setStyleSheet("background-color:rgb(255, 255, 255);");

    main_layout->addWidget(device_info_group, Qt::AlignTop);
    main_layout->addWidget(rw_group, Qt::AlignTop);
    main_layout->addWidget(scrol, Qt::AlignTop);

    setCentralWidget(main_widget);
}

QGroupBox *IICEditorWindow::createDeviceInfoGroupbox()
{
    QGroupBox * groupbox = new QGroupBox();
    groupbox->setTitle("I2C Device Information");
    groupbox->setMaximumSize(960, 150);
    QGridLayout *device_layout = new QGridLayout(groupbox);

    QLabel *id_label = new QLabel("Device ID: 0x", this);
    QLabel *addr_label = new QLabel("Device Address: 0x", this);
    QLabel *abytes_label = new QLabel("Address Bytes:", this);
    QLabel *dbytes_label = new QLabel("Data Bytes:", this);
    QRegExp regx("[A-Fa-f0-9]{1,}");
    QValidator *validator = new QRegExpValidator(regx);
    id_edit = new QLineEdit(this);
    id_edit->setValidator(validator);
    addr_edit = new QLineEdit(this);
    addr_edit->setValidator(validator);
    QStringList str_list = {"1 Byte", "2 Byte", "4 Byte"};
    abytes_box = new QComboBox(this);
    abytes_box->addItems(str_list);
    abytes_box->setCurrentIndex(0);
    dbytes_box = new QComboBox(this);
    dbytes_box->addItems(str_list);
    dbytes_box->setCurrentIndex(0);
    connect(abytes_box, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &IICEditorWindow::clearWidget);
    connect(dbytes_box, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &IICEditorWindow::clearWidget);

    load_btn = new QPushButton("Load", this);
    connect(load_btn, &QPushButton::clicked, this, &IICEditorWindow::handleLoadData);
    save_btn = new QPushButton("Save", this);
    connect(save_btn, &QPushButton::clicked, this, &IICEditorWindow::handleSaveData);

    device_layout->addWidget(id_label, 0, 0, Qt::AlignRight);
    device_layout->addWidget(id_edit, 0, 1, Qt::AlignLeft);
    device_layout->addWidget(abytes_label, 0, 2, Qt::AlignRight);
    device_layout->addWidget(abytes_box, 0, 3, Qt::AlignLeft);
    device_layout->addWidget(load_btn, 0, 4, Qt::AlignRight);
    device_layout->addWidget(addr_label, 1, 0, Qt::AlignRight);
    device_layout->addWidget(addr_edit, 1, 1, Qt::AlignLeft);
    device_layout->addWidget(dbytes_label, 1, 2, Qt::AlignRight);
    device_layout->addWidget(dbytes_box, 1, 3, Qt::AlignLeft);
    device_layout->addWidget(save_btn, 1, 4, Qt::AlignRight);

    return groupbox;
}

void IICEditorWindow::initEvent()
{
    connect(timer, &QTimer::timeout, this, &IICEditorWindow::handleTimeOut);
    connect(NetworkClientHelper::getInstance().get(), &NetworkClientHelper::sigI2CReadStatus, this,
            &IICEditorWindow::handleReadStatus);
    connect(NetworkClientHelper::getInstance().get(), &NetworkClientHelper::sigI2CWriteStatus, this,
            &IICEditorWindow::handleWriteStatus);
}

void IICEditorWindow::handleTimeOut()
{
    timer->stop();
    enableComponents();
    QMessageBox::warning(this, "warning", "Timeout, can not get response from board!", QMessageBox::Ok);
}

void IICEditorWindow::handleLoadData()
{
    QString file_path = QFileDialog::getOpenFileName(this, tr("Import I2C Data"), "", "*.json");
    if (file_path.isEmpty()) {
        return;
    }

    QJsonObject data;
    QString error_msg;
    if (utils::loadJson(file_path, data, error_msg) != 0) {
        QMessageBox::warning(this, "warning", error_msg, QMessageBox::Ok);
        return;
    }

    if (data.contains("Device ID")) {
        int id = data["Device ID"].toInt(0);
        id_edit->setText(QString::number(id, 16).toLower());
    }

    if (data.contains("Device Address")) {
        int addr = data["Device Address"].toInt(0);
        addr_edit->setText(QString::number(addr, 16).toLower());
    }

    if (data.contains("Address Bytes")) {
        int abyte = data["Address Bytes"].toInt(0);
        abytes_box->setCurrentIndex(abyte);
    }

    if (data.contains("Data Bytes")) {
        int dbyte = data["Data Bytes"].toInt(0);
        dbytes_box->setCurrentIndex(dbyte);
    }
}

void IICEditorWindow::handleSaveData()
{
    QString filePath = QFileDialog::getSaveFileName(this, tr("Save I2C Data"), "", tr("Json files (*.json)"));
    if (filePath.isEmpty()) {
        return;
    }

    QJsonObject data;
    data["Device ID"] = id_edit->text().toInt(nullptr, 16);
    data["Device Address"] = addr_edit->text().toInt(nullptr, 16);
    data["Address Bytes"] = abytes_box->currentIndex();
    data["Data Bytes"] = dbytes_box->currentIndex();

    QFile saveFile(filePath);
    if (!saveFile.open(QIODevice::WriteOnly)) {
        QString msg = "Couldn't open save file " + filePath;
        QMessageBox::warning(this, "warning", msg, QMessageBox::Ok);
        return;
    }

    QJsonDocument saveDoc(data);
    saveFile.write(saveDoc.toJson());
    saveFile.close();
}

void IICEditorWindow::handleShow()
{
    int abytes = qPow(2, abytes_box->currentIndex());
    int dbytes = qPow(2, dbytes_box->currentIndex());
    int start = start_edit->text().toInt(nullptr, 16);
    int length = length_edit->text().toInt(nullptr, 16);

    QString mesg;

    if (start_edit->text().isEmpty()) {
        QMessageBox::warning(this, tr("Warning"), tr("Start Address error!"), QMessageBox::Ok);
        return;
    }

    if (length_edit->text().isEmpty()) {
        QMessageBox::warning(this, tr("Warning"), tr("Length error!"), QMessageBox::Ok);
        return;
    }

    if ((start + length) > qPow(2, abytes * 8)) {
        mesg = "The address range is out of limit!\n" + QString("%1 Byte max is %2 ").arg(abytes).arg(qPow(2, abytes * 8));
        QMessageBox::warning(this, "warning", mesg, QMessageBox::Ok);
        return;
    }

    if (start % dbytes) {
        mesg = QString("The address is not aligned by %1 bytes!").arg(dbytes);
        QMessageBox::warning(this, "warning", mesg, QMessageBox::Ok);
        return;
    }

    if (length % dbytes) {
        mesg = QString("The length is not aligned by %1 bytes!").arg(dbytes);
        QMessageBox::warning(this, "warning", mesg, QMessageBox::Ok);
        return;
    }

    if (length > 1024) {
        mesg = "You can only show 1024 I2C items at most!";
        QMessageBox::warning(this, "warning", mesg, QMessageBox::Ok);
        return;
    }

    if (length <= 0) {
        mesg = "Length must greater than 0!";
        QMessageBox::warning(this, "warning", mesg, QMessageBox::Ok);
        return;
    }

    clearWidget();

    for (int i = 0; i < length/dbytes; i++) {
        RegisterEdit *item = new RegisterEdit("", this);
        item->setEditProperty("0x", 16, dbytes * 2);
        int addr = start_edit->text().toInt(nullptr, 16) + i * dbytes;
        QString addr_str = QString::number(addr, 16);
        QString title = "0x";
        title.resize(abytes * 2 - addr_str.size() + 2, '0');
        title += addr_str;
        item->setTitle(title);
        QString data_str;
        data_str.resize(dbytes * 2, '0');
        item->setData(data_str);
        reg_list.append(item);
    }

    QList<QHBoxLayout *> layout_list;
    int row = reg_list.size() / 8 + 1;
    for (int i = 0; i < row; i++) {
        QHBoxLayout *layout = new QHBoxLayout();
        layout->setContentsMargins(0, 0, 0, 0);
        layout->setSpacing(0);
        layout->setAlignment(Qt::AlignLeft | Qt::AlignTop);
        layout_list.append(layout);
    }
    for (int i = 0; i < reg_list.size(); i++) {
        layout_list[i/8]->addWidget(reg_list[i], Qt::AlignLeft);
    }
    if (reg_list.size() % 8) {
        layout_list[row - 1]->addStretch(8 - reg_list.size() % 8);
    }
    for (int i = 0; i < row; i++) {
        show_layout->addLayout(layout_list[i]);
    }

    read_btn->setEnabled(true);
    write_btn->setEnabled(true);
}

void IICEditorWindow::handleRead()
{
    if (!NetworkClientHelper::getInstance()->isNormalStatus()) {
        QMessageBox::warning(this, tr("Warning"), tr("Network is not connected!"), QMessageBox::Ok);
        return;
    }

    if (id_edit->text().isEmpty()) {
        QMessageBox::warning(this, tr("Warning"), tr("Device ID error!"), QMessageBox::Ok);
        return;
    }

    if (addr_edit->text().isEmpty()) {
        QMessageBox::warning(this, tr("Warning"), tr("Device Address error!"), QMessageBox::Ok);
        return;
    }

    disableComponents();

    NetworkClientHelper::getInstance()->setCallerInfo(0, NULL);

    QJsonObject id_info;
    id_info[MSG_KEY_CMD] = CMD_I2C_READ;
    int temp_id = GlobalData::getInstance()->getJsonrpcId();
    NetworkClientHelper::getInstance()->pushIdInfoJsonrpc(temp_id, id_info);

    QJsonObject param_obj;
    param_obj["Device ID"] = id_edit->text().toInt(nullptr, 16);
    param_obj["Device Address"] = addr_edit->text().toInt(nullptr, 16);
    param_obj["Address Bytes"] = qPow(2, abytes_box->currentIndex());
    param_obj["Data Bytes"] = qPow(2, dbytes_box->currentIndex());
    param_obj["Start"] = start_edit->text().toInt(nullptr, 16);
    param_obj["Length"] = length_edit->text().toInt(nullptr, 16);
    QJsonObject send_obj;
    send_obj[MSG_KEY_METHOD] = METHOD_I2C_READ;
    send_obj[MSG_KEY_ID] = temp_id;
    send_obj[MSG_KEY_JSONRPC] = MSG_JSONRPC_VERSION;
    send_obj[MSG_KEY_PARAMS] = param_obj;
    NetworkClientHelper::getInstance()->sendCmd(std::move(send_obj), false);
    timer->start(5000);
}

void IICEditorWindow::handleReadStatus(int id)
{
    timer->stop();
    enableComponents();

    QJsonObject obj = NetworkClientHelper::getInstance()->popCmdDataJsonrpc(id);
    QJsonObject result_obj = obj[MSG_KEY_RESULT].toObject();
    if (result_obj.isEmpty()) {
        QMessageBox::warning(this, tr("warning"), tr("I2C read failed!"), QMessageBox::Ok);
        return;
    }

    QJsonObject params_obj = result_obj[MSG_KEY_PARAMS].toObject();
    QJsonArray data = params_obj["I2C Data"].toArray();

    int dBytes = qPow(2, dbytes_box->currentIndex());
    uint item_data = 0;
    for (int i = 0; i < data.size(); i++) {
        item_data <<= 8;
        uchar c = data[i].toInt();
        item_data += c;
        if ((i + 1) % dBytes == 0) {
            if (reg_list[i/dBytes]) {
                reg_list[i/dBytes]->setData(QString::number(item_data,16).toLower());
                item_data = 0;
            } else {
                break;
            }
        }
    }

    QMessageBox::information(this, tr("Info"), tr("I2C read success!"), QMessageBox::Ok);
}

void IICEditorWindow::handleWrite()
{
    if (!NetworkClientHelper::getInstance()->isNormalStatus()) {
        QMessageBox::warning(this, tr("Warning"), tr("Network is not connected!"), QMessageBox::Ok);
        return;
    }

    if (id_edit->text().isEmpty()) {
        QMessageBox::warning(this, tr("Warning"), tr("Device ID error!"), QMessageBox::Ok);
        return;
    }

    if (addr_edit->text().isEmpty()) {
        QMessageBox::warning(this, tr("Warning"), tr("Device Address error!"), QMessageBox::Ok);
        return;
    }

    disableComponents();

    NetworkClientHelper::getInstance()->setCallerInfo(0, NULL);

    QJsonObject id_info;
    id_info[MSG_KEY_CMD] = CMD_I2C_WRITE;
    int temp_id = GlobalData::getInstance()->getJsonrpcId();
    NetworkClientHelper::getInstance()->pushIdInfoJsonrpc(temp_id, id_info);

    QJsonObject param_obj;
    param_obj["Device ID"] = id_edit->text().toInt(nullptr, 16);
    param_obj["Device Address"] = addr_edit->text().toInt(nullptr, 16);
    param_obj["Address Bytes"] = qPow(2, abytes_box->currentIndex());
    param_obj["Data Bytes"] = qPow(2, dbytes_box->currentIndex());
    param_obj["Start"] = start_edit->text().toInt(nullptr, 16);
    param_obj["Length"] = length_edit->text().toInt(nullptr, 16);
    QJsonArray data_array;
    int base;
    int dBytes = qPow(2, dbytes_box->currentIndex());
    for (int i = 0; i < reg_list.size(); i++) {
        reg_list[i]->getEditProperty(nullptr, &base, nullptr);
        uint data = reg_list[i]->getData().toInt(nullptr, base);
        for (int j = dBytes - 1; j >= 0; j--) {
            data_array.append(uchar((data >> (j * 8)) & 0xff));
        }
    }
    param_obj["I2C Data"] = data_array;
    QJsonObject send_obj;
    send_obj[MSG_KEY_METHOD] = METHOD_I2C_WRITE;
    send_obj[MSG_KEY_ID] = temp_id;
    send_obj[MSG_KEY_JSONRPC] = MSG_JSONRPC_VERSION;
    send_obj[MSG_KEY_PARAMS] = param_obj;
    NetworkClientHelper::getInstance()->sendCmd(std::move(send_obj), false);
    timer->start(5000);
}

void IICEditorWindow::handleWriteStatus(int id)
{
    timer->stop();
    enableComponents();

    QJsonObject obj = NetworkClientHelper::getInstance()->popCmdDataJsonrpc(id);
    QJsonObject result_obj = obj[MSG_KEY_RESULT].toObject();
    if (result_obj.isEmpty()) {
        QMessageBox::warning(this, tr("warning"), tr("I2C write failed!"), QMessageBox::Ok);
        return;
    }

    QMessageBox::information(this, tr("Info"), tr("I2C write success!"), QMessageBox::Ok);
}

void IICEditorWindow::clearWidget()
{
    if (reg_list.isEmpty()) {
        return;
    }

    read_btn->setEnabled(false);
    write_btn->setEnabled(false);
    for (int i = 0; i < reg_list.size(); i++) {
        if (reg_list[i]) {
            delete reg_list[i];
        }
    }
    reg_list.clear();
}

void IICEditorWindow::disableComponents()
{
    id_edit->setEnabled(false);
    addr_edit->setEnabled(false);
    abytes_box->setEnabled(false);
    dbytes_box->setEnabled(false);
    load_btn->setEnabled(false);
    save_btn->setEnabled(false);
    start_edit->setEnabled(false);
    length_edit->setEnabled(false);
    show_btn->setEnabled(false);
    show_widget->setEnabled(false);
    read_btn->setEnabled(false);
    write_btn->setEnabled(false);
}

void IICEditorWindow::enableComponents()
{
    id_edit->setEnabled(true);
    addr_edit->setEnabled(true);
    abytes_box->setEnabled(true);
    dbytes_box->setEnabled(true);
    load_btn->setEnabled(true);
    save_btn->setEnabled(true);
    start_edit->setEnabled(true);
    length_edit->setEnabled(true);
    show_btn->setEnabled(true);
    show_widget->setEnabled(true);
    read_btn->setEnabled(true);
    write_btn->setEnabled(true);
}
