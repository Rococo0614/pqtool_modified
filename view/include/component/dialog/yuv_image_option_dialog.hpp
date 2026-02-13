#ifndef YUV_IMG_OPTION_DIALOG_H
#define YUV_IMG_OPTION_DIALOG_H

#include <QDialog>
#include <QComboBox>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QPlainTextEdit>
#include <QLineEdit>
#include <QGroupBox>
#include "base_image_option.hpp"

class YuvImgOption {
public:
    QString file_path;
    int width{1920};
    int height{1080};
    QVariant bitwidth;
    QVariant pixelType;
    QVariant videoType;
};

class YuvImageOptionDialog : public BaseImageOptionDialog
{
    Q_OBJECT

public:
    explicit YuvImageOptionDialog(QWidget *parent = nullptr);
    YuvImgOption getOptionData();
    void setOptionData(YuvImgOption opt);
    void setFilePath(QString file);

private:
    void initOptions();

private:
    QComboBox *bitwidth_box;
    QComboBox *pixeltype_box;
    QComboBox *videotype_box;
};

#endif // YUV_IMG_OPTION_DIALOG_H
