#ifndef RAW_DUMP_HPP
#define RAW_DUMP_HPP

#include <QObject>
#include <QtNetwork>

struct RawDumpInfo {
    std::string pathPrefix;
    int totalFrame;

    int roiRectX;
    int roiRectY;
    int roiRectW;
    int roiRectH;

    bool bDump2Remote;
};

Q_DECLARE_METATYPE(RawDumpInfo);

typedef enum {
    RAW_DUMP_CONNECT_SUCCEED,
    RAW_DUMP_CONNECT_FAIL,
    RAW_DUMP_FINISH,
    RAW_DUMP_ERROR,
    RAW_DUMP_STATUS_MAX
} RAW_DUMP_STATUS_E;

class RawDump : public QObject
{
    Q_OBJECT
public:
    explicit RawDump(QObject *parent = nullptr);

    ~RawDump();

    void Connect(QString ip);
    void StartRawDump(struct RawDumpInfo info);
    void Disconnect();

public:
    bool m_bEnableRecvData;
    struct RawDumpInfo m_info;
    int ReadData(char *data, int length);

private:
    QTcpSocket *m_tcpClient{nullptr};

signals:
    void DumpStatus(int status);

signals:
    void ConnectSignals(QString ip);
    void StartRawDumpSignals(QVariant data);
    void DisconnectSignals();

private slots:
    void ConnectSlots(QString ip);
    void StartRawDumpSlots(QVariant data);
    void DisconnectSlots();
    void handleConnectStatteChanged(QAbstractSocket::SocketState stsate);

private:
    void RecvData();

private:
    QThread m_workThread;
};

#endif // RAW_DUMP_H
