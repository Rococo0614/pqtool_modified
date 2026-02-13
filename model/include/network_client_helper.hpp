#ifndef NETWORK_CLIENT_HELPER_HPP
#define NETWORK_CLIENT_HELPER_HPP

#include "status_log_widget.hpp"
#include "module_define.hpp"
#include "utils.hpp"

#include <QtWebSockets/QtWebSockets>
#include <QUrl>
#include <QSerialPort>
#include <QSerialPortInfo>

const QString MSG_JSONRPC_VERSION = "2.0";
const QString MSG_KEY_JSONRPC = "jsonrpc";
const QString MSG_KEY_METHOD = "method";
const QString MSG_KEY_ID = "id";
const QString MSG_KEY_ID_INFO = "id_info";
const QString MSG_KEY_RESULT  = "result";
const QString MSG_KEY_ERROR = "error";
const QString MSG_KEY_STRUCT = "struct";
const QString MSG_KEY_CODE = "code";
const QString MSG_KEY_MESSAGE = "message";
const QString MSG_KEY_PARAMS = "params";
const QString MSG_KEY_STMANUAL = "stManual";
const QString MSG_KEY_STAUTO = "stAuto";
const QString MSG_KEY_MAPPING = "mapping";

const QString CMD_READ_GAMMA = "READ_GAMMA";
const QString CMD_READ_BLC_CALIBRATION = "READ_BLC_CALIBRATION";
const QString CMD_WRITE_BLC_CALIBRATION = "WRITE_BLC_CALIBRATION";
const QString CMD_WRITE_LSC_CALIBRATION = "WRITE_LSC_CALIBRATION";
const QString CMD_WRITE_LBLC_CALIBRATION = "WRITE_LBLC_CALIBRATION";
const QString CMD_WRITE_AWB_CALIBRATION = "WRITE_AWB_CALIBRATION";
const QString CMD_READ_AWB_CALIBRATION = "READ_AWB_CALIBRATION";
const QString CMD_READ_CCM_CALIBRATION = "READ_CCM_CALIBRATION";
const QString CMD_READ_RLSC_CALIBRATION = "READ_RLSC_CALIBRATION";
const QString CMD_READ_MLSC_CALIBRATION = "READ_MLSC_CALIBRATION";
const QString CMD_WRITE_CCM_CALIBRATION = "WRITE_CCM_CALIBRATION";
const QString CMD_READ_RGBIR_CALIBRATION = "READ_RGBIR_CALIBRATION";
const QString CMD_WRITE_RGBIR2RGGB_CALIBRATION = "WRITE_RGBIR2RGGB_CALIBRATION";
const QString CMD_WRITE_RGBIR_CALIBRATION = "WRITE_RGBIR_CALIBRATION";
const QString CMD_READ_NOISE_PROFILE_CALIBRATION = "READ_NOISE_PROFILE_CALIBRATION";
const QString CMD_READ_NOISE_PROFILE_BNR_CALIBRATION = "READ_NOISE_PROFILE_BNR_CALIBRATION";
const QString CMD_WRITE_NOISE_PROFILE_CALIBRATION = "WRITE_NOISE_PROFILE_CALIBRATION";
const QString CMD_WRITE_NOISE_PROFILE_BNR_CALIBRATION = "WRITE_NOISE_PROFILE_BNR_CALIBRATION";
const QString CMD_WRITE_CLUT_CALIBRATION = "WRITE_CLUT_CALIBRATION";
const QString CMD_CALIBRATION_GET_MULTI_YUV = "CALIBRATION_GET_MULTI_YUV";

const QString MSG_KEY_CMD  = "cmd";
const QString MSG_KEY_DATA = "data";
const QString MSG_KEY_STATUS = "status";
const QString MSG_KEY_SUCCESS = "success";
const QString MSG_KEY_FAILURE = "failure";
const QString MSG_KEY_WARNING = "warning";
const QString MSG_KEY_INFO = "info";
const QString MSG_KEY_MODULE = "module";
const QString MSG_KEY_REGISTERS = "registers";
const QString MSG_KEY_ITEMS = "items";
const QString MSG_KEY_OP_TYPE = "optype";
const QString MSG_KEY_MODE = "mode";
const QString MSG_KEY_IMAGE = "image";

const QString CMD_READ_REGISTERS = "READ_REGISTERS";
const QString CMD_WRITE_REGISTERS = "WRITE_REGISTERS";
const QString CMD_READ_MODULE = "READ_MODULE";
const QString CMD_WRITE_MODULE = "WRITE_MODULE";
const QString CMD_PUT_SINGLE_RAW_DATA = "PUT_SINGLE_RAW_DATA";
const QString CMD_GET_SINGLE_YUV_DATA = "GET_SINGLE_YUV_DATA";
const QString CMD_SET_SENSOR_MODE = "SET_SENSOR_MODE";
const QString CMD_GET_MULTI_YUV_DATA = "GET_MULTI_YUV_DATA";
const QString CMD_GET_MULTI_RAW_DATA = "GET_MULTI_RAW_DATA";
const QString CMD_BRACKETING_START = "BRACKETING_START";
const QString CMD_GET_BRACKETING_DATA = "GET_BRACKETING_DATA";
const QString CMD_BRACKETING_FINISH = "BRACKETING_FINISH";
const QString CMD_IMPORT_BIN_FILE = "IMPORT_BIN_FILE";
const QString CMD_EXPORT_BIN_FILE = "EXPORT_BIN_FILE";
const QString CMD_FIX_BIN_TO_FLASH = "FIX_BIN_TO_FLASH";
const QString CMD_GET_IMAGE_DATA = "GET_IMAGE_DATA";
const QString CMD_STOP_GET_IMAGE_DATA = "STOP_GET_IMAGE_DATA";
const QString CMD_GET_BLACK_LEVEL = "GET_BLACK_LEVEL";
const QString CMD_READ_REG = "READ_SINGLE_REGISTER";
const QString CMD_WRITE_REG = "WRITE_SINGLE_REGISTER";
const QString CMD_GET_3A_STATISTICS = "GET_3A_STATISTICS";
const QString CMD_GET_AF_STATISTICS = "GET_AF_STATISTICS";
const QString CMD_GET_CHIP_INFO = "GET_CHIP_INFO";
const QString CMD_SEND_RAW_REPLAY_DATA = "SEND_RAW_REPLAY_DATA";
const QString CMD_START_RAW_REPLAY = "START_RAW_REPLAY";
const QString CMD_CANCEL_RAW_REPLAY = "CANCEL_RAW_REPLAY";
const QString CMD_GET_ISO_LV_VALUE = "GET_ISO_LV_VALUE";
const QString CMD_GET_LDC_CHN_SIZE = "GET_LDC_CHN_SIZE";
const QString CMD_LDC_VI_BIN_SIZE = "LDC_VI_BIN_SIZE";
const QString CMD_LDC_VPSS_BIN_SIZE = "LDC_VPSS_BIN_SIZE";
const QString CMD_LDC_VI_BIN_DATA = "LDC_VI_BIN_DATA";
const QString CMD_LDC_VPSS_BIN_DATA = "LDC_VPSS_BIN_DATA";

const QString CMD_PREVIEW_GET_SINGLE_YUV = "PREVIEW_GET_SINGLE_YUV";
const QString CMD_CAPTURE_GET_SINGLE_RAW = "CAPTURE_GET_SINGLE_RAW";
const QString CMD_CAPTURE_GET_MULTI_YUV = "CAPTURE_GET_MULTI_YUV";
const QString CMD_CAPTURE_GET_MULTI_RAW = "CAPTURE_GET_MULTI_RAW";
const QString CMD_CAPTURE_GET_AE_BIN = "CAPTURE_GET_AE_BIN";
const QString CMD_CAPTURE_GET_AWB_BIN = "CAPTURE_GET_AWB_BIN";
const QString CMD_BINUTIL_EXPORT_BIN = "BINUTIL_EXPORT_BIN";
const QString CMD_BINUTIL_IMPORT_BIN = "BINUTIL_IMPORT_BIN";
const QString CMD_BINUTIL_FIX_BIN_TO_FLASH = "BINUTIL_FIX_BIN_TO_FLASH";
const QString CMD_3A_GET_SINGLE_YUV = "3A_GET_SINGLE_YUV";
const QString CMD_3A_GET_STATISTICS_INFO = "3A_GET_STATISTICS_INFO";
const QString CMD_3A_GET_AWB_BIN = "3A_GET_AWB_BIN";
const QString CMD_AF_GET_SINGLE_YUV = "AF_GET_SINGLE_YUV";
const QString CMD_AF_GET_STATISTICS_INFO = "AF_GET_STATISTICS_INFO";
const QString CMD_GET_WDR_MODE = "GET_WDR_MODE";
const QString CMD_BINUTIL_SEND_BIN_DATA = "BINUTIL_SEND_BIN_DATA";
const QString CMD_RAWUTIL_SEND_RAW_HEAD = "RAWUTIL_SEND_RAW_HEAD";
const QString CMD_RAWUTIL_SEND_RAW_DATA = "RAWUTIL_SEND_RAW_DATA";
const QString CMD_RAWUTIL_START_RAW_REPLAY = "RAWUTIL_START_RAW_REPLAY";
const QString CMD_RAWUTIL_CANCEL_RAW_REPLAY = "RAWUTIL_CANCEL_RAW_REPLAY";
const QString CMD_CONTINUOUS_GET_SINGLE_YUV = "CONTINUOUS_GET_SINGLE_YUV";
const QString CMD_BRACKET_GET_SINGLE_RAW = "BRACKET_GET_SINGLE_RAW";
const QString CMD_BRACKET_GET_SINGLE_YUV = "BRACKET_GET_SINGLE_YUV";
const QString CMD_BRACKET_GET_AE_BIN = "BRACKET_GET_AE_BIN";
const QString CMD_BRACKET_GET_AWB_BIN = "BRACKET_GET_AWB_BIN";
const QString CMD_BRACKET_START = "BRACKET_START";
const QString CMD_BRACKET_FINISH = "BRACKET_FINISH";
const QString CMD_BRACKET_GET_DATA = "BRACKET_GET_DATA";
const QString CMD_BRACKET_GET_MULTI_AWB_BIN = "BRACKET_GET_MULTI_AWB_BIN";
const QString CMD_GET_TOOL_JSON = "GET_TOOL_JSON";
const QString CMD_GET_DRC_HISTOGRAM_INFO = "GET_DRC_HISTOGRAM_INFO";
const QString CMD_CALI_GET_MULTI_RAW = "CALI_GET_MULTI_RAW";
const QString CMD_CALI_GET_AE_BIN = "CALI_GET_AE_BIN";
const QString CMD_CALI_GET_AWB_BIN = "CALI_GET_AWB_BIN";
const QString CMD_I2C_READ = "I2C_READ";
const QString CMD_I2C_WRITE = "I2C_WRITE";
const QString CMD_REG_GET = "REG_GET";
const QString CMD_REG_SET = "REG_SET";
const QString CMD_RAWREPLAY_GET_MULTI_YUV = "RAWREPLAY_GET_MULTI_YUV";
const QString CMD_CONTINUOUS_GET_MAX_FRAMES = "CONTINUOUS_GET_MAX_FRAMES";
const QString CMD_CTRL_CMODEL = "CTRL_CMODEL";
const QString CMD_DISCONNECT_FROM_UART = "Close_Uart";
const QString CMD_SPI_TRANSMIT = "SPI_TRANSMIT";
const QString CMD_GET_FSHDR_MAPCURVE_INFO = "GET_FSHDR_MAPCURVE_INFO";

const QString METHOD_GET_SINGLE_YUV = "CVI_GetSingleYUVFrame";
const QString METHOD_GET_MULTI_YUV = "CVI_GetMultipleYUVFrame";
const QString METHOD_GET_IMAGE_DATA = "CVI_GetImageData";
const QString METHOD_STOP_GET_IMAGE_DATA = "CVI_StopGetImageData";
const QString METHOD_GET_MULTI_RAW = "CVI_GetMultipleRAWFrame";
const QString METHOD_PREPARE_BIN = "CVI_PrepareBinaryData";
const QString METHOD_GET_BIN_INFO = "CVI_GetBinaryInfo";
const QString METHOD_GET_BIN_DATA = "CVI_GetBinaryData";
const QString METHOD_EXPORT_BIN = "CVI_ExportBinFile";
const QString METHOD_SEND_BIN_INFO = "CVI_SetBinaryData";
const QString METHOD_FIX_BIN_TO_FLASH = "CVI_FixBinToFlash";
const QString METHOD_GET_WDR_INFO = "CVI_GetWDRInfo";
const QString METHOD_GET_3A_STATISTICS_INFO = "CVI_Get3AStatisticsInfo";
const QString METHOD_GET_AF_STATISTICS_INFO = "CVI_GetAFStatisticsInfo";
const QString METHOD_START_RAW_REPLAY = "CVI_ISP_StartRawReplay";
const QString METHOD_CANCEL_RAW_REPLAY = "CVI_ISP_CancelRawReplay";
const QString METHOD_BRACKET_START = "CVI_StartBracketing";
const QString METHOD_BRACKET_FINISH = "CVI_FinishBracketing";
const QString METHOD_BRACKET_GET_DATA = "CVI_GetBracketingData";
const QString METHOD_GET_CHIP_INFO = "CVI_GetChipInfo";
const QString METHOD_GET_DRC_HISTOGRAM_INFO = "CVI_GetDRCHistogramInfo";
const QString METHOD_CVI_ISP_QueryExposureInfo = "CVI_ISP_QueryExposureInfo";
const QString METHOD_I2C_READ = "CVI_I2cRead";
const QString METHOD_I2C_WRITE = "CVI_I2cWrite";
const QString METHOD_REG_GET = "CVI_RegGet";
const QString METHOD_REG_SET = "CVI_RegSet";
const QString METHOD_GET_CONTINUOUS_MAX_FRAMES = "CVI_GetRawDumpMaxFrames";
const QString METHOD_GET_VI_VPSS_LDC_CHN_SIZE = "CVI_VI_VPSS_GetChnLDCSize";
const QString METHOD_DISCONNECT_FROM_UART = "CVI_Uart_Close";
const QString METHOD_SPI_TRANSMIT = "CVI_SPI_Transmit";
const QString METHOD_GET_FSHDR_MAPCURVE_INFO = "CVI_GetFSHDRMapCurveInfo";

const int SEND_IMAGE_BLOB_HEADER_MAX = 16;
const int READ_TIMEOUT = 3; // second

enum class CONNECTION_MODE {
    NORMAL,
    GET_TEMPLATE,
};

using namespace std;

class NetworkClientHelper : public QObject
{
    Q_OBJECT

public:
    explicit NetworkClientHelper();
    ~NetworkClientHelper();
    static shared_ptr<NetworkClientHelper> getInstance();
    void connect();
    void disconnect(bool emit_log = false);
    bool sendCmd(QString cmd);
    bool sendCmd(QJsonObject &&obj, bool flag = true);
    void parseCmdData(const QJsonObject &obj);
    void parseImageData(const QJsonObject &obj);
    QByteArray popCmdData(QString target_cmd);
    void clearData();
    bool isNormalStatus();
    void setMode(bool mode);
    bool sendData(QByteArray &data);
    void setCounts(int totalCounts);
    void clearFlags();

    void connectJsonrpc(CONNECTION_MODE mode);
    void disconnectJsonrpc(bool emit_log = false);
    void parseCmdDataJsonrpc(const QJsonObject &obj);
    QJsonObject popCmdDataJsonrpc(int id);
    void pushIdInfoJsonrpc(int id, const QJsonObject &obj) { id_info_map.insert(id, obj); }
    void removeIdInfoJsonrpc(int id) { id_info_map.remove(id); }
    void clearIdInfoJsonrpc() {
        if (id_info_map.size() > 2)
        {
            QMap<int, QJsonObject>::const_iterator iter = id_info_map.constBegin();
            id_info_map.remove(iter.key());
        }
    }
    void setCallerInfo(int size, QString cmd);
    void getWDRInfo();
    void getTemplateFile();
    void waitForUartSend(int ms = 10000) { uart_.waitForBytesWritten(ms); }

signals:
    void connected_uart();
    void disconnected_uart();
    void sigStatusLog(QString message, LOG_LEVEL log_level);
    void sigReceiveCmdResp(QString cmd);
    void sigReceiveSingleImageData(QString cmd, QString status, QString info);
    void sigReceiveImageData(QString cmd);
    void sigReceiveMultiYuvData(QString cmd, QString status);
    void sigReceiveYuvData(QString cmd);
    void sigReceiveMultiRawData(QString cmd, QString status);
    void sigReceiveRawData(QString cmd);
    void sigReceiveMultiBracketingData(QString cmd, QString status);
    void sigReceiveBracketingData(QString cmd);
    void sigReceiveBinFile(QString cmd);
    void sigReceiveBinMessage(QString cmd, QString status);
    void sigReceiveBlackLvlMsg(QString cmd);
    void sigReceiveRegMsg(QString cmd);
    void sigReceive3AMsg(QString cmd);
    void sigReceive3AImage(QString cmd);
    void sigReceiveGamma();
    void sigReceiveFocusAssistImage(QString cmd);
    void sigReceiveFocusAssistData(QString cmd);
    void sigReceiveChipInfo(QString cmd);
    void sigReceiveRawUtilityMessage(QString cmd, QString status);
    void sigReceiveBracketImage(QString cmd);
    void sigReceiveBracketRawData(QString cmd);
    void sigReceiveWDRMode(QString cmd);
    void sigReceiveContinuousYuv(QString cmd);
    void sigReadAllFinished();

    void sigReceiveCmdRespJsonrpc(int id);
    void sigReceiveGammaJsonrpc(int id);
    void sigReceiveCcmCalibration(int id);
    void sigReceiveRlscCalibration(int id);
    void sigReceiveMlscCalibration(int id);
    void sigReceiveBlcCalibration(int id);
    void sigReceiveAWBCalibration(int id);
    void sigReceiveRgbirCalibration(int id);
    void sigReceiveNoiseProfileCalibration(int id);
    void sigPreviewYuvStatus(int id);
    void sigPreviewYuvData(QString cmd);
    void sigCaptueYuvStatus(int id);
    void sigCaptureYuvData(QString cmd);
    void sigGetYuvStatus(int id);
    void sigGetYuvData(QString cmd);
    void sigCaptueRawStatus(int id);
    void sigCaptue3ARawStatus(int id);
    void sig3ARawData(QString cmd);
    void sigCaptureBinInfo(int id);
    void sigCaptureBinData(QString cmd);
    void sigCaptureRawData(QString cmd);
    void sigBinUtilStatus(int id);
    void sigBinUtilData(QString cmd);
    void sig3AYuvStatus(int id);
    void sig3AYuvData(QString cmd);
    void sigAFYuvStatus(int id);
    void sigAFYuvData(QString cmd);
    void sigGetWDRMode(int id);
    void sig3AStatisticsInfo(int id);
    void sig3ABinInfo(int id);
    void sig3ABinData(QString cmd);
    void sigAFStatisticsInfo(int id);
    void sigRawUtilityMessage(int id);
    void sigContinuousYuvStatus(int id);
    void sigContinuousYuvData(QString cmd);
    void sigBracketSingleRawStatus(int id);
    void sigBracketBinInfo(int id);
    void sigBracketBinData(QString cmd);
    void sigBracketSingleRawData(QString cmd);
    void sigBracketSingleYuvStatus(int id);
    void sigBracketSingleYuvData(QString cmd);
    void sigBracketStatus(int id);
    void sigBracketData(QString cmd);
    void sigToolJsonStatus(int id);
    void sigToolJsonData(QString cmd);
    void sigChipInfoStatus(int id);
    void sigRecDrcHistogramInfo(int id);
    void sigReadAllPageData();
    void sigEnableAllPageButtons(QString module, QString cmd, BUTTON_MODE mode);
    void sigReceiveEvValue(int id);
    void sigReceiveLdcSize(int id);
    void sigSendLDCBinData(int id);
    void sigCaliRawStatus(int id);
    void sigCaliBinInfo(int id);
    void sigCaliBinData(QString cmd);
    void sigCaliRawData(QString cmd);
    void sigI2CReadStatus(int id);
    void sigI2CWriteStatus(int id);
    void sigRegGetStatus(int id);
    void sigRegSetStatus(int id);
    void sigSPITransmitStatus(int id);
    void sigRawUtilYuvStatus(int id);
    void sigRawUtilYuvData(QString cmd);
    void sigContinuousGetMaxFrames(int id);
    void sigCtrlCmodel(int id);
    void sigRecFshdrMapcurveInfo(int id);

public slots:
    void handleConnectStatteChanged(QAbstractSocket::SocketState state);
    void handleConnected();
    void handleDisconnected();
    void onMessageReceived(QString message);
    void onFrameReceived(QByteArray frame, bool isLastFrame);
    void handleReconnect();

    void handleConnectedJsonrpc();
    void handleReconnectJsonrpc();
    void handleDisconnectedJsonrpc();
    void onSocketReadyRead();
    void onUartReadyRead();
    void onFrameReceivedJasonrpc(const QByteArray frame);
    void onMessageReceivedJasonrpc(QByteArray data);

private:
    static shared_ptr<NetworkClientHelper> m_instance;
    QMutex data_mutex;
    QMap<QString, QList<QByteArray>> cmd_data_map_;
    QUrl url_;
    QWebSocket websocket_;
    QString message_;
    QTimer *conn_timer;
    bool normalMode = true;
    bool receiveRawHead = false;
    bool receiveYuvHead = false;
    bool receivePreYuvHead = false;
    bool receive3AYuvHead = false;
    bool receiveAfYuvHead = false;
    bool receiveBracketingRawHead = false;
    bool receiveBraYuvHead = false;
    bool receiveBraDumpRawHead = false;
    bool receiveConYuvHead = false;
    bool readAllPageParam = false;
    int totalCounts = 0;
    int receiveCounts = 0;

    QTcpSocket tcpsocket_;
    QMap<int, QJsonObject> id_data_map;
    QMap<int, QJsonObject> id_info_map;
    QByteArray received_data;

    QSerialPort uart_;

    int recvSize = 0;
    QString cmdReq;
    CONNECTION_MODE connection_mode;
    bool reconnect = true;
    QString last_connect_type;
};

#endif // NETWORK_CLIENT_HELPER_HPP
