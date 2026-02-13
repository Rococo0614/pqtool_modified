#ifndef RAW_IMG_OPTION_DIALOG_H
#define RAW_IMG_OPTION_DIALOG_H

#include <QDialog>
#include <QComboBox>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QPlainTextEdit>
#include <QLineEdit>
#include <QGroupBox>
#include "base_image_option.hpp"

class RawImgOption {
public:
    QString file_path;
    QVariant mode{0};
    int width{1920};
    int height{1080};
    QVariant bayer{3};
    QVariant bits;
};

class RawImageOptionDialog : public BaseImageOptionDialog
{
    Q_OBJECT

public:
    explicit RawImageOptionDialog(QWidget *parent = nullptr);
    RawImgOption getOptionData();
    void setFilePath(QString file);
    void setData(RawImgOption options);

private:
    void initOptions();

private:
    QComboBox *stream_mode_line;
    QComboBox *bitdepth_box;
    QComboBox *bayer_box;
};

#endif // RAW_IMG_OPTION_DIALOG_H
