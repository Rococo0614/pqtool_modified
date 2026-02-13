#include "spi_editor_window.hpp"
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
QList<QStringList> csn_list = {{"spidev0.0"}, {"spidev1.0"}, {"spidev2.0"}, {"spidev3.0"}};

SPIEditorWindow::SPIEditorWindow(QWidget *parent) :
    QMainWindow(parent)
{
    setWindowTitle(tr("SPI Editor"));
    setWindowFlags(Qt::WindowMaximizeButtonHint|Qt::WindowCloseButtonHint);

    timer = new QTimer(this);
    initMenu();
    initUI();
    initEvent();
}

void SPIEditorWindow::initMenu()
{
}

void SPIEditorWindow::initUI()
{
    main_widget = new QWidget();
    main_widget->setMinimumSize(QSize(MIN_WINDOW_WIDTH, MIN_WINDOW_HEIGHT));
    main_layout = new QVBoxLayout(main_widget);

    device_info_group = createDeviceInfoGroupbox();

    QRegExp regx("[0-9]{1,}");
    QValidator *validator = new QRegExpValidator(regx);
    QGroupBox * tr_group = new QGroupBox();
    QHBoxLayout *tr_layout = new QHBoxLayout(tr_group);
    tr_group->setMaximumSize(960, 80);
    QLabel *len_tx_lab = new QLabel("TX Length:", this);
    len_tx_edit = new QLineEdit(this);
    len_tx_edit->setValidator(validator);
    len_tx_edit->setMinimumWidth(100);
    connect(len_tx_edit, &QLineEdit::textChanged, this, &SPIEditorWindow::clearWidget);
    QLabel *len_rx_lab = new QLabel("RX Length:", this);
    len_rx_edit = new QLineEdit(this);
    len_rx_edit->setValidator(validator);
    len_rx_edit->setMinimumWidth(100);
    connect(len_rx_edit, &QLineEdit::textChanged, this, &SPIEditorWindow::clearWidget);
    show_btn = new QPushButton("Show", this);
    clear_btn = new QPushButton("Clear", this);
    clear_btn->setEnabled(false);
    trans_btn = new QPushButton("Transmit", this);
    trans_btn->setEnabled(false);
    tr_layout->addWidget(len_tx_lab,0);
    tr_layout->addWidget(len_tx_edit,0);
    tr_layout->addWidget(len_rx_lab,0);
    tr_layout->addWidget(len_rx_edit,0);
    tr_layout->addWidget(show_btn,0);
    tr_layout->addWidget(clear_btn,0);
    tr_layout->addStretch(1);
    tr_layout->addWidget(trans_btn,0);

    connect(show_btn, &QPushButton::clicked, this, &SPIEditorWindow::handleShow);
    connect(clear_btn, &QPushButton::clicked, this, &SPIEditorWindow::clearWidget);
    connect(trans_btn, &QPushButton::clicked, this, &SPIEditorWindow::handleTransmit);

    show_widget = new QWidget();
    show_layout = new QVBoxLayout(show_widget);
    show_layout->setAlignment(Qt::AlignLeft | Qt::AlignTop);

    QScrollArea *scrol = new QScrollArea(this);
    scrol->setMinimumSize(960, 500);
    scrol->setWidgetResizable(true);
    scrol->setWidget(show_widget);
    scrol->setStyleSheet("background-color:rgb(255, 255, 255);");

    main_layout->addWidget(device_info_group, Qt::AlignTop);
    main_layout->addWidget(tr_group, Qt::AlignTop);
    main_layout->addWidget(scrol, Qt::AlignTop);

    setCentralWidget(main_widget);
}

QGroupBox *SPIEditorWindow::createDeviceInfoGroupbox()
{
    QGroupBox * groupbox = new QGroupBox();
    groupbox->setTitle("SPI Device Information");
    groupbox->setMaximumSize(960, 150);
    QGridLayout *device_layout = new QGridLayout(groupbox);

    QLabel *num_label = new QLabel("Num:", this);
    QStringList num_list = {"SPI 0", "SPI 1", "SPI 2", "SPI 3"};
    num_box = new QComboBox(this);
    num_box->addItems(num_list);
    num_box->setCurrentIndex(0);
    num_box->setMinimumWidth(100);
    connect(num_box, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &SPIEditorWindow::spiNumChange);
    QLabel *csn_label = new QLabel("CSN:", this);
    QStringList csn_str = csn_list[0];
    csn_box = new QComboBox(this);
    csn_box->addItems(csn_str);
    csn_box->setCurrentIndex(0);
    csn_box->setMinimumWidth(100);

    QRegExp regx("[0-9]{1,}");
    QValidator *validator = new QRegExpValidator(regx);

    QLabel *mode_label = new QLabel("Mode:", this);
    QLabel *speed_label = new QLabel("Speed:", this);
    QStringList mode_list = {"SPI_MODE_0", "SPI_MODE_1", "SPI_MODE_2", "SPI_MODE_3"};
    mode_box = new QComboBox(this);
    mode_box->addItems(mode_list);
    mode_box->setCurrentIndex(0);
    mode_box->setMinimumWidth(100);
    speed_edit = new QLineEdit(this);
    speed_edit->setText("500000");
    speed_edit->setMaxLength(8);
    speed_edit->setValidator(validator);
    speed_edit->setMinimumWidth(100);
    speed_edit->setToolTip("Speed At Hz!");

    QLabel *dbytes_label = new QLabel("Data Bytes:", this);
    QStringList str_list = {"1 Byte", "2 Byte"};
    dbytes_box = new QComboBox(this);
    dbytes_box->addItems(str_list);
    dbytes_box->setCurrentIndex(0);
    dbytes_box->setMinimumWidth(100);
    connect(dbytes_box, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &SPIEditorWindow::clearWidget);

    QLabel *dorder_label = new QLabel("Data Order:", this);
    QStringList order_str = {"Big Endian", "Small Endian"};
    dorder_box = new QComboBox(this);
    dorder_box->addItems(order_str);
    dorder_box->setCurrentIndex(0);
    dorder_box->setMinimumWidth(100);


    load_btn = new QPushButton("Load", this);
    connect(load_btn, &QPushButton::clicked, this, &SPIEditorWindow::handleLoadData);
    save_btn = new QPushButton("Save", this);
    connect(save_btn, &QPushButton::clicked, this, &SPIEditorWindow::handleSaveData);

    device_layout->addWidget(num_label, 0, 0, Qt::AlignRight);
    device_layout->addWidget(num_box, 0, 1, Qt::AlignLeft);
    device_layout->addWidget(mode_label, 0, 2, Qt::AlignRight);
    device_layout->addWidget(mode_box, 0, 3, Qt::AlignLeft);
    device_layout->addWidget(dbytes_label, 0, 4, Qt::AlignRight);
    device_layout->addWidget(dbytes_box, 0, 5, Qt::AlignLeft);
    device_layout->addWidget(load_btn, 0, 6, Qt::AlignRight);
    device_layout->addWidget(csn_label, 1, 0, Qt::AlignRight);
    device_layout->addWidget(csn_box, 1, 1, Qt::AlignLeft);
    device_layout->addWidget(speed_label, 1, 2, Qt::AlignRight);
    device_layout->addWidget(speed_edit, 1, 3, Qt::AlignLeft);
    device_layout->addWidget(dorder_label, 1, 4, Qt::AlignRight);
    device_layout->addWidget(dorder_box, 1, 5, Qt::AlignLeft);
    device_layout->addWidget(save_btn, 1, 6, Qt::AlignRight);

    return groupbox;
}

void SPIEditorWindow::initEvent()
{
    connect(timer, &QTimer::timeout, this, &SPIEditorWindow::handleTimeOut);
    connect(NetworkClientHelper::getInstance().get(), &NetworkClientHelper::sigSPITransmitStatus, this,
            &SPIEditorWindow::handleTransmitStatus);
}

void SPIEditorWindow::handleTimeOut()
{
    timer->stop();
    enableComponents();
    QMessageBox::warning(this, "warning", "Timeout, can not get response from board!", QMessageBox::Ok);
}

void SPIEditorWindow::handleLoadData()
{
    QString file_path = QFileDialog::getOpenFileName(this, tr("Import SPI Data"), "", "*.json");
    if (file_path.isEmpty()) {
        return;
    }

    QJsonObject data;
    QString error_msg;
    if (utils::loadJson(file_path, data, error_msg) != 0) {
        QMessageBox::warning(this, "warning", error_msg, QMessageBox::Ok);
        return;
    }

    if (data.contains("Num")) {
        int num = data["Num"].toInt(0);
        num_box->setCurrentIndex(num);
    }

    if (data.contains("CSN")) {
        int csn = data["CSN"].toInt(0);
        csn_box->setCurrentIndex(csn);
    }

    if (data.contains("Mode")) {
        int mode = data["Mode"].toInt(0);
        mode_box->setCurrentIndex(mode);
    }

    if (data.contains("Speed")) {
        int speed = data["Speed"].toInt(0);
        speed_edit->setText(QString::number(speed));
    }

    if (data.contains("Data Bytes")) {
        int dbyte = data["Data Bytes"].toInt(0);
        dbytes_box->setCurrentIndex(dbyte);
    }

    if (data.contains("Data Order")) {
        int dorder = data["Data Order"].toInt(0);
        dorder_box->setCurrentIndex(dorder);
    }
}

void SPIEditorWindow::handleSaveData()
{
    QString filePath = QFileDialog::getSaveFileName(this, tr("Save SPI Data"), "", tr("Json files (*.json)"));
    if (filePath.isEmpty()) {
        return;
    }

    QJsonObject data;
    data["Num"] = num_box->currentIndex();
    data["CSN"] = csn_box->currentIndex();
    data["Mode"] = mode_box->currentIndex();
    data["Speed"] = speed_edit->text().toInt();
    data["Data Order"] = dorder_box->currentIndex();
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

void SPIEditorWindow::handleShow()
{
    int dbytes = qPow(2, dbytes_box->currentIndex());
    int len_tx = len_tx_edit->text().toInt();
    int len_rx = len_rx_edit->text().toInt();

    QString mesg;

    if (len_tx_edit->text().isEmpty() || len_tx <= 0) {
        QMessageBox::warning(this, tr("Warning"), tr("TX Length error!"), QMessageBox::Ok);
        return;
    }

    if (len_tx % dbytes) {
        mesg = QString("The tx length is not aligned by %1 bytes!").arg(dbytes);
        QMessageBox::warning(this, "warning", mesg, QMessageBox::Ok);
        return;
    }

    if (len_rx % dbytes) {
        mesg = QString("The rx length is not aligned by %1 bytes!").arg(dbytes);
        QMessageBox::warning(this, "warning", mesg, QMessageBox::Ok);
        return;
    }

    clearWidget();

    for (int i = 0; i < len_tx / dbytes; i++) {
        RegisterEdit *item = new RegisterEdit("", this);
        item->setEditProperty("0x", 16, dbytes * 2);
        item->setTitle(QString("tx-%1").arg(i));
        QString data_str;
        data_str.resize(dbytes * 2, '0');
        item->setData(data_str);
        reg_list_tx.append(item);
    }

    for (int i = 0; i < len_rx / dbytes; i++) {
        RegisterEdit *item = new RegisterEdit("", this);
        item->setEditProperty("0x", 16, dbytes * 2, true);
        item->setTitle(QString("rx-%1").arg(i));
        QString data_str;
        data_str.resize(dbytes * 2, '0');
        item->setData(data_str);
        reg_list_rx.append(item);
    }

    QList<QHBoxLayout *> layout_list;
    int row_tx = (reg_list_tx.size() % 8) ? (reg_list_tx.size() / 8 + 1) : (reg_list_tx.size() / 8);
    int row_rx = (reg_list_rx.size() % 8) ? (reg_list_rx.size() / 8 + 1) : (reg_list_rx.size() / 8);
    for (int i = 0; i < row_tx + row_rx; i++) {
        QHBoxLayout *layout = new QHBoxLayout();
        layout->setContentsMargins(0, 0, 0, 0);
        layout->setSpacing(0);
        layout->setAlignment(Qt::AlignLeft | Qt::AlignTop);
        layout_list.append(layout);
    }
    for (int i = 0; i < reg_list_tx.size(); i++) {
        layout_list[i/8]->addWidget(reg_list_tx[i], Qt::AlignLeft);
    }
    if (reg_list_tx.size() % 8) {
        layout_list[row_tx - 1]->addStretch(8 - reg_list_tx.size() % 8);
    }
    for (int i = 0; i < reg_list_rx.size(); i++) {
        layout_list[i/8 + row_tx]->addWidget(reg_list_rx[i], Qt::AlignLeft);
    }
    if (reg_list_rx.size() % 8) {
        layout_list[row_tx + row_rx - 1]->addStretch(8 - reg_list_rx.size() % 8);
    }

    for (int i = 0; i < row_tx + row_rx; i++) {
        show_layout->addLayout(layout_list[i]);
    }

    trans_btn->setEnabled(true);
    clear_btn->setEnabled(true);
}


void SPIEditorWindow::handleTransmitStatus(int id)
{
    timer->stop();
    enableComponents();

    QJsonObject obj = NetworkClientHelper::getInstance()->popCmdDataJsonrpc(id);
    QJsonObject result_obj = obj[MSG_KEY_RESULT].toObject();
    if (result_obj.isEmpty()) {
        QMessageBox::warning(this, tr("warning"), tr("SPI Transmit failed!"), QMessageBox::Ok);
        return;
    }

    QJsonObject params_obj = result_obj[MSG_KEY_PARAMS].toObject();
    QJsonArray data = params_obj["RX Data"].toArray();
    int dBytes = qPow(2, dbytes_box->currentIndex());
    if (data.size() != reg_list_rx.size() * dBytes) {
        QMessageBox::warning(this, tr("warning"), tr("SPI rx data error!"), QMessageBox::Ok);
        return;
    }

    for (int i = 0; i < data.size(); i += dBytes) {
        uint item_data = 0;
        for (int j = 0; j < dBytes; j++) {
            uchar c = data[i + j].toInt();
            if (dorder_box->currentIndex() == 0) {
                item_data <<= 8 * j;
                item_data += c;
            } else {
                item_data |= (c << (j * 8));
            }
        }
        if (reg_list_rx[i/dBytes]) {
            reg_list_rx[i/dBytes]->setData(QString::number(item_data,16).toLower());
        } else {
            break;
        }
    }

    QMessageBox::information(this, tr("Info"), tr("SPI Transmit success!"), QMessageBox::Ok);
}

void SPIEditorWindow::handleTransmit()
{
    if (!NetworkClientHelper::getInstance()->isNormalStatus()) {
        QMessageBox::warning(this, tr("Warning"), tr("Network is not connected!"), QMessageBox::Ok);
        return;
    }

    disableComponents();

    NetworkClientHelper::getInstance()->setCallerInfo(0, NULL);

    QJsonObject id_info;
    id_info[MSG_KEY_CMD] = CMD_SPI_TRANSMIT;
    int temp_id = GlobalData::getInstance()->getJsonrpcId();
    NetworkClientHelper::getInstance()->pushIdInfoJsonrpc(temp_id, id_info);

    QJsonObject param_obj;
    param_obj["Num"] = num_box->currentIndex();
    param_obj["CSN"] = csn_box->currentIndex();
    param_obj["Mode"] = mode_box->currentIndex();;
    param_obj["Speed"] = speed_edit->text().toInt();
    param_obj["Bits"] = qPow(2, dbytes_box->currentIndex()) * 8;
    param_obj["TX Length"] = len_tx_edit->text().toInt();
    param_obj["RX Length"] = len_rx_edit->text().toInt();

    QJsonArray data_array;
    int base;
    int dBytes = qPow(2, dbytes_box->currentIndex());
    for (int i = 0; i < reg_list_tx.size(); i++) {
        reg_list_tx[i]->getEditProperty(nullptr, &base, nullptr);
        uint data = reg_list_tx[i]->getData().toInt(nullptr, base);
        if (dorder_box->currentIndex() == 0) {
            for (int j = dBytes - 1; j >= 0; j--) {
                data_array.append(uchar((data >> (j * 8)) & 0xff));
            }
        } else {
            for (int j = 0; j < dBytes; j--) {
                data_array.append(uchar((data >> (j * 8)) & 0xff));
            }
        }
    }
    param_obj["TX Data"] = data_array;
    QJsonObject send_obj;
    send_obj[MSG_KEY_METHOD] = METHOD_SPI_TRANSMIT;
    send_obj[MSG_KEY_ID] = temp_id;
    send_obj[MSG_KEY_JSONRPC] = MSG_JSONRPC_VERSION;
    send_obj[MSG_KEY_PARAMS] = param_obj;
    NetworkClientHelper::getInstance()->sendCmd(std::move(send_obj), false);
    timer->start(5000);
}

void SPIEditorWindow::clearWidget()
{
    if (reg_list_tx.isEmpty() && reg_list_rx.isEmpty()) {
        return;
    }

    trans_btn->setEnabled(false);
    clear_btn->setEnabled(false);
    for (int i = 0; i < reg_list_tx.size(); i++) {
        if (reg_list_tx[i]) {
            delete reg_list_tx[i];
        }
    }
    reg_list_tx.clear();

    for (int i = 0; i < reg_list_rx.size(); i++) {
        if (reg_list_rx[i]) {
            delete reg_list_rx[i];
        }
    }
    reg_list_rx.clear();
}

void SPIEditorWindow::spiNumChange(int index)
{
    if (index < 0 || index > csn_list.size()) {
        return;
    }

    if (csn_list[index].empty()) {
        return;
    }

    csn_box->clear();
    csn_box->addItems(csn_list[index]);
}

void SPIEditorWindow::disableComponents()
{
    dbytes_box->setEnabled(false);
    dorder_box->setEnabled(false);
    load_btn->setEnabled(false);
    save_btn->setEnabled(false);
    len_tx_edit->setEnabled(false);
    len_rx_edit->setEnabled(false);
    show_btn->setEnabled(false);
    clear_btn->setEnabled(false);
    trans_btn->setEnabled(false);
    show_widget->setEnabled(false);
}

void SPIEditorWindow::enableComponents()
{
    dbytes_box->setEnabled(true);
    dorder_box->setEnabled(true);
    load_btn->setEnabled(true);
    save_btn->setEnabled(true);
    len_tx_edit->setEnabled(true);
    len_rx_edit->setEnabled(true);
    show_btn->setEnabled(true);
    clear_btn->setEnabled(true);
    trans_btn->setEnabled(true);
    show_widget->setEnabled(true);
}
