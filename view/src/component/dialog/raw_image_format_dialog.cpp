#include "raw_image_format_dialog.hpp"
#include "utils.hpp"
#include <QFileDialog>

const QString DEFAULT_WIDTH = "1920";
const QString DEFAULT_HEIGHT = "1080";
const int ITEM_WIDTH = 120;

using utils::RAW_BITS;
using utils::RAW_PACK_TYPE;

RawImageFormatDialog::RawImageFormatDialog(QWidget *parent) :
    QDialog(parent)
{
    setWindowTitle(tr("Raw Format Dialog"));
    setWindowFlags(Qt::WindowCloseButtonHint);
    this->setMinimumWidth(600);

    initUI();
    retranslateUi();
}

void RawImageFormatDialog::initUI()
{
    main_layout = new QVBoxLayout(this);

    QHBoxLayout *path_layout = new QHBoxLayout();
    filepath_label = new QLabel;
    filepath_edit = new QLineEdit;
    filepath_edit->setMinimumWidth(500);
    filepath_edit->setReadOnly(true);
    path_layout->addWidget(filepath_label);
    path_layout->addWidget(filepath_edit);

    QHBoxLayout *scene_layout = new QHBoxLayout();
    scene_label = new QLabel;
    scene_label->setFixedWidth(ITEM_WIDTH);
    scene_combobox = new QComboBox;
    scene_combobox->setFixedWidth(ITEM_WIDTH);
    for (int i = static_cast<int>(SCENE::BEGIN); i < static_cast<int>(SCENE::COLOR124); i++) {
        scene_combobox->addItem(getSceneName(static_cast<SCENE>(i)), i);
    }
    scene_layout->addWidget(scene_label);
    scene_layout->addWidget(scene_combobox);
    scene_layout->addStretch();

    width_label = new QLabel;
    width_label->setFixedWidth(ITEM_WIDTH);
    width_line_edit = new QLineEdit(DEFAULT_WIDTH);
    width_line_edit->setFixedWidth(ITEM_WIDTH);

    height_label = new QLabel;
    height_label->setFixedWidth(ITEM_WIDTH);
    height_line_edit = new QLineEdit(DEFAULT_HEIGHT);
    height_line_edit->setFixedWidth(ITEM_WIDTH);

    bits_label = new QLabel;
    bits_label->setFixedWidth(ITEM_WIDTH);
    bits_combobox = new QComboBox;
    bits_combobox->setFixedWidth(ITEM_WIDTH);
    QMap<QString, RAW_BITS> bits_map = {{"8", RAW_BITS::BITS_8},
                                        {"10", RAW_BITS::BITS_10},
                                        {"12", RAW_BITS::BITS_12},
                                        {"16", RAW_BITS::BITS_16}};
    for (auto bits_key : bits_map.keys()) {
        bits_combobox->addItem(bits_key, static_cast<int>(bits_map[bits_key]));
    }
    bits_combobox->setCurrentIndex(bits_combobox->findData(static_cast<int>(RAW_BITS::BITS_12)));

    bayer_label = new QLabel;
    bayer_label->setFixedWidth(ITEM_WIDTH);
    bayer_combobox = new QComboBox;
    bayer_combobox->setFixedWidth(ITEM_WIDTH);
    for (int i = utils::BAYER_ID_BEGIN; i < utils::BAYER_ID_RGB; i++) {
        bayer_combobox->addItem(utils::getBayerName(i), i);
    }
    bayer_combobox->setCurrentIndex(utils::BAYER_ID_BG);

    ir_position_label = new QLabel;
    ir_position_label->setFixedWidth(ITEM_WIDTH);
    ir_position_combobox = new QComboBox;
    ir_position_combobox->setFixedWidth(ITEM_WIDTH);
    for (int i = utils::IR_NO; i < utils::IR_MAX; i++) {
       ir_position_combobox->addItem(utils::getIrPositionName(i), i);
    }
    ir_position_combobox->setCurrentIndex(utils::IR_NO);

    packed_label = new QLabel;
    packed_label->setFixedWidth(ITEM_WIDTH);
    packed_combobox = new QComboBox;
    packed_combobox->setFixedWidth(ITEM_WIDTH);
    QMap<QString, RAW_PACK_TYPE> pack_map = {{"Non-packed", RAW_PACK_TYPE::RAW_UNPACKED},
                                        {"Packed", RAW_PACK_TYPE::RAW_PACKED},
                                        {"Dpcm6", RAW_PACK_TYPE::RAW_DPCM6}};
    for (auto pack_key : pack_map.keys()) {
        packed_combobox->addItem(pack_key, static_cast<int>(pack_map[pack_key]));
    }

    packed_combobox->setCurrentIndex(packed_combobox->findData(static_cast<int>(RAW_PACK_TYPE::RAW_UNPACKED)));

    frame_mode_label = new QLabel;
    frame_mode_label->setFixedWidth(ITEM_WIDTH);
    frame_mode_combobox = new QComboBox;
    frame_mode_combobox->setFixedWidth(ITEM_WIDTH);
    QMap<QString, int> frame_mode_list = {{"Linear", 0}, {"WDR", 1}};
    for (auto frame_mode_item : frame_mode_list.toStdMap()) {
        frame_mode_combobox->addItem(frame_mode_item.first, frame_mode_item.second);
    }

    QGroupBox *option_group = new QGroupBox("Option");
    QGridLayout *option_layout = new QGridLayout;
    option_layout->addWidget(width_label, 0, 0, Qt::AlignLeft);
    option_layout->addWidget(width_line_edit, 0, 1, Qt::AlignLeft);
    option_layout->addWidget(height_label, 0, 3, Qt::AlignLeft);
    option_layout->addWidget(height_line_edit, 0, 4, Qt::AlignLeft);
    option_layout->addWidget(bits_label, 1, 0, Qt::AlignLeft);
    option_layout->addWidget(bits_combobox, 1, 1, Qt::AlignLeft);
    option_layout->addWidget(bayer_label, 1, 3, Qt::AlignLeft);
    option_layout->addWidget(bayer_combobox, 1, 4, Qt::AlignLeft);
    option_layout->addWidget(ir_position_label, 2, 0, Qt::AlignLeft);
    option_layout->addWidget(ir_position_combobox, 2, 1, Qt::AlignLeft);
    option_layout->addWidget(packed_label, 2, 3, Qt::AlignLeft);
    option_layout->addWidget(packed_combobox, 2, 4, Qt::AlignLeft);
    option_layout->addWidget(frame_mode_label, 3, 0, Qt::AlignLeft);
    option_layout->addWidget(frame_mode_combobox, 3, 1, Qt::AlignLeft);
    option_group->setLayout(option_layout);

    QHBoxLayout *select_layout = new QHBoxLayout();
    ok_btn = new QPushButton;
    ok_btn->setFixedWidth(ITEM_WIDTH);
    cancel_btn = new QPushButton;
    cancel_btn->setFixedWidth(ITEM_WIDTH);
    select_layout->addStretch();
    select_layout->addWidget(ok_btn);
    select_layout->addWidget(cancel_btn);

    main_layout->addLayout(path_layout);
    main_layout->addWidget(option_group);
    main_layout->addLayout(scene_layout);
    main_layout->addLayout(select_layout);

    connect(ok_btn, &QPushButton::clicked, this, &RawImageFormatDialog::handleOk);
    connect(cancel_btn, &QPushButton::clicked, this, &RawImageFormatDialog::handleCancel);
    // 12bit -> packtype
    connect(bits_combobox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &RawImageFormatDialog::bitsComboboxTextChanged);
}

void RawImageFormatDialog::retranslateUi()
{
    ok_btn->setText(tr("OK"));
    cancel_btn->setText(tr("CANCEL"));
    width_label->setText(tr("Width:"));
    height_label->setText(tr("Height:"));
    bits_label->setText(tr("Bits:"));
    bayer_label->setText(tr("Bayer:"));
    ir_position_label->setText(tr("IR position:"));
    packed_label->setText(tr("Packeted:"));
    filepath_label->setText("File Path:");
    scene_label->setText("RAW Scene:");
    frame_mode_label->setText("Frame Mode:");
}

void RawImageFormatDialog::setSceneLabelState(bool state)
{
    scene_label->setVisible(state);
    scene_combobox->setVisible(state);
}

void RawImageFormatDialog::handleOk()
{
    this->accept();
}

void RawImageFormatDialog::handleCancel()
{
    this->reject();
}

std::tuple<QString, QString> RawImageFormatDialog::getSize()
{
    std::tuple<QString, QString> result = std::make_tuple(width_line_edit->text(), height_line_edit->text());
    return result;
}

QVariant RawImageFormatDialog::getBits()
{
    return bits_combobox->currentData();
}

QVariant RawImageFormatDialog::getBayer()
{
    return bayer_combobox->currentData();
}

utils::RAW_PACK_TYPE RawImageFormatDialog::getPacked()
{
    return static_cast<utils::RAW_PACK_TYPE>(packed_combobox->currentData().toInt());
}

QVariant RawImageFormatDialog::getIrPosition()
{
    return ir_position_combobox->currentData();
}

void RawImageFormatDialog::setScene(SCENE sence)
{
    scene_combobox->setCurrentIndex(static_cast<int>(sence));
}

SCENE RawImageFormatDialog::getScene()
{
    return static_cast<SCENE>(scene_combobox->currentData().toInt());
}

FRAME_MODE RawImageFormatDialog::getFrameMode()
{
    return static_cast<FRAME_MODE>(frame_mode_combobox->currentData().toInt());
}

void RawImageFormatDialog::setData(QString w, QString h, QVariant bits, QVariant bayer,
                                   utils::RAW_PACK_TYPE pack, QVariant ir_pst, SCENE scene, FRAME_MODE frame_mode)
{
    width_line_edit->setText(w);
    height_line_edit->setText(h);
    bits_combobox->setCurrentIndex(bits_combobox->findText(bits.toString()));
    bayer_combobox->setCurrentIndex(bayer.toUInt());
    packed_combobox->setCurrentIndex(packed_combobox->findText(utils::getPackTypeName(pack)));
    ir_position_combobox->setCurrentIndex(ir_pst.toUInt());
    scene_combobox->setCurrentIndex(static_cast<int>(scene));
    frame_mode_combobox->setCurrentIndex(static_cast<int>(frame_mode));

    qDebug("raw config,w:%d,h:%d,bits:%d,bayer:%d,pack:%s,scene:%d,frame_mode:%d\n",
           w.toInt(),h.toInt(),bits.toInt(),bayer.toInt(),utils::getPackTypeName(pack).data(),static_cast<int>(scene),static_cast<int>(frame_mode));
}

void RawImageFormatDialog::setFilepath(QString path)
{
    if (path.isEmpty()) {
        return;
    }

    filepath_edit->setText(path);
    QStringList strNameList = path.split("/");
    if (strNameList.size() > 0) {
        QString strName = strNameList[strNameList.size() - 1];
        QStringList strParaList = strName.split("_");
        if (strParaList.size() >= 9) {
            QStringList strResList = strParaList[0].split("X");
            if (strResList.size() == 2) {
                int resX = strResList[0].toInt();
                int resY = strResList[1].toInt();
                if ((resX != 0) && (resY != 0) ) {
                    width_line_edit->setText(strResList[0]);
                    height_line_edit->setText(strResList[1]);
                } else {
                    return;
                }
            }

            QString strBayer = strParaList[1];
            if (strBayer == "RGGB") {
                bayer_combobox->setCurrentIndex(3);
            } else if (strBayer == "GRBG") {
                bayer_combobox->setCurrentIndex(2);
            } else if (strBayer == "GBRG") {
                bayer_combobox->setCurrentIndex(1);
            } else if (strBayer == "BGGR") {
                bayer_combobox->setCurrentIndex(0);
            } else {
                return;
            }

            QStringList strBitsList = strParaList[4].split("=");
            if ((strBitsList.size() == 2) && (strBitsList[0] == "-bits")) {
                if (strBitsList[1] == "8") {
                    bits_combobox->setCurrentIndex(bits_combobox->findText("8"));
                } else if (strBitsList[1] == "10") {
                    bits_combobox->setCurrentIndex(bits_combobox->findText("10"));
                } else if (strBitsList[1] == "12") {
                    bits_combobox->setCurrentIndex(bits_combobox->findText("12"));
                } else if (strBitsList[1] == "16") {
                    bits_combobox->setCurrentIndex(bits_combobox->findText("16"));
                } else {
                    return;
                }
            }

            QString strFrameMode = strParaList[2];
            if (strFrameMode == "Linear") {
                frame_mode_combobox->setCurrentIndex(0);
            } else if (strFrameMode == "WDR") {
                frame_mode_combobox->setCurrentIndex(1);
            } else {
                return;
            }
        }
    }
}

void RawImageFormatDialog::bitsComboboxTextChanged(int index)
{
    packed_combobox->clear();
    if (index == bits_combobox->findData(static_cast<int>(RAW_BITS::BITS_12))) {
        QMap<QString, RAW_PACK_TYPE> pack_map = {{"Non-packed", RAW_PACK_TYPE::RAW_UNPACKED},
                                            {"Packed", RAW_PACK_TYPE::RAW_PACKED},
                                            {"Dpcm6", RAW_PACK_TYPE::RAW_DPCM6}};
        for (auto pack_key : pack_map.keys()) {
            packed_combobox->addItem(pack_key, static_cast<int>(pack_map[pack_key]));
        }
        packed_combobox->setCurrentIndex(packed_combobox->findData(static_cast<int>(RAW_PACK_TYPE::RAW_UNPACKED)));
    } else {
        packed_combobox->addItem("Non-packed", 0);
        packed_combobox->setCurrentIndex(0);
    }
}

