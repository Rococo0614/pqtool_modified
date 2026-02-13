#include "raw_dump.hpp"

#define REMOTE_PORT  5567
#define CMD_LENGTH   64
#define FILE_NAME_MAX_LENGHT 256
#define DATA_SIZE_5M (1024*1024*5)

RawDump::RawDump(QObject *parent) : QObject(parent)
{
    connect(this, &RawDump::ConnectSignals, this, &RawDump::ConnectSlots);
    connect(this, &RawDump::StartRawDumpSignals, this, &RawDump::StartRawDumpSlots);
    connect(this, &RawDump::DisconnectSignals, this, &RawDump::DisconnectSlots);

    this->moveToThread(&m_workThread);
    m_workThread.start();
}

RawDump::~RawDump()
{
    m_bEnableRecvData = false;
    m_workThread.quit();
    m_workThread.wait();
}

void RawDump::Connect(QString ip)
{
    emit ConnectSignals(ip);
}

void RawDump::StartRawDump(struct RawDumpInfo info)
{
    QVariant data;
    data.setValue(info);
    emit StartRawDumpSignals(data);
}

void RawDump::Disconnect()
{
    emit DisconnectSignals();
}

void RawDump::handleConnectStatteChanged(QAbstractSocket::SocketState state)
{
    if (state == QAbstractSocket::UnconnectedState) {
        qDebug() << "raw dump, network connect fail!\n";
        emit DumpStatus(RAW_DUMP_STATUS_E::RAW_DUMP_CONNECT_FAIL);
    } else if (state == QAbstractSocket::ConnectedState) {
        qDebug() << "raw dump, network connect succeed!\n";
        emit DumpStatus(RAW_DUMP_STATUS_E::RAW_DUMP_CONNECT_SUCCEED);
    }
}

void RawDump::ConnectSlots(QString ip)
{
    DisconnectSlots();

    if (m_tcpClient == nullptr) {
        m_tcpClient = new QTcpSocket(this);
    }
    m_tcpClient->setProxy(QNetworkProxy::NoProxy);
    m_tcpClient->close();

    connect(m_tcpClient, &QTcpSocket::stateChanged, this, &RawDump::handleConnectStatteChanged);
    m_tcpClient->connectToHost(ip.toStdString().c_str(), REMOTE_PORT);

}

void RawDump::StartRawDumpSlots(QVariant data)
{
    char cmd[CMD_LENGTH] = {0};
    char param[FILE_NAME_MAX_LENGHT] = {0};

    if (m_tcpClient == nullptr) {
        qDebug() << "Create tcp err!\n";
        emit DumpStatus(RAW_DUMP_STATUS_E::RAW_DUMP_CONNECT_FAIL);
        return;
    }

    struct RawDumpInfo info = data.value<struct RawDumpInfo>();
    m_info = info;

    snprintf(param, FILE_NAME_MAX_LENGHT, "%s %d,%d,%d,%d,%d",
             info.bDump2Remote ? "remote" : info.pathPrefix.c_str(),
             info.totalFrame,
             info.roiRectX, info.roiRectY,
             info.roiRectW, info.roiRectH);

    snprintf(cmd, CMD_LENGTH, "%s %lld", "cvitek raw_dump", strlen(param));

    qDebug() << "send cmd: " << cmd;

    m_tcpClient->write(cmd, CMD_LENGTH);

    qDebug() << "send param: " << param;

    m_tcpClient->write(param, strlen(param));

    qDebug() << "recv data...";

    m_bEnableRecvData = true;

    RecvData();

    emit DumpStatus(RAW_DUMP_STATUS_E::RAW_DUMP_FINISH);
    qDebug() << "raw dump finish...";

}

void RawDump::DisconnectSlots()
{
    if (m_tcpClient == nullptr) {
        return;
    }
    m_bEnableRecvData = false;
    disconnect(m_tcpClient, &QTcpSocket::stateChanged, this, &RawDump::handleConnectStatteChanged);
    m_tcpClient->close();
    delete m_tcpClient;
    m_tcpClient = nullptr;
    qDebug() << "raw dump, network disconnect succeed!\n";
}

typedef int (*pcmd_fun)(RawDump *thiz, int data_length);

typedef struct {
    const char *cmd_str;
    pcmd_fun fun;
} cmd_item_s;

static int handle_recv_file(RawDump *thiz, int data_length);
static int handle_dump_status(RawDump *thiz, int data_length);

cmd_item_s cmd_list[] = {
    {
        "file",
        handle_recv_file
    },

    {
        "raw",
        handle_recv_file
    },

    {
        "status",
        handle_dump_status
    }
};

int RawDump::ReadData(char *data, int length)
{
    int time_out_max = 20000 / 500;
    int time_out = 0;
    int recv_count = 0;

    do {
        if (0 != m_tcpClient->bytesAvailable() || m_tcpClient->waitForReadyRead(500)) {
            recv_count += m_tcpClient->read(data + recv_count, length - recv_count);
            if (recv_count <= 0) {
                qDebug() << "recv data size err...";
                emit DumpStatus(RAW_DUMP_STATUS_E::RAW_DUMP_ERROR);
                break;
            }
            time_out = 0;
        } else {
            time_out++;

            if (time_out > time_out_max) {
                qDebug() << "recv data time out...";
                m_bEnableRecvData = false;
                emit DumpStatus(RAW_DUMP_STATUS_E::RAW_DUMP_ERROR);
            }
        }

        if (!m_bEnableRecvData){
            qDebug() << "exit recv data...";
            return -1;
        }

    } while (recv_count != length);

    return recv_count;
}

void RawDump::RecvData()
{
    char recv_cmd_buffer[CMD_LENGTH];

    int recv_count = 0;

    while (m_bEnableRecvData) {

        recv_count = ReadData(recv_cmd_buffer, CMD_LENGTH);

        if (recv_count <= 0) {
            qDebug() << "recv error!!!";
            break;
        }

        qDebug() << "recv cmd: " << recv_cmd_buffer;

        if (0x00 != strncmp((const char *) recv_cmd_buffer, "cvitek", strlen("cvitek"))) {
            qDebug() << "recv unknown cmd!!!";
            continue;
        }

        char *p = strchr((const char *) recv_cmd_buffer, ' ');

        if (p == NULL) {
            continue;
        }

        p++;

        for (uint32_t i = 0; i < sizeof(cmd_list) / sizeof(cmd_item_s); i++) {

            if (strstr(p, cmd_list[i].cmd_str) != NULL) {

                uint32_t data_length = 0;

                qDebug() << "exec cmd: " << p;

                p = strchr(p, ' ');

                if (p == NULL) {
                    break;
                }

                p++;

                data_length = atoi(p);

                cmd_list[i].fun(this, data_length);
            }
        }
    }
}

static void ModifyFileName(std::string dirPrefix, char *fileName)
{
    char *str_pos = NULL;
    size_t start_index = dirPrefix.find_last_of("_");
    std::string time_string = dirPrefix.substr(start_index + 1, 14);

    str_pos = strstr(fileName, "ISO=");
    str_pos = strstr(str_pos, "_");
    strncpy(str_pos + 1, time_string.c_str(), 14);
}

static int handle_recv_file(RawDump *thiz, int data_length)
{
    int recv_count = 0, len = 0;
    char file_name[FILE_NAME_MAX_LENGHT] = {0};
    char *data = nullptr;
    QString dirPath = QString(thiz->m_info.pathPrefix.c_str());

    recv_count = thiz->ReadData(file_name, FILE_NAME_MAX_LENGHT);

    if (recv_count <= 0) {
        qDebug() << "recv file name error!!!";
        return -1;
    }

    qDebug() << "recv file: " << file_name << " length: " << data_length;
    ModifyFileName(dirPath.toStdString(), file_name);

    QString path = dirPath + "/" + file_name;
    len = data_length - FILE_NAME_MAX_LENGHT;
    QFile file(path);
    file.open(QIODevice::WriteOnly);

    if (len <= DATA_SIZE_5M) {
        data = new char[len];
        recv_count = thiz->ReadData(data, len);
        if (recv_count <= 0) {
            delete [] data;
            file.close();
            qDebug() << "recv file data error!!!";
            return -1;
        }
        file.write(data, len);
    } else {
        data = new char[DATA_SIZE_5M];
        int loops = (len % DATA_SIZE_5M) ? (len / DATA_SIZE_5M + 1) : (len / DATA_SIZE_5M);
        for (int i = 0; i < loops; i++) {
            int tmpLen = DATA_SIZE_5M;
            if (i == loops - 1) {
                tmpLen = len - DATA_SIZE_5M * i;
            }
            recv_count = thiz->ReadData(data, tmpLen);
            if (recv_count <= 0) {
                delete [] data;
                file.close();
                qDebug() << "recv file data error!!!";
                return -1;
            }
            file.write(data, tmpLen);
        }
    }

    qDebug() << "save file: " << path<<" size:"<<len;

    file.close();
    delete [] data;

    return 0;
}

static int handle_dump_status(RawDump *thiz, int data_length)
{
    if (data_length == 0) {
        thiz->m_bEnableRecvData = false;
    } else if (data_length == -1) {
        emit thiz->DumpStatus(RAW_DUMP_STATUS_E::RAW_DUMP_ERROR);
        thiz->m_bEnableRecvData = false;
        qDebug() << "raw dump error...";
    } else {
        qDebug() << "dump roi count: " << data_length;
    }

    return 0;
}
