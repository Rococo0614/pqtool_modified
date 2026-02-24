/****************************************************************************
** Meta object code from reading C++ file 'network_client_helper.hpp'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../model/include/network_client_helper.hpp"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'network_client_helper.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_NetworkClientHelper_t {
    QByteArrayData data[124];
    char stringdata0[2275];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_NetworkClientHelper_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_NetworkClientHelper_t qt_meta_stringdata_NetworkClientHelper = {
    {
QT_MOC_LITERAL(0, 0, 19), // "NetworkClientHelper"
QT_MOC_LITERAL(1, 20, 14), // "connected_uart"
QT_MOC_LITERAL(2, 35, 0), // ""
QT_MOC_LITERAL(3, 36, 17), // "disconnected_uart"
QT_MOC_LITERAL(4, 54, 12), // "sigStatusLog"
QT_MOC_LITERAL(5, 67, 7), // "message"
QT_MOC_LITERAL(6, 75, 9), // "LOG_LEVEL"
QT_MOC_LITERAL(7, 85, 9), // "log_level"
QT_MOC_LITERAL(8, 95, 17), // "sigReceiveCmdResp"
QT_MOC_LITERAL(9, 113, 3), // "cmd"
QT_MOC_LITERAL(10, 117, 25), // "sigReceiveSingleImageData"
QT_MOC_LITERAL(11, 143, 6), // "status"
QT_MOC_LITERAL(12, 150, 4), // "info"
QT_MOC_LITERAL(13, 155, 19), // "sigReceiveImageData"
QT_MOC_LITERAL(14, 175, 22), // "sigReceiveMultiYuvData"
QT_MOC_LITERAL(15, 198, 17), // "sigReceiveYuvData"
QT_MOC_LITERAL(16, 216, 22), // "sigReceiveMultiRawData"
QT_MOC_LITERAL(17, 239, 17), // "sigReceiveRawData"
QT_MOC_LITERAL(18, 257, 29), // "sigReceiveMultiBracketingData"
QT_MOC_LITERAL(19, 287, 24), // "sigReceiveBracketingData"
QT_MOC_LITERAL(20, 312, 17), // "sigReceiveBinFile"
QT_MOC_LITERAL(21, 330, 20), // "sigReceiveBinMessage"
QT_MOC_LITERAL(22, 351, 21), // "sigReceiveBlackLvlMsg"
QT_MOC_LITERAL(23, 373, 16), // "sigReceiveRegMsg"
QT_MOC_LITERAL(24, 390, 15), // "sigReceive3AMsg"
QT_MOC_LITERAL(25, 406, 17), // "sigReceive3AImage"
QT_MOC_LITERAL(26, 424, 15), // "sigReceiveGamma"
QT_MOC_LITERAL(27, 440, 26), // "sigReceiveFocusAssistImage"
QT_MOC_LITERAL(28, 467, 25), // "sigReceiveFocusAssistData"
QT_MOC_LITERAL(29, 493, 18), // "sigReceiveChipInfo"
QT_MOC_LITERAL(30, 512, 27), // "sigReceiveRawUtilityMessage"
QT_MOC_LITERAL(31, 540, 22), // "sigReceiveBracketImage"
QT_MOC_LITERAL(32, 563, 24), // "sigReceiveBracketRawData"
QT_MOC_LITERAL(33, 588, 17), // "sigReceiveWDRMode"
QT_MOC_LITERAL(34, 606, 23), // "sigReceiveContinuousYuv"
QT_MOC_LITERAL(35, 630, 18), // "sigReadAllFinished"
QT_MOC_LITERAL(36, 649, 24), // "sigReceiveCmdRespJsonrpc"
QT_MOC_LITERAL(37, 674, 2), // "id"
QT_MOC_LITERAL(38, 677, 22), // "sigReceiveGammaJsonrpc"
QT_MOC_LITERAL(39, 700, 24), // "sigReceiveCcmCalibration"
QT_MOC_LITERAL(40, 725, 25), // "sigReceiveRlscCalibration"
QT_MOC_LITERAL(41, 751, 25), // "sigReceiveMlscCalibration"
QT_MOC_LITERAL(42, 777, 24), // "sigReceiveBlcCalibration"
QT_MOC_LITERAL(43, 802, 24), // "sigReceiveAWBCalibration"
QT_MOC_LITERAL(44, 827, 26), // "sigReceiveRgbirCalibration"
QT_MOC_LITERAL(45, 854, 33), // "sigReceiveNoiseProfileCalibra..."
QT_MOC_LITERAL(46, 888, 19), // "sigPreviewYuvStatus"
QT_MOC_LITERAL(47, 908, 17), // "sigPreviewYuvData"
QT_MOC_LITERAL(48, 926, 18), // "sigCaptueYuvStatus"
QT_MOC_LITERAL(49, 945, 17), // "sigCaptureYuvData"
QT_MOC_LITERAL(50, 963, 15), // "sigGetYuvStatus"
QT_MOC_LITERAL(51, 979, 13), // "sigGetYuvData"
QT_MOC_LITERAL(52, 993, 18), // "sigCaptueRawStatus"
QT_MOC_LITERAL(53, 1012, 20), // "sigCaptue3ARawStatus"
QT_MOC_LITERAL(54, 1033, 12), // "sig3ARawData"
QT_MOC_LITERAL(55, 1046, 17), // "sigCaptureBinInfo"
QT_MOC_LITERAL(56, 1064, 17), // "sigCaptureBinData"
QT_MOC_LITERAL(57, 1082, 17), // "sigCaptureRawData"
QT_MOC_LITERAL(58, 1100, 16), // "sigBinUtilStatus"
QT_MOC_LITERAL(59, 1117, 14), // "sigBinUtilData"
QT_MOC_LITERAL(60, 1132, 14), // "sig3AYuvStatus"
QT_MOC_LITERAL(61, 1147, 12), // "sig3AYuvData"
QT_MOC_LITERAL(62, 1160, 14), // "sigAFYuvStatus"
QT_MOC_LITERAL(63, 1175, 12), // "sigAFYuvData"
QT_MOC_LITERAL(64, 1188, 13), // "sigGetWDRMode"
QT_MOC_LITERAL(65, 1202, 19), // "sig3AStatisticsInfo"
QT_MOC_LITERAL(66, 1222, 12), // "sig3ABinInfo"
QT_MOC_LITERAL(67, 1235, 12), // "sig3ABinData"
QT_MOC_LITERAL(68, 1248, 19), // "sigAFStatisticsInfo"
QT_MOC_LITERAL(69, 1268, 20), // "sigRawUtilityMessage"
QT_MOC_LITERAL(70, 1289, 22), // "sigContinuousYuvStatus"
QT_MOC_LITERAL(71, 1312, 20), // "sigContinuousYuvData"
QT_MOC_LITERAL(72, 1333, 25), // "sigBracketSingleRawStatus"
QT_MOC_LITERAL(73, 1359, 17), // "sigBracketBinInfo"
QT_MOC_LITERAL(74, 1377, 17), // "sigBracketBinData"
QT_MOC_LITERAL(75, 1395, 23), // "sigBracketSingleRawData"
QT_MOC_LITERAL(76, 1419, 25), // "sigBracketSingleYuvStatus"
QT_MOC_LITERAL(77, 1445, 23), // "sigBracketSingleYuvData"
QT_MOC_LITERAL(78, 1469, 16), // "sigBracketStatus"
QT_MOC_LITERAL(79, 1486, 14), // "sigBracketData"
QT_MOC_LITERAL(80, 1501, 17), // "sigToolJsonStatus"
QT_MOC_LITERAL(81, 1519, 15), // "sigToolJsonData"
QT_MOC_LITERAL(82, 1535, 17), // "sigChipInfoStatus"
QT_MOC_LITERAL(83, 1553, 22), // "sigRecDrcHistogramInfo"
QT_MOC_LITERAL(84, 1576, 18), // "sigReadAllPageData"
QT_MOC_LITERAL(85, 1595, 23), // "sigEnableAllPageButtons"
QT_MOC_LITERAL(86, 1619, 6), // "module"
QT_MOC_LITERAL(87, 1626, 11), // "BUTTON_MODE"
QT_MOC_LITERAL(88, 1638, 4), // "mode"
QT_MOC_LITERAL(89, 1643, 17), // "sigReceiveEvValue"
QT_MOC_LITERAL(90, 1661, 17), // "sigReceiveLdcSize"
QT_MOC_LITERAL(91, 1679, 17), // "sigSendLDCBinData"
QT_MOC_LITERAL(92, 1697, 16), // "sigCaliRawStatus"
QT_MOC_LITERAL(93, 1714, 14), // "sigCaliBinInfo"
QT_MOC_LITERAL(94, 1729, 14), // "sigCaliBinData"
QT_MOC_LITERAL(95, 1744, 14), // "sigCaliRawData"
QT_MOC_LITERAL(96, 1759, 16), // "sigI2CReadStatus"
QT_MOC_LITERAL(97, 1776, 17), // "sigI2CWriteStatus"
QT_MOC_LITERAL(98, 1794, 15), // "sigRegGetStatus"
QT_MOC_LITERAL(99, 1810, 15), // "sigRegSetStatus"
QT_MOC_LITERAL(100, 1826, 20), // "sigSPITransmitStatus"
QT_MOC_LITERAL(101, 1847, 19), // "sigRawUtilYuvStatus"
QT_MOC_LITERAL(102, 1867, 17), // "sigRawUtilYuvData"
QT_MOC_LITERAL(103, 1885, 25), // "sigContinuousGetMaxFrames"
QT_MOC_LITERAL(104, 1911, 13), // "sigCtrlCmodel"
QT_MOC_LITERAL(105, 1925, 23), // "sigRecFshdrMapcurveInfo"
QT_MOC_LITERAL(106, 1949, 26), // "handleConnectStatteChanged"
QT_MOC_LITERAL(107, 1976, 28), // "QAbstractSocket::SocketState"
QT_MOC_LITERAL(108, 2005, 5), // "state"
QT_MOC_LITERAL(109, 2011, 15), // "handleConnected"
QT_MOC_LITERAL(110, 2027, 18), // "handleDisconnected"
QT_MOC_LITERAL(111, 2046, 17), // "onMessageReceived"
QT_MOC_LITERAL(112, 2064, 15), // "onFrameReceived"
QT_MOC_LITERAL(113, 2080, 5), // "frame"
QT_MOC_LITERAL(114, 2086, 11), // "isLastFrame"
QT_MOC_LITERAL(115, 2098, 15), // "handleReconnect"
QT_MOC_LITERAL(116, 2114, 22), // "handleConnectedJsonrpc"
QT_MOC_LITERAL(117, 2137, 22), // "handleReconnectJsonrpc"
QT_MOC_LITERAL(118, 2160, 25), // "handleDisconnectedJsonrpc"
QT_MOC_LITERAL(119, 2186, 17), // "onSocketReadyRead"
QT_MOC_LITERAL(120, 2204, 15), // "onUartReadyRead"
QT_MOC_LITERAL(121, 2220, 23), // "onFrameReceivedJasonrpc"
QT_MOC_LITERAL(122, 2244, 25), // "onMessageReceivedJasonrpc"
QT_MOC_LITERAL(123, 2270, 4) // "data"

    },
    "NetworkClientHelper\0connected_uart\0\0"
    "disconnected_uart\0sigStatusLog\0message\0"
    "LOG_LEVEL\0log_level\0sigReceiveCmdResp\0"
    "cmd\0sigReceiveSingleImageData\0status\0"
    "info\0sigReceiveImageData\0"
    "sigReceiveMultiYuvData\0sigReceiveYuvData\0"
    "sigReceiveMultiRawData\0sigReceiveRawData\0"
    "sigReceiveMultiBracketingData\0"
    "sigReceiveBracketingData\0sigReceiveBinFile\0"
    "sigReceiveBinMessage\0sigReceiveBlackLvlMsg\0"
    "sigReceiveRegMsg\0sigReceive3AMsg\0"
    "sigReceive3AImage\0sigReceiveGamma\0"
    "sigReceiveFocusAssistImage\0"
    "sigReceiveFocusAssistData\0sigReceiveChipInfo\0"
    "sigReceiveRawUtilityMessage\0"
    "sigReceiveBracketImage\0sigReceiveBracketRawData\0"
    "sigReceiveWDRMode\0sigReceiveContinuousYuv\0"
    "sigReadAllFinished\0sigReceiveCmdRespJsonrpc\0"
    "id\0sigReceiveGammaJsonrpc\0"
    "sigReceiveCcmCalibration\0"
    "sigReceiveRlscCalibration\0"
    "sigReceiveMlscCalibration\0"
    "sigReceiveBlcCalibration\0"
    "sigReceiveAWBCalibration\0"
    "sigReceiveRgbirCalibration\0"
    "sigReceiveNoiseProfileCalibration\0"
    "sigPreviewYuvStatus\0sigPreviewYuvData\0"
    "sigCaptueYuvStatus\0sigCaptureYuvData\0"
    "sigGetYuvStatus\0sigGetYuvData\0"
    "sigCaptueRawStatus\0sigCaptue3ARawStatus\0"
    "sig3ARawData\0sigCaptureBinInfo\0"
    "sigCaptureBinData\0sigCaptureRawData\0"
    "sigBinUtilStatus\0sigBinUtilData\0"
    "sig3AYuvStatus\0sig3AYuvData\0sigAFYuvStatus\0"
    "sigAFYuvData\0sigGetWDRMode\0"
    "sig3AStatisticsInfo\0sig3ABinInfo\0"
    "sig3ABinData\0sigAFStatisticsInfo\0"
    "sigRawUtilityMessage\0sigContinuousYuvStatus\0"
    "sigContinuousYuvData\0sigBracketSingleRawStatus\0"
    "sigBracketBinInfo\0sigBracketBinData\0"
    "sigBracketSingleRawData\0"
    "sigBracketSingleYuvStatus\0"
    "sigBracketSingleYuvData\0sigBracketStatus\0"
    "sigBracketData\0sigToolJsonStatus\0"
    "sigToolJsonData\0sigChipInfoStatus\0"
    "sigRecDrcHistogramInfo\0sigReadAllPageData\0"
    "sigEnableAllPageButtons\0module\0"
    "BUTTON_MODE\0mode\0sigReceiveEvValue\0"
    "sigReceiveLdcSize\0sigSendLDCBinData\0"
    "sigCaliRawStatus\0sigCaliBinInfo\0"
    "sigCaliBinData\0sigCaliRawData\0"
    "sigI2CReadStatus\0sigI2CWriteStatus\0"
    "sigRegGetStatus\0sigRegSetStatus\0"
    "sigSPITransmitStatus\0sigRawUtilYuvStatus\0"
    "sigRawUtilYuvData\0sigContinuousGetMaxFrames\0"
    "sigCtrlCmodel\0sigRecFshdrMapcurveInfo\0"
    "handleConnectStatteChanged\0"
    "QAbstractSocket::SocketState\0state\0"
    "handleConnected\0handleDisconnected\0"
    "onMessageReceived\0onFrameReceived\0"
    "frame\0isLastFrame\0handleReconnect\0"
    "handleConnectedJsonrpc\0handleReconnectJsonrpc\0"
    "handleDisconnectedJsonrpc\0onSocketReadyRead\0"
    "onUartReadyRead\0onFrameReceivedJasonrpc\0"
    "onMessageReceivedJasonrpc\0data"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_NetworkClientHelper[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
     107,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      94,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,  549,    2, 0x06 /* Public */,
       3,    0,  550,    2, 0x06 /* Public */,
       4,    2,  551,    2, 0x06 /* Public */,
       8,    1,  556,    2, 0x06 /* Public */,
      10,    3,  559,    2, 0x06 /* Public */,
      13,    1,  566,    2, 0x06 /* Public */,
      14,    2,  569,    2, 0x06 /* Public */,
      15,    1,  574,    2, 0x06 /* Public */,
      16,    2,  577,    2, 0x06 /* Public */,
      17,    1,  582,    2, 0x06 /* Public */,
      18,    2,  585,    2, 0x06 /* Public */,
      19,    1,  590,    2, 0x06 /* Public */,
      20,    1,  593,    2, 0x06 /* Public */,
      21,    2,  596,    2, 0x06 /* Public */,
      22,    1,  601,    2, 0x06 /* Public */,
      23,    1,  604,    2, 0x06 /* Public */,
      24,    1,  607,    2, 0x06 /* Public */,
      25,    1,  610,    2, 0x06 /* Public */,
      26,    0,  613,    2, 0x06 /* Public */,
      27,    1,  614,    2, 0x06 /* Public */,
      28,    1,  617,    2, 0x06 /* Public */,
      29,    1,  620,    2, 0x06 /* Public */,
      30,    2,  623,    2, 0x06 /* Public */,
      31,    1,  628,    2, 0x06 /* Public */,
      32,    1,  631,    2, 0x06 /* Public */,
      33,    1,  634,    2, 0x06 /* Public */,
      34,    1,  637,    2, 0x06 /* Public */,
      35,    0,  640,    2, 0x06 /* Public */,
      36,    1,  641,    2, 0x06 /* Public */,
      38,    1,  644,    2, 0x06 /* Public */,
      39,    1,  647,    2, 0x06 /* Public */,
      40,    1,  650,    2, 0x06 /* Public */,
      41,    1,  653,    2, 0x06 /* Public */,
      42,    1,  656,    2, 0x06 /* Public */,
      43,    1,  659,    2, 0x06 /* Public */,
      44,    1,  662,    2, 0x06 /* Public */,
      45,    1,  665,    2, 0x06 /* Public */,
      46,    1,  668,    2, 0x06 /* Public */,
      47,    1,  671,    2, 0x06 /* Public */,
      48,    1,  674,    2, 0x06 /* Public */,
      49,    1,  677,    2, 0x06 /* Public */,
      50,    1,  680,    2, 0x06 /* Public */,
      51,    1,  683,    2, 0x06 /* Public */,
      52,    1,  686,    2, 0x06 /* Public */,
      53,    1,  689,    2, 0x06 /* Public */,
      54,    1,  692,    2, 0x06 /* Public */,
      55,    1,  695,    2, 0x06 /* Public */,
      56,    1,  698,    2, 0x06 /* Public */,
      57,    1,  701,    2, 0x06 /* Public */,
      58,    1,  704,    2, 0x06 /* Public */,
      59,    1,  707,    2, 0x06 /* Public */,
      60,    1,  710,    2, 0x06 /* Public */,
      61,    1,  713,    2, 0x06 /* Public */,
      62,    1,  716,    2, 0x06 /* Public */,
      63,    1,  719,    2, 0x06 /* Public */,
      64,    1,  722,    2, 0x06 /* Public */,
      65,    1,  725,    2, 0x06 /* Public */,
      66,    1,  728,    2, 0x06 /* Public */,
      67,    1,  731,    2, 0x06 /* Public */,
      68,    1,  734,    2, 0x06 /* Public */,
      69,    1,  737,    2, 0x06 /* Public */,
      70,    1,  740,    2, 0x06 /* Public */,
      71,    1,  743,    2, 0x06 /* Public */,
      72,    1,  746,    2, 0x06 /* Public */,
      73,    1,  749,    2, 0x06 /* Public */,
      74,    1,  752,    2, 0x06 /* Public */,
      75,    1,  755,    2, 0x06 /* Public */,
      76,    1,  758,    2, 0x06 /* Public */,
      77,    1,  761,    2, 0x06 /* Public */,
      78,    1,  764,    2, 0x06 /* Public */,
      79,    1,  767,    2, 0x06 /* Public */,
      80,    1,  770,    2, 0x06 /* Public */,
      81,    1,  773,    2, 0x06 /* Public */,
      82,    1,  776,    2, 0x06 /* Public */,
      83,    1,  779,    2, 0x06 /* Public */,
      84,    0,  782,    2, 0x06 /* Public */,
      85,    3,  783,    2, 0x06 /* Public */,
      89,    1,  790,    2, 0x06 /* Public */,
      90,    1,  793,    2, 0x06 /* Public */,
      91,    1,  796,    2, 0x06 /* Public */,
      92,    1,  799,    2, 0x06 /* Public */,
      93,    1,  802,    2, 0x06 /* Public */,
      94,    1,  805,    2, 0x06 /* Public */,
      95,    1,  808,    2, 0x06 /* Public */,
      96,    1,  811,    2, 0x06 /* Public */,
      97,    1,  814,    2, 0x06 /* Public */,
      98,    1,  817,    2, 0x06 /* Public */,
      99,    1,  820,    2, 0x06 /* Public */,
     100,    1,  823,    2, 0x06 /* Public */,
     101,    1,  826,    2, 0x06 /* Public */,
     102,    1,  829,    2, 0x06 /* Public */,
     103,    1,  832,    2, 0x06 /* Public */,
     104,    1,  835,    2, 0x06 /* Public */,
     105,    1,  838,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
     106,    1,  841,    2, 0x0a /* Public */,
     109,    0,  844,    2, 0x0a /* Public */,
     110,    0,  845,    2, 0x0a /* Public */,
     111,    1,  846,    2, 0x0a /* Public */,
     112,    2,  849,    2, 0x0a /* Public */,
     115,    0,  854,    2, 0x0a /* Public */,
     116,    0,  855,    2, 0x0a /* Public */,
     117,    0,  856,    2, 0x0a /* Public */,
     118,    0,  857,    2, 0x0a /* Public */,
     119,    0,  858,    2, 0x0a /* Public */,
     120,    0,  859,    2, 0x0a /* Public */,
     121,    1,  860,    2, 0x0a /* Public */,
     122,    1,  863,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, 0x80000000 | 6,    5,    7,
    QMetaType::Void, QMetaType::QString,    9,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::QString,    9,   11,   12,
    QMetaType::Void, QMetaType::QString,    9,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,    9,   11,
    QMetaType::Void, QMetaType::QString,    9,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,    9,   11,
    QMetaType::Void, QMetaType::QString,    9,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,    9,   11,
    QMetaType::Void, QMetaType::QString,    9,
    QMetaType::Void, QMetaType::QString,    9,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,    9,   11,
    QMetaType::Void, QMetaType::QString,    9,
    QMetaType::Void, QMetaType::QString,    9,
    QMetaType::Void, QMetaType::QString,    9,
    QMetaType::Void, QMetaType::QString,    9,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    9,
    QMetaType::Void, QMetaType::QString,    9,
    QMetaType::Void, QMetaType::QString,    9,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,    9,   11,
    QMetaType::Void, QMetaType::QString,    9,
    QMetaType::Void, QMetaType::QString,    9,
    QMetaType::Void, QMetaType::QString,    9,
    QMetaType::Void, QMetaType::QString,    9,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   37,
    QMetaType::Void, QMetaType::Int,   37,
    QMetaType::Void, QMetaType::Int,   37,
    QMetaType::Void, QMetaType::Int,   37,
    QMetaType::Void, QMetaType::Int,   37,
    QMetaType::Void, QMetaType::Int,   37,
    QMetaType::Void, QMetaType::Int,   37,
    QMetaType::Void, QMetaType::Int,   37,
    QMetaType::Void, QMetaType::Int,   37,
    QMetaType::Void, QMetaType::Int,   37,
    QMetaType::Void, QMetaType::QString,    9,
    QMetaType::Void, QMetaType::Int,   37,
    QMetaType::Void, QMetaType::QString,    9,
    QMetaType::Void, QMetaType::Int,   37,
    QMetaType::Void, QMetaType::QString,    9,
    QMetaType::Void, QMetaType::Int,   37,
    QMetaType::Void, QMetaType::Int,   37,
    QMetaType::Void, QMetaType::QString,    9,
    QMetaType::Void, QMetaType::Int,   37,
    QMetaType::Void, QMetaType::QString,    9,
    QMetaType::Void, QMetaType::QString,    9,
    QMetaType::Void, QMetaType::Int,   37,
    QMetaType::Void, QMetaType::QString,    9,
    QMetaType::Void, QMetaType::Int,   37,
    QMetaType::Void, QMetaType::QString,    9,
    QMetaType::Void, QMetaType::Int,   37,
    QMetaType::Void, QMetaType::QString,    9,
    QMetaType::Void, QMetaType::Int,   37,
    QMetaType::Void, QMetaType::Int,   37,
    QMetaType::Void, QMetaType::Int,   37,
    QMetaType::Void, QMetaType::QString,    9,
    QMetaType::Void, QMetaType::Int,   37,
    QMetaType::Void, QMetaType::Int,   37,
    QMetaType::Void, QMetaType::Int,   37,
    QMetaType::Void, QMetaType::QString,    9,
    QMetaType::Void, QMetaType::Int,   37,
    QMetaType::Void, QMetaType::Int,   37,
    QMetaType::Void, QMetaType::QString,    9,
    QMetaType::Void, QMetaType::QString,    9,
    QMetaType::Void, QMetaType::Int,   37,
    QMetaType::Void, QMetaType::QString,    9,
    QMetaType::Void, QMetaType::Int,   37,
    QMetaType::Void, QMetaType::QString,    9,
    QMetaType::Void, QMetaType::Int,   37,
    QMetaType::Void, QMetaType::QString,    9,
    QMetaType::Void, QMetaType::Int,   37,
    QMetaType::Void, QMetaType::Int,   37,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, 0x80000000 | 87,   86,    9,   88,
    QMetaType::Void, QMetaType::Int,   37,
    QMetaType::Void, QMetaType::Int,   37,
    QMetaType::Void, QMetaType::Int,   37,
    QMetaType::Void, QMetaType::Int,   37,
    QMetaType::Void, QMetaType::Int,   37,
    QMetaType::Void, QMetaType::QString,    9,
    QMetaType::Void, QMetaType::QString,    9,
    QMetaType::Void, QMetaType::Int,   37,
    QMetaType::Void, QMetaType::Int,   37,
    QMetaType::Void, QMetaType::Int,   37,
    QMetaType::Void, QMetaType::Int,   37,
    QMetaType::Void, QMetaType::Int,   37,
    QMetaType::Void, QMetaType::Int,   37,
    QMetaType::Void, QMetaType::QString,    9,
    QMetaType::Void, QMetaType::Int,   37,
    QMetaType::Void, QMetaType::Int,   37,
    QMetaType::Void, QMetaType::Int,   37,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 107,  108,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    5,
    QMetaType::Void, QMetaType::QByteArray, QMetaType::Bool,  113,  114,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QByteArray,  113,
    QMetaType::Void, QMetaType::QByteArray,  123,

       0        // eod
};

void NetworkClientHelper::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<NetworkClientHelper *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->connected_uart(); break;
        case 1: _t->disconnected_uart(); break;
        case 2: _t->sigStatusLog((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< LOG_LEVEL(*)>(_a[2]))); break;
        case 3: _t->sigReceiveCmdResp((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 4: _t->sigReceiveSingleImageData((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3]))); break;
        case 5: _t->sigReceiveImageData((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 6: _t->sigReceiveMultiYuvData((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 7: _t->sigReceiveYuvData((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 8: _t->sigReceiveMultiRawData((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 9: _t->sigReceiveRawData((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 10: _t->sigReceiveMultiBracketingData((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 11: _t->sigReceiveBracketingData((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 12: _t->sigReceiveBinFile((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 13: _t->sigReceiveBinMessage((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 14: _t->sigReceiveBlackLvlMsg((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 15: _t->sigReceiveRegMsg((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 16: _t->sigReceive3AMsg((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 17: _t->sigReceive3AImage((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 18: _t->sigReceiveGamma(); break;
        case 19: _t->sigReceiveFocusAssistImage((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 20: _t->sigReceiveFocusAssistData((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 21: _t->sigReceiveChipInfo((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 22: _t->sigReceiveRawUtilityMessage((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 23: _t->sigReceiveBracketImage((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 24: _t->sigReceiveBracketRawData((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 25: _t->sigReceiveWDRMode((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 26: _t->sigReceiveContinuousYuv((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 27: _t->sigReadAllFinished(); break;
        case 28: _t->sigReceiveCmdRespJsonrpc((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 29: _t->sigReceiveGammaJsonrpc((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 30: _t->sigReceiveCcmCalibration((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 31: _t->sigReceiveRlscCalibration((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 32: _t->sigReceiveMlscCalibration((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 33: _t->sigReceiveBlcCalibration((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 34: _t->sigReceiveAWBCalibration((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 35: _t->sigReceiveRgbirCalibration((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 36: _t->sigReceiveNoiseProfileCalibration((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 37: _t->sigPreviewYuvStatus((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 38: _t->sigPreviewYuvData((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 39: _t->sigCaptueYuvStatus((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 40: _t->sigCaptureYuvData((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 41: _t->sigGetYuvStatus((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 42: _t->sigGetYuvData((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 43: _t->sigCaptueRawStatus((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 44: _t->sigCaptue3ARawStatus((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 45: _t->sig3ARawData((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 46: _t->sigCaptureBinInfo((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 47: _t->sigCaptureBinData((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 48: _t->sigCaptureRawData((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 49: _t->sigBinUtilStatus((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 50: _t->sigBinUtilData((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 51: _t->sig3AYuvStatus((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 52: _t->sig3AYuvData((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 53: _t->sigAFYuvStatus((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 54: _t->sigAFYuvData((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 55: _t->sigGetWDRMode((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 56: _t->sig3AStatisticsInfo((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 57: _t->sig3ABinInfo((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 58: _t->sig3ABinData((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 59: _t->sigAFStatisticsInfo((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 60: _t->sigRawUtilityMessage((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 61: _t->sigContinuousYuvStatus((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 62: _t->sigContinuousYuvData((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 63: _t->sigBracketSingleRawStatus((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 64: _t->sigBracketBinInfo((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 65: _t->sigBracketBinData((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 66: _t->sigBracketSingleRawData((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 67: _t->sigBracketSingleYuvStatus((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 68: _t->sigBracketSingleYuvData((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 69: _t->sigBracketStatus((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 70: _t->sigBracketData((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 71: _t->sigToolJsonStatus((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 72: _t->sigToolJsonData((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 73: _t->sigChipInfoStatus((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 74: _t->sigRecDrcHistogramInfo((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 75: _t->sigReadAllPageData(); break;
        case 76: _t->sigEnableAllPageButtons((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< BUTTON_MODE(*)>(_a[3]))); break;
        case 77: _t->sigReceiveEvValue((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 78: _t->sigReceiveLdcSize((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 79: _t->sigSendLDCBinData((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 80: _t->sigCaliRawStatus((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 81: _t->sigCaliBinInfo((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 82: _t->sigCaliBinData((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 83: _t->sigCaliRawData((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 84: _t->sigI2CReadStatus((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 85: _t->sigI2CWriteStatus((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 86: _t->sigRegGetStatus((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 87: _t->sigRegSetStatus((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 88: _t->sigSPITransmitStatus((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 89: _t->sigRawUtilYuvStatus((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 90: _t->sigRawUtilYuvData((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 91: _t->sigContinuousGetMaxFrames((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 92: _t->sigCtrlCmodel((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 93: _t->sigRecFshdrMapcurveInfo((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 94: _t->handleConnectStatteChanged((*reinterpret_cast< QAbstractSocket::SocketState(*)>(_a[1]))); break;
        case 95: _t->handleConnected(); break;
        case 96: _t->handleDisconnected(); break;
        case 97: _t->onMessageReceived((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 98: _t->onFrameReceived((*reinterpret_cast< QByteArray(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 99: _t->handleReconnect(); break;
        case 100: _t->handleConnectedJsonrpc(); break;
        case 101: _t->handleReconnectJsonrpc(); break;
        case 102: _t->handleDisconnectedJsonrpc(); break;
        case 103: _t->onSocketReadyRead(); break;
        case 104: _t->onUartReadyRead(); break;
        case 105: _t->onFrameReceivedJasonrpc((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        case 106: _t->onMessageReceivedJasonrpc((*reinterpret_cast< QByteArray(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 94:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QAbstractSocket::SocketState >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (NetworkClientHelper::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkClientHelper::connected_uart)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (NetworkClientHelper::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkClientHelper::disconnected_uart)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (NetworkClientHelper::*)(QString , LOG_LEVEL );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkClientHelper::sigStatusLog)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (NetworkClientHelper::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkClientHelper::sigReceiveCmdResp)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (NetworkClientHelper::*)(QString , QString , QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkClientHelper::sigReceiveSingleImageData)) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (NetworkClientHelper::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkClientHelper::sigReceiveImageData)) {
                *result = 5;
                return;
            }
        }
        {
            using _t = void (NetworkClientHelper::*)(QString , QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkClientHelper::sigReceiveMultiYuvData)) {
                *result = 6;
                return;
            }
        }
        {
            using _t = void (NetworkClientHelper::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkClientHelper::sigReceiveYuvData)) {
                *result = 7;
                return;
            }
        }
        {
            using _t = void (NetworkClientHelper::*)(QString , QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkClientHelper::sigReceiveMultiRawData)) {
                *result = 8;
                return;
            }
        }
        {
            using _t = void (NetworkClientHelper::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkClientHelper::sigReceiveRawData)) {
                *result = 9;
                return;
            }
        }
        {
            using _t = void (NetworkClientHelper::*)(QString , QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkClientHelper::sigReceiveMultiBracketingData)) {
                *result = 10;
                return;
            }
        }
        {
            using _t = void (NetworkClientHelper::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkClientHelper::sigReceiveBracketingData)) {
                *result = 11;
                return;
            }
        }
        {
            using _t = void (NetworkClientHelper::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkClientHelper::sigReceiveBinFile)) {
                *result = 12;
                return;
            }
        }
        {
            using _t = void (NetworkClientHelper::*)(QString , QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkClientHelper::sigReceiveBinMessage)) {
                *result = 13;
                return;
            }
        }
        {
            using _t = void (NetworkClientHelper::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkClientHelper::sigReceiveBlackLvlMsg)) {
                *result = 14;
                return;
            }
        }
        {
            using _t = void (NetworkClientHelper::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkClientHelper::sigReceiveRegMsg)) {
                *result = 15;
                return;
            }
        }
        {
            using _t = void (NetworkClientHelper::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkClientHelper::sigReceive3AMsg)) {
                *result = 16;
                return;
            }
        }
        {
            using _t = void (NetworkClientHelper::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkClientHelper::sigReceive3AImage)) {
                *result = 17;
                return;
            }
        }
        {
            using _t = void (NetworkClientHelper::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkClientHelper::sigReceiveGamma)) {
                *result = 18;
                return;
            }
        }
        {
            using _t = void (NetworkClientHelper::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkClientHelper::sigReceiveFocusAssistImage)) {
                *result = 19;
                return;
            }
        }
        {
            using _t = void (NetworkClientHelper::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkClientHelper::sigReceiveFocusAssistData)) {
                *result = 20;
                return;
            }
        }
        {
            using _t = void (NetworkClientHelper::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkClientHelper::sigReceiveChipInfo)) {
                *result = 21;
                return;
            }
        }
        {
            using _t = void (NetworkClientHelper::*)(QString , QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkClientHelper::sigReceiveRawUtilityMessage)) {
                *result = 22;
                return;
            }
        }
        {
            using _t = void (NetworkClientHelper::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkClientHelper::sigReceiveBracketImage)) {
                *result = 23;
                return;
            }
        }
        {
            using _t = void (NetworkClientHelper::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkClientHelper::sigReceiveBracketRawData)) {
                *result = 24;
                return;
            }
        }
        {
            using _t = void (NetworkClientHelper::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkClientHelper::sigReceiveWDRMode)) {
                *result = 25;
                return;
            }
        }
        {
            using _t = void (NetworkClientHelper::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkClientHelper::sigReceiveContinuousYuv)) {
                *result = 26;
                return;
            }
        }
        {
            using _t = void (NetworkClientHelper::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkClientHelper::sigReadAllFinished)) {
                *result = 27;
                return;
            }
        }
        {
            using _t = void (NetworkClientHelper::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkClientHelper::sigReceiveCmdRespJsonrpc)) {
                *result = 28;
                return;
            }
        }
        {
            using _t = void (NetworkClientHelper::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkClientHelper::sigReceiveGammaJsonrpc)) {
                *result = 29;
                return;
            }
        }
        {
            using _t = void (NetworkClientHelper::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkClientHelper::sigReceiveCcmCalibration)) {
                *result = 30;
                return;
            }
        }
        {
            using _t = void (NetworkClientHelper::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkClientHelper::sigReceiveRlscCalibration)) {
                *result = 31;
                return;
            }
        }
        {
            using _t = void (NetworkClientHelper::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkClientHelper::sigReceiveMlscCalibration)) {
                *result = 32;
                return;
            }
        }
        {
            using _t = void (NetworkClientHelper::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkClientHelper::sigReceiveBlcCalibration)) {
                *result = 33;
                return;
            }
        }
        {
            using _t = void (NetworkClientHelper::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkClientHelper::sigReceiveAWBCalibration)) {
                *result = 34;
                return;
            }
        }
        {
            using _t = void (NetworkClientHelper::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkClientHelper::sigReceiveRgbirCalibration)) {
                *result = 35;
                return;
            }
        }
        {
            using _t = void (NetworkClientHelper::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkClientHelper::sigReceiveNoiseProfileCalibration)) {
                *result = 36;
                return;
            }
        }
        {
            using _t = void (NetworkClientHelper::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkClientHelper::sigPreviewYuvStatus)) {
                *result = 37;
                return;
            }
        }
        {
            using _t = void (NetworkClientHelper::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkClientHelper::sigPreviewYuvData)) {
                *result = 38;
                return;
            }
        }
        {
            using _t = void (NetworkClientHelper::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkClientHelper::sigCaptueYuvStatus)) {
                *result = 39;
                return;
            }
        }
        {
            using _t = void (NetworkClientHelper::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkClientHelper::sigCaptureYuvData)) {
                *result = 40;
                return;
            }
        }
        {
            using _t = void (NetworkClientHelper::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkClientHelper::sigGetYuvStatus)) {
                *result = 41;
                return;
            }
        }
        {
            using _t = void (NetworkClientHelper::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkClientHelper::sigGetYuvData)) {
                *result = 42;
                return;
            }
        }
        {
            using _t = void (NetworkClientHelper::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkClientHelper::sigCaptueRawStatus)) {
                *result = 43;
                return;
            }
        }
        {
            using _t = void (NetworkClientHelper::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkClientHelper::sigCaptue3ARawStatus)) {
                *result = 44;
                return;
            }
        }
        {
            using _t = void (NetworkClientHelper::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkClientHelper::sig3ARawData)) {
                *result = 45;
                return;
            }
        }
        {
            using _t = void (NetworkClientHelper::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkClientHelper::sigCaptureBinInfo)) {
                *result = 46;
                return;
            }
        }
        {
            using _t = void (NetworkClientHelper::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkClientHelper::sigCaptureBinData)) {
                *result = 47;
                return;
            }
        }
        {
            using _t = void (NetworkClientHelper::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkClientHelper::sigCaptureRawData)) {
                *result = 48;
                return;
            }
        }
        {
            using _t = void (NetworkClientHelper::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkClientHelper::sigBinUtilStatus)) {
                *result = 49;
                return;
            }
        }
        {
            using _t = void (NetworkClientHelper::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkClientHelper::sigBinUtilData)) {
                *result = 50;
                return;
            }
        }
        {
            using _t = void (NetworkClientHelper::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkClientHelper::sig3AYuvStatus)) {
                *result = 51;
                return;
            }
        }
        {
            using _t = void (NetworkClientHelper::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkClientHelper::sig3AYuvData)) {
                *result = 52;
                return;
            }
        }
        {
            using _t = void (NetworkClientHelper::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkClientHelper::sigAFYuvStatus)) {
                *result = 53;
                return;
            }
        }
        {
            using _t = void (NetworkClientHelper::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkClientHelper::sigAFYuvData)) {
                *result = 54;
                return;
            }
        }
        {
            using _t = void (NetworkClientHelper::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkClientHelper::sigGetWDRMode)) {
                *result = 55;
                return;
            }
        }
        {
            using _t = void (NetworkClientHelper::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkClientHelper::sig3AStatisticsInfo)) {
                *result = 56;
                return;
            }
        }
        {
            using _t = void (NetworkClientHelper::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkClientHelper::sig3ABinInfo)) {
                *result = 57;
                return;
            }
        }
        {
            using _t = void (NetworkClientHelper::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkClientHelper::sig3ABinData)) {
                *result = 58;
                return;
            }
        }
        {
            using _t = void (NetworkClientHelper::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkClientHelper::sigAFStatisticsInfo)) {
                *result = 59;
                return;
            }
        }
        {
            using _t = void (NetworkClientHelper::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkClientHelper::sigRawUtilityMessage)) {
                *result = 60;
                return;
            }
        }
        {
            using _t = void (NetworkClientHelper::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkClientHelper::sigContinuousYuvStatus)) {
                *result = 61;
                return;
            }
        }
        {
            using _t = void (NetworkClientHelper::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkClientHelper::sigContinuousYuvData)) {
                *result = 62;
                return;
            }
        }
        {
            using _t = void (NetworkClientHelper::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkClientHelper::sigBracketSingleRawStatus)) {
                *result = 63;
                return;
            }
        }
        {
            using _t = void (NetworkClientHelper::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkClientHelper::sigBracketBinInfo)) {
                *result = 64;
                return;
            }
        }
        {
            using _t = void (NetworkClientHelper::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkClientHelper::sigBracketBinData)) {
                *result = 65;
                return;
            }
        }
        {
            using _t = void (NetworkClientHelper::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkClientHelper::sigBracketSingleRawData)) {
                *result = 66;
                return;
            }
        }
        {
            using _t = void (NetworkClientHelper::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkClientHelper::sigBracketSingleYuvStatus)) {
                *result = 67;
                return;
            }
        }
        {
            using _t = void (NetworkClientHelper::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkClientHelper::sigBracketSingleYuvData)) {
                *result = 68;
                return;
            }
        }
        {
            using _t = void (NetworkClientHelper::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkClientHelper::sigBracketStatus)) {
                *result = 69;
                return;
            }
        }
        {
            using _t = void (NetworkClientHelper::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkClientHelper::sigBracketData)) {
                *result = 70;
                return;
            }
        }
        {
            using _t = void (NetworkClientHelper::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkClientHelper::sigToolJsonStatus)) {
                *result = 71;
                return;
            }
        }
        {
            using _t = void (NetworkClientHelper::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkClientHelper::sigToolJsonData)) {
                *result = 72;
                return;
            }
        }
        {
            using _t = void (NetworkClientHelper::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkClientHelper::sigChipInfoStatus)) {
                *result = 73;
                return;
            }
        }
        {
            using _t = void (NetworkClientHelper::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkClientHelper::sigRecDrcHistogramInfo)) {
                *result = 74;
                return;
            }
        }
        {
            using _t = void (NetworkClientHelper::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkClientHelper::sigReadAllPageData)) {
                *result = 75;
                return;
            }
        }
        {
            using _t = void (NetworkClientHelper::*)(QString , QString , BUTTON_MODE );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkClientHelper::sigEnableAllPageButtons)) {
                *result = 76;
                return;
            }
        }
        {
            using _t = void (NetworkClientHelper::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkClientHelper::sigReceiveEvValue)) {
                *result = 77;
                return;
            }
        }
        {
            using _t = void (NetworkClientHelper::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkClientHelper::sigReceiveLdcSize)) {
                *result = 78;
                return;
            }
        }
        {
            using _t = void (NetworkClientHelper::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkClientHelper::sigSendLDCBinData)) {
                *result = 79;
                return;
            }
        }
        {
            using _t = void (NetworkClientHelper::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkClientHelper::sigCaliRawStatus)) {
                *result = 80;
                return;
            }
        }
        {
            using _t = void (NetworkClientHelper::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkClientHelper::sigCaliBinInfo)) {
                *result = 81;
                return;
            }
        }
        {
            using _t = void (NetworkClientHelper::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkClientHelper::sigCaliBinData)) {
                *result = 82;
                return;
            }
        }
        {
            using _t = void (NetworkClientHelper::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkClientHelper::sigCaliRawData)) {
                *result = 83;
                return;
            }
        }
        {
            using _t = void (NetworkClientHelper::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkClientHelper::sigI2CReadStatus)) {
                *result = 84;
                return;
            }
        }
        {
            using _t = void (NetworkClientHelper::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkClientHelper::sigI2CWriteStatus)) {
                *result = 85;
                return;
            }
        }
        {
            using _t = void (NetworkClientHelper::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkClientHelper::sigRegGetStatus)) {
                *result = 86;
                return;
            }
        }
        {
            using _t = void (NetworkClientHelper::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkClientHelper::sigRegSetStatus)) {
                *result = 87;
                return;
            }
        }
        {
            using _t = void (NetworkClientHelper::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkClientHelper::sigSPITransmitStatus)) {
                *result = 88;
                return;
            }
        }
        {
            using _t = void (NetworkClientHelper::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkClientHelper::sigRawUtilYuvStatus)) {
                *result = 89;
                return;
            }
        }
        {
            using _t = void (NetworkClientHelper::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkClientHelper::sigRawUtilYuvData)) {
                *result = 90;
                return;
            }
        }
        {
            using _t = void (NetworkClientHelper::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkClientHelper::sigContinuousGetMaxFrames)) {
                *result = 91;
                return;
            }
        }
        {
            using _t = void (NetworkClientHelper::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkClientHelper::sigCtrlCmodel)) {
                *result = 92;
                return;
            }
        }
        {
            using _t = void (NetworkClientHelper::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkClientHelper::sigRecFshdrMapcurveInfo)) {
                *result = 93;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject NetworkClientHelper::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_NetworkClientHelper.data,
    qt_meta_data_NetworkClientHelper,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *NetworkClientHelper::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *NetworkClientHelper::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_NetworkClientHelper.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int NetworkClientHelper::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 107)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 107;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 107)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 107;
    }
    return _id;
}

// SIGNAL 0
void NetworkClientHelper::connected_uart()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void NetworkClientHelper::disconnected_uart()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void NetworkClientHelper::sigStatusLog(QString _t1, LOG_LEVEL _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void NetworkClientHelper::sigReceiveCmdResp(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void NetworkClientHelper::sigReceiveSingleImageData(QString _t1, QString _t2, QString _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void NetworkClientHelper::sigReceiveImageData(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void NetworkClientHelper::sigReceiveMultiYuvData(QString _t1, QString _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void NetworkClientHelper::sigReceiveYuvData(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}

// SIGNAL 8
void NetworkClientHelper::sigReceiveMultiRawData(QString _t1, QString _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}

// SIGNAL 9
void NetworkClientHelper::sigReceiveRawData(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 9, _a);
}

// SIGNAL 10
void NetworkClientHelper::sigReceiveMultiBracketingData(QString _t1, QString _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 10, _a);
}

// SIGNAL 11
void NetworkClientHelper::sigReceiveBracketingData(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 11, _a);
}

// SIGNAL 12
void NetworkClientHelper::sigReceiveBinFile(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 12, _a);
}

// SIGNAL 13
void NetworkClientHelper::sigReceiveBinMessage(QString _t1, QString _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 13, _a);
}

// SIGNAL 14
void NetworkClientHelper::sigReceiveBlackLvlMsg(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 14, _a);
}

// SIGNAL 15
void NetworkClientHelper::sigReceiveRegMsg(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 15, _a);
}

// SIGNAL 16
void NetworkClientHelper::sigReceive3AMsg(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 16, _a);
}

// SIGNAL 17
void NetworkClientHelper::sigReceive3AImage(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 17, _a);
}

// SIGNAL 18
void NetworkClientHelper::sigReceiveGamma()
{
    QMetaObject::activate(this, &staticMetaObject, 18, nullptr);
}

// SIGNAL 19
void NetworkClientHelper::sigReceiveFocusAssistImage(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 19, _a);
}

// SIGNAL 20
void NetworkClientHelper::sigReceiveFocusAssistData(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 20, _a);
}

// SIGNAL 21
void NetworkClientHelper::sigReceiveChipInfo(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 21, _a);
}

// SIGNAL 22
void NetworkClientHelper::sigReceiveRawUtilityMessage(QString _t1, QString _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 22, _a);
}

// SIGNAL 23
void NetworkClientHelper::sigReceiveBracketImage(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 23, _a);
}

// SIGNAL 24
void NetworkClientHelper::sigReceiveBracketRawData(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 24, _a);
}

// SIGNAL 25
void NetworkClientHelper::sigReceiveWDRMode(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 25, _a);
}

// SIGNAL 26
void NetworkClientHelper::sigReceiveContinuousYuv(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 26, _a);
}

// SIGNAL 27
void NetworkClientHelper::sigReadAllFinished()
{
    QMetaObject::activate(this, &staticMetaObject, 27, nullptr);
}

// SIGNAL 28
void NetworkClientHelper::sigReceiveCmdRespJsonrpc(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 28, _a);
}

// SIGNAL 29
void NetworkClientHelper::sigReceiveGammaJsonrpc(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 29, _a);
}

// SIGNAL 30
void NetworkClientHelper::sigReceiveCcmCalibration(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 30, _a);
}

// SIGNAL 31
void NetworkClientHelper::sigReceiveRlscCalibration(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 31, _a);
}

// SIGNAL 32
void NetworkClientHelper::sigReceiveMlscCalibration(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 32, _a);
}

// SIGNAL 33
void NetworkClientHelper::sigReceiveBlcCalibration(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 33, _a);
}

// SIGNAL 34
void NetworkClientHelper::sigReceiveAWBCalibration(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 34, _a);
}

// SIGNAL 35
void NetworkClientHelper::sigReceiveRgbirCalibration(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 35, _a);
}

// SIGNAL 36
void NetworkClientHelper::sigReceiveNoiseProfileCalibration(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 36, _a);
}

// SIGNAL 37
void NetworkClientHelper::sigPreviewYuvStatus(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 37, _a);
}

// SIGNAL 38
void NetworkClientHelper::sigPreviewYuvData(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 38, _a);
}

// SIGNAL 39
void NetworkClientHelper::sigCaptueYuvStatus(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 39, _a);
}

// SIGNAL 40
void NetworkClientHelper::sigCaptureYuvData(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 40, _a);
}

// SIGNAL 41
void NetworkClientHelper::sigGetYuvStatus(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 41, _a);
}

// SIGNAL 42
void NetworkClientHelper::sigGetYuvData(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 42, _a);
}

// SIGNAL 43
void NetworkClientHelper::sigCaptueRawStatus(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 43, _a);
}

// SIGNAL 44
void NetworkClientHelper::sigCaptue3ARawStatus(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 44, _a);
}

// SIGNAL 45
void NetworkClientHelper::sig3ARawData(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 45, _a);
}

// SIGNAL 46
void NetworkClientHelper::sigCaptureBinInfo(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 46, _a);
}

// SIGNAL 47
void NetworkClientHelper::sigCaptureBinData(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 47, _a);
}

// SIGNAL 48
void NetworkClientHelper::sigCaptureRawData(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 48, _a);
}

// SIGNAL 49
void NetworkClientHelper::sigBinUtilStatus(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 49, _a);
}

// SIGNAL 50
void NetworkClientHelper::sigBinUtilData(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 50, _a);
}

// SIGNAL 51
void NetworkClientHelper::sig3AYuvStatus(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 51, _a);
}

// SIGNAL 52
void NetworkClientHelper::sig3AYuvData(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 52, _a);
}

// SIGNAL 53
void NetworkClientHelper::sigAFYuvStatus(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 53, _a);
}

// SIGNAL 54
void NetworkClientHelper::sigAFYuvData(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 54, _a);
}

// SIGNAL 55
void NetworkClientHelper::sigGetWDRMode(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 55, _a);
}

// SIGNAL 56
void NetworkClientHelper::sig3AStatisticsInfo(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 56, _a);
}

// SIGNAL 57
void NetworkClientHelper::sig3ABinInfo(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 57, _a);
}

// SIGNAL 58
void NetworkClientHelper::sig3ABinData(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 58, _a);
}

// SIGNAL 59
void NetworkClientHelper::sigAFStatisticsInfo(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 59, _a);
}

// SIGNAL 60
void NetworkClientHelper::sigRawUtilityMessage(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 60, _a);
}

// SIGNAL 61
void NetworkClientHelper::sigContinuousYuvStatus(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 61, _a);
}

// SIGNAL 62
void NetworkClientHelper::sigContinuousYuvData(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 62, _a);
}

// SIGNAL 63
void NetworkClientHelper::sigBracketSingleRawStatus(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 63, _a);
}

// SIGNAL 64
void NetworkClientHelper::sigBracketBinInfo(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 64, _a);
}

// SIGNAL 65
void NetworkClientHelper::sigBracketBinData(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 65, _a);
}

// SIGNAL 66
void NetworkClientHelper::sigBracketSingleRawData(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 66, _a);
}

// SIGNAL 67
void NetworkClientHelper::sigBracketSingleYuvStatus(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 67, _a);
}

// SIGNAL 68
void NetworkClientHelper::sigBracketSingleYuvData(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 68, _a);
}

// SIGNAL 69
void NetworkClientHelper::sigBracketStatus(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 69, _a);
}

// SIGNAL 70
void NetworkClientHelper::sigBracketData(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 70, _a);
}

// SIGNAL 71
void NetworkClientHelper::sigToolJsonStatus(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 71, _a);
}

// SIGNAL 72
void NetworkClientHelper::sigToolJsonData(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 72, _a);
}

// SIGNAL 73
void NetworkClientHelper::sigChipInfoStatus(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 73, _a);
}

// SIGNAL 74
void NetworkClientHelper::sigRecDrcHistogramInfo(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 74, _a);
}

// SIGNAL 75
void NetworkClientHelper::sigReadAllPageData()
{
    QMetaObject::activate(this, &staticMetaObject, 75, nullptr);
}

// SIGNAL 76
void NetworkClientHelper::sigEnableAllPageButtons(QString _t1, QString _t2, BUTTON_MODE _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))) };
    QMetaObject::activate(this, &staticMetaObject, 76, _a);
}

// SIGNAL 77
void NetworkClientHelper::sigReceiveEvValue(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 77, _a);
}

// SIGNAL 78
void NetworkClientHelper::sigReceiveLdcSize(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 78, _a);
}

// SIGNAL 79
void NetworkClientHelper::sigSendLDCBinData(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 79, _a);
}

// SIGNAL 80
void NetworkClientHelper::sigCaliRawStatus(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 80, _a);
}

// SIGNAL 81
void NetworkClientHelper::sigCaliBinInfo(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 81, _a);
}

// SIGNAL 82
void NetworkClientHelper::sigCaliBinData(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 82, _a);
}

// SIGNAL 83
void NetworkClientHelper::sigCaliRawData(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 83, _a);
}

// SIGNAL 84
void NetworkClientHelper::sigI2CReadStatus(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 84, _a);
}

// SIGNAL 85
void NetworkClientHelper::sigI2CWriteStatus(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 85, _a);
}

// SIGNAL 86
void NetworkClientHelper::sigRegGetStatus(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 86, _a);
}

// SIGNAL 87
void NetworkClientHelper::sigRegSetStatus(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 87, _a);
}

// SIGNAL 88
void NetworkClientHelper::sigSPITransmitStatus(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 88, _a);
}

// SIGNAL 89
void NetworkClientHelper::sigRawUtilYuvStatus(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 89, _a);
}

// SIGNAL 90
void NetworkClientHelper::sigRawUtilYuvData(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 90, _a);
}

// SIGNAL 91
void NetworkClientHelper::sigContinuousGetMaxFrames(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 91, _a);
}

// SIGNAL 92
void NetworkClientHelper::sigCtrlCmodel(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 92, _a);
}

// SIGNAL 93
void NetworkClientHelper::sigRecFshdrMapcurveInfo(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 93, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
