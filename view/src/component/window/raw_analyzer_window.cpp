#include "raw_analyzer_window.hpp"
#include "utils.hpp"

#include <QFileDialog>
#include <QSlider>
#include <QMessageBox>
#include <algorithm>
#include <QSet>
#include <QString>
#include <QFile>
#include <QDateTime>

#define ITEM_WIDTH 100
#define DEFAULT_WIDTH "1920"
#define DEFAULT_HEIGHT "1080"

using utils::RAW_PACK_TYPE;
using utils::RAW_BITS;


static utils::RAW_BITS raw_bits_arr[] = {
    RAW_BITS::BITS_8,
    RAW_BITS::BITS_10,
    RAW_BITS::BITS_12,
    RAW_BITS::BITS_16
};

static utils::RAW_PACK_TYPE raw_pack_type_arr[] = {
    RAW_PACK_TYPE::RAW_UNPACKED,
    RAW_PACK_TYPE::RAW_PACKED,
    RAW_PACK_TYPE::RAW_DPCM6
};

RawAnalyzerWindow::RawAnalyzerWindow(QWidget *parent) :
    QMainWindow(parent)
{
    setWindowTitle(tr("RAW Analyzer"));
    setWindowFlags(Qt::WindowMaximizeButtonHint | Qt::WindowCloseButtonHint);
    setMinimumSize(1080, 720);

    initUI();
    initEvent();
}

void RawAnalyzerWindow::initUI()
{
    main_widget = new QWidget();
    main_layout = new QHBoxLayout(main_widget);
    main_layout->setContentsMargins(5, 5, 5, 5);
    main_layout->setSpacing(15);

    // left area: save param setting
    QWidget *param_widget = new QWidget();
    param_widget->setMinimumWidth(ITEM_WIDTH * 3);
    QVBoxLayout *param_layout = new QVBoxLayout(param_widget);
    param_layout->setContentsMargins(5, 5, 5, 5);
    param_layout->setSpacing(10);

    // right area: image viewer
    img_viewer = new MultiImageViewerWidget();
    img_viewer->setFilePathHidden();
    img_viewer->showRawStatisticBtn();
    img_viewer->showLevelMeterBtn();

    ImageLabel *image_label = new ImageLabel(this, ImageLabel::NORMAL);
    connect(image_label, &ImageLabel::sigPositionChange, img_viewer, &MultiImageViewerWidget::slotPositionChange);
    connect(image_label, &ImageLabel::sigUpdateSingleRect, img_viewer, &MultiImageViewerWidget::handleRectToCropInfo);
    img_viewer->setImageLabel(image_label);
    img_viewer->setMinimumHeight(600);
    img_viewer->setMinimumWidth(800);

    // src image
    src_filepath_label = new QLabel(tr("File Path:"));
    src_filepath_label->setFixedWidth(ITEM_WIDTH);
    src_filepath_edit = new QLineEdit();
    src_filepath_edit->setReadOnly(true);

    src_width_label = new QLabel(tr("Width:"));
    src_width_label->setFixedWidth(ITEM_WIDTH);
    src_width_line_edit = new QLineEdit(DEFAULT_WIDTH);
    src_width_line_edit->setReadOnly(true);

    src_height_label = new QLabel(tr("Height:"));
    src_height_label->setFixedWidth(ITEM_WIDTH);
    src_height_line_edit = new QLineEdit(DEFAULT_HEIGHT);
    src_height_line_edit->setReadOnly(true);

    src_bits_label = new QLabel(tr("Bits:"));
    src_bits_label->setFixedWidth(ITEM_WIDTH);
    src_bits_combobox = new QComboBox;
    src_bits_combobox->setFixedWidth(ITEM_WIDTH);
    src_bits_combobox->setEnabled(false);
    for (auto src_bit : raw_bits_arr) {
        src_bits_combobox->addItem(utils::getBitsName(src_bit), static_cast<int>(src_bit));
    }
    src_bits_combobox->setCurrentIndex(src_bits_combobox->findData(static_cast<int>(RAW_BITS::BITS_12)));

    src_bayer_label = new QLabel(tr("Bayer Pattern:"));
    src_bayer_label->setFixedWidth(ITEM_WIDTH);
    src_bayer_combobox = new QComboBox;
    src_bayer_combobox->setFixedWidth(ITEM_WIDTH);
    src_bayer_combobox->setEnabled(false);
    for (int i = utils::BAYER_ID_BEGIN; i < utils::BAYER_ID_RGB; i++) {
        src_bayer_combobox->addItem(utils::getBayerName(i), i);
    }
    src_bayer_combobox->setCurrentIndex(utils::BAYER_ID_BG);

    src_packed_label = new QLabel(tr("Pack Type:"));
    src_packed_label->setFixedWidth(ITEM_WIDTH);
    src_packed_combobox = new QComboBox;
    src_packed_combobox->setFixedWidth(ITEM_WIDTH);
    src_packed_combobox->setEnabled(false);

    for (auto pack_type : raw_pack_type_arr) {
        src_packed_combobox->addItem(utils::getPackTypeName(pack_type), static_cast<int>(pack_type));
    }
    src_packed_combobox->setCurrentIndex(src_packed_combobox->findData(static_cast<int>(RAW_PACK_TYPE::RAW_UNPACKED)));

    // src group
    img_src_group = new QGroupBox("Source Image");
    QGridLayout *img_src_layout = new QGridLayout;
    img_src_layout->setContentsMargins(5, 5, 5, 5);
    img_src_layout->setSpacing(10);
    img_src_layout->addWidget(src_filepath_label, 0, 0, Qt::AlignRight | Qt::AlignVCenter);
    img_src_layout->addWidget(src_filepath_edit, 0, 1, Qt::AlignLeft);
    img_src_layout->addWidget(src_width_label, 1, 0, Qt::AlignRight | Qt::AlignVCenter);
    img_src_layout->addWidget(src_width_line_edit, 1, 1, Qt::AlignLeft);
    img_src_layout->addWidget(src_height_label, 2, 0, Qt::AlignRight | Qt::AlignVCenter);
    img_src_layout->addWidget(src_height_line_edit, 2, 1, Qt::AlignLeft);
    img_src_layout->addWidget(src_bits_label, 3, 0, Qt::AlignRight | Qt::AlignVCenter);
    img_src_layout->addWidget(src_bits_combobox, 3, 1, Qt::AlignLeft);
    img_src_layout->addWidget(src_bayer_label, 4, 0, Qt::AlignRight | Qt::AlignVCenter);
    img_src_layout->addWidget(src_bayer_combobox, 4, 1, Qt::AlignLeft);
    img_src_layout->addWidget(src_packed_label, 5, 0, Qt::AlignRight | Qt::AlignVCenter);
    img_src_layout->addWidget(src_packed_combobox, 5, 1, Qt::AlignLeft);
    img_src_group->setLayout(img_src_layout);

    // dst image
    dst_filepath_label = new QLabel(tr("File Path:"));
    dst_filepath_label->setFixedWidth(ITEM_WIDTH);
    dst_filepath_edit = new QLineEdit();
    dst_filepath_edit->setReadOnly(false);

    dst_width_label = new QLabel(tr("Width:"));
    dst_width_label->setFixedWidth(ITEM_WIDTH);
    dst_width_line_edit = new QLineEdit(DEFAULT_WIDTH);

    dst_height_label = new QLabel(tr("Height:"));
    dst_height_label->setFixedWidth(ITEM_WIDTH);
    dst_height_line_edit = new QLineEdit(DEFAULT_HEIGHT);

    dst_bits_label = new QLabel(tr("Bits:"));
    dst_bits_label->setFixedWidth(ITEM_WIDTH);
    dst_bits_combobox = new QComboBox;
    dst_bits_combobox->setFixedWidth(ITEM_WIDTH);

    for (auto dst_bit : raw_bits_arr) {
        dst_bits_combobox->addItem(utils::getBitsName(dst_bit), static_cast<int>(dst_bit));
    }
    dst_bits_combobox->setCurrentIndex(dst_bits_combobox->findData(static_cast<int>(RAW_BITS::BITS_12)));
    dst_bits_combobox->setEnabled(false);

    dst_bayer_label = new QLabel(tr("Bayer Pattern:"));
    dst_bayer_label->setFixedWidth(ITEM_WIDTH);
    dst_bayer_combobox = new QComboBox;
    dst_bayer_combobox->setFixedWidth(ITEM_WIDTH);
    dst_bayer_combobox->setEnabled(false);
    for (int i = utils::BAYER_ID_BEGIN; i < utils::BAYER_ID_RGB; i++) {
        dst_bayer_combobox->addItem(utils::getBayerName(i), i);
    }
    dst_bayer_combobox->setCurrentIndex(utils::BAYER_ID_BG);
    dst_bayer_combobox->setEnabled(false);

    dst_packed_label = new QLabel(tr("Pack Type:"));
    dst_packed_label->setFixedWidth(ITEM_WIDTH);
    dst_packed_combobox = new QComboBox;
    dst_packed_combobox->setFixedWidth(ITEM_WIDTH);
    for (auto pack_type : raw_pack_type_arr) {
        dst_packed_combobox->addItem(utils::getPackTypeName(pack_type), static_cast<int>(pack_type));
    }
    dst_packed_combobox->setCurrentIndex(dst_packed_combobox->findData(static_cast<int>(RAW_PACK_TYPE::RAW_UNPACKED)));

    dst_frames_label = new QLabel(tr("Frames:"));
    dst_frames_label->setFixedWidth(ITEM_WIDTH);
    dst_frames_line_edit = new QLineEdit("1");

    roi_enable_label = new QLabel(tr("Roi Enable:"));
    roi_enable_label->setFixedWidth(ITEM_WIDTH);
    roi_enable_checkbox = new QCheckBox;
    roi_enable_checkbox->setCheckState(Qt::CheckState::Unchecked);

    // dst image group
    img_dst_group = new QGroupBox("Destination Image");
    QGridLayout *img_dst_layout = new QGridLayout;
    img_dst_layout->setContentsMargins(5, 5, 5, 5);
    img_dst_layout->setSpacing(10);
    img_dst_layout->addWidget(dst_filepath_label, 0, 0, Qt::AlignRight | Qt::AlignVCenter);
    img_dst_layout->addWidget(dst_filepath_edit, 0, 1, Qt::AlignLeft);
    img_dst_layout->addWidget(dst_width_label, 1, 0, Qt::AlignRight | Qt::AlignVCenter);
    img_dst_layout->addWidget(dst_width_line_edit, 1, 1, Qt::AlignLeft);
    img_dst_layout->addWidget(dst_height_label, 2, 0, Qt::AlignRight | Qt::AlignVCenter);
    img_dst_layout->addWidget(dst_height_line_edit, 2, 1, Qt::AlignLeft);
    img_dst_layout->addWidget(dst_bits_label, 3, 0, Qt::AlignRight | Qt::AlignVCenter);
    img_dst_layout->addWidget(dst_bits_combobox, 3, 1, Qt::AlignLeft);
    img_dst_layout->addWidget(dst_bayer_label, 4, 0, Qt::AlignRight | Qt::AlignVCenter);
    img_dst_layout->addWidget(dst_bayer_combobox, 4, 1, Qt::AlignLeft);
    img_dst_layout->addWidget(dst_packed_label, 5, 0, Qt::AlignRight | Qt::AlignVCenter);
    img_dst_layout->addWidget(dst_packed_combobox, 5, 1, Qt::AlignLeft);
    img_dst_layout->addWidget(dst_frames_label, 6, 0, Qt::AlignRight | Qt::AlignVCenter);
    img_dst_layout->addWidget(dst_frames_line_edit, 6, 1, Qt::AlignLeft);
    img_dst_layout->addWidget(roi_enable_label, 7, 0, Qt::AlignRight | Qt::AlignVCenter);
    img_dst_layout->addWidget(roi_enable_checkbox, 7, 1, Qt::AlignLeft);
    img_dst_group->setLayout(img_dst_layout);

    QVBoxLayout *src_dst_layout = new QVBoxLayout();
    src_dst_layout->setSpacing(15);
    src_dst_layout->addWidget(img_src_group);
    src_dst_layout->addWidget(img_dst_group);
    src_dst_layout->setStretch(0, 1);
    src_dst_layout->setStretch(1, 1);

    QGroupBox *param_group = new QGroupBox("Image Parameters");
    QVBoxLayout *param_group_layout = new QVBoxLayout(param_group);
    param_group_layout->setContentsMargins(5, 5, 5, 5);
    param_group_layout->setSpacing(15);
    param_group_layout->addLayout(src_dst_layout);

    // btn
    save_dst_btn = new QPushButton("Save Dst");
    save_dst_btn->setFixedWidth(ITEM_WIDTH);
    connect(save_dst_btn, &QPushButton::clicked, this, &RawAnalyzerWindow::handleSaveDstImage);

    QHBoxLayout *btn_layout = new QHBoxLayout();
    btn_layout->addStretch();
    btn_layout->addWidget(save_dst_btn);
    btn_layout->setSpacing(10);
    param_group_layout->addLayout(btn_layout);

    param_layout->addWidget(param_group);
    param_layout->addStretch();

    main_layout->addWidget(param_widget);
    main_layout->addWidget(img_viewer);
    main_layout->setStretch(0, 1);
    main_layout->setStretch(1, 4);

    setCentralWidget(main_widget);
}


void RawAnalyzerWindow::initEvent()
{
    connect(img_viewer, &MultiImageViewerWidget::sigSetImage, this, &RawAnalyzerWindow::updateImageParameters);
    connect(roi_enable_checkbox, &QCheckBox::stateChanged, this, [=](int state) {
        if (state == Qt::Checked) {
            img_viewer->roiRectRefAdd();
            img_viewer->enableRect();
        } else {
            img_viewer->roiRectRefSub();
            img_viewer->disableRect();
        }
    });
}

void RawAnalyzerWindow::updateImageParameters()
{
    // src
    src_filepath_edit->setText(img_viewer->file_path);
    src_width_line_edit->setText(QString::number(img_viewer->mul_width));
    src_height_line_edit->setText(QString::number(img_viewer->mul_height));
    src_bits_combobox->setCurrentIndex(src_bits_combobox->findData(static_cast<int>(img_viewer->mul_bits)));
    src_bayer_combobox->setCurrentIndex(img_viewer->mul_bayer);
    src_packed_combobox->setCurrentIndex(src_packed_combobox->findData(static_cast<int>(img_viewer->mul_raw_pack_type)));
    // dst
    dst_width_line_edit->setText(QString::number(img_viewer->mul_width));
    dst_height_line_edit->setText(QString::number(img_viewer->mul_height));
    dst_bits_combobox->setCurrentIndex(dst_bits_combobox->findData(static_cast<int>(img_viewer->mul_bits)));
    dst_bayer_combobox->setCurrentIndex(img_viewer->mul_bayer);
    dst_packed_combobox->setCurrentIndex(dst_packed_combobox->findData(static_cast<int>(img_viewer->mul_raw_pack_type)));
    int total_frame = img_viewer->total_frame_cnt;
    QString frame_str;
    if (total_frame > 1) {
        frame_str = "1-" + QString::number(total_frame) + ",";
    } else {
        frame_str = "1";
    }
    dst_frames_line_edit->setText(frame_str);
}

void RawAnalyzerWindow::handleSaveDstImage()
{
    dst_filepath_edit->clear();

    QString frames_str = dst_frames_line_edit->text();
    QVector<int> frame_idx_vec = parseNumberString(frames_str);
    QVector<int> frame_idx_vec_check;

    for (auto frame_idx : frame_idx_vec) {
        if (frame_idx <= 0 || frame_idx > img_viewer->total_frame_cnt) {
            continue;
        }
        frame_idx_vec_check.push_back(frame_idx - 1);
    }

    // src, dst info
    utils::RAW_BITS src_bits = static_cast<utils::RAW_BITS>(src_bits_combobox->currentData().toInt());
    utils::RAW_BAYER_ID src_bayer = static_cast<utils::RAW_BAYER_ID>(src_bayer_combobox->currentData().toInt());
    utils::RAW_PACK_TYPE src_pack_type = static_cast<utils::RAW_PACK_TYPE>(src_packed_combobox->currentData().toInt());
    unsigned int src_width = img_viewer->mul_width;
    unsigned int src_height = img_viewer->mul_height;

    utils::RAW_BITS dst_bits = static_cast<utils::RAW_BITS>(dst_bits_combobox->currentData().toInt());
    utils::RAW_BAYER_ID dst_bayer = static_cast<utils::RAW_BAYER_ID>(dst_bayer_combobox->currentData().toInt());
    utils::RAW_PACK_TYPE dst_pack_type = static_cast<utils::RAW_PACK_TYPE>(dst_packed_combobox->currentData().toInt());

    unsigned int crop_x = static_cast<unsigned int>(img_viewer->selected_rect.x());
    unsigned int crop_y = static_cast<unsigned int>(img_viewer->selected_rect.y());
    unsigned int crop_w = static_cast<unsigned int>(img_viewer->selected_rect.width());
    unsigned int crop_h = static_cast<unsigned int>(img_viewer->selected_rect.height());

    if (roi_enable_checkbox->checkState() == Qt::Unchecked) {
        crop_x =  crop_y = 0;
        crop_w = src_width;
        crop_h = src_height;
    }

    // deal the crop
    if (src_bits == utils::RAW_BITS::BITS_10) {
        crop_x = (crop_x / 5 * 5);
        crop_w = (crop_w / 5 * 5);
    } else if (src_bits == utils::RAW_BITS::BITS_12) {
        crop_x = (crop_x >> 1 << 1);
        crop_w = (crop_w >> 1 << 1);
    }

    if (src_pack_type == utils::RAW_PACK_TYPE::RAW_DPCM6) {
        crop_x = (crop_x >> 2 << 2);
        crop_w = (crop_w >> 2 << 2);
    }

    // check is copy, pack, unpack
    bool is_copy = false;
    bool is_pack = false;
    bool is_unpack = false;
    unsigned int src_real_bits = 12;

    if (dst_pack_type == src_pack_type) {
        is_copy = true;
    }
    if (dst_pack_type == utils::RAW_PACK_TYPE::RAW_UNPACKED) {
        is_unpack = true;
        if (src_bits == utils::RAW_BITS::BITS_8) {
            src_real_bits = 8;
        } else {
            src_real_bits = 16;
        }
    } else if (dst_pack_type == utils::RAW_PACK_TYPE::RAW_PACKED) {
        is_pack = true;
        src_real_bits = static_cast<unsigned int>(src_bits);
    } else if (dst_pack_type == utils::RAW_PACK_TYPE::RAW_DPCM6) {
        // TODO: can not convert to the dpcm
        is_copy = true;
        src_real_bits = 6;
    }

    if (!img_viewer->yuv_list.isEmpty()) {
        is_copy = true;
        // TODO: yuv
        src_real_bits = 12;
    }

    // construct the file path
    QFileInfo src_file_info(img_viewer->file_path);
    QString save_file_dir = src_file_info.path();
    QString save_file_name;
    QString current_time_str = QDateTime::currentDateTime().toString("yyyyMMddhhmmss");

    if (img_viewer->img_type == utils::IMAGE_TYPE::IMAGE_RAW) {
        save_file_name = QString("%1X%2_%3_-bits=%4_-frame=%5_%6.raw")
            .arg(crop_w)
            .arg(crop_h)
            .arg(utils::getBayerName(dst_bayer))
            .arg(utils::getBitsName(dst_bits))
            .arg(frame_idx_vec_check.size())
            .arg(current_time_str);
    } else if (!img_viewer->yuv_list.isEmpty()) {
        save_file_name = QString("%1X%2_-frame=%3_%4.yuv")
            .arg(crop_w)
            .arg(crop_h)
            .arg(frame_idx_vec_check.size())
            .arg(current_time_str);
    } else {
        qWarning() << "Save the image just support the raw and the yuv!";
        return;
    }

    QString output_file_path = QString("%1%2%3")
            .arg(save_file_dir)
            .arg(QDir::separator())
            .arg(save_file_name);

    QFile output_file(output_file_path);

    if (output_file.exists()) {
        output_file.remove();
    }

    if (!output_file.open(QIODevice::Append)) {
        qWarning() << "Failed to open ouput file: " << output_file_path;
        return;
    }

    QList<QByteArray> r_y_list;
    if (img_viewer->img_type == utils::IMAGE_TYPE::IMAGE_RAW) {
        r_y_list = img_viewer->raw_list;
    } else if (!img_viewer->yuv_list.isEmpty()) {
        r_y_list = img_viewer->yuv_list;
    }

    unsigned int read_bytes_each_row;
    if (is_copy) {
        read_bytes_each_row = crop_w * src_real_bits  / 8;
        for (auto frame_idx : frame_idx_vec_check) {
            const QByteArray& frame = r_y_list[frame_idx];
            for (unsigned int h = crop_y; h < crop_y + crop_h; ++h) {
                unsigned int crop_offset = (h * src_width + crop_x) * src_real_bits / 8;
                output_file.write(frame.constData() + crop_offset, read_bytes_each_row);
            }
        }
    } else {
        for (auto frame_idx : frame_idx_vec_check) {
            const QByteArray& frame = r_y_list[frame_idx];
            const unsigned char *praw_data = reinterpret_cast<const unsigned char *>(frame.constData());
            unsigned char *op_raw_data = nullptr;
            unsigned char *unpacked_raw_data = convertAnyRaw2Raw16(praw_data,
                                                   src_width,
                                                   src_height,
                                                   src_bits,
                                                   src_pack_type);

            if (unpacked_raw_data == nullptr) {
                qWarning() << "Failed to unpack data!";
                goto SAVE_END;
            }

            if (dst_pack_type == utils::RAW_PACK_TYPE::RAW_PACKED) {
                unsigned int pack_size = utils::getRawSize(src_width, src_height, src_bits, utils::RAW_PACK_TYPE::RAW_PACKED);
                unsigned char *pack_raw_data = reinterpret_cast<unsigned char *>(malloc(pack_size));
                utils::raw_unpacked2packed(reinterpret_cast<const unsigned short *>(unpacked_raw_data),
                                           pack_raw_data, src_width, src_height);

                op_raw_data = pack_raw_data;
                free(unpacked_raw_data);
                src_real_bits = static_cast<unsigned int>(src_bits);
            } else {
                op_raw_data = unpacked_raw_data;
                src_real_bits = (src_bits == utils::RAW_BITS::BITS_8) ? 8 : 16;
            }
            read_bytes_each_row = crop_w * src_real_bits / 8;
            for (unsigned int h = crop_y; h < crop_y + crop_h; ++h) {
                unsigned int crop_offset = (h * img_viewer->mul_width + crop_x) * src_real_bits / 8;
                output_file.write(reinterpret_cast<const char *>(op_raw_data) +
                                  crop_offset, read_bytes_each_row);
            }

            free(op_raw_data);
        }
    }
SAVE_END:
    output_file.close();
    // update the save path
    dst_filepath_edit->setText(output_file_path);
}


void RawAnalyzerWindow::closeEvent(QCloseEvent *event)
{
    event->accept();
    this->close();
    emit sigcloseChildWindow();
}

QVector<int> RawAnalyzerWindow::parseNumberString(const QString& str) {
    QVector<int> result;
    QSet<int> uniqueNumbers;

    QStringList segments = str.split(',');

    foreach (const QString& segment, segments) {
        QString trimmed = segment.trimmed();
        if (trimmed.isEmpty()) continue;

        if (trimmed.contains('-')) {
            QStringList rangeParts = trimmed.split('-');
            if (rangeParts.size() == 2) {
                bool okStart, okEnd;
                int start = rangeParts[0].trimmed().toInt(&okStart);
                int end = rangeParts[1].trimmed().toInt(&okEnd);

                if (okStart && okEnd && start <= end) {
                    for (int i = start; i <= end; ++i) {
                        uniqueNumbers.insert(i);
                    }
                }
            }
        } else {
            bool ok;
            int num = trimmed.toInt(&ok);
            if (ok) {
                uniqueNumbers.insert(num);
            }
        }
    }

    result = QVector<int>::fromList(uniqueNumbers.toList());
    std::sort(result.begin(), result.end());

    return result;
}
