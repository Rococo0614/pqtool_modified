#include "isp_info_dialog.hpp"
#include "network_client_helper.hpp"

#include <QFileDialog>
#include <QSlider>
#include <QMessageBox>


IspInfoDialog::IspInfoDialog(QWidget *parent, int frame_num, bool roiMode) :
    QDialog(parent),
    frame_num(frame_num), roiMode(roiMode)
{
    setWindowTitle(tr("RAW ISP Information"));
    setWindowFlags(Qt::WindowCloseButtonHint);
    this->setFixedSize(800, 480);

    initUI();
    initEvent();
    retranslateUi();
}

void IspInfoDialog::initUI()
{
    main_layout = new QVBoxLayout(this);

    isp_ifo_group = new QGroupBox("ISP Information");
    QVBoxLayout *isp_grp_layout = new QVBoxLayout;
    isp_ifo_group->setLayout(isp_grp_layout);

    QGroupBox *frame_grp = new QGroupBox;
    frame_grp->setFixedHeight(60);
    QLabel *totalframes_label = new QLabel("TotalFrames:");
    totalframes_label->setFixedSize(100, 25);
    totalframes_line = new QLineEdit("0");
    totalframes_line->setFixedSize(40, 25);
    totalframes_line->setReadOnly(true);
    QLabel *frame_label = new QLabel("Pre/Next:");
    frame_label->setFixedSize(100, 25);
    frame_label->setEnabled(false);
    frame_line = new QLineEdit("0");
    frame_line->setFixedSize(40, 25);
    frame_line->setEnabled(false);
    QPushButton *head_btn = new QPushButton("<<");
    head_btn->setFixedSize(35, 25);
    head_btn->setEnabled(false);
    QPushButton *tail_btn = new QPushButton(">>");
    tail_btn->setFixedSize(35, 25);
    tail_btn->setEnabled(false);
    frame_slider = new QSlider(Qt::Horizontal);
    frame_slider->setFixedSize(80, 25);
    frame_slider->setRange(0, frame_num - 1);
    frame_slider->setEnabled(false);
    QPushButton *prev_btn = new QPushButton("<");
    prev_btn->setFixedSize(35, 25);
    prev_btn->setEnabled(false);
    QPushButton *next_btn = new QPushButton(">");
    next_btn->setFixedSize(35, 25);
    next_btn->setEnabled(false);
    QHBoxLayout *frame_layout = new QHBoxLayout;
    frame_layout->addWidget(totalframes_label, 1);
    frame_layout->addWidget(totalframes_line, 1);
    frame_layout->addStretch(1);
    frame_layout->addWidget(frame_label, 1);
    frame_layout->addWidget(frame_line, 1);
    frame_layout->addWidget(head_btn, 1);
    frame_layout->addWidget(prev_btn, 1);
    frame_layout->addWidget(frame_slider, 1);
    frame_layout->addWidget(next_btn, 1);
    frame_layout->addWidget(tail_btn, 1);
    frame_layout->setAlignment(Qt::AlignLeft);
    frame_grp->setLayout(frame_layout);

    QGridLayout *param_layout = new QGridLayout();
    QLabel *iso_label = new QLabel("ISO:");
    iso_label->setFixedSize(100, 25);
    iso_line = new QLineEdit("0");
    iso_line->setFixedSize(80, 25);
    iso_line->setReadOnly(true);
    QLabel *lv_label = new QLabel("LightValue:");
    lv_label->setFixedSize(100, 25);
    lv_line = new QLineEdit("0");
    lv_line->setFixedSize(80, 25);
    lv_line->setReadOnly(true);
    QLabel *ct_label = new QLabel("ColorTemp:");
    ct_label->setFixedSize(100, 25);
    ct_line = new QLineEdit("0");
    ct_line->setFixedSize(80, 25);
    ct_line->setReadOnly(true);
    param_layout->addWidget(iso_label, 0, 0, Qt::AlignLeft);
    param_layout->addWidget(iso_line, 0, 1, Qt::AlignLeft);
    param_layout->addWidget(lv_label, 0, 3, Qt::AlignLeft);
    param_layout->addWidget(lv_line, 0, 4, Qt::AlignLeft);
    param_layout->addWidget(ct_label, 0, 6, Qt::AlignLeft);
    param_layout->addWidget(ct_line, 0, 7, Qt::AlignLeft);

    QLabel *again_label = new QLabel("AGain:");
    again_label->setFixedSize(100, 25);
    again_line = new QLineEdit("0");
    again_line->setFixedSize(80, 25);
    again_line->setReadOnly(true);
    QLabel *exp_label = new QLabel("LongExpTime:");
    exp_label->setFixedSize(100, 25);
    exp_line = new QLineEdit("0");
    exp_line->setFixedSize(80, 25);
    exp_line->setReadOnly(true);
    QLabel *exp_label1 = new QLabel("ShortExpTime:");
    exp_label1->setFixedSize(100, 25);
    exp_line1 = new QLineEdit("0");
    exp_line1->setFixedSize(80, 25);
    exp_line1->setReadOnly(true);
    param_layout->addWidget(again_label, 1, 0, Qt::AlignLeft);
    param_layout->addWidget(again_line, 1, 1, Qt::AlignLeft);
    param_layout->addWidget(exp_label, 1, 3, Qt::AlignLeft);
    param_layout->addWidget(exp_line, 1, 4, Qt::AlignLeft);
    param_layout->addWidget(exp_label1, 1, 6, Qt::AlignLeft);
    param_layout->addWidget(exp_line1, 1, 7, Qt::AlignLeft);

    QLabel *dgain_label = new QLabel("DGain:");
    dgain_label->setFixedSize(100, 25);
    dgain_line = new QLineEdit("0");
    dgain_line->setFixedSize(80, 25);
    dgain_line->setReadOnly(true);
    QLabel *ratio_label = new QLabel("Ratio:");
    ratio_label->setFixedSize(100, 25);
    ratio_line = new QLineEdit("0");
    ratio_line->setFixedSize(80, 25);
    ratio_line->setReadOnly(true);
    QLabel *ispdgain_label = new QLabel("ISPDGain:");
    ispdgain_label->setFixedSize(100, 25);
    ispdgain_line = new QLineEdit("0");
    ispdgain_line->setFixedSize(80, 25);
    ispdgain_line->setReadOnly(true);
    param_layout->addWidget(dgain_label, 2, 0, Qt::AlignLeft);
    param_layout->addWidget(dgain_line, 2, 1, Qt::AlignLeft);
    param_layout->addWidget(ispdgain_label, 2, 3, Qt::AlignLeft);
    param_layout->addWidget(ispdgain_line, 2, 4, Qt::AlignLeft);
    param_layout->addWidget(ratio_label, 2, 6, Qt::AlignLeft);
    param_layout->addWidget(ratio_line, 2, 7, Qt::AlignLeft);

    QHBoxLayout *set_all_layout = new QHBoxLayout();
    QPushButton *set_all_btn = new QPushButton("Apply To All");
    set_all_btn->setFixedSize(110, 25);
    set_all_layout->addStretch();
    set_all_layout->addWidget(set_all_btn);

    isp_grp_layout->addWidget(frame_grp);
    isp_grp_layout->addLayout(param_layout);
    //isp_grp_layout->addLayout(set_all_layout);

    QGroupBox *blacklvl_group = new QGroupBox;
    QHBoxLayout *blc_layout = new QHBoxLayout;
    QLabel *blc_label = new QLabel("BlackLevel:");
    blc_label->setFixedSize(100, 25);
    blc1 = new QLineEdit("0");
    blc1->setFixedSize(80, 25);
    blc1->setReadOnly(true);
    blc2= new QLineEdit("0");
    blc2->setFixedSize(80, 25);
    blc2->setReadOnly(true);
    blc3 = new QLineEdit("0");
    blc3->setFixedSize(80, 25);
    blc3->setReadOnly(true);
    blc4 = new QLineEdit("0");
    blc4->setFixedSize(80, 25);
    blc4->setReadOnly(true);
    QPushButton *refresh_btn = new QPushButton("Refresh");
    refresh_btn->setFixedSize(100, 25);
    blc_layout->addWidget(blc_label, 1);
    blc_layout->addWidget(blc1, 1);
    blc_layout->addWidget(blc2, 1);
    blc_layout->addWidget(blc3, 1);
    blc_layout->addWidget(blc4, 1);
    blc_layout->addStretch(2);
    //blc_layout->addWidget(refresh_btn, 2);
    blacklvl_group->setFixedHeight(60);
    blacklvl_group->setLayout(blc_layout);

    QGroupBox *roi_group = new QGroupBox("ROI Infomation");
    QVBoxLayout *roi_layout = new QVBoxLayout;
    QHBoxLayout *src_layout = new QHBoxLayout;
    QLabel *roi_frame_label = new QLabel("Frames:");
    roi_frame_label->setFixedSize(100, 25);
    roi_frame_line = new QLineEdit("0");
    roi_frame_line->setReadOnly(true);
    roi_frame_line->setFixedSize(60, 25);
    QLabel *roi_width_label = new QLabel("Width:");
    roi_width_label->setFixedSize(100, 25);
    roi_width_line = new QLineEdit("0");
    roi_width_line->setReadOnly(true);
    roi_width_line->setFixedSize(60, 25);
    QLabel *roi_height_label = new QLabel("Height:");
    roi_height_label->setFixedSize(100, 25);
    roi_height_line = new QLineEdit("0");
    roi_height_line->setReadOnly(true);
    roi_height_line->setFixedSize(60, 25);
    src_layout->addWidget(roi_frame_label);
    src_layout->addWidget(roi_frame_line);
    src_layout->addWidget(roi_width_label);
    src_layout->addWidget(roi_width_line);
    src_layout->addWidget(roi_height_label);
    src_layout->addWidget(roi_height_line);
    src_layout->addStretch(2);
    QHBoxLayout *crop_layout = new QHBoxLayout;
    QLabel *cropX_label = new QLabel("Crop X:");
    cropX_label->setFixedSize(100, 25);
    cropX_line = new QLineEdit("0");
    cropX_line->setFixedSize(60, 25);
    cropX_line->setToolTip("Must be even number!");
    connect(cropX_line, &QLineEdit::editingFinished, [=]{
        int x = cropX_line->text().toInt();
        x = (x+1)/2*2;
        x = qBound(0, x, roiInfo.width - roiInfo.cropWidth);
        cropX_line->setText(QString::number(x));
        roiInfo.cropX = x;
    });
    QLabel *cropY_label = new QLabel("Crop Y:");
    cropY_label->setFixedSize(100, 25);
    cropY_line = new QLineEdit("0");
    cropY_line->setFixedSize(60, 25);
    connect(cropY_line, &QLineEdit::editingFinished, [=]{
        int y = cropY_line->text().toInt();
        y = qBound(0, y, roiInfo.height - roiInfo.cropHeight);
        cropY_line->setText(QString::number(y));
        roiInfo.cropY = y;
    });
    QLabel *crop_width_label = new QLabel("Crop Width:");
    crop_width_label->setFixedSize(100, 25);
    crop_width_line = new QLineEdit("0");
    crop_width_line->setFixedSize(60, 25);
    connect(crop_width_line, &QLineEdit::editingFinished, [=]{
        int w = crop_width_line->text().toInt();
        w = qBound(64, w, roiInfo.width - roiInfo.cropX);
        crop_width_line->setText(QString::number(w));
        roiInfo.cropWidth = w;
    });
    QLabel *crop_height_label = new QLabel("Crop Height:");
    crop_height_label->setFixedSize(100, 25);
    crop_height_line = new QLineEdit("0");
    crop_height_line->setFixedSize(60, 25);
    connect(crop_height_line, &QLineEdit::editingFinished, [=]{
        int h = crop_height_line->text().toInt();
        h = qBound(32, h, roiInfo.height - roiInfo.cropY);
        crop_height_line->setText(QString::number(h));
        roiInfo.cropHeight = h;
    });
    crop_layout->addWidget(cropX_label);
    crop_layout->addWidget(cropX_line);
    crop_layout->addWidget(cropY_label);
    crop_layout->addWidget(cropY_line);
    crop_layout->addWidget(crop_width_label);
    crop_layout->addWidget(crop_width_line);
    crop_layout->addWidget(crop_height_label);
    crop_layout->addWidget(crop_height_line, 20);
    crop_layout->addStretch(1);
    roi_layout->addLayout(src_layout);
    roi_layout->addLayout(crop_layout);
    roi_group->setFixedHeight(120);
    roi_group->setLayout(roi_layout);
    if (!roiMode) {
        roi_group->setEnabled(false);
    }

    save_btn = new QPushButton;
    QHBoxLayout *h_layout = new QHBoxLayout();
    h_layout->addStretch();
    h_layout->addWidget(save_btn);

    main_layout->addWidget(isp_ifo_group);
    main_layout->addWidget(blacklvl_group);
    main_layout->addWidget(roi_group);
    main_layout->addLayout(h_layout);

    connect(head_btn, &QPushButton::clicked, [=]{
        cur_idx = 0;
        frame_slider->setValue(cur_idx);
        frame_line->setText(QString::number(cur_idx));
        updateIspInfoDisp();
    });
    connect(tail_btn, &QPushButton::clicked, [=]{
        cur_idx = frame_num - 1;
        frame_slider->setValue(cur_idx);
        frame_line->setText(QString::number(cur_idx));
        updateIspInfoDisp();
    });
    connect(prev_btn, &QPushButton::clicked, [=]{
        cur_idx = qBound(0, cur_idx - 1, frame_num - 1);
        frame_slider->setValue(cur_idx);
        frame_line->setText(QString::number(cur_idx));
        updateIspInfoDisp();
    });
    connect(next_btn, &QPushButton::clicked, [=]{
        cur_idx = qBound(0, cur_idx + 1, frame_num - 1);
        frame_slider->setValue(cur_idx);
        frame_line->setText(QString::number(cur_idx));
        updateIspInfoDisp();
    });

    connect(frame_slider, &QSlider::valueChanged, [=](int value) {
        cur_idx = qBound(0, value, frame_num - 1);
        frame_line->setText(QString::number(cur_idx));
        updateIspInfoDisp();
    });

    connect(frame_line, &QLineEdit::textChanged, this, std::bind(&IspInfoDialog::cellEdited, this, 0, frame_line));
    connect(again_line, &QLineEdit::textChanged, this, std::bind(&IspInfoDialog::cellEdited, this, 1, again_line));
    connect(dgain_line, &QLineEdit::textChanged, this, std::bind(&IspInfoDialog::cellEdited, this, 2, dgain_line));
    connect(ispdgain_line, &QLineEdit::textChanged, this, std::bind(&IspInfoDialog::cellEdited, this, 3, ispdgain_line));
    connect(exp_line, &QLineEdit::textChanged, this, std::bind(&IspInfoDialog::cellEdited, this, 4, exp_line));
    connect(ratio_line, &QLineEdit::textChanged, this, std::bind(&IspInfoDialog::cellEdited, this, 5, ratio_line));

    connect(set_all_btn, &QPushButton::clicked, [=]{
        RawIspInfo info;
        info.AGain = again_line->text().toInt();
        info.ExpTime[0] = exp_line->text().toInt();
        info.ExpTime[1] = exp_line1->text().toInt();
        info.DGain = dgain_line->text().toInt();
        info.ISPDGain = ispdgain_line->text().toInt();
        info.Ratio = ratio_line->text().toInt();
        this->RawIspInfo_list.clear();
        for (int i = 0; i < frame_num; i++)
        {
            RawIspInfo_list.append(info);
        }
    });

    connect(refresh_btn, &QPushButton::clicked, [=] {
        if (!NetworkClientHelper::getInstance()->isNormalStatus()) {
            QMessageBox::warning(this, tr("Warning"), tr("Network is not connected!"), QMessageBox::Ok);
            return;
        }
        else {
            NetworkClientHelper::getInstance()->sendCmd(CMD_GET_BLACK_LEVEL);
        }
    });
    connect(save_btn, &QPushButton::clicked, this, &IspInfoDialog::handleOk);
}

void IspInfoDialog::updateIspInfoDisp()
{
    totalframes_line->setText(QString::number(frame_num));
    iso_line->setText(QString::number(RawIspInfo_list[cur_idx].ISO));
    lv_line->setText(QString::number(RawIspInfo_list[cur_idx].LightValue));
    ct_line->setText(QString::number(RawIspInfo_list[cur_idx].ColorTemp));
    again_line->setText(QString::number(RawIspInfo_list[cur_idx].AGain));
    dgain_line->setText(QString::number(RawIspInfo_list[cur_idx].DGain));
    ispdgain_line->setText(QString::number(RawIspInfo_list[cur_idx].ISPDGain));
    exp_line->setText(QString::number(RawIspInfo_list[cur_idx].ExpTime[0]));
    exp_line1->setText(QString::number(RawIspInfo_list[cur_idx].ExpTime[1]));
    ratio_line->setText(QString::number(RawIspInfo_list[cur_idx].Ratio));
}

void IspInfoDialog::updateBlackLvlInfoDisp()
{
    blc1->setText(QString::number(black_lvl[0]));
    blc2->setText(QString::number(black_lvl[1]));
    blc3->setText(QString::number(black_lvl[2]));
    blc4->setText(QString::number(black_lvl[3]));
}

void IspInfoDialog::slotGetBlackLvl(QString cmd)
{
    if (cmd == CMD_GET_BLACK_LEVEL) {
        QByteArray data = NetworkClientHelper::getInstance()->popCmdData(cmd);
        //Todo
    }
}

void IspInfoDialog::cellEdited(int idx, QLineEdit *lineEdit)
{
    if (idx < 0 || idx > 3 || lineEdit == nullptr)
        return;

    int value = lineEdit->text().toInt();
    switch(idx) {
        case 0:
        {
            value = qBound(0, value, frame_num - 1);
            cur_idx = value;
            lineEdit->setText(QString::number(value));
            frame_slider->setValue(value);
            updateIspInfoDisp();
            break;
        }
        case 1:
        {
            RawIspInfo_list[cur_idx].AGain = value;
            break;
        }
        case 2:
        {
            RawIspInfo_list[cur_idx].DGain = value;
            break;
        }
        case 3:
        {
            RawIspInfo_list[cur_idx].ISPDGain = value;
            break;
        }
        case 4:
        {
            RawIspInfo_list[cur_idx].ExpTime[0] = value;
            break;
        }
        case 5:
        {
            RawIspInfo_list[cur_idx].Ratio = value;
            break;
        }
    default:
        break;
    }
}

void IspInfoDialog::initEvent()
{
    shared_ptr<NetworkClientHelper> network_helper = NetworkClientHelper::getInstance();
    connect(network_helper.get(), &NetworkClientHelper::sigReceiveBlackLvlMsg, this, &IspInfoDialog::slotGetBlackLvl);
}

void IspInfoDialog::retranslateUi()
{
    save_btn->setText(tr("OK"));
}

void IspInfoDialog::handleOk()
{
    this->accept();
}

void IspInfoDialog::loadIspInfo(int frame_num, QList<RawIspInfo> RawIspInfo_list)
{
    this->frame_num = frame_num;
    this->RawIspInfo_list = RawIspInfo_list;
    if (this->RawIspInfo_list.size() == 0) {
        RawIspInfo info;
        for (int i = 0; i < frame_num; i++)
        {
            this->RawIspInfo_list.append(info);
        }
    }
}

QList<RawIspInfo> IspInfoDialog::getIspInfo()
{
    return this->RawIspInfo_list;
}

void IspInfoDialog::loadBlackLvlInfo(QList<int> black_lvl_list)
{
    this->black_lvl = black_lvl_list;
}

QList<int> IspInfoDialog::getBlackLvl()
{
    QList<int> black_lvl;
    black_lvl.append(blc1->text().toInt());
    black_lvl.append(blc2->text().toInt());
    black_lvl.append(blc3->text().toInt());
    black_lvl.append(blc4->text().toInt());
    return black_lvl;
}

void IspInfoDialog::loadRoiInfo(RoiInfo roiInfo)
{
    this->roiInfo = roiInfo;
}

void IspInfoDialog::updateRoiInfo()
{
    roi_frame_line->setText(QString::number(roiInfo.frames));
    roi_width_line->setText(QString::number(roiInfo.width));
    roi_height_line->setText(QString::number(roiInfo.height));
    cropX_line->setText(QString::number(roiInfo.cropX));
    cropY_line->setText(QString::number(roiInfo.cropY));
    crop_width_line->setText(QString::number(roiInfo.cropWidth));
    crop_height_line->setText(QString::number(roiInfo.cropHeight));
}

RoiInfo IspInfoDialog::getRoiInfo()
{
    return this->roiInfo;
}


