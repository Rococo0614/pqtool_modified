#include "bin_util_window.hpp"
#include "network_client_helper.hpp"
#include "raw_image_format_dialog.hpp"
#include "utils.hpp"

#include <QLabel>
#include <QCloseEvent>
#include <QFileDialog>
#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include <QMessageBox>
#include <QDateTime>

const int MIN_WINDOW_WIDTH = 720;
const int MIN_WINDOW_HEIGHT = 560;


BinUtilWindow::BinUtilWindow(QWidget *parent) :
    QMainWindow(parent)
{
    setWindowTitle(tr("Bin Utility Tool"));
    setWindowFlags(Qt::WindowCloseButtonHint);
    setFixedSize(QSize(MIN_WINDOW_WIDTH, MIN_WINDOW_HEIGHT));

    initMenu();
    initUI();
    initEvent();
}

void BinUtilWindow::initMenu()
{
}

void BinUtilWindow::initEvent()
{
    shared_ptr<NetworkClientHelper> network_helper = NetworkClientHelper::getInstance();
    connect(network_helper.get(), &NetworkClientHelper::sigBinUtilData, this, &BinUtilWindow::handleReceiveBinFile);
    connect(network_helper.get(), &NetworkClientHelper::sigBinUtilStatus, this, &BinUtilWindow::handleReceiveBinMessage);
}

void BinUtilWindow::refresh()
{

}

void BinUtilWindow::initUI()
{
    main_widget = new QWidget();
    main_layout = new QVBoxLayout;

    import_export_group = createImportExportGroupbox();
    QGroupBox *bin_mode = setBinFileModeGroupbox();
    bin_fix_group = createBinFixGroupbox();

    QPushButton *close_button = new QPushButton("Close");
    close_button->setMaximumWidth(100);
    connect(close_button, &QPushButton::clicked, this, &BinUtilWindow::close);

    QHBoxLayout *bottom_layout = new QHBoxLayout;
    bottom_layout->addWidget(close_button);

    main_layout->addWidget(import_export_group);
    main_layout->addWidget(bin_mode);
    main_layout->addWidget(bin_fix_group);
    main_layout->addLayout(bottom_layout);

    setCentralWidget(main_widget);
    centralWidget()->setLayout(main_layout);
}

QGroupBox* BinUtilWindow::setBinFileModeGroupbox()
{
    QGroupBox *group = new QGroupBox(tr("Bin File"));
    QHBoxLayout *mode_layout = new QHBoxLayout();
    QString soc = GlobalData::getInstance()->getCvipqToolSoc();

    QLabel *bin_type_label = new QLabel(tr("bin type:"));
    bin_type_label->setToolTip("The other items can genenrate independent bin file except all item.");
    bin_type_combox = new QComboBox();
    bin_type_combox->setFixedSize(120, 30);
    bin_type_combox->setToolTip("The other items can genenrate independent bin file except all item.");
    QStringList strBinNameTypeList = {"All", "Sensor_0", "Sensor_1", "Sensor_2", "Sensor_3", "Vpss", "Vdec", "Venc", "Vo"};
    if (soc == CV186X_SOC) {
        QStringList sensorList = {"Sensor_4", "Sensor_5", "Sensor_6", "Sensor_7"};
        strBinNameTypeList.pop_back();
        for (int i = 0; i < 4; i++) {
            strBinNameTypeList.insert(5 + i, sensorList.at(i));
        }
        strBinNameTypeList.append("Vo_0");
        strBinNameTypeList.append("Vo_1");
    }

    for (int i = 0; i < strBinNameTypeList.size(); i++) {
        bin_type_combox->addItem(strBinNameTypeList.at(i));
    }

    mode_layout->addWidget(bin_type_label);
    mode_layout->addWidget(bin_type_combox);
    mode_layout->addStretch(1);

    group->setLayout(mode_layout);

    return group;
}

QGroupBox* BinUtilWindow::createImportExportGroupbox()
{
    QPushButton *import_btn = new QPushButton(tr("Import Bin File"));
    QPushButton *export_btn = new QPushButton(tr("Export Bin File"));
    QHBoxLayout *layout = new QHBoxLayout;
    layout->setStretch(0, 1);
    layout->addWidget(import_btn);
    layout->addWidget(export_btn);

    QGroupBox *groupbox = new QGroupBox(tr("Bin Import Export"));
    groupbox->setLayout(layout);

    connect(import_btn, &QPushButton::clicked, this, &BinUtilWindow::handleImportClick);
    connect(export_btn, &QPushButton::clicked, this, &BinUtilWindow::handleExportClick);

    return groupbox;
}


QGroupBox* BinUtilWindow::createBinFixGroupbox()
{
    QGridLayout *layout = new QGridLayout;

    QLabel *author = new QLabel(tr("Author"));
    author->setMinimumWidth(50);
    author_edit = new QLineEdit;
    author_edit->setMaximumWidth(300);
    author_edit->setText(QDir::home().dirName());

    QLabel *desp = new QLabel(tr("Description"));
    desp->setMinimumWidth(50);
    desp_edit = new QTextEdit;

    QLabel *time = new QLabel(tr("Time"));
    time->setMinimumWidth(50);
    time_edit = new QLineEdit;
    time_edit->setMaximumWidth(300);
    QPushButton *get_time_btn = new QPushButton("Get TimeStamp");
    get_time_btn->setMinimumWidth(50);
    QHBoxLayout *h_layout = new QHBoxLayout;
    h_layout->addWidget(time_edit);
    h_layout->addWidget(get_time_btn);
    connect(get_time_btn, &QPushButton::clicked, this, &BinUtilWindow::handleGetTime);

    QPushButton *bin_fix_btn = new QPushButton(tr("Fix Bin to Flash"));
    bin_fix_btn->setMaximumWidth(230);
    connect(bin_fix_btn, &QPushButton::clicked, this, &BinUtilWindow::handleBinFixClick);

    layout->addWidget(author, 0, 0);
    layout->addWidget(author_edit, 0, 1);
    layout->addWidget(desp, 1, 0);
    layout->addWidget(desp_edit, 1, 1);
    layout->addWidget(time, 2, 0);
    layout->addLayout(h_layout, 2, 1);
    layout->addWidget(bin_fix_btn, 3, 1);

    QGroupBox *group = new QGroupBox(tr("BIN Fix"));
    group->setLayout(layout);

    return group;
}

void BinUtilWindow::convertBinToCFiles(QByteArray &data, QString path)
{
    if (data.isEmpty() || path.isEmpty()) {
        return;
    }

    QFileInfo info(path);
    QString fileName = info.fileName();
    int lastDotPos = fileName.lastIndexOf('.');
    if (lastDotPos != -1) {
        fileName = fileName.left(lastDotPos);
    }
    QString fileNameLower = fileName.toLower();
    QString c_file_path = info.absolutePath() +  QString("/%1_param.c").arg(fileNameLower);
    QString h_file_path = info.absolutePath() + QString("/%1_param.h").arg(fileNameLower);

    QString c_file_str = QString("#include \"%1_param.h\"\n").arg(fileNameLower);
    c_file_str += QString("\nunsigned char cvi_%1_pq_param[] = {\n").arg(fileNameLower);
    for (int i = 0; i < data.size(); i++) {
        c_file_str += "0x" + QString::number((unsigned char)data[i], 16).toUpper();
        if (i  == data.size() - 1) {
            c_file_str += "\n";
            break;
        }
        c_file_str += ", ";
        if ((i != 0) && ((i + 1) % 50 == 0)) {
            c_file_str += "\n";
        }
    }
    c_file_str += "};\n";

    QFile c_file(c_file_path);
    c_file.open(QIODevice::WriteOnly);
    c_file.write(c_file_str.toStdString().c_str());
    c_file.close();

    QString h_file_str = QString("#ifndef _%1_PARAM_H_\n").arg(fileName.toUpper());
    h_file_str += QString("#define _%1_PARAM_H_\n").arg(fileName.toUpper());
    h_file_str += QString("\n#define BIN_DATA_SIZE %1\n").arg(data.size());
    h_file_str += QString("\nextern unsigned char cvi_%1_pq_param[];\n").arg(fileNameLower);
    h_file_str += "\n#endif\n";

    QFile h_file(h_file_path);
    h_file.open(QIODevice::WriteOnly);
    h_file.write(h_file_str.toStdString().c_str());
    h_file.close();
}

void BinUtilWindow::handleBinFixClick()
{
    if (author_edit->text().isEmpty()) {
        QMessageBox::warning(this, tr("Warning"), tr("Please Input Author Name!"), QMessageBox::Ok);
        return;
    }

    if (desp_edit->toPlainText().isEmpty()) {
        QMessageBox::warning(this, tr("Warning"), tr("Please Input Description!"), QMessageBox::Ok);
        return;
    }

    if (time_edit->text().isEmpty()) {
        QMessageBox::warning(this, tr("Warning"), tr("Please Input Timestamp!"), QMessageBox::Ok);
        return;
    }

    if (!NetworkClientHelper::getInstance()->isNormalStatus()) {
        QMessageBox::warning(this, tr("Warning"), tr("Network is not connected!"), QMessageBox::Ok);
        return;
    }

    QJsonObject id_info;
    id_info[MSG_KEY_CMD] = CMD_BINUTIL_FIX_BIN_TO_FLASH;
    int id = GlobalData::getInstance()->getJsonrpcId();
    NetworkClientHelper::getInstance()->pushIdInfoJsonrpc(id, id_info);

    QJsonObject param_obj;
    param_obj["author"] = author_edit->text();
    param_obj["desp"] = desp_edit->toPlainText();
    param_obj["time"] = time_edit->text();
    param_obj["version"] = CVIPQ_TOOL_VERSION;
    QJsonObject send_obj;
    send_obj[MSG_KEY_METHOD] = METHOD_FIX_BIN_TO_FLASH;
    send_obj[MSG_KEY_ID] = id;
    send_obj[MSG_KEY_JSONRPC] = MSG_JSONRPC_VERSION;
    send_obj[MSG_KEY_PARAMS] = param_obj;
    NetworkClientHelper::getInstance()->sendCmd(std::move(send_obj));
}

void BinUtilWindow::handleImportClick()
{
    binPath = QFileDialog::getOpenFileName(this, tr("Open bin file"), "", tr("*.bin"));
    if (binPath.isEmpty()) {
        return;
    }

    QFile bin_file(binPath);
    bin_file.open(QIODevice::ReadOnly);
    QByteArray data = bin_file.readAll();
    bin_file.close();

    convertBinToCFiles(data, binPath);

    if (!NetworkClientHelper::getInstance()->isNormalStatus()) {
        QMessageBox::warning(this, tr("Warning"), tr("Network is not connected!"), QMessageBox::Ok);
        return;
    }

    QJsonObject id_info;
    id_info[MSG_KEY_CMD] = CMD_BINUTIL_IMPORT_BIN;
    int id = GlobalData::getInstance()->getJsonrpcId();
    NetworkClientHelper::getInstance()->pushIdInfoJsonrpc(id, id_info);

    QJsonObject param_obj;
    param_obj["content"] = TUNING_BIN_DATA;
    param_obj["size"] = data.size();
    QJsonObject send_obj;
    send_obj[MSG_KEY_METHOD] = METHOD_SEND_BIN_INFO;
    send_obj[MSG_KEY_ID] = id;
    send_obj[MSG_KEY_JSONRPC] = MSG_JSONRPC_VERSION;
    send_obj[MSG_KEY_PARAMS] = param_obj;
    NetworkClientHelper::getInstance()->sendCmd(std::move(send_obj));
}

void BinUtilWindow::handleExportClick()
{
    if (author_edit->text().isEmpty()) {
        QMessageBox::warning(this, tr("Warning"), tr("Please Input Author Name!"), QMessageBox::Ok);
        return;
    }

    if (desp_edit->toPlainText().isEmpty()) {
        QMessageBox::warning(this, tr("Warning"), tr("Please Input Description!"), QMessageBox::Ok);
        return;
    }

    if (time_edit->text().isEmpty()) {
        QMessageBox::warning(this, tr("Warning"), tr("Please Input Timestamp!"), QMessageBox::Ok);
        return;
    }

    if (!NetworkClientHelper::getInstance()->isNormalStatus()) {
        QMessageBox::warning(this, tr("Warning"), tr("Network is not connected!"), QMessageBox::Ok);
        return;
    }

    NetworkClientHelper::getInstance()->setCallerInfo(0, NULL);

    QJsonObject id_info;
    id_info[MSG_KEY_CMD] = CMD_BINUTIL_EXPORT_BIN;
    int id = GlobalData::getInstance()->getJsonrpcId();
    NetworkClientHelper::getInstance()->pushIdInfoJsonrpc(id, id_info);

    QJsonObject param_obj;
    param_obj["author"] = author_edit->text();
    param_obj["desp"] = desp_edit->toPlainText();
    param_obj["time"] = time_edit->text();
    param_obj["version"] = CVIPQ_TOOL_VERSION;
    param_obj["bin_type"] = bin_type_combox->currentText();

    QJsonObject send_obj;
    send_obj[MSG_KEY_METHOD] = METHOD_EXPORT_BIN;
    send_obj[MSG_KEY_ID] = id;
    send_obj[MSG_KEY_JSONRPC] = MSG_JSONRPC_VERSION;
    send_obj[MSG_KEY_PARAMS] = param_obj;
    NetworkClientHelper::getInstance()->sendCmd(std::move(send_obj));
}

void BinUtilWindow::handleReceiveBinMessage(int id)
{
    qDebug("CaptureWindow::handleReceiveBinMessage\n");
    NetworkClientHelper::getInstance()->setCallerInfo(0, NULL);

    QJsonObject obj = NetworkClientHelper::getInstance()->popCmdDataJsonrpc(id);
    QJsonObject result_obj = obj[MSG_KEY_RESULT].toObject();
    QJsonObject info_obj = obj[MSG_KEY_ID_INFO].toObject();
    QString cmd = info_obj[MSG_KEY_CMD].toString();

    if (result_obj.isEmpty()) {
        QJsonObject error_obj = obj[MSG_KEY_ERROR].toObject();
        QVariant code = error_obj[MSG_KEY_CODE].toVariant();
        QString message = error_obj[MSG_KEY_MESSAGE].toString();
        QString log;

        if (cmd == CMD_BINUTIL_EXPORT_BIN) {
            log = "Export bin failed: ";
        } else if (cmd == CMD_BINUTIL_IMPORT_BIN) {
            log = "Import bin failed: ";
        } else if (cmd == CMD_BINUTIL_SEND_BIN_DATA) {
            log = "Send bin failed: ";
        } else if (cmd == CMD_BINUTIL_FIX_BIN_TO_FLASH) {
            log = "Fix bin to flash failed: ";
        }

        log = log + message + " ,errorcode = " + code.toString();
        QMessageBox::warning(this, tr("Error"), log, QMessageBox::Ok);
        return;
    }

    if (cmd == CMD_BINUTIL_EXPORT_BIN) {
        QJsonObject params_obj = result_obj[MSG_KEY_PARAMS].toObject();
        int size = params_obj["Data size"].toInt();
        NetworkClientHelper::getInstance()->setCallerInfo(size, CMD_BINUTIL_EXPORT_BIN);
        qDebug("Set receive size:%d\n", size);

        int temp_id = GlobalData::getInstance()->getJsonrpcId();
        QJsonObject param_obj;
        param_obj["content"] = TUNING_BIN_DATA;
        QJsonObject send_obj;
        send_obj[MSG_KEY_METHOD] = METHOD_GET_BIN_DATA;
        send_obj[MSG_KEY_ID] = temp_id;
        send_obj[MSG_KEY_JSONRPC] = MSG_JSONRPC_VERSION;
        send_obj[MSG_KEY_PARAMS] = param_obj;
        NetworkClientHelper::getInstance()->sendCmd(std::move(send_obj));
    } else if (cmd == CMD_BINUTIL_IMPORT_BIN) {
        if (!binPath.isEmpty()) {
            QFile bin_file(binPath);
            bin_file.open(QIODevice::ReadOnly);
            QByteArray data = bin_file.readAll();
            bin_file.close();

            NetworkClientHelper::getInstance()->setCallerInfo(0, CMD_BINUTIL_SEND_BIN_DATA);
            NetworkClientHelper::getInstance()->sendData(data);
        } else {
            QMessageBox::information(this, tr("Info"), tr("Bin path is empty!"), QMessageBox::Ok);
        }
    } else if (cmd == CMD_BINUTIL_SEND_BIN_DATA) {
        QMessageBox::information(this, tr("Info"), tr("Import bin  successfully!"), QMessageBox::Ok);
    } else if (cmd == CMD_BINUTIL_FIX_BIN_TO_FLASH) {
        QMessageBox::information(this, tr("Info"), tr("Fix bin to flash successfully!"), QMessageBox::Ok);
    }
}

void BinUtilWindow::handleReceiveBinFile(QString cmd)
{
    NetworkClientHelper::getInstance()->setCallerInfo(0, NULL);
    QByteArray data = NetworkClientHelper::getInstance()->popCmdData(cmd);

    if (!data.isEmpty()) {
        QString module_name = bin_type_combox->currentText();
        QString filePath = QFileDialog::getSaveFileName(this,
                            QString("Save %1 bin file").arg(module_name), module_name,
                            tr("*.bin"));

        if (filePath.isEmpty()) {
            return;
        }

        QFile bin_file(filePath);
        bin_file.open(QIODevice::WriteOnly);
        bin_file.write(data);
        bin_file.close();

        convertBinToCFiles(data, filePath);

        QMessageBox::information(this, tr("Info"), tr("Export bin successfully!"), QMessageBox::Ok);
    } else {
        QMessageBox::information(this, tr("Error"), tr("Export bin failed!"), QMessageBox::Ok);
    }
}

void BinUtilWindow::closeEvent(QCloseEvent *event)
{
    event->accept();
    QMainWindow::closeEvent(event);
}

void BinUtilWindow::handleGetTime()
{
    QString dt_str = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    time_edit->setText(dt_str);
}

void BinUtilWindow::keyReleaseEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape) {
        this->close();
    }
}
