#include "yuv_image_option_dialog.hpp"
#include "global_data.hpp"

YuvImageOptionDialog::YuvImageOptionDialog(QWidget *parent):
    BaseImageOptionDialog(parent)
{
    setWindowTitle(tr("Yuv Image Options"));

    initOptions();
}

void YuvImageOptionDialog::initOptions()
{
    QLabel *bitwidth_label = new QLabel("BitWidth:");
    bitwidth_box = new QComboBox;
    bitwidth_box->addItems(bitwidth_list);
    bitwidth_box->setCurrentIndex(0);
    bitwidth_box->setDisabled(true);
    QHBoxLayout *stream_layout = new QHBoxLayout;
    stream_layout->addStretch(0);
    stream_layout->addWidget(bitwidth_label);
    stream_layout->addWidget(bitwidth_box);
    stream_layout->addStretch(1);

    QLabel *pixeltype_label = new QLabel("PixelType:");
    pixeltype_box = new QComboBox;
    pixeltype_box->addItems(pixeltype_list);
    pixeltype_box->setCurrentIndex(0);

    QLabel *videotype_label = new QLabel("VideoType:");
    videotype_box = new QComboBox;
    videotype_box->addItems(videotype_list);
    videotype_box->setCurrentIndex(0);
    videotype_box->setDisabled(true);

    QHBoxLayout *bitbayer_layout = new QHBoxLayout;
    bitbayer_layout->addStretch(0);
    bitbayer_layout->addWidget(pixeltype_label);
    bitbayer_layout->addWidget(pixeltype_box);
    bitbayer_layout->addWidget(videotype_label);
    bitbayer_layout->addWidget(videotype_box);
    bitbayer_layout->addStretch(1);

    addOptionItem(stream_layout);
    addOptionItem(bitbayer_layout);
}

void YuvImageOptionDialog::setOptionData(YuvImgOption opt)
{
    setResolution(QString::number(opt.width), QString::number(opt.height));
    pixeltype_box->setCurrentIndex(opt.pixelType.toUInt());
}

YuvImgOption YuvImageOptionDialog::getOptionData()
{
    YuvImgOption op;
    op.file_path = this->file_path->text();
    op.width = this->w_res_line->text().toInt();
    op.height = this->h_res_line->text().toInt();
    op.bitwidth = bitwidth_box->currentIndex();
    op.pixelType = pixeltype_box->currentIndex();
    op.videoType = this->videotype_box->currentIndex();

    return  op;
}

void YuvImageOptionDialog::setFilePath(QString file)
{
    this->file_path->setText(file);
    QStringList strNameList = file.split("/");
    if (strNameList.size() > 0) {
        QString strName = strNameList[strNameList.size() - 1];
        QStringList strParaList = strName.split("_");
        if (strParaList.size() > 2) {
            QStringList strResList = strParaList[0].split("X");
            if (strResList.size() == 2) {
                int resX = strResList[0].toInt();
                int resY = strResList[1].toInt();
                if ((resX != 0) && (resY != 0) ) {
                    setResolution(strResList[0], strResList[1]);
                } else {
                    return;
                }
            }

            QString strBayer = strParaList[1];
            if (strBayer == "YUYV") {
                pixeltype_box->setCurrentIndex(0);
            } else if (strBayer == "NV21") {
                pixeltype_box->setCurrentIndex(1);
            } else if (strBayer == "YUV420") {
                pixeltype_box->setCurrentIndex(2);
            } else if (strBayer == "NV12") {
                pixeltype_box->setCurrentIndex(3);
            } else {
                return;
            }
        }
    }
}
