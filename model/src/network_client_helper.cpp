#include "network_client_helper.hpp"
#include "global_data.hpp"
#include "utils.hpp"

shared_ptr<NetworkClientHelper> NetworkClientHelper::m_instance = nullptr;

shared_ptr<NetworkClientHelper> NetworkClientHelper::getInstance()
{
    if (nullptr == NetworkClientHelper::m_instance) {
        NetworkClientHelper::m_instance = make_shared<NetworkClientHelper>();
    }

    return NetworkClientHelper::m_instance;
}

NetworkClientHelper::NetworkClientHelper()
{
    tcpsocket_.setProxy(QNetworkProxy::NoProxy);
    conn_timer = new QTimer();
    conn_timer->setSingleShot(true);
    QObject::connect(&tcpsocket_, SIGNAL(connected()), this, SLOT(handleConnectedJsonrpc()));
    QObject::connect(&tcpsocket_, SIGNAL(disconnected()), this, SLOT(handleDisconnectedJsonrpc()));
    QObject::connect(&tcpsocket_, SIGNAL(stateChanged(QAbstractSocket::SocketState)),
            this, SLOT(handleConnectStatteChanged(QAbstractSocket::SocketState)));
    QObject::connect(this, SIGNAL(connected_uart()), this, SLOT(handleConnectedJsonrpc()));
    QObject::connect(this, SIGNAL(disconnected_uart()), this, SLOT(handleDisconnectedJsonrpc()));
    QObject::connect(conn_timer, &QTimer::timeout, this, &NetworkClientHelper::handleReconnectJsonrpc);
}

NetworkClientHelper::~NetworkClientHelper()
{
    // disconnect();
    QObject::disconnect(&tcpsocket_, SIGNAL(connected()), this, SLOT(handleConnectedJsonrpc()));
    QObject::disconnect(&tcpsocket_, SIGNAL(disconnected()), this, SLOT(handleDisconnectedJsonrpc()));
    QObject::disconnect(&tcpsocket_, SIGNAL(stateChanged(QAbstractSocket::SocketState)),
            this, SLOT(handleConnectStatteChanged(QAbstractSocket::SocketState)));
    QObject::disconnect(this, SIGNAL(connected_uart()), this, SLOT(handleConnectedJsonrpc()));
    QObject::disconnect(this, SIGNAL(disconnected_uart()), this, SLOT(handleDisconnectedJsonrpc()));
    QObject::disconnect(&tcpsocket_,SIGNAL(readyRead()), this, SLOT(onSocketReadyRead()));
    QObject::disconnect(&uart_, SIGNAL(readyRead()), this, SLOT(onUartReadyRead()));
    QObject::disconnect(conn_timer, &QTimer::timeout, this, &NetworkClientHelper::handleReconnectJsonrpc);
}

void NetworkClientHelper::connect()
{
    disconnect();

    shared_ptr<GlobalData> global_data = GlobalData::getInstance();
    QString ip_address = global_data->getSettings(DEVICE_SECTION, KEY_IP_ADDRESS).toString();
    QString port = global_data->getSettings(DEVICE_SECTION, KEY_PORT).toString();

    QObject::connect(&websocket_, &QWebSocket::connected, this, &NetworkClientHelper::handleConnected);
    QObject::connect(&websocket_, &QWebSocket::disconnected, this, &NetworkClientHelper::handleDisconnected);
    QObject::connect(&websocket_, &QWebSocket::stateChanged, this, &NetworkClientHelper::handleConnectStatteChanged);
    // QObject::connect(conn_timer, &QTimer::timeout, this, &NetworkClientHelper::handleReconnect);

    websocket_.open(QUrl("ws://" + ip_address + ":" + port));
}

void NetworkClientHelper::disconnect(bool emit_log)
{
    if (QAbstractSocket::UnconnectedState == websocket_.state()) {
        return;
    }

    QObject::disconnect(&websocket_, &QWebSocket::connected, this, &NetworkClientHelper::handleConnected);
    QObject::disconnect(&websocket_, &QWebSocket::disconnected, this, &NetworkClientHelper::handleDisconnected);
    QObject::disconnect(&websocket_, &QWebSocket::stateChanged, this, &NetworkClientHelper::handleConnectStatteChanged);
    QObject::disconnect(&websocket_, &QWebSocket::textMessageReceived,
                        this, &NetworkClientHelper::onMessageReceived);
    QObject::disconnect(&websocket_, &QWebSocket::binaryFrameReceived,
                        this, &NetworkClientHelper::onFrameReceived);
    // QObject::disconnect(conn_timer, &QTimer::timeout, this, &NetworkClientHelper::handleReconnect);

    // conn_timer->stop();
    websocket_.close();
    clearData();

    if (emit_log) {
        emit sigStatusLog("Disconnect device " + GlobalData::getInstance()->getSettings(DEVICE_SECTION, KEY_IP_ADDRESS).toString() +
                          " successfully", LOG_LEVEL::LOG_INFO);
    }
}

bool NetworkClientHelper::sendCmd(QString cmd)
{
#ifndef TCP_SOCKET_CLIENT
    if (QAbstractSocket::UnconnectedState == websocket_.state()) {
        return false;
    }

    QJsonObject send_obj;
    send_obj[MSG_KEY_CMD] = cmd;
    QJsonObject data_obj;
    send_obj[MSG_KEY_DATA] = data_obj;
    qDebug("send command:%s", qUtf8Printable(utils::stringFromObject(send_obj)));
    emit sigStatusLog("send command:" + utils::stringFromObject(send_obj), LOG_LEVEL::LOG_DEBUG);
    websocket_.sendTextMessage(utils::stringFromObject(send_obj));
#else
    QString connect_type = GlobalData::getInstance()->getSettings(DEVICE_SECTION, KEY_CONNECT_TYPE).toString();

    if (connect_type == CONNECT_TYPE_NETWORK) {
        if (tcpsocket_.state() == QAbstractSocket::UnconnectedState)
            return false;
    } else if (connect_type == CONNECT_TYPE_UART) {
        if (!uart_.isOpen())
            return false;
    }
    emit sigStatusLog("Failed to send command. " + cmd + " command is not supported.", LOG_LEVEL::LOG_ERROR);
    received_data.clear();
    // tcpsocket_.write(utils::stringFromObject(send_obj).toUtf8());
#endif

    return true;
}

bool NetworkClientHelper::sendCmd(QJsonObject &&obj, bool flag)
{
#ifndef TCP_SOCKET_CLIENT
    if (QAbstractSocket::UnconnectedState == websocket_.state()) {
        return false;
    }

    qDebug("send command:%s", qUtf8Printable(utils::stringFromObject(obj)));
    emit sigStatusLog("send command:" + utils::stringFromObject(obj), LOG_LEVEL::LOG_DEBUG);
    websocket_.sendTextMessage(utils::stringFromObject(obj));
#else

    shared_ptr<GlobalData> global_data = GlobalData::getInstance();
    QString connect_type = global_data->getSettings(DEVICE_SECTION, KEY_CONNECT_TYPE).toString();

    if (connect_type == CONNECT_TYPE_NETWORK) {
        if (tcpsocket_.state() == QAbstractSocket::UnconnectedState)
            return false;
    } else if (connect_type == CONNECT_TYPE_UART) {
        if (!uart_.isOpen())
            return false;
    } else {
        return false;
    }
    QString send_data = utils::stringFromObject(obj);
    qDebug("send data size = %d", send_data.size());
    qDebug("send command:%s", qUtf8Printable(send_data));
    emit sigStatusLog(QString("send data size = %1").arg(send_data.size()), LOG_LEVEL::LOG_DEBUG);
    emit sigStatusLog("send command:" + utils::stringFromObject(obj), LOG_LEVEL::LOG_DEBUG);
    received_data.clear();

    if (connect_type == CONNECT_TYPE_NETWORK) {
        tcpsocket_.write(utils::stringFromObject(obj).toUtf8());
    } else if (connect_type == CONNECT_TYPE_UART) {
        uart_.write(utils::stringFromObject(obj).toUtf8());
    }
    GlobalData::getInstance()->setWaitForResponse(flag);
#endif

    return true;
}

bool NetworkClientHelper::sendData(QByteArray &data)
{
#ifndef TCP_SOCKET_CLIENT
    if (QAbstractSocket::UnconnectedState == websocket_.state()) {
        return false;
    }

    emit sigStatusLog("send binary data", LOG_LEVEL::LOG_DEBUG);
    websocket_.sendBinaryMessage(data);
#else

    shared_ptr<GlobalData> global_data = GlobalData::getInstance();
    QString connect_type = global_data->getSettings(DEVICE_SECTION, KEY_CONNECT_TYPE).toString();

    if (connect_type == CONNECT_TYPE_NETWORK) {
        if (tcpsocket_.state() == QAbstractSocket::UnconnectedState)
            return false;
        emit sigStatusLog("send binary data", LOG_LEVEL::LOG_DEBUG);
        tcpsocket_.write(data);
    }else if (connect_type == CONNECT_TYPE_UART) {
        if (!uart_.isOpen())
            return false;
        emit sigStatusLog("send binary data", LOG_LEVEL::LOG_DEBUG);
        uart_.write(data);
    }
#endif

    return true;
}

void NetworkClientHelper::parseCmdData(const QJsonObject &obj)
{
    QString status = obj[MSG_KEY_STATUS].toString();
    QString info = obj[MSG_KEY_INFO].toString();

    if ((status == MSG_KEY_SUCCESS) && (obj.contains(MSG_KEY_INFO))) {
        emit sigStatusLog(obj[MSG_KEY_INFO].toString(), LOG_LEVEL::LOG_INFO);
    } else if ((status == MSG_KEY_FAILURE) && (obj.contains(MSG_KEY_INFO))) {
        emit sigStatusLog(obj[MSG_KEY_INFO].toString(), LOG_LEVEL::LOG_ERROR);
    } else if ((status == MSG_KEY_WARNING) && (obj.contains(MSG_KEY_INFO))) {
        emit sigStatusLog(obj[MSG_KEY_INFO].toString(), LOG_LEVEL::LOG_WARNING);
    }

    QString cmd = obj[MSG_KEY_CMD].toString();
    QMutexLocker locker(&data_mutex);
    if (obj[MSG_KEY_DATA].isArray()) {
        cmd_data_map_[cmd] << QJsonDocument(obj[MSG_KEY_DATA].toArray()).toJson();
    } else if (obj[MSG_KEY_DATA].isObject()) {
        cmd_data_map_[cmd] << QJsonDocument(obj[MSG_KEY_DATA].toObject()).toJson();
    } else if (obj[MSG_KEY_DATA].isString()) {
        cmd_data_map_[cmd] << obj[MSG_KEY_DATA].toString().toUtf8();
    }

    locker.unlock();

    if (cmd == CMD_GET_SINGLE_YUV_DATA) {
        emit sigReceiveSingleImageData(cmd, status, info);
    } else if (cmd == CMD_GET_MULTI_YUV_DATA) {
        emit sigReceiveMultiYuvData(cmd, status);
    } else if (cmd == CMD_GET_MULTI_RAW_DATA) {
        emit sigReceiveMultiRawData(cmd, status);
    } else if (cmd == CMD_GET_BRACKETING_DATA || cmd == CMD_BRACKETING_START || cmd == CMD_BRACKETING_FINISH) {
        emit sigReceiveMultiBracketingData(cmd, status);
    } else if (cmd == CMD_EXPORT_BIN_FILE) {
        emit sigReceiveBinFile(cmd);
    } else if (cmd == CMD_IMPORT_BIN_FILE || cmd == CMD_FIX_BIN_TO_FLASH) {
        emit sigReceiveBinMessage(cmd, status);
    } else if (cmd == CMD_GET_BLACK_LEVEL) {
        emit sigReceiveBlackLvlMsg(cmd);
    } else if (cmd == CMD_READ_REG) {
        emit sigReceiveRegMsg(cmd);
    } else if (cmd == CMD_GET_3A_STATISTICS) {
        emit sigReceive3AMsg(CMD_GET_3A_STATISTICS);
    } else if (cmd == CMD_GET_AF_STATISTICS) {
        emit sigReceiveFocusAssistData(CMD_GET_AF_STATISTICS);
    } else if (cmd == CMD_GET_CHIP_INFO) {
        emit sigReceiveChipInfo(cmd);
    } else if (cmd == CMD_GET_WDR_MODE) {
        emit sigReceiveWDRMode(cmd);
    } else if (cmd == CMD_SEND_RAW_REPLAY_DATA || cmd == CMD_START_RAW_REPLAY || cmd == CMD_CANCEL_RAW_REPLAY) {
        emit sigReceiveRawUtilityMessage(cmd, status);
    } else {
        emit sigReceiveCmdResp(cmd);
        emit sigReceiveGamma();
        if ((cmd == CMD_READ_MODULE) && (GlobalData::getInstance()->getRestoreFlag())) {
            int num = GlobalData::getInstance()->getReadingPagesNumber();
            GlobalData::getInstance()->setReadingPagesNumber(--num);
            qDebug() << "reading_pages_number = " << num;
            if (num == 0) {
                GlobalData::getInstance()->setRestoreFlag(false);
                emit sigReadAllFinished();
            }
        }
    }
}

QByteArray NetworkClientHelper::popCmdData(QString target_cmd)
{
    QByteArray data;

    QMutexLocker locker(&data_mutex);
    auto list_item_iter = cmd_data_map_.find(target_cmd);
    if (list_item_iter != cmd_data_map_.end()) {
        auto &list_item = list_item_iter.value();
        if (!list_item.isEmpty()) {
            data = list_item.front();
            list_item.pop_front();
        }
    }

    return data;
}

void NetworkClientHelper::clearData()
{
    QMutexLocker locker(&data_mutex);
    cmd_data_map_.clear();
}

bool NetworkClientHelper::isNormalStatus()
{
#ifndef TCP_SOCKET_CLIENT
    return (websocket_.state() == QAbstractSocket::ConnectedState);
#else
    shared_ptr<GlobalData> global_data = GlobalData::getInstance();
    QString connect_type = global_data->getSettings(DEVICE_SECTION, KEY_CONNECT_TYPE).toString();

    if (connect_type == CONNECT_TYPE_NETWORK) {
        return (tcpsocket_.state() == QAbstractSocket::ConnectedState);
    }else if (connect_type == CONNECT_TYPE_UART) {
        return uart_.isOpen();
    }
    return false;
#endif
}

void NetworkClientHelper::handleConnectStatteChanged(QAbstractSocket::SocketState state)
{
    if (QAbstractSocket::UnconnectedState == state) {
        setCallerInfo(0, NULL);
        if (reconnect) {
            emit sigEnableAllPageButtons("", "",BUTTON_MODE::MODE_NORMAL);
            emit sigStatusLog("Connect to device " + GlobalData::getInstance()->getSettings(DEVICE_SECTION, KEY_IP_ADDRESS).toString()
                + " failed," + QString(" error code:%1.").arg(tcpsocket_.error()), LOG_LEVEL::LOG_ERROR);
            conn_timer->start(5000);
        }
    } else if (QAbstractSocket::ConnectedState == state) {
        emit sigEnableAllPageButtons("", "",BUTTON_MODE::MODE_NORMAL);
        emit sigStatusLog("Connect to device " + GlobalData::getInstance()->getSettings(DEVICE_SECTION, KEY_IP_ADDRESS).toString() +
                          " successfully.", LOG_LEVEL::LOG_INFO);
    } else if (QAbstractSocket::ConnectingState == state) {
        emit sigStatusLog("Try to connect device " + GlobalData::getInstance()->getSettings(DEVICE_SECTION, KEY_IP_ADDRESS).toString() +
                          "...", LOG_LEVEL::LOG_INFO);
    }
}

void NetworkClientHelper::handleReconnect()
{
    shared_ptr<GlobalData> global_data = GlobalData::getInstance();
    QString ip_address = global_data->getSettings(DEVICE_SECTION, KEY_IP_ADDRESS).toString();
    QString port = global_data->getSettings(DEVICE_SECTION, KEY_PORT).toString();

    websocket_.open(QUrl("ws://" + ip_address + ":" + port));
}

void NetworkClientHelper::handleConnected()
{
    // conn_timer->stop();
    QObject::connect(&websocket_, &QWebSocket::textMessageReceived,
                     this, &NetworkClientHelper::onMessageReceived);
    QObject::connect(&websocket_, &QWebSocket::binaryFrameReceived,
                     this, &NetworkClientHelper::onFrameReceived);

    sendCmd(CMD_GET_CHIP_INFO);
    sendCmd(CMD_GET_WDR_MODE);
}

void NetworkClientHelper::handleDisconnected()
{
    // conn_timer->start(5000);
    QObject::disconnect(&websocket_, &QWebSocket::textMessageReceived,
                        this, &NetworkClientHelper::onMessageReceived);
    QObject::disconnect(&websocket_, &QWebSocket::binaryFrameReceived,
                        this, &NetworkClientHelper::onFrameReceived);
}

void NetworkClientHelper::onMessageReceived(QString message)
{
    qDebug("receive message:%s", qUtf8Printable(message));
    emit sigStatusLog("receive message:" + message, LOG_LEVEL::LOG_DEBUG);
    QJsonObject data_obj = utils::jsonObjectFromString(message);

    if (data_obj.contains(MSG_KEY_CMD)) {
        parseCmdData(data_obj);
    }
}

void NetworkClientHelper::setMode(bool mode)
{
    normalMode = mode;
}

void NetworkClientHelper::setCallerInfo(int size, QString cmd)
{
    this->recvSize = size;
    this->cmdReq = cmd;
}

void NetworkClientHelper::setCounts(int totalCounts)
{
    this->totalCounts = totalCounts;
}

void NetworkClientHelper::clearFlags()
{
    receiveRawHead = false;
    receiveYuvHead = false;
    receivePreYuvHead = false;
    receive3AYuvHead = false;
    receiveAfYuvHead = false;
    receiveBracketingRawHead = false;
    receiveBraYuvHead = false;
    receiveBraDumpRawHead = false;
    receiveConYuvHead = false;
    receiveCounts = 0;
    totalCounts = 0;
}

void NetworkClientHelper::onFrameReceived(const QByteArray frame, bool isLastFrame)
{
    QString soc = GlobalData::getInstance()->getCvipqToolSoc();
    qDebug("receive frame:mode=%c size = %d, isLastFrame = %d", frame[0], frame.size(), isLastFrame);

    if (CV182X_SOC == soc) {
        if (normalMode) {
            if (frame[0] == 'y') {
                cmd_data_map_[CMD_GET_SINGLE_YUV_DATA] << frame;
                emit sigReceiveImageData(CMD_GET_SINGLE_YUV_DATA);
            } else if (frame[0] == 'f') {
                cmd_data_map_[CMD_GET_SINGLE_YUV_DATA] << frame;
                emit sigReceiveFocusAssistImage(CMD_GET_SINGLE_YUV_DATA);
            } else if (frame[0] == 'Y') {
                cmd_data_map_[CMD_GET_IMAGE_DATA] << frame;
                emit sigReceiveYuvData(CMD_GET_IMAGE_DATA);
            } else if (frame[0] == 'R') {
                cmd_data_map_[CMD_GET_IMAGE_DATA] << frame;
                emit sigReceiveRawData(CMD_GET_IMAGE_DATA);
            } else if (frame[0] == 'B') {
                cmd_data_map_[CMD_GET_IMAGE_DATA] << frame;
                emit sigReceiveBracketingData(CMD_GET_IMAGE_DATA);
            } else if (frame[0] == 'a') {
                cmd_data_map_[CMD_GET_3A_STATISTICS] << frame;
                emit sigReceive3AImage(CMD_GET_3A_STATISTICS);
            }
        } else {
            if ((frame[0] == 'Y') && (frame.count() == sizeof(YUV_HEADER))) {
                receiveYuvHead = true;
                cmd_data_map_[CMD_GET_IMAGE_DATA] << frame;
                emit sigReceiveYuvData(CMD_GET_IMAGE_DATA);
            } else if ((frame[0] == 'R') && (frame.count() == sizeof(RAW_HEADER))) {
                receiveRawHead = true;
                cmd_data_map_[CMD_GET_IMAGE_DATA] << frame;
                emit sigReceiveRawData(CMD_GET_IMAGE_DATA);
            } else if ((frame[0] == 'y') && (frame.count() == sizeof(YUV_HEADER))) {
                receivePreYuvHead = true;
                cmd_data_map_[CMD_GET_SINGLE_YUV_DATA] << frame;
                emit sigReceiveImageData(CMD_GET_SINGLE_YUV_DATA);
            } else if ((frame[0] == 'f') && (frame.count() == sizeof(YUV_HEADER))) {
                receiveAfYuvHead = true;
                cmd_data_map_[CMD_GET_SINGLE_YUV_DATA] << frame;
                emit sigReceiveFocusAssistImage(CMD_GET_SINGLE_YUV_DATA);
            } else if ((frame[0] == 'a') && (frame.count() == sizeof(YUV_HEADER))) {
                receive3AYuvHead = true;
                cmd_data_map_[CMD_GET_3A_STATISTICS] << frame;
                emit sigReceive3AImage(CMD_GET_3A_STATISTICS);
            } else if ((frame[0] == 'B') && (frame.count() == sizeof(RAW_HEADER))) {
                receiveBracketingRawHead = true;
                cmd_data_map_[CMD_GET_IMAGE_DATA] << frame;
                emit sigReceiveBracketingData(CMD_GET_IMAGE_DATA);
            } else if ((frame[0] == 'b') && (frame.count() == sizeof(YUV_HEADER))) {
                receiveBraYuvHead = true;
                cmd_data_map_[CMD_GET_SINGLE_YUV_DATA] << frame;
                emit sigReceiveBracketImage(CMD_GET_SINGLE_YUV_DATA);
            } else if ((frame[0] == 'r') && (frame.count() == sizeof(RAW_HEADER))) {
                receiveBraDumpRawHead = true;
                cmd_data_map_[CMD_GET_IMAGE_DATA] << frame;
                emit sigReceiveBracketRawData(CMD_GET_IMAGE_DATA);
            } else if ((frame[0] == 'C') && (frame.count() == sizeof(YUV_HEADER))) {
                receiveConYuvHead = true;
                cmd_data_map_[CMD_GET_SINGLE_YUV_DATA] << frame;
                emit sigReceiveContinuousYuv(CMD_GET_SINGLE_YUV_DATA);
            } else {
                if (receiveYuvHead) {
                    cmd_data_map_[CMD_GET_IMAGE_DATA] << frame;
                    emit sigReceiveYuvData(CMD_GET_IMAGE_DATA);
                } else if (receiveRawHead) {
                    cmd_data_map_[CMD_GET_IMAGE_DATA] << frame;
                    emit sigReceiveRawData(CMD_GET_IMAGE_DATA);
                } else if (receivePreYuvHead) {
                    cmd_data_map_[CMD_GET_SINGLE_YUV_DATA] << frame;
                    emit sigReceiveImageData(CMD_GET_SINGLE_YUV_DATA);
                } else if (receive3AYuvHead) {
                    cmd_data_map_[CMD_GET_3A_STATISTICS] << frame;
                    emit sigReceive3AImage(CMD_GET_3A_STATISTICS);
                } else if (receiveAfYuvHead) {
                    cmd_data_map_[CMD_GET_SINGLE_YUV_DATA] << frame;
                    emit sigReceiveFocusAssistImage(CMD_GET_SINGLE_YUV_DATA);
                } else if (receiveBracketingRawHead) {
                    cmd_data_map_[CMD_GET_IMAGE_DATA] << frame;
                    emit sigReceiveBracketingData(CMD_GET_IMAGE_DATA);
                } else if (receiveBraYuvHead) {
                    cmd_data_map_[CMD_GET_SINGLE_YUV_DATA] << frame;
                    emit sigReceiveBracketImage(CMD_GET_SINGLE_YUV_DATA);
                } else if (receiveBraDumpRawHead) {
                    cmd_data_map_[CMD_GET_IMAGE_DATA] << frame;
                    emit sigReceiveBracketRawData(CMD_GET_IMAGE_DATA);
                } else if (receiveConYuvHead) {
                    cmd_data_map_[CMD_GET_SINGLE_YUV_DATA] << frame;
                    emit sigReceiveContinuousYuv(CMD_GET_SINGLE_YUV_DATA);
                }

                receiveCounts++;
                if (receiveCounts >= totalCounts) {
                    clearFlags();
                }
            }
        }
    } else {
        if (frame[0] == 'y') {
            cmd_data_map_[CMD_GET_SINGLE_YUV_DATA] << frame;
            emit sigReceiveImageData(CMD_GET_SINGLE_YUV_DATA);
        } else if (frame[0] == 'f') {
            cmd_data_map_[CMD_GET_SINGLE_YUV_DATA] << frame;
            emit sigReceiveFocusAssistImage(CMD_GET_SINGLE_YUV_DATA);
        } else if (frame[0] == 'Y') {
            cmd_data_map_[CMD_GET_IMAGE_DATA] << frame;
            emit sigReceiveYuvData(CMD_GET_IMAGE_DATA);
        } else if (frame[0] == 'R') {
            cmd_data_map_[CMD_GET_IMAGE_DATA] << frame;
            emit sigReceiveRawData(CMD_GET_IMAGE_DATA);
        } else if (frame[0] == 'B') {
            cmd_data_map_[CMD_GET_IMAGE_DATA] << frame;
            emit sigReceiveBracketingData(CMD_GET_IMAGE_DATA);
        } else if (frame[0] == 'a') {
            cmd_data_map_[CMD_GET_3A_STATISTICS] << frame;
            emit sigReceive3AImage(CMD_GET_3A_STATISTICS);
        } else if (frame[0] == 'b') {
            cmd_data_map_[CMD_GET_SINGLE_YUV_DATA] << frame;
            emit sigReceiveBracketImage(CMD_GET_SINGLE_YUV_DATA);
        } else if (frame[0] == 'r') {
            cmd_data_map_[CMD_GET_IMAGE_DATA] << frame;
            emit sigReceiveBracketRawData(CMD_GET_IMAGE_DATA);
        } else if (frame[0] == 'C') {
            cmd_data_map_[CMD_GET_SINGLE_YUV_DATA] << frame;
            emit sigReceiveContinuousYuv(CMD_GET_SINGLE_YUV_DATA);
        }
    }
}

void NetworkClientHelper::connectJsonrpc(CONNECTION_MODE mode)
{
    disconnectJsonrpc();
    reconnect = true;
    connection_mode = mode;
    shared_ptr<GlobalData> global_data = GlobalData::getInstance();
    QString connect_type = global_data->getSettings(DEVICE_SECTION, KEY_CONNECT_TYPE).toString();

    if (connect_type == CONNECT_TYPE_NETWORK) {
        QString ip_address = global_data->getSettings(DEVICE_SECTION, KEY_IP_ADDRESS).toString();
        QString port = global_data->getSettings(DEVICE_SECTION, KEY_PORT).toString();
        tcpsocket_.connectToHost(ip_address, port.toInt());
    } else if (connect_type == CONNECT_TYPE_UART) {
        QString port_name = global_data->getSettings(DEVICE_SECTION, KEY_UART_PORT).toString();
        QSerialPortInfo *port = new QSerialPortInfo(port_name);
        QString baud_rate = global_data->getSettings(DEVICE_SECTION, KEY_BAUD_RATE, BAUD_RATE_115200).toString();
        uart_.setPort(*port);
        uart_.setBaudRate(baud_rate.toInt());
        if (uart_.open(QIODevice::ReadWrite)) {
            qDebug("success to open uart %s!", qUtf8Printable(port_name));
            emit connected_uart();
        } else {
            qDebug("fail to open uart %s!", qUtf8Printable(port_name));
        }
    }
}

void NetworkClientHelper::handleReconnectJsonrpc()
{
    qDebug()<<"handleReconnectJsonrpc";

    shared_ptr<GlobalData> global_data = GlobalData::getInstance();
    QString connect_type = global_data->getSettings(DEVICE_SECTION, KEY_CONNECT_TYPE).toString();

    if (connect_type == CONNECT_TYPE_NETWORK) {
        QString ip_address = global_data->getSettings(DEVICE_SECTION, KEY_IP_ADDRESS).toString();
        QString port = global_data->getSettings(DEVICE_SECTION, KEY_PORT).toString();
        tcpsocket_.connectToHost(ip_address, port.toInt());
    }else if (connect_type == CONNECT_TYPE_UART) {
        QString port_name = global_data->getSettings(DEVICE_SECTION, KEY_UART_PORT).toString();
        QSerialPortInfo *port = new QSerialPortInfo(port_name);
        QString baud_rate = global_data->getSettings(DEVICE_SECTION, KEY_BAUD_RATE, BAUD_RATE_115200).toString();
        uart_.setPort(*port);
        uart_.setBaudRate(baud_rate.toInt());
        if (uart_.open(QIODevice::ReadWrite)) {
            qDebug("success to open uart %s!", qUtf8Printable(port_name));
            emit connected_uart();
        } else {
            qDebug("fail to open uart %s!", qUtf8Printable(port_name));
        }
    }
}

void NetworkClientHelper::disconnectJsonrpc(bool emit_log)
{
    reconnect = false;
    conn_timer->stop();
    tcpsocket_.abort();
    uart_.close();

    emit disconnected_uart();
    emit sigEnableAllPageButtons("", "",BUTTON_MODE::MODE_NORMAL);

    if (emit_log) {
        if (last_connect_type == CONNECT_TYPE_NETWORK) {
            emit sigStatusLog("Disconnect device " + GlobalData::getInstance()->getSettings(DEVICE_SECTION, KEY_IP_ADDRESS).toString()
                +  " successfully", LOG_LEVEL::LOG_INFO);
        } else if (last_connect_type == CONNECT_TYPE_UART) {
            emit sigStatusLog("Disconnect device " + GlobalData::getInstance()->getSettings(DEVICE_SECTION, KEY_UART_PORT).toString()
                +  " successfully", LOG_LEVEL::LOG_INFO);
        }
    }
}

void NetworkClientHelper::handleConnectedJsonrpc()
{
    conn_timer->stop();

    shared_ptr<GlobalData> global_data = GlobalData::getInstance();
    QString connect_type = global_data->getSettings(DEVICE_SECTION, KEY_CONNECT_TYPE).toString();

    if (connect_type == CONNECT_TYPE_NETWORK) {
        QObject::connect(&tcpsocket_, SIGNAL(readyRead()), this, SLOT(onSocketReadyRead()));
    } else if (connect_type == CONNECT_TYPE_UART) {
       QObject::connect(&uart_, SIGNAL(readyRead()), this, SLOT(onUartReadyRead()));
    }
    last_connect_type = connect_type;

    if (connection_mode == CONNECTION_MODE::NORMAL) {
        // Todo: get and check soc & read all page param
        if (!readAllPageParam) {
            emit sigReadAllPageData();
            // first init connect statusTemplate change, reset statusTemplate
            global_data->setStatusTemplate(false);
        } else {
            // if change template ,read all page param
            if (global_data->getStatusTemplate()) {
                emit sigReadAllPageData();
                // reset statusTemplate
                global_data->setStatusTemplate(false);
            }
        }
        getWDRInfo();
    } else if (connection_mode == CONNECTION_MODE::GET_TEMPLATE) {
        getTemplateFile();
    }
}

void NetworkClientHelper::getWDRInfo()
{
    QJsonObject id_info;
    id_info[MSG_KEY_CMD] = CMD_GET_WDR_MODE;
    int id = GlobalData::getInstance()->getJsonrpcId();
    NetworkClientHelper::getInstance()->pushIdInfoJsonrpc(id, id_info);

    QJsonObject send_obj;
    send_obj[MSG_KEY_METHOD] = METHOD_GET_WDR_INFO;
    send_obj[MSG_KEY_ID] = id;
    send_obj[MSG_KEY_JSONRPC] = MSG_JSONRPC_VERSION;
    sendCmd(std::move(send_obj));
}

void NetworkClientHelper::handleDisconnectedJsonrpc()
{
    shared_ptr<GlobalData> global_data = GlobalData::getInstance();
    QString connect_type = global_data->getSettings(DEVICE_SECTION, KEY_CONNECT_TYPE).toString();

    if (connect_type == CONNECT_TYPE_NETWORK) {
        QObject::disconnect(&tcpsocket_,SIGNAL(readyRead()), this, SLOT(onSocketReadyRead()));
    }else if (connect_type == CONNECT_TYPE_UART) {
        QObject::disconnect(&uart_, SIGNAL(readyRead()), this, SLOT(onUartReadyRead()));
    }
}

void NetworkClientHelper::onSocketReadyRead()
{
    qDebug("NetworkClientHelper::onSocketReadyRead");
    qDebug() << "thread thread: " << QThread::currentThreadId();
    if (tcpsocket_.bytesAvailable()) {
        QByteArray data = tcpsocket_.readAll();
        received_data.append(data);
    }

    if (recvSize != 0) {
        if (received_data.size() == recvSize) {
            onFrameReceivedJasonrpc(received_data);
            received_data.clear();
        }
    } else {
        onMessageReceivedJasonrpc(received_data);
    }
}

void NetworkClientHelper::onUartReadyRead()
{
    qDebug("NetworkClientHelper::onUartReadyRead");
    qDebug() << "thread thread: " << QThread::currentThreadId();
    if (uart_.bytesAvailable()) {
        QByteArray data = uart_.readAll();
        received_data.append(data);
    }

    if (recvSize != 0) {
        if (received_data.size() == recvSize) {
            onFrameReceivedJasonrpc(received_data);
            received_data.clear();
        }
    } else {
        onMessageReceivedJasonrpc(received_data);
    }
}

void NetworkClientHelper::onFrameReceivedJasonrpc(const QByteArray frame)
{
    qDebug("receive frame mode:%d, size:%d, cmdReq:%s\n", frame[0], frame.size(), qUtf8Printable(cmdReq));

    if (cmdReq == CMD_PREVIEW_GET_SINGLE_YUV) {
        cmd_data_map_[cmdReq] << frame;
        emit sigPreviewYuvData(cmdReq);
    } else if (cmdReq == CMD_CAPTURE_GET_SINGLE_RAW) {
        cmd_data_map_[cmdReq] << frame;
        emit sig3ARawData(cmdReq);
    } else if (cmdReq == CMD_CAPTURE_GET_MULTI_YUV) {
        cmd_data_map_[cmdReq] << frame;
        emit sigCaptureYuvData(cmdReq);
    } else if (cmdReq == CMD_CALIBRATION_GET_MULTI_YUV) {
        cmd_data_map_[cmdReq] << frame;
        emit sigGetYuvData(cmdReq);
    } else if ((cmdReq == CMD_CAPTURE_GET_AE_BIN) || (cmdReq == CMD_CAPTURE_GET_AWB_BIN)) {
        cmd_data_map_[cmdReq] << frame;
        emit sigCaptureBinData(cmdReq);
    } else if (cmdReq == CMD_CAPTURE_GET_MULTI_RAW) {
        cmd_data_map_[cmdReq] << frame;
        emit sigCaptureRawData(cmdReq);
    } else if (cmdReq == CMD_BINUTIL_EXPORT_BIN) {
        cmd_data_map_[cmdReq] << frame;
        emit sigBinUtilData(cmdReq);
    } else if (cmdReq == CMD_3A_GET_SINGLE_YUV) {
        cmd_data_map_[cmdReq] << frame;
        emit sig3AYuvData(cmdReq);
    } else if (cmdReq == CMD_3A_GET_AWB_BIN) {
        cmd_data_map_[cmdReq] << frame;
        emit sig3ABinData(cmdReq);
    } else if (cmdReq == CMD_AF_GET_SINGLE_YUV) {
        cmd_data_map_[cmdReq] << frame;
        emit sigAFYuvData(cmdReq);
    } else if (cmdReq == CMD_CONTINUOUS_GET_SINGLE_YUV) {
        cmd_data_map_[cmdReq] << frame;
        emit sigContinuousYuvData(cmdReq);
    } else if ((cmdReq == CMD_BRACKET_GET_AE_BIN) || (cmdReq == CMD_BRACKET_GET_AWB_BIN)
        || (cmdReq == CMD_BRACKET_GET_MULTI_AWB_BIN)) {
        cmd_data_map_[cmdReq] << frame;
        emit sigBracketBinData(cmdReq);
    } else if (cmdReq == CMD_BRACKET_GET_SINGLE_RAW) {
        cmd_data_map_[cmdReq] << frame;
        emit sigBracketSingleRawData(cmdReq);
    } else if (cmdReq == CMD_BRACKET_GET_SINGLE_YUV) {
        cmd_data_map_[cmdReq] << frame;
        emit sigBracketSingleYuvData(cmdReq);
    } else if (cmdReq == CMD_BRACKET_GET_DATA) {
        cmd_data_map_[cmdReq] << frame;
        emit sigBracketData(cmdReq);
    } else if (cmdReq == CMD_GET_TOOL_JSON) {
        cmd_data_map_[cmdReq] << frame;
        emit sigToolJsonData(cmdReq);
    } else if (cmdReq == CMD_CALI_GET_MULTI_RAW) {
        cmd_data_map_[cmdReq] << frame;
        emit sigCaliRawData(cmdReq);
    } else if ((cmdReq == CMD_CALI_GET_AE_BIN) || (cmdReq == CMD_CALI_GET_AWB_BIN)) {
        cmd_data_map_[cmdReq] << frame;
        emit sigCaliBinData(cmdReq);
    } else if (cmdReq == CMD_RAWREPLAY_GET_MULTI_YUV) {
        cmd_data_map_[cmdReq] << frame;
        emit sigRawUtilYuvData(cmdReq);
    }
}

void NetworkClientHelper::onMessageReceivedJasonrpc(QByteArray data)
{
    qDebug("NetworkClientHelper::onMessageReceivedJasonrpc");

    QString data_str = QString(data);
    qDebug() << "received data: " << data_str;
    emit sigStatusLog("received data:" + data_str, LOG_LEVEL::LOG_DEBUG);
    QJsonObject data_obj = utils::jsonObjectFromString(data_str);
    int jsonrpc_id = -1;
    if (data_obj.contains(MSG_KEY_ID)) {
        if ((cmdReq == CMD_BINUTIL_SEND_BIN_DATA) || (cmdReq == CMD_RAWUTIL_SEND_RAW_DATA)
            || (cmdReq == CMD_LDC_VI_BIN_DATA) || (cmdReq == CMD_LDC_VPSS_BIN_DATA)) {
            jsonrpc_id = GlobalData::getInstance()->getJsonrpcId();
            QJsonObject id_info;
            id_info[MSG_KEY_CMD] = cmdReq;
            id_info[MSG_KEY_ID] = jsonrpc_id;
            NetworkClientHelper::getInstance()->pushIdInfoJsonrpc(jsonrpc_id, id_info);
        } else {
            jsonrpc_id = data_obj[MSG_KEY_ID].toInt();
        }
    } else {
        return;
    }

    received_data.clear();
    GlobalData::getInstance()->setWaitForResponse(false);
    if (id_info_map.contains(jsonrpc_id)) {
        QJsonObject id_info = id_info_map.take(jsonrpc_id);
        if (id_info.contains(MSG_KEY_CMD)) {
            // for cmodel
            if (id_info[MSG_KEY_CMD] == CMD_CTRL_CMODEL) {
                NetworkClientHelper::getInstance()->pushIdInfoJsonrpc(jsonrpc_id, id_info);
            }
            data_obj.insert(MSG_KEY_ID_INFO, id_info);
            parseCmdDataJsonrpc(data_obj);
        } else {
            emit sigStatusLog("Can't find command in recevied data.", LOG_LEVEL::LOG_ERROR);
        }
    } else {
        emit sigStatusLog("Can't find id info in recevied data.", LOG_LEVEL::LOG_ERROR);
    }
}

void NetworkClientHelper::parseCmdDataJsonrpc(const QJsonObject &obj)
{
    qDebug("NetworkClientHelper::parseCmdDataFromJsonrpc");

    QVariant id;
    QJsonObject result_obj = obj[MSG_KEY_RESULT].toObject();
    QJsonObject info_obj = obj[MSG_KEY_ID_INFO].toObject();
    QString cmd = info_obj[MSG_KEY_CMD].toString();
    QString module = info_obj[MSG_KEY_MODULE].toString();
    QString struct_id = info_obj[MSG_KEY_STRUCT].toString();

    if ((cmdReq == CMD_BINUTIL_SEND_BIN_DATA) || (cmdReq == CMD_RAWUTIL_SEND_RAW_DATA)
        || (cmdReq == CMD_LDC_VI_BIN_DATA) || (cmdReq == CMD_LDC_VPSS_BIN_DATA)) {
        id = info_obj[MSG_KEY_ID].toVariant();
    } else {
        id = obj[MSG_KEY_ID].toVariant();
    }

    if (!result_obj.isEmpty()) {
        QVariant status = result_obj[MSG_KEY_STATUS].toVariant();
        QString log;
        if (cmd == CMD_READ_MODULE || cmd == CMD_READ_GAMMA) {
            log = module + " Page: Read " + struct_id + " successfully." + " id = " + id.toString() + ", status = " + status.toString();
        } else if (cmd == CMD_WRITE_MODULE) {
            log = module + " Page: Write " + struct_id + " successfully." + " id = " + id.toString() + ", status = " + status.toString();
        } else {
            log = cmd + ". id = " + id.toString() + ", status = " + status.toString();
        }

        if (cmd == CMD_READ_MODULE || cmd == CMD_READ_GAMMA || cmd == CMD_WRITE_MODULE) {
            emit sigEnableAllPageButtons(module, cmd, BUTTON_MODE::MODE_COUNT);
        }
        emit sigStatusLog(log, LOG_LEVEL::LOG_INFO);
    } else {
        QJsonObject error_obj = obj[MSG_KEY_ERROR].toObject();
        QVariant code = error_obj[MSG_KEY_CODE].toVariant();
        QString message = error_obj[MSG_KEY_MESSAGE].toString();
        QString log;
        LOG_LEVEL log_type = LOG_LEVEL::LOG_ERROR;

        if (cmd == CMD_READ_MODULE || cmd == CMD_READ_GAMMA) {
            log = module + " Page: Failed to read " + struct_id + ". id = " + id.toString() + ", code = " + code.toString() + ", " + message;
        } else if (cmd == CMD_WRITE_MODULE) {
            log = module + " Page: Failed to write " + struct_id + ". id = " + id.toString() + ", code = " + code.toString() + ", " + message;
        } else {
            log = cmd + " " + struct_id + ". id = " + id.toString() + ", code = " + code.toString() + ", " + message;
        }

        if (cmd == CMD_READ_MODULE || cmd == CMD_READ_GAMMA || cmd == CMD_WRITE_MODULE) {
            emit sigEnableAllPageButtons(module, cmd, BUTTON_MODE::MODE_COUNT);
        }

        if (cmd == CMD_3A_GET_SINGLE_YUV) {
            log_type = LOG_LEVEL::LOG_WARNING;
        }
        emit sigStatusLog(log, log_type);
    }

    QMutexLocker locker(&data_mutex);
    id_data_map.clear();
    id_data_map.insert(id.toInt(), obj);
    locker.unlock();

    if ((cmd == CMD_READ_MODULE) || (cmd == CMD_WRITE_MODULE)) {
        emit sigReceiveCmdRespJsonrpc(id.toInt());
    } else if (cmd == CMD_READ_GAMMA) {
        emit sigReceiveGammaJsonrpc(id.toInt());
    } else if (cmd == CMD_READ_CCM_CALIBRATION) {
        emit sigReceiveCcmCalibration(id.toInt());
    } else if (cmd == CMD_READ_RLSC_CALIBRATION) {
        emit sigReceiveRlscCalibration(id.toInt());
    } else if (cmd == CMD_READ_MLSC_CALIBRATION) {
        emit sigReceiveMlscCalibration(id.toInt());
    } else if (cmd == CMD_READ_BLC_CALIBRATION) {
        emit sigReceiveBlcCalibration(id.toInt());
    } else if (cmd == CMD_READ_AWB_CALIBRATION) {
        emit sigReceiveAWBCalibration(id.toInt());
    } else if (cmd == CMD_READ_RGBIR_CALIBRATION) {
        emit sigReceiveRgbirCalibration(id.toInt());
    } else if ((cmd == CMD_READ_NOISE_PROFILE_CALIBRATION) || (cmd == CMD_READ_NOISE_PROFILE_BNR_CALIBRATION)) {
        emit sigReceiveNoiseProfileCalibration(id.toInt());
    } else if (cmd == CMD_PREVIEW_GET_SINGLE_YUV) {
        emit sigPreviewYuvStatus(id.toInt());
    } else if (cmd == CMD_CAPTURE_GET_SINGLE_RAW) {
        emit sigCaptue3ARawStatus(id.toInt());
    } else if (cmd == CMD_CAPTURE_GET_MULTI_YUV) {
        emit sigCaptueYuvStatus(id.toInt());
    } else if (cmd == CMD_CALIBRATION_GET_MULTI_YUV) {
        emit sigGetYuvStatus(id.toInt());
    } else if (cmd == CMD_CAPTURE_GET_MULTI_RAW) {
        emit sigCaptueRawStatus(id.toInt());
    } else if ((cmd == CMD_CAPTURE_GET_AE_BIN) || (cmd == CMD_CAPTURE_GET_AWB_BIN)) {
        emit sigCaptureBinInfo(id.toInt());
    } else if ((cmd == CMD_BINUTIL_EXPORT_BIN) || (cmd == CMD_BINUTIL_IMPORT_BIN)
        || (cmd == CMD_BINUTIL_SEND_BIN_DATA) || (cmd == CMD_BINUTIL_FIX_BIN_TO_FLASH)) {
        emit sigBinUtilStatus(id.toInt());
    } else if (cmd == CMD_3A_GET_SINGLE_YUV) {
        emit sig3AYuvStatus(id.toInt());
    } else if (cmd == CMD_LDC_VI_BIN_SIZE) {
        emit sigSendLDCBinData(id.toInt());
    } else if (cmd == CMD_LDC_VI_BIN_DATA) {
        emit sigSendLDCBinData(id.toInt());
    } else if ((cmd == CMD_LDC_VPSS_BIN_DATA) || (cmd == CMD_LDC_VPSS_BIN_SIZE)) {
        emit sigSendLDCBinData(id.toInt());
    } else if (cmd == CMD_3A_GET_AWB_BIN) {
        emit sig3ABinInfo(id.toInt());
    } else if (cmd == CMD_AF_GET_SINGLE_YUV) {
        emit sigAFYuvStatus(id.toInt());
    } else if (cmd == CMD_GET_WDR_MODE) {
        emit sigGetWDRMode(id.toInt());
    } else if (cmd == CMD_3A_GET_STATISTICS_INFO) {
        emit sig3AStatisticsInfo(id.toInt());
    } else if (cmd == CMD_AF_GET_STATISTICS_INFO) {
        emit sigAFStatisticsInfo(id.toInt());
    } else if ((cmd == CMD_RAWUTIL_SEND_RAW_HEAD) || (cmd == CMD_RAWUTIL_SEND_RAW_DATA)
        || (cmd == CMD_RAWUTIL_START_RAW_REPLAY) || (cmd == CMD_RAWUTIL_CANCEL_RAW_REPLAY)) {
        emit sigRawUtilityMessage(id.toInt());
    } else if (cmd == CMD_CONTINUOUS_GET_SINGLE_YUV) {
        emit sigContinuousYuvStatus(id.toInt());
    } else if (cmd == CMD_BRACKET_GET_SINGLE_RAW) {
        emit sigBracketSingleRawStatus(id.toInt());
    } else if ((cmd == CMD_BRACKET_GET_AE_BIN) || (cmd == CMD_BRACKET_GET_AWB_BIN)
        || (cmd == CMD_BRACKET_GET_MULTI_AWB_BIN)) {
        emit sigBracketBinInfo(id.toInt());
    } else if ((cmd == CMD_BRACKET_START) || (cmd == CMD_BRACKET_GET_DATA) || (cmd == CMD_BRACKET_FINISH)) {
        emit sigBracketStatus(id.toInt());
    } else if (cmd == CMD_BRACKET_GET_SINGLE_YUV) {
        emit sigBracketSingleYuvStatus(id.toInt());
    } else if (cmd == CMD_GET_TOOL_JSON) {
        emit sigToolJsonStatus(id.toInt());
    } else if (cmd == CMD_GET_CHIP_INFO) {
        emit sigChipInfoStatus(id.toInt());
        readAllPageParam = true;
    } else if (cmd == CMD_GET_ISO_LV_VALUE) {
        emit sigReceiveEvValue(id.toInt());
    } else if (cmd == CMD_GET_LDC_CHN_SIZE) {
        emit sigReceiveLdcSize(id.toInt());
        GlobalData::getInstance()->setWaitForResponse(false);
    } else if (cmd == CMD_GET_DRC_HISTOGRAM_INFO) {
        emit sigRecDrcHistogramInfo(id.toInt());
    } else if (cmd == CMD_CALI_GET_MULTI_RAW) {
        emit sigCaliRawStatus(id.toInt());
    } else if ((cmd == CMD_CALI_GET_AE_BIN) || (cmd == CMD_CALI_GET_AWB_BIN)) {
        emit sigCaliBinInfo(id.toInt());
    } else if (cmd == CMD_I2C_READ) {
        emit sigI2CReadStatus(id.toInt());
    } else if (cmd == CMD_I2C_WRITE) {
        emit sigI2CWriteStatus(id.toInt());
    } else if (cmd == CMD_REG_GET) {
        emit sigRegGetStatus(id.toInt());
    } else if (cmd == CMD_REG_SET) {
        emit sigRegSetStatus(id.toInt());
    } else if (cmd == CMD_SPI_TRANSMIT) {
        emit sigSPITransmitStatus(id.toInt());
    } else if (cmd == CMD_RAWREPLAY_GET_MULTI_YUV) {
        emit sigRawUtilYuvStatus(id.toInt());
    } else if (cmd == CMD_CONTINUOUS_GET_MAX_FRAMES) {
        emit sigContinuousGetMaxFrames(id.toInt());
    } else if (cmd == CMD_CTRL_CMODEL) {
        emit sigCtrlCmodel(id.toInt());
    } else if (cmd == CMD_GET_FSHDR_MAPCURVE_INFO) {
        emit sigRecFshdrMapcurveInfo(id.toInt());
    }
}

QJsonObject NetworkClientHelper::popCmdDataJsonrpc(int id)
{
    QJsonObject data;

    if (id_data_map.contains(id))
        data = id_data_map.take(id);

    return data;
}

void NetworkClientHelper::getTemplateFile()
{
    GlobalData::getInstance()->getToolJson();
}

