#include "noise_profile_calibration_page_widget.hpp"
#include "global_data.hpp"
#include "otp_pdaf_cali.h"
#include "network_client_helper.hpp"
#include "color_block_detect.hpp"
#include <QMessageBox>
#include <Eigen>
//#include <eigen3/Eigen/Eigen> // for linux
#include <QFileDialog>

using namespace Eigen;

// #define DEBUG

NoiseProfileCalibrationPageWidget::NoiseProfileCalibrationPageWidget(QWidget *parent) :
    BaseCalibrationPageWidget(static_cast<int>(CALIBRATION_MODULE_ID::NOISE_PROFILE), parent)
{
    setAcceptScene(SCENE::COLOR24);
    initLeftArea();
    initEvent();
    roi_control_widget = createRoiControlUI(this->input_image_label);
    retranslateUi();
}

void NoiseProfileCalibrationPageWidget::ClearData()
{
    for (int i = 0; i < rawItem.size(); i++) {
        if (rawItem[i]) {
            delete rawItem[i];
        }
    }
    rawItem.clear();
    for (int i = 0; i < iso_item_list.size(); i++) {
        if (iso_item_list[i]) {
            delete iso_item_list[i];
        }
    }
    iso_item_list.clear();
    raw_img_item_vec.clear();
    cali_status = CALIBRATION_STATUS::READY;
    quickMode = false;
    blc_imported = false;
}

void NoiseProfileCalibrationPageWidget::initLeftArea()
{
    initImportRawUI();
    initImportBlcUI();

    edit_roi_btn = new QPushButton;
    QHBoxLayout *roi_layout = new QHBoxLayout;
    QHBoxLayout *TEAISP_chekout_layout = new QHBoxLayout;
    import_roi_btn = new QPushButton;
    export_roi_btn = new QPushButton;
    //roi_layout->addWidget(import_roi_btn);
    //roi_layout->addWidget(export_roi_btn);
    QString tool_soc = GlobalData::getInstance()->getCvipqToolSoc();
    if (tool_soc == CV186X_SOC) {
        QLabel *TEAISP_bnr_label = new QLabel("Enable TEAISP.bnr:");
        TEAISP_bnr_checkbox = new QCheckBox();
        TEAISP_chekout_layout->addWidget(TEAISP_bnr_label);
        TEAISP_chekout_layout->addStretch(3);
        TEAISP_chekout_layout->addWidget(TEAISP_bnr_checkbox);
        connect(TEAISP_bnr_checkbox, &QCheckBox::stateChanged,[=](int state) {
                if (state == Qt::Checked) {
                    bnr_check_state = true;
                } else {
                    bnr_check_state = false;
                }
            });
        TEAISP_chekout_layout->addStretch(1);
    }

    frame_item = new LineEditCaliItemWidget("Frames");
    frame_item->setDisabled(true);
    frame_item->setData(1);
    QLabel *label = new QLabel("ISO: ");
    iso_item = new QComboBox;
    for (int i = 0; i < iso_list.size(); i++) {
        iso_item->addItem(QString("%1").arg(iso_list[i]));
    }
    iso_item->setCurrentIndex(0);

    param_layout->addWidget(edit_roi_btn);
    param_layout->addLayout(TEAISP_chekout_layout);
    param_layout->addLayout(roi_layout);
    param_layout->addWidget(frame_item);
    QHBoxLayout *h_layout = new QHBoxLayout;
    h_layout->addWidget(label);
    h_layout->addWidget(iso_item);
    param_layout->addLayout(h_layout);

    //events
    connect(edit_roi_btn, &QPushButton::clicked, [=]() {
        if (roi_control_widget != nullptr) {
            roi_control_widget->show();
        }
    });

    initCaliUI();
    initWriteDataUI();
    initImportDataUI();
    initExportDataUI();
    initIsoListUI();
}

void NoiseProfileCalibrationPageWidget::addRawImgItem(RawImageItem *raw_img_item)
{
    if (raw_img_item != nullptr) {
        this->raw_img_item_vec.push_back(raw_img_item);
        frame_item->setData(raw_img_item->getFramesCnt());
    }
    displayInput();
}

bool NoiseProfileCalibrationPageWidget::checkShallExport()
{
    int list_size = 0;
    if (quickMode) {
        list_size = iso_item_list.size();
    } else {
        list_size = iso_list_widget->getDisplayIsoList().size();
    }
    if (list_size >= 1 && list_size <= 16)
        return true;

    return false;
}

void NoiseProfileCalibrationPageWidget::exportData(QString file_path)
{
    BaseCalibrationPageWidget::exportData(file_path);

    if (iso_list_widget->getDisplayIsoList().size() == 16) {
        QString data = getCalibrationHeaderData();
        QStringList strList = file_path.split(".");
        QString path = {""};

        for(int i = 0; i < strList.size() - 1; i++) {
            path = path + file_path.split(".").at(i) + ".";
        }

        path = path + "h";
        QFile saveFile(path);

        if (!saveFile.open(QIODevice::WriteOnly)) {
            qWarning("Couldn't open save file [%s]", qUtf8Printable(file_path));
            return;
        }

        saveFile.write(data.toUtf8());
        saveFile.close();
    }
}

static bool compareData(IsoItem *item1, IsoItem *item2)
{
    if (item1->getIso() < item2->getIso()) {
        return true;
    }

    return false;
}

QString NoiseProfileCalibrationPageWidget::getCalibrationHeaderData()
{
    QList<IsoItem *> list = iso_list_widget->getDisplayIsoList();

    std::sort(list.begin(), list.end(), compareData);
    QString data = "ISP_CMOS_NOISE_CALIBRATION_S np = {\n";
    data.append("	.CalibrationCoef = {\n");
    for (int i = 0; i < iso_list.size(); i++) {
        data.append(QString("		{	//iso  %1  \n").arg(iso_list[i]));
        data.append(QString("			{%1,	%2}, //B: slope, intercept \n")
                    .arg(QString::number(list[i]->getfCaliSlopeB(), 10, 20))
                    .arg(QString::number(list[i]->getfCaliInterceptB(), 10, 20)));
        data.append(QString("			{%1,	%2}, //Gb: slope, intercept \n")
                    .arg(QString::number(list[i]->getfCaliSlopeGb(), 10, 20))
                    .arg(QString::number(list[i]->getfCaliInterceptGb(), 10, 20)));
        data.append(QString("			{%1,	%2}, //Gr: slope, intercept \n")
                    .arg(QString::number(list[i]->getfCaliSlopeGr(), 10, 20))
                    .arg(QString::number(list[i]->getfCaliInterceptGr(), 10, 20)));
        data.append(QString("			{%1,	%2}, //R: slope, intercept \n		},\n")
                    .arg(QString::number(list[i]->getfCaliSlopeR(), 10, 20))
                    .arg(QString::number(list[i]->getfCaliInterceptR(), 10, 20)));
    }
    data.append("	},\n");
    data.append("	.CalibrationCoef = {   //CalibrationCoef of TEAISP_BNR_NP_S\n");
    for (int i = 0; i < iso_list.size(); i++) {
        data.append(QString("		{	//iso  %1  \n").arg(iso_list[i]));
        data.append(QString("			{%1,	%2}, //fCaliSlopeMax, fCaliInterceptMax \n		},\n")
                    .arg(QString::number(list[i]->getfCaliSlopeMax(), 10, 20))
                    .arg(QString::number(list[i]->getffCaliInterceptMax(), 10, 20)));
    }
    data.append("	}\n");
    data.append("};\n");

    return data;
}

bool NoiseProfileCalibrationPageWidget::calibration()
{
    if (raw_img_item_vec.isEmpty()) {
        return false;
    }
    cali_status = CALIBRATION_STATUS::CALIBRATION;
    using utils::RAW_BAYER_ID;
    using utils::RAW_BITS;

    RawImageItem * raw_img_item = this->raw_img_item_vec.first();
    int bayer_id = raw_img_item->getBayer().toInt(); // 0: BGGR, 1: GBRG, 2: GRBG, 3: RGGB
    int width = raw_img_item->getWidth();
    int height = raw_img_item->getHeight();

    // read all frames for one raw file
    std::vector<unique_ptr<int []>> image_vec = utils::readMultiRawIntData(raw_img_item->getFilePath(), width, height);
    //qDebug("Read Raw Frames: %d", image_vec.size());

    // calibration params
    int num_of_roi = 6;
    int roi_width = 16;
    int roi_height = 16;
    int num_of_images = static_cast<int>(image_vec.size());

    vector< vector<int*> > raw_roi_list;
    QVector<QRectF> color_rects = this->input_image_label->getColorCheckerRect();
    double scale_factor = this->input_img->getScaleFactor();
    if (quickMode) {
        roi_width = color_block.roi_width;
        roi_height = color_block.roi_height;
    } else {
        if (color_rects.size() != 24) {
            return false;
        }
        roi_width = qFloor(color_rects.first().width()/scale_factor);
        roi_height = qFloor(color_rects.first().height()/scale_factor);
    }

    int*** roi_images = new int** [num_of_roi];
    for (int k = 0; k < num_of_roi; k++) {
        roi_images[k] = new int* [num_of_images];
        for (int j = 0; j < num_of_images; j++) {
            roi_images[k][j] = new int[roi_width * roi_height];
            memset(roi_images[k][j], 0, sizeof(int) * roi_width * roi_height);
        }
    }

    raw_roi_list.resize(num_of_roi);
    for (int k = 0; k < num_of_roi; k++) {
       raw_roi_list[k].resize(num_of_images);
    }

    for (int k = 0; k < num_of_roi; k++) {
        int x,y;
        if (quickMode) {
            x = color_block.Points.at(18 + k).x;
            y = color_block.Points.at(18 + k).y;
        } else {
            QRectF rect = color_rects[18 + k];
            // map back to raw position
            if (scale_factor > 0) {
                rect.setX(qFloor(rect.x()/scale_factor));
                rect.setY(qFloor(rect.y()/scale_factor));
            }

            // color rect position need even
            if (qFloor(rect.x()) % 2 == 1) {
                rect.setX(qFloor(rect.x()) + 1);
            }
            if (qFloor(rect.y()) % 2 == 1) {
                rect.setY(qFloor(rect.y()) + 1);
            }

            x = rect.x();
            y = rect.y();
        }

        for (int j = 0; j < num_of_images; j++) {
            int i = 0;
            for (int rect_j = y; rect_j < (y + roi_height); rect_j++) {
                for (int rect_i = x; rect_i < (x + roi_width); rect_i++) {
                    int rect_pos = rect_i + rect_j * width;
                    int raw_pixel_val = image_vec[j][rect_pos];
                    if (raw_pixel_val < 0)
                        raw_pixel_val = 0;
                    roi_images[k][j][i++] = raw_pixel_val;
                }

            }
            raw_roi_list[k][j] = roi_images[k][j];
        }
    }

    qDebug("bayer_id: %d blc_b_offset:%d blc_gb_offset:%d blc_gr_offset:%d blc_r_offset:%d",
           bayer_id, blc_b_offset, blc_gb_offset, blc_gr_offset, blc_r_offset);

    // calibration
    cali_isp_np_float_calc(
        raw_roi_list,
        roi_width,
        roi_height,
        num_of_roi,
        num_of_images,
        bayer_id,
        blc_b_offset, blc_gb_offset,
        blc_gr_offset, blc_r_offset,
        fCaliSlopeB, fCaliInterceptB,
        fCaliSlopeGb, fCaliInterceptGb,
        fCaliSlopeGr, fCaliInterceptGr,
        fCaliSlopeR, fCaliInterceptR
    );

    if (bnr_check_state) {
        cali_isp_np_float_calc_ainr(
            raw_roi_list,
            roi_width,
            roi_height,
            num_of_roi,
            num_of_images,
            bayer_id,
            blc_b_offset, blc_gb_offset,
            blc_gr_offset, blc_r_offset,
            fCaliSlopeMax, fCaliInterceptMax
        );
    } else {
        fCaliSlopeMax = 0.0;
        fCaliInterceptMax = 0.0;
    }
    frames_cnt = num_of_images;

    qDebug("fCaliSlopeB:%f fCaliInterceptB:%f fCaliSlopeGb:%f fCaliInterceptGb:%f fCaliSlopeGr:%f "
           "CaliInterceptGr:%f fCaliSlopeR:%f fCaliInterceptR:%f",
           fCaliSlopeB, fCaliInterceptB, fCaliSlopeGb, fCaliInterceptGb, fCaliSlopeGr,
           fCaliInterceptGr, fCaliSlopeR, fCaliInterceptR);
    qDebug("fCaliSlopeMax:%f fCaliInterceptMax:%f ", fCaliSlopeMax, fCaliInterceptMax);

    // Relesase Memory
    for (int k = 0; k < num_of_roi; k++) {
        for (int j = 0; j < num_of_images; j++) {
            delete[] roi_images[k][j];
        }
    }
    cali_status = CALIBRATION_STATUS::READY;
    return true;
}

void NoiseProfileCalibrationPageWidget::initEvent()
{
    connect(this, &BaseCalibrationPageWidget::sigImportData, this, &NoiseProfileCalibrationPageWidget::handleImportData);
    shared_ptr<NetworkClientHelper> network_helper = NetworkClientHelper::getInstance();
    connect(network_helper.get(), &NetworkClientHelper::sigReceiveNoiseProfileCalibration,
        this, &NoiseProfileCalibrationPageWidget::handleReceiveNoiseProfileCalibration);
}

QJsonArray NoiseProfileCalibrationPageWidget::getCalibrationData()
{
    QJsonArray data_array;
    QJsonObject data, iso_data;
    int iso_array[CalibrationIsoListWidget::MAX_ISO_ITEM_NUM] = {};
    float slope_array[CalibrationIsoListWidget::MAX_ISO_ITEM_NUM][4] = {};
    float intercept_array[CalibrationIsoListWidget::MAX_ISO_ITEM_NUM][4] = {};
    float slope_max_array[CalibrationIsoListWidget::MAX_ISO_ITEM_NUM] = {};
    float intercept_max_array[CalibrationIsoListWidget::MAX_ISO_ITEM_NUM] = {};

    QList<IsoItem *> list;
    if (quickMode) {
        list = iso_item_list;
    } else {
        list = iso_list_widget->getDisplayIsoList();
    }
    std::sort(list.begin(), list.end(), compareData);

    int size = list.size();
    for (int i = 0; i < size; i++) {
        iso_array[i] = list[i]->getIso();
        slope_array[i][0] = list[i]->getfCaliSlopeB();
        slope_array[i][1] = list[i]->getfCaliSlopeGb();
        slope_array[i][2] = list[i]->getfCaliSlopeGr();
        slope_array[i][3] = list[i]->getfCaliSlopeR();
        intercept_array[i][0] = list[i]->getfCaliInterceptB();
        intercept_array[i][1] = list[i]->getfCaliInterceptGb();
        intercept_array[i][2] = list[i]->getfCaliInterceptGr();
        intercept_array[i][3] = list[i]->getfCaliInterceptR();
        slope_max_array[i] = list[i]->getfCaliSlopeMax();
        intercept_max_array[i] = list[i]->getffCaliInterceptMax();
    }

    iso_data[KEY_REG_KEY] = "ISO";
    iso_data[KEY_VALUE] = utils::arrayPtrToJsonArray(iso_array, size);
    iso_data[KEY_VALUE_TYPE] = TYPE_ARRAY;
    iso_data[KEY_REG_OP_TYPE] = OP_TYPE_MANUAL;
    data_array.append(iso_data);

    for (int i = 0; i < size; i++) {
        data[KEY_REG_KEY] = QString("slope[%1]").arg(i);
        data[KEY_VALUE] = utils::arrayPtrToJsonArray(slope_array[i], 4);
        data[KEY_VALUE_TYPE] = TYPE_ARRAY_FLOAT;
        data[KEY_REG_OP_TYPE] = OP_TYPE_MANUAL;
        data_array.append(data);
        data[KEY_REG_KEY] = QString("intercept[%1]").arg(i);
        data[KEY_VALUE] = utils::arrayPtrToJsonArray(intercept_array[i], 4);
        data[KEY_VALUE_TYPE] = TYPE_ARRAY_FLOAT;
        data[KEY_REG_OP_TYPE] = OP_TYPE_MANUAL;
        data_array.append(data);
        data[KEY_REG_KEY] = QString("slope_max[%1]").arg(i);
        data[KEY_VALUE] = slope_max_array[i];
        data[KEY_VALUE_TYPE] = TYPE_DOUBLE;
        data[KEY_REG_OP_TYPE] = OP_TYPE_MANUAL;
        data_array.append(data);
        data[KEY_REG_KEY] = QString("intercept_max[%1]").arg(i);
        data[KEY_VALUE] = intercept_max_array[i];
        data[KEY_VALUE_TYPE] = TYPE_DOUBLE;
        data[KEY_REG_OP_TYPE] = OP_TYPE_MANUAL;
        data_array.append(data);
    }

    return data_array;
}

double NoiseProfileCalibrationPageWidget::calculateMean(QList<double> values)
{
    double mean;
    double sum = 0;

    for (double value : values) {
        sum += value;
    }
    mean = double(sum) / values.size();

    return mean;
}

double NoiseProfileCalibrationPageWidget::calculateStandardDeviation(QList<double> values)
{
    double var = 0;
    double avg = calculateMean(values);

    for (double value : values) {
        var += (value - avg) * (value - avg);
    }
    var /= values.size();

    return qSqrt(var);
}

void NoiseProfileCalibrationPageWidget::solveLeastMeanSquare(QList<double> means, QList<double> stds, double &m, double &c)
{
    if (means.size() != stds.size()) {
        qWarning() << "length of mean list and std list should be the same:" << means.size() << stds.size();
    }

    MatrixXd a = MatrixXd::Ones(means.size(), 2);
    MatrixXd b = MatrixXd::Zero(means.size(), 1);

    for (int i = 0; i < means.size(); i++) {
        a(i, 0) = means[i];
        b(i, 0) = stds[i];
    }

    MatrixXd ret = (a.transpose() * a).inverse() * a.transpose() * b;
//    (a.bdcSvd(ComputeThinU | ComputeThinV).solve(b));
    m = ret(0, 0);
    c = ret(1, 0);
}

void NoiseProfileCalibrationPageWidget::calibrateBNR()
{
    QStringList slope = {"reg_bnr_ns_slope_b", "reg_bnr_ns_slope_gb", "reg_bnr_ns_slope_gr", "reg_bnr_ns_slope_r"};
    QStringList low_offset = {"reg_bnr_ns_low_offset_b", "reg_bnr_ns_low_offset_gb", "reg_bnr_ns_low_offset_gr", "reg_bnr_ns_low_offset_r"};
    QStringList high_offset = {"reg_bnr_ns_high_offset_b", "reg_bnr_ns_high_offset_gb", "reg_bnr_ns_high_offset_gr", "reg_bnr_ns_high_offset_r"};
    QStringList luma_th = {"reg_bnr_ns_luma_th_b", "reg_bnr_ns_luma_th_gb", "reg_bnr_ns_luma_th_gr", "reg_bnr_ns_luma_th_r"};

    QList<QList<QList<double>>> pixels;
    for (int i = 0; i < 6; i++) {
        QList<QList<QColor>> rect = this->input_image_label->getColorCheckerPixels(23 - i);
        QList<double> b;
        QList<double> gb;
        QList<double> gr;
        QList<double> r;

        for (int y = 0; y < rect.size(); y++) {
            for (int x = 0; x < rect[y].size(); x++) {
                QColor color = rect[y][x];

                if (color.red() != 0) {
                    r.append(color.red());
                } else if (color.blue() != 0) {
                    b.append(color.blue());
                } else {
                    if ((x == 0 && rect[y][x + 1].red() > 0) || ((x > 0 && rect[y][x - 1].red() > 0))) {
                        gr.append(color.green());
                    } else {
                        gb.append(color.green());
                    }
                }
            }
        }

        pixels.append({b, gb, gr, r});
    }

    for (int i = 0; i < 4; i++) {
        QList<double> means, stds;
        for (int j = 0; j < 6; j++) {
            QList<double> values = pixels[j][i];
            double avg = calculateMean(values);
            double std = calculateStandardDeviation(values);
            means.append(avg);
            stds.append(std);
        }
        double m, c;
        solveLeastMeanSquare(means, stds, m, c);
        double luma0 = means[0];

        GlobalData::getInstance()->setRegValue("isp_bnr", slope[i], int(m * 1024));
        GlobalData::getInstance()->setRegValue("isp_bnr", low_offset[i], int(m * luma0 + c));
        GlobalData::getInstance()->setRegValue("isp_bnr", high_offset[i], 1023);
        GlobalData::getInstance()->setRegValue("isp_bnr", luma_th[i], luma0);
    }
}

void NoiseProfileCalibrationPageWidget::calibrateYNR()
{
    QStringList ns_luma_th = {"reg_ynr_ns_luma_th_0", "reg_ynr_ns_luma_th_1", "reg_ynr_ns_luma_th_2", "reg_ynr_ns_luma_th_3", "reg_ynr_ns_luma_th_4", "reg_ynr_ns_luma_th_5"};
    QStringList ns_offset_th = {"reg_ynr_ns_offset_th_0", "reg_ynr_ns_offset_th_1", "reg_ynr_ns_offset_th_2", "reg_ynr_ns_offset_th_3", "reg_ynr_ns_offset_th_4", "reg_ynr_ns_offset_th_5"};
    QStringList ns_slope = {"reg_ynr_ns_slope_0", "reg_ynr_ns_slope_1", "reg_ynr_ns_slope_2", "reg_ynr_ns_slope_3", "reg_ynr_ns_slope_4"};
    QStringList ns_luma_th2 = {"reg_ynr_ns_luma_th2_0", "reg_ynr_ns_luma_th2_1", "reg_ynr_ns_luma_th2_2", "reg_ynr_ns_luma_th2_3", "reg_ynr_ns_luma_th2_4", "reg_ynr_ns_luma_th2_5"};
    QStringList ns_offset_th2 = {"reg_ynr_ns_offset_th2_0", "reg_ynr_ns_offset_th2_1", "reg_ynr_ns_offset_th2_2", "reg_ynr_ns_offset_th2_3", "reg_ynr_ns_offset_th2_4", "reg_ynr_ns_offset_th2_5"};
    QStringList ns_slope2 = {"reg_ynr_ns_slope2_0", "reg_ynr_ns_slope2_1", "reg_ynr_ns_slope2_2", "reg_ynr_ns_slope2_3", "reg_ynr_ns_slope2_4"};

    QList<double> means, stds;
    for (int i = 0; i < 6; i++) {
        QList<QList<QColor>> pixels = this->input_image_label->getColorCheckerPixels(23 - i);
        QList<double> values;
        for (QList<QColor> rows : pixels) {
            for (QColor pixel : rows) {
                double gray = 0.299 * pixel.red() + 0.587 * pixel.green() + 0.114 * pixel.blue();
                values.append(gray);
            }
        }
        double avg = calculateMean(values);
        double std = calculateStandardDeviation(values);
        means.append(avg);
        stds.append(std);

        GlobalData::getInstance()->setRegValue("isp_bnr", ns_luma_th[i], avg);
        GlobalData::getInstance()->setRegValue("isp_bnr", ns_offset_th[i], std);
        GlobalData::getInstance()->setRegValue("isp_bnr", ns_luma_th2[i], avg);
        GlobalData::getInstance()->setRegValue("isp_bnr", ns_offset_th2[i], std);
    }

    for (int i = 0; i < 5; i++) {
        double luma0 = means[i];
        double luma1 = means[i + 1];
        double std0 = stds[i];
        double std1 = stds[i + 1];

        double m = (std1 - std0) / (luma1 - luma0);
        GlobalData::getInstance()->setRegValue("isp_bnr", ns_slope[i], int(m * 512));
        GlobalData::getInstance()->setRegValue("isp_bnr", ns_slope2[i], int(m * 512));
    }
}

void NoiseProfileCalibrationPageWidget::initIsoListUI()
{
    iso_list_widget = new CalibrationIsoListWidget;
    param_layout->addWidget(iso_list_widget);

    connect(this, &BaseCalibrationPageWidget::sigCalibration, [=]() {
        if (raw_img_item_vec.isEmpty()) {
            return;
        }

        if (iso_list_widget->getIsoList().size() == CalibrationIsoListWidget::MAX_ISO_ITEM_NUM) {
            QMessageBox message(QMessageBox::NoIcon, "Error", "Can only add " +
                                QString::number(CalibrationIsoListWidget::MAX_ISO_ITEM_NUM) + " iso items");
            message.exec();
            return;
        }

        IsoItem *iso_item = new IsoItem();
        // set iso attr
        iso_item->setFrames(this->frame_item->getData().toInt());
        iso_item->setIso(iso_list[this->iso_item->currentIndex()]);
        iso_item->setfCaliSlope(fCaliSlopeB, fCaliSlopeGb, fCaliSlopeGr, fCaliSlopeR, fCaliSlopeMax);
        iso_item->setfCaliIntercept(fCaliInterceptB, fCaliInterceptGb, fCaliInterceptGr, fCaliInterceptR, fCaliInterceptMax);
        iso_item->setFrames(frames_cnt);

        bool success = iso_list_widget->addTableItem(iso_item);
        if (!success) {
            QMessageBox message(QMessageBox::NoIcon, "Error", "Add iso item failed");
            message.exec();
            return;
        }
        if (this->input_image_label->pixmap() != nullptr) {
            iso_item->setInputPixmap(*this->input_image_label->pixmap());
        }
        if (this->output_image_label->pixmap() != nullptr) {
            iso_item->setOutputPixmap(*this->output_image_label->pixmap());
        }
        iso_item->setDisplay(Qt::CheckState::Checked);
    });

    connect(iso_list_widget, &CalibrationIsoListWidget::sigSetInputPixmap, [=](QPixmap *pixmap) {
        if (pixmap != nullptr) this->input_img->setImg(*pixmap);
    });

    connect(iso_list_widget, &CalibrationIsoListWidget::sigSetOutputPixmap, [=](QPixmap *pixmap) {
        if (pixmap != nullptr) this->output_img->setImg(*pixmap);
    });
}

void NoiseProfileCalibrationPageWidget::handleImportData(QString name, QJsonObject data)
{
    qDebug()<<"NoiseProfileCalibrationPageWidget import file: "<<name;

    // import blc data
    QString blc_module_name = PREFIX_CALI_STR + getCaliModuleName(CALIBRATION_MODULE_ID::BLC);
    if (data.contains(blc_module_name)) {
        blc_imported = true;
        QJsonArray blc_data_array = data[blc_module_name].toArray();
        for (auto blc_data : blc_data_array) {
            QJsonObject blc_data_obj = blc_data.toObject();
            if (blc_data_obj[KEY_REG_KEY] == "blc_r_offset") {
                blc_r_offset = static_cast<unsigned int>(blc_data_obj[KEY_VALUE].toInt());
            } else if (blc_data_obj[KEY_REG_KEY] == "blc_gr_offset") {
                blc_gr_offset = static_cast<unsigned int>(blc_data_obj[KEY_VALUE].toInt());
            } else if (blc_data_obj[KEY_REG_KEY] == "blc_gb_offset") {
                blc_gb_offset = static_cast<unsigned int>(blc_data_obj[KEY_VALUE].toInt());
            } else if (blc_data_obj[KEY_REG_KEY] == "blc_b_offset") {
                blc_b_offset= static_cast<unsigned int>(blc_data_obj[KEY_VALUE].toInt());
            } else if (blc_data_obj[KEY_REG_KEY] == "blc_r_gain") {
                blc_r_gain = static_cast<float>(blc_data_obj[KEY_VALUE].toInt()/1024.0);
            } else if (blc_data_obj[KEY_REG_KEY] == "blc_gr_gain") {
                blc_gr_gain = static_cast<float>(blc_data_obj[KEY_VALUE].toInt()/1024.0);
            } else if (blc_data_obj[KEY_REG_KEY] == "blc_gb_gain") {
                blc_gb_gain = static_cast<float>(blc_data_obj[KEY_VALUE].toInt()/1024.0);
            } else if (blc_data_obj[KEY_REG_KEY] == "blc_b_gain") {
                blc_b_gain= static_cast<float>(blc_data_obj[KEY_VALUE].toInt()/1024.0);
            }
        }
    }

    // import noise profile data
    QString np_module_name = PREFIX_CALI_STR + getCaliModuleName(CALIBRATION_MODULE_ID::NOISE_PROFILE);
    if (data.contains(np_module_name)) {
        int iso[16]{0};
        float slope[4 * 16]{0}, intercept[4 * 16]{0};
        float slope_max[1 * 16]{0}, intercept_max[1 * 16]{0};
        QJsonArray np_data_array = data[np_module_name].toArray();
        for (auto np_data : np_data_array) {
            QJsonObject np_data_obj = np_data.toObject();
            if (np_data_obj[KEY_REG_KEY] == "ISO") {
                QJsonArray value_array = np_data_obj[KEY_VALUE].toArray();
                for (int i = 0; (i < 16) && (i < value_array.size()); i++) {
                    iso[i] = value_array[i].toInt();
                }
            } else {
                for (int i=0;i<16;i++) {
                    QString slope_key = QString("slope[%1]").arg(i);
                    QString intercept_key = QString("intercept[%1]").arg(i);
                    QString slope_max_key = QString("slope_max[%1]").arg(i);
                    QString intercept_max_key = QString("intercept_max[%1]").arg(i);
                    if (np_data_obj[KEY_REG_KEY] == slope_key) {
                        QJsonArray value_array = np_data_obj[KEY_VALUE].toArray();
                        for (int j = 0; (j < 4) && (j < value_array.size()); j++) {
                            slope[i * 4 + j] = value_array[j].toDouble();
                        }
                        break;
                    } else if (np_data_obj[KEY_REG_KEY] == intercept_key) {
                        QJsonArray value_array = np_data_obj[KEY_VALUE].toArray();
                        for (int j = 0; (j < 4) && (j < value_array.size()); j++) {
                            intercept[i * 4 + j] = value_array[j].toDouble();
                        }
                        break;
                    } else if (np_data_obj[KEY_REG_KEY] == slope_max_key) {
                        slope_max[i] = np_data_obj[KEY_VALUE].toDouble();
                        break;
                    } else if (np_data_obj[KEY_REG_KEY] == intercept_max_key) {
                        intercept_max[i] = np_data_obj[KEY_VALUE].toDouble();
                        break;
                    }
                }
            }
        }
        for (int i=0;i<16;i++) {
            if (iso[i] > 0) {
                IsoItem *iso_item = new IsoItem();
                iso_item->setFrames(1);
                iso_item->setIso(iso[i]);
                iso_item->setfCaliSlope(slope[i * 4], slope[i * 4 + 1], slope[i * 4 + 2], slope[i * 4 + 3], slope_max[i]);
                iso_item->setfCaliIntercept(intercept[i * 4], intercept[i * 4 + 1], intercept[i * 4 + 2], intercept[i * 4 + 3], intercept_max[i]);
                iso_item->setDisplay(Qt::CheckState::Checked);

                bool success = iso_list_widget->addTableItem(iso_item);
                if (!success) {
                    QMessageBox message(QMessageBox::NoIcon, "Error", "Import noise calibration data, add iso item failed");
                    message.exec();
                    return;
                }
            }
        }
    }
}

void NoiseProfileCalibrationPageWidget::retranslateUi()
{
    BaseCalibrationPageWidget::retranslateUi();

    edit_roi_btn->setText(tr("Edit ROIs"));
    import_roi_btn->setText(tr("Import ROIs"));
    export_roi_btn->setText(tr("Export ROIs"));
}

int NoiseProfileCalibrationPageWidget::readCalibrationData(bool showMessage)
{
    qDebug("NoiseProfileCalibrationPageWidget::readModuleFromNetworkJsonrpc");
    if (!NetworkClientHelper::getInstance()->isNormalStatus()) {
        if (showMessage) {
            QMessageBox::warning(this, tr("Warning"), tr("Network is not connected!"), QMessageBox::Ok);
        }
        return -1;
    }

    QStringList struct_list = getStructList();
    QMap<QString, QString> struct_get_api_map = getStructGetApiMap();
    CALIBRATION_MODULE_ID module_id = static_cast<CALIBRATION_MODULE_ID>(property("cali_id").toInt());
    NetworkClientHelper::getInstance()->clearIdInfoJsonrpc();
    for (int i = 0; i < struct_list.size(); i++) {
        if ((!bnr_check_state) && (struct_list[i] == "TEAISP_BNR_NP_S") ) {
            continue;
        }

        QString cmd = CMD_READ_NOISE_PROFILE_CALIBRATION;
        if (struct_list[i] == "TEAISP_BNR_NP_S") {
            cmd = CMD_READ_NOISE_PROFILE_BNR_CALIBRATION;
        }

        QJsonObject send_obj;
        QJsonObject id_info;
        int jsonrpc_id = GlobalData::getInstance()->getJsonrpcId();
        send_obj[MSG_KEY_JSONRPC] = MSG_JSONRPC_VERSION;
        send_obj[MSG_KEY_METHOD] = struct_get_api_map[struct_list[i]];
        send_obj[MSG_KEY_ID] = jsonrpc_id;
        id_info[MSG_KEY_CMD] = cmd;
        id_info[MSG_KEY_MODULE] = getCaliModuleName(module_id);
        id_info[MSG_KEY_STRUCT] = struct_list[i];

        NetworkClientHelper::getInstance()->pushIdInfoJsonrpc(jsonrpc_id, id_info);
        NetworkClientHelper::getInstance()->sendCmd(std::move(send_obj));

        bool gotResponse = false;
        QTime stopTime = QTime::currentTime().addSecs(READ_WRITE_TIMEOUT);
        while (QTime::currentTime() < stopTime) {
            QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
            if (!GlobalData::getInstance()->getWaitForResponse()) {
                gotResponse = true;
                break;
            }
        }

        if (!gotResponse) {
            NetworkClientHelper::getInstance()->removeIdInfoJsonrpc(jsonrpc_id);
            GlobalData::getInstance()->setWaitForResponse(false);
            if (showMessage) {
                QString message = getTitle() + QString(" Page: Failed to read %1. Timeout: Can't get reponse from board within %2 seconds.").arg(struct_list[i]).arg(READ_WRITE_TIMEOUT);
                emit sigStatusLog(message, LOG_LEVEL::LOG_ERROR);
            }
            return -1;
        }
    }

    return 0;
}

void NoiseProfileCalibrationPageWidget::handleReceiveNoiseProfileCalibration(int id)
{
    qDebug("NoiseProfileCalibrationPageWidget::handleReceiveNoiseProfileCalibration.");
    QJsonObject data_obj = NetworkClientHelper::getInstance()->popCmdDataJsonrpc(id);
    if (!data_obj.isEmpty()) {
        QJsonObject result_obj = data_obj[MSG_KEY_RESULT].toObject();
        QJsonArray CalibrationCoef_array_temp;
        if (!result_obj.isEmpty()) {
            QJsonObject params_obj = result_obj[MSG_KEY_PARAMS].toObject();
            CalibrationCoef_array_temp = params_obj["CalibrationCoef"].toArray();
            if (CalibrationCoef_array_temp.size() == 32) {
                CalibrationCoef_max_array = CalibrationCoef_array_temp;
            } else {
                CalibrationCoef_array = CalibrationCoef_array_temp;
            }
        } else {
            qDebug("NoiseProfileCalibrationPageWidget::handleReceiveNoiseProfileCalibration. Error: result_obj is empty.");
        }
    } else {
        qDebug("NoiseProfileCalibrationPageWidget::handleReceiveNoiseProfileCalibration. Error: data_obj is empty.");
    }
}

QJsonObject NoiseProfileCalibrationPageWidget::GetModuleObjectToNetworkJsonrpc()
{
    qDebug("NoiseProfileCalibrationPageWidget::GetModuleObjectToNetworkJsonrpc.");
    QJsonObject module_obj;
    QStringList struct_list = getStructList();
    QMap<QString, QString> struct_set_api_map = getStructSetApiMap();
    CALIBRATION_MODULE_ID id = static_cast<CALIBRATION_MODULE_ID>(property("cali_id").toInt());
    QString module_name = getCaliModuleName(id);

    NetworkClientHelper::getInstance()->clearIdInfoJsonrpc();

    QList<IsoItem *> list = iso_list_widget->getDisplayIsoList();
    if (quickMode) {
        list = iso_item_list;
    } else {
        list = iso_list_widget->getDisplayIsoList();
    }
    std::sort(list.begin(), list.end(), compareData);

    QJsonObject send_obj;
    QJsonObject params_obj;
    QString cmd;
    for (auto struct_id : struct_list) {
        if ((!bnr_check_state) && (struct_id == "TEAISP_BNR_NP_S") ) {
            continue;
        }

        for (int i = 0; i < list.size(); i++) {
            int index = 0;
            int bnr_index = 0;
            int iso = list[i]->getIso();
            for (int j = 0; j < iso_list.count(); j++) {
                if (iso == iso_list[j]) {
                    index = j * 8;
                    bnr_index = j;
                    break;
                }
            }
            if (struct_id == "ISP_CMOS_NOISE_CALIBRATION_S") {
                CalibrationCoef_array[index] = list[i]->getfCaliSlopeB();
                CalibrationCoef_array[index+1] = list[i]->getfCaliInterceptB();
                CalibrationCoef_array[index+2] = list[i]->getfCaliSlopeGb();
                CalibrationCoef_array[index+3] = list[i]->getfCaliInterceptGb();
                CalibrationCoef_array[index+4] = list[i]->getfCaliSlopeGr();
                CalibrationCoef_array[index+5] = list[i]->getfCaliInterceptGr();
                CalibrationCoef_array[index+6] = list[i]->getfCaliSlopeR();
                CalibrationCoef_array[index+7] = list[i]->getfCaliInterceptR();
                params_obj["CalibrationCoef"] = CalibrationCoef_array;
                cmd = CMD_WRITE_NOISE_PROFILE_CALIBRATION;
            } else {
                if (struct_id == "TEAISP_BNR_NP_S") {
                    CalibrationCoef_max_array[bnr_index] = list[i]->getfCaliSlopeMax();
                    CalibrationCoef_max_array[16 + bnr_index] = list[i]->getffCaliInterceptMax();
                    params_obj["CalibrationCoef"] = CalibrationCoef_max_array;
                    cmd = CMD_WRITE_NOISE_PROFILE_BNR_CALIBRATION;
                }
            }
        }

        int jsonrpc_id = GlobalData::getInstance()->getJsonrpcId();
        send_obj[MSG_KEY_JSONRPC] = MSG_JSONRPC_VERSION;
        send_obj[MSG_KEY_METHOD] = struct_set_api_map[struct_id];
        send_obj[MSG_KEY_ID] = jsonrpc_id;
        send_obj[MSG_KEY_PARAMS] = params_obj;
        module_obj[struct_id] = send_obj;

        QJsonObject id_info;
        id_info[MSG_KEY_CMD] = cmd;
        id_info[MSG_KEY_MODULE] = module_name;
        id_info[MSG_KEY_STRUCT] = struct_id;
        NetworkClientHelper::getInstance()->pushIdInfoJsonrpc(jsonrpc_id, id_info);
    }

    return module_obj;
}

bool NoiseProfileCalibrationPageWidget::getCalibrationBnrBoxState()
{
    return bnr_check_state;
}

int NoiseProfileCalibrationPageWidget::handleQuickCalibration(QString path)
{
    if (rawItem.isEmpty()) {
        ClearData();
        return -1;
    }

    quickMode = true;
    for (int i=0; i<rawItem.size(); i++) {
        int ret = GetColorBlocks(i);
        if (ret < 0) {
            ClearData();
            return -1;
        }

        ret = loadBlcData(path, rawItem[i]->getISO());
        if (ret < 0) {
            ClearData();
            return -1;
        }

        raw_img_item_vec.clear();
        raw_img_item_vec.append(rawItem[i]);
        bool status = calibration();
        if (!status) {
            ClearData();
            return -1;
        }

        IsoItem *iso_item = new IsoItem();
        iso_item->setIso(rawItem[i]->getISO());
        iso_item->setfCaliSlope(fCaliSlopeB, fCaliSlopeGb, fCaliSlopeGr, fCaliSlopeR, fCaliSlopeMax);
        iso_item->setfCaliIntercept(fCaliInterceptB, fCaliInterceptGb, fCaliInterceptGr, fCaliInterceptR, fCaliInterceptMax);
        iso_item_list.append(iso_item);
    }

    QString exportfile = path + "/noise.json";
    handleExportData(exportfile);

    int ret = writeCalibrationData(false);
    ClearData();
    if (ret < 0) {
        return -2;
    }

    return 0;
}

void NoiseProfileCalibrationPageWidget::handleExportData(QString path)
{
    QJsonObject data;
    QJsonArray arr = getCalibrationData();
    CALIBRATION_MODULE_ID id = static_cast<CALIBRATION_MODULE_ID>(property("cali_id").toInt());
    data[PREFIX_CALI_STR + getCaliModuleName(id)] = arr;

    utils::saveJson(path, data);
}

int NoiseProfileCalibrationPageWidget::GetColorBlocks(int idx)
{
    if (rawItem.isEmpty() || rawItem.size() < idx) {
        return -1;
    }

    QString path = rawItem[idx]->getFilePath();
    int w = rawItem[idx]->getWidth();
    int h = rawItem[idx]->getHeight();
    int bayerid = rawItem[idx]->getBayer().toInt();
    bool wdr_mode = rawItem[idx]->getFrameMode() == FRAME_MODE::WDR ? true : false;
    vector<ColorBlock> bloks;
    if (idx == 0) {
        shared_ptr<GlobalData> global_data = GlobalData::getInstance();
        int lowThr = global_data->getSettings(SETTINGS_SECTION, COLOR_BLOCKS_DETECT_LOWTHR, 10).toInt();
        int highThr = global_data->getSettings(SETTINGS_SECTION, COLOR_BLOCKS_DETECT_HIGHTHR, 40).toInt();
        bloks = ColorBlocksDetect(path.toLocal8Bit().constData(), lowThr, highThr, w, h,
                                  bayer_strlist[bayerid].toStdString(), wdr_mode);
        if (bloks.size() != 24) {
            bloks.clear();
            qWarning() << "edge detect the bloks fail! Try to use the Y means detect!";
            bloks = ColorBlocksDetectV2(path.toLocal8Bit().constData(), w, h,
                         static_cast<utils::RAW_BAYER_ID>(bayerid), wdr_mode);
            if (bloks.size() != 24) {
                qWarning() << "Y means detect the bloks fail!";
                return -1;
            }
        }
        int roiW = bloks[0].width;
        int roiH = bloks[0].height;
        for (uint i = 0; i < bloks.size(); i++) {
            if (bloks[i].width < roiW) {
                roiW = bloks[i].width;
            }
            if (bloks[i].height < roiH) {
                roiH = bloks[i].height;
            }
        }

        roiW = roiW / 2;
        roiH = roiH / 2;
        roiW = roiW / 2 * 2;
        roiH = roiH / 2 * 2;

        int invalid_block_num = 0;
        QList<ISP_Point_S> pList;
        for (uint i = 0; i < bloks.size(); i++) {
            int x, y;
            int idx = bloks[i].id;
            x = bloks[i].centerX - roiW / 2;
            y = bloks[i].centerY - roiH / 2;
            x = x < 0 ? 0 : x;
            y = y < 0 ? 0 : y;

            ISP_Point_S p;
            p.x = x / 2 * 2;
            p.y = y / 2 * 2;
            pList.insert(idx, p);
            if (bloks[i].centerX == 0 || bloks[i].centerY == 0 ||
                    bloks[i].width == 0 || bloks[i].height == 0) {
                invalid_block_num++;
            }
        }
        color_block.roi_width = roiW;
        color_block.roi_height = roiH;
        color_block.Points.clear();

        for (int i = 0; i < pList.size(); i++) {
            color_block.Points.push_back(pList[i]);
            qDebug("color block %d: x=%d,y=%d,w=%d,h=%d", i, pList[i].x, pList[i].y, roiW, roiH);
        }
        if (invalid_block_num) {
            QMessageBox::warning(this, "Warning", QString("The raw is not clear enough! 24 colors were not all detected."), QMessageBox::Ok);
        }
    } else {
        qDebug("Using saved color block positions for image %d", idx);
        for (int i = 0; i < color_block.Points.size(); i++) {
            ISP_Point_S point = color_block.Points[i];
            qDebug("Reusing color block %d: x=%d, y=%d, w=%d, h=%d", i, point.x, point.y, color_block.roi_width, color_block.roi_height);
        }
    }

    return 0;
}

int NoiseProfileCalibrationPageWidget::QuickCalibrationLoadData(QString path)
{
    ClearData();
    QDir dir(path);
    QFileInfoList folder_list = dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);
    for(int i = 0; i < folder_list.size(); i++) {
        QString dir_name = folder_list.at(i).fileName();
        for (int j=0; j<iso_list.size(); j++) {
            QString name = "ISO" + QString::number(iso_list[j]);
            if (dir_name == name) {
                QString dirPath = path + "/" + dir_name;
                QStringList fileList;
                utils::findFiles(dirPath, ".raw", fileList, false);
                if (!fileList.isEmpty()) {
                    qDebug() << fileList[0];
                    RawImageItem *item = new RawImageItem(fileList[0], SCENE::COLOR24);
                    item->setISO(iso_list[j]);
                    rawItem.append(item);
                }
                break;
            }
        }
        if (rawItem.size() >= iso_list.size()) {
            break;
        }
    }

    if (!rawItem.isEmpty()) {
        int width = config.getWidth().toInt();
        int height = config.getHeight().toInt();
        QVariant bits = config.getBits();
        QVariant bayer = config.getBayer();
        utils::RAW_PACK_TYPE packed = config.getPacked();
        FRAME_MODE frame_mode = config.getFrameMode();
        for (int i=0; i<rawItem.size(); i++) {
            rawItem[i]->setImgSize(width, height);
            rawItem[i]->setBits(bits);
            rawItem[i]->setBayer(bayer);
            rawItem[i]->setPacked(packed);
            rawItem[i]->setFrameMode(frame_mode);
        }
    } else {
        ClearData();
        return -1;
    }

    return 0;
}

int NoiseProfileCalibrationPageWidget::loadBlcData(QString path, int iso)
{
    blc_imported = false;
    QString blc_json_file = path + QString("/../BLC/ISO%1/blc_iso=%2.json").arg(iso).arg(iso);
    QJsonObject data;
    QString error_msg;

    if (utils::loadJson(blc_json_file, data, error_msg) != 0) {
        return -1;
    }

    handleImportData(blc_json_file, data);

    return 0;
}

void NoiseProfileCalibrationPageWidget::HandleCalibrationRequst(QJsonObject params, RawConfigItem config)
{
    QString path;
    int ret = 0;
    if (params.contains("path") && !(params["path"].toString().isEmpty())) {
        path = params["path"].toString();
    }

    if (path.isEmpty()) {
        emit sigCaliStatus(CALIBRATION_MODULE_ID::NOISE_PROFILE, CALIBRATION_STATUS::CAL_ERROR, "Image path is empty!");
        return;
    }

    QDir dir(path);
    if (!dir.exists()) {
        emit sigCaliStatus(CALIBRATION_MODULE_ID::NOISE_PROFILE, CALIBRATION_STATUS::CAL_ERROR, "Image path not exists!");
        return;
    }

    if (cali_status == CALIBRATION_STATUS::CALIBRATION) {
        emit sigCaliStatus(CALIBRATION_MODULE_ID::NOISE_PROFILE, CALIBRATION_STATUS::CAL_ERROR, "Calibration is busy!");
        return;
    }

    if (params.contains("enable aibnr")) {
        bool en = params["enable aibnr"].toBool();
        if (TEAISP_bnr_checkbox != nullptr) {
            TEAISP_bnr_checkbox->setChecked(en);
        }
    }

    this->config = config;

    ret = QuickCalibrationLoadData(path);
    if (ret < 0) {
        emit sigCaliStatus(CALIBRATION_MODULE_ID::NOISE_PROFILE, CALIBRATION_STATUS::CAL_ERROR, "Load file fail!");
        return;
    }

    ret = handleQuickCalibration(path);
    if (ret == -1) {
        emit sigCaliStatus(CALIBRATION_MODULE_ID::NOISE_PROFILE, CALIBRATION_STATUS::CAL_ERROR, "Calibration fail!");
        return;
    } else if (ret == -2) {
        emit sigCaliStatus(CALIBRATION_MODULE_ID::NOISE_PROFILE, CALIBRATION_STATUS::WARNING, "Write data fail!");
        return;
    }

    emit sigCaliStatus(CALIBRATION_MODULE_ID::NOISE_PROFILE, CALIBRATION_STATUS::READY, "Calibration finish!");
}
