#include "raw_image_option_dialog.hpp"
#include "global_data.hpp"

RawImageOptionDialog::RawImageOptionDialog(QWidget *parent):
    BaseImageOptionDialog(parent)
{
    setWindowTitle(tr("Raw Image Options"));

    initOptions();
}

void RawImageOptionDialog::initOptions()
{
    QLabel *stream_mode_label = new QLabel("Stream Mode:       ");
    stream_mode_line = new QComboBox;
    stream_mode_line->addItems(mode_strlist);
    stream_mode_line->setCurrentIndex(0);
    QHBoxLayout *stream_layout = new QHBoxLayout;
    stream_layout->addStretch(0);
    stream_layout->addWidget(stream_mode_label);
    stream_layout->addWidget(stream_mode_line);
    stream_layout->addStretch(1);

    QLabel *bitdepth_label = new QLabel("Bit Depth:         ");
    bitdepth_box = new QComboBox;
    bitdepth_box->addItems(bits_list);
    bitdepth_box->setCurrentIndex(2);
    bitdepth_box->setEnabled(false);

    QLabel *bayer_label = new QLabel("     Bayer Format:");
    bayer_box = new QComboBox;
    bayer_box->addItems(bayer_strlist);
    bayer_box->setCurrentIndex(0);

    QHBoxLayout *bitbayer_layout = new QHBoxLayout;
    bitbayer_layout->addStretch(0);
    bitbayer_layout->addWidget(bitdepth_label);
    bitbayer_layout->addWidget(bitdepth_box);
    bitbayer_layout->addWidget(bayer_label);
    bitbayer_layout->addWidget(bayer_box);
    bitbayer_layout->addStretch(1);

    addOptionItem(stream_layout);
    addOptionItem(bitbayer_layout);
}

RawImgOption RawImageOptionDialog::getOptionData()
{
    RawImgOption op;
    op.file_path = this->file_path->text();
    op.width = this->w_res_line->text().toInt();
    op.height = this->h_res_line->text().toInt();
    op.mode = this->stream_mode_line->currentIndex();
    op.bits = this->bitdepth_box->currentIndex();
    op.bayer = this->bayer_box->currentIndex();

    return  op;
}

void RawImageOptionDialog::setData(RawImgOption options)
{
    w_res_line->setText(QString::number(options.width));
    h_res_line->setText(QString::number(options.height));
    stream_mode_line->setCurrentIndex(options.mode.toInt());
    bayer_box->setCurrentIndex(options.bayer.toInt());
    qDebug("Raw Option,width:%d,height:%d,mode:%d,bayer:%d\n",options.width,options.height,options.mode.toInt(),options.bayer.toInt());
}

void RawImageOptionDialog::setFilePath(QString file)
{
    this->file_path->setText(file);
    QStringList strNameList = file.split("/");
    if (strNameList.size() > 0) {
        QString strName = strNameList[strNameList.size() - 1];
        QStringList strParaList = strName.split("_");
        if (strParaList.size() >= 9) {
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
            if (strBayer == "RGGB") {
                bayer_box->setCurrentIndex(3);
            } else if (strBayer == "GRBG") {
                bayer_box->setCurrentIndex(2);
            } else if (strBayer == "GBRG") {
                bayer_box->setCurrentIndex(1);
            } else if (strBayer == "BGGR") {
                bayer_box->setCurrentIndex(0);
            } else {
                return;
            }

            QString strMode = strParaList[2];
            if (strMode == "Linear") {
                stream_mode_line->setCurrentIndex(0);
            } else if (strMode == "WDR") {
                stream_mode_line->setCurrentIndex(1);
            } else {
                return;
            }
        }
    }
}
