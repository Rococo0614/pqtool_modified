#include "raw_image_fetch_window.hpp"
#include "network_client_helper.hpp"
#include "raw_image_format_dialog.hpp"
#include "utils.hpp"
#include "global_data.hpp"

#include <QLabel>
#include <QCloseEvent>
#include <QFileDialog>
#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include <QMessageBox>

const int MIN_WINDOW_WIDTH = 1150;
const int MIN_WINDOW_HEIGHT = 750;

const QString SENSOR_MODE_CAMERA = "camera";
const QString SENSOR_MODE_PICTURE = "picture";

static QList<QString> sensor_mode_list{SENSOR_MODE_CAMERA, SENSOR_MODE_PICTURE};

RawImageFetchWindow::RawImageFetchWindow(QWidget *parent) :
    QMainWindow(parent)
{
    setWindowTitle(tr("Preview"));
    setWindowFlags(Qt::WindowMaximizeButtonHint|Qt::WindowCloseButtonHint);

    initUI();
    initEvent();
}

void RawImageFetchWindow::initUI()
{
    main_widget = new QWidget();
    main_widget->setMinimumSize(QSize(MIN_WINDOW_WIDTH, MIN_WINDOW_HEIGHT));
    main_layout = new QGridLayout();
    image = new QPixmap();
    img_viewer = new ImageViewerWidget();
    ImageLabel *image_label = new ImageLabel(this, ImageLabel::GRID);
    image_label->enableGrid(false);
    image_label->enableRect(false);
    image_label->setGridSize(3, 3);
    img_viewer->setImageLabel(image_label);
    img_viewer->setFilePathHidden();
    img_viewer->showEnGridCheckBox();

    QHBoxLayout *control_h_layout = new QHBoxLayout;
    sensor_mode_combobox = new QComboBox;
    sensor_mode_combobox->setEnabled(false);
    for (auto sensor_mode : sensor_mode_list) {
        sensor_mode_combobox->addItem(sensor_mode);
    }
    sensor_mode_combobox->setHidden(true);
    connect(sensor_mode_combobox, &QComboBox::currentTextChanged, this, &RawImageFetchWindow::handleChangeSensorMode);
    get_single_img_btn = new QPushButton(tr("Get Single Image"));
    get_single_img_btn->setEnabled(true);
    connect(get_single_img_btn, &QPushButton::clicked, this, &RawImageFetchWindow::handleGetSingleImg);
    cancel_btn = new QPushButton(tr("Cancel"));
    cancel_btn->setEnabled(false);
    connect(cancel_btn, &QPushButton::clicked, this, &RawImageFetchWindow::handleCancelEvent);
    control_h_layout->addWidget(sensor_mode_combobox);
    control_h_layout->addWidget(get_single_img_btn);
    control_h_layout->addWidget(cancel_btn);
    main_layout->addLayout(control_h_layout, 0, 0);
    main_layout->addWidget(img_viewer);

    setCentralWidget(main_widget);
    centralWidget()->setLayout(main_layout);
}

void RawImageFetchWindow::initEvent()
{
    shared_ptr<NetworkClientHelper> network_helper = NetworkClientHelper::getInstance();
    connect(network_helper.get(), &NetworkClientHelper::sigPreviewYuvStatus, this, &RawImageFetchWindow::handleSingleImageStatus);
    connect(network_helper.get(), &NetworkClientHelper::sigPreviewYuvData, this, &RawImageFetchWindow::handleImageUpdate);
}

void RawImageFetchWindow::handleGetSingleImg()
{
    emit fetchStart();
    if (!NetworkClientHelper::getInstance()->isNormalStatus()) {
        QMessageBox::warning(this, tr("Warning"), tr("Network is not connected!"), QMessageBox::Ok);
        return;
    }

    NetworkClientHelper::getInstance()->setCallerInfo(0, NULL);
    get_single_img_btn->setEnabled(false);
    cancel_btn->setEnabled(true);

    QJsonObject id_info;
    id_info[MSG_KEY_CMD] = CMD_PREVIEW_GET_SINGLE_YUV;
    int id = GlobalData::getInstance()->getJsonrpcId();
    NetworkClientHelper::getInstance()->pushIdInfoJsonrpc(id, id_info);

    QJsonObject send_obj;
    send_obj[MSG_KEY_METHOD] = METHOD_GET_SINGLE_YUV;
    send_obj[MSG_KEY_ID] = id;
    send_obj[MSG_KEY_JSONRPC] = MSG_JSONRPC_VERSION;
    NetworkClientHelper::getInstance()->sendCmd(std::move(send_obj));
}

void RawImageFetchWindow::handleSingleImageStatus(int id)
{
    QJsonObject obj = NetworkClientHelper::getInstance()->popCmdDataJsonrpc(id);
    QJsonObject result_obj = obj[MSG_KEY_RESULT].toObject();

    if (result_obj.isEmpty()) {
        get_single_img_btn->setEnabled(true);
        cancel_btn->setEnabled(false);
        QMessageBox::warning(this, tr("Error"), "Get Single Image failed!", QMessageBox::Ok);
        return;
    }
    QJsonObject params_obj = result_obj[MSG_KEY_PARAMS].toObject();
    int size = params_obj["Data size"].toInt();
    NetworkClientHelper::getInstance()->setCallerInfo(size, CMD_PREVIEW_GET_SINGLE_YUV);
    qDebug("Set receive size:%d\n", size);

    int temp_id = GlobalData::getInstance()->getJsonrpcId();
    QJsonObject send_obj;
    send_obj[MSG_KEY_METHOD] = METHOD_GET_IMAGE_DATA;
    send_obj[MSG_KEY_ID] = temp_id;
    send_obj[MSG_KEY_JSONRPC] = MSG_JSONRPC_VERSION;
    NetworkClientHelper::getInstance()->sendCmd(std::move(send_obj));
}

void RawImageFetchWindow::handleImageUpdate(QString cmd)
{
    NetworkClientHelper::getInstance()->setCallerInfo(0, NULL);
    QByteArray data = NetworkClientHelper::getInstance()->popCmdData(cmd);

    QByteArray header = data.left(sizeof(YUV_HEADER));
    YUV_HEADER *pstHeader = (YUV_HEADER *) header.data();
    data.remove(0, sizeof(YUV_HEADER));

    qDebug("numframe:%d, curframe:%d, width:%d, height:%d, stride:%d,%d,%d, format::%d\n", pstHeader->numFrame, pstHeader->curFrame,
        pstHeader->width, pstHeader->height, pstHeader->stride[0], pstHeader->stride[1], pstHeader->stride[2], pstHeader->pixelFormat);
    qDebug("data.size:%d, pstHeader->size:%d\n", data.size(), pstHeader->size);

    if (data.size() != (int)(pstHeader->size)) {
        QMessageBox::warning(this, tr("Error"), tr("Preview, The received yuv data size is wrong!"), QMessageBox::Ok);
        return;
    }

    if (!data.isEmpty()) {
        if (image)
        {
            *image = QPixmap::fromImage(utils::convertYUV2RGBImage(data, pstHeader));
            img_viewer->setImg(*image);
            get_single_img_btn->setEnabled(true);
            cancel_btn->setEnabled(false);
        }
    }
    emit fetchCompleted();
}

void RawImageFetchWindow::handleChangeSensorMode(QString mode)
{

}

void RawImageFetchWindow::closeEvent(QCloseEvent *event)
{
    event->accept();
}

void RawImageFetchWindow::refresh()
{
}

void RawImageFetchWindow::handleFVCheck(bool checked)
{
    if (checked) img_viewer->zoomImage(0.64);

    img_viewer->triggerGrid3AEnable(checked);
}

void RawImageFetchWindow::keyReleaseEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape) {
        this->close();
    }
}

void RawImageFetchWindow::handleCancelEvent()
{
    get_single_img_btn->setEnabled(true);
    cancel_btn->setEnabled(false);
    NetworkClientHelper::getInstance()->setCallerInfo(0, NULL);
}
