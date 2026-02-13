#ifndef YUV_IMAGE_LIST_WIDGET_H
#define YUV_IMAGE_LIST_WIDGET_H

#include "utils.hpp"
#include "yuv_image_option_dialog.hpp"
#include "isp_info_dialog.hpp"

#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QTableWidget>


class YuvImageListItem: public QObject
{
    Q_OBJECT
public:
    explicit YuvImageListItem()
    {
    }

    YuvImageListItem(QString file_path) {
        this->file_path = file_path;
    }

    void fromYuvImgOption(YuvImgOption op)
    {
        this->file_path = op.file_path;
        this->width = op.width;
        this->height = op.height;
        this->bitwidth = op.bitwidth;
        this->pixelType = op.pixelType;
        this->videoType = op.videoType;
    }

    void setFStart(int start) {
        f_start = start;
    }

    void setFEnd(int end) {
        f_end = end;
    }

    void setFGop(int gop) {
        f_gop = gop;
    }

    void setImgSize(int width, int height) {
        this->width = width;
        this->height = height;
    }

    QString getFilePath() {
        return this->file_path;
    }

    QString getFileName() {
        if (this->file_path.isEmpty()) {
            return "";
        }
        QFileInfo info(this->file_path);
        return info.fileName();
    }

    void setCheckState(Qt::CheckState state) {
        if (this->state != state) {
            this->state = state;
            emit displayStateChanged(state);
        }
    }

    Qt::CheckState getCheckState() {
       return this->state;
    }

    int getWidth() {
        return this->width;
    }

    int getHeight() {
        return this->height;
    }

    int getFStart() {
        return f_start;
    }

    int getFEnd() {
        return f_end;
    }

    int getFGop() {
        return f_gop;
    }

    void setYuvdata(QByteArray &yuv_data) {
        if (this->yuv_data == nullptr)
            this->yuv_data = new QByteArray(yuv_data);
    }

    QByteArray* getYuvdata() {
        return this->yuv_data;
    }

    void setYuvInfo(YUV_HEADER info) {
        this->yuvInfo = info;
    }

    YUV_HEADER getYuvInfo() {
        return this->yuvInfo;
    }

    void setFrameNum(int frame_num) {
        this->frame_num = frame_num;
    }

    int getFrameNum()
    {
        return frame_num;
    }

    QVariant getBitwidth() const;
    void setBitwidth(const QVariant &value);

    QVariant getPixelType() const;
    void setPixelType(const QVariant &value);

    QVariant getVideoType() const;
    void setVideoType(const QVariant &value);

signals:
    void displayStateChanged(Qt::CheckState display);
private:
    Qt::CheckState state;
    QString file_path;
    int width{1920};
    int height{1080};
    int frame_num;
    QVariant bitwidth;
    QVariant pixelType;
    QVariant videoType;
    int f_start;
    int f_end;
    int f_gop;
    QByteArray * yuv_data{nullptr};
    YUV_HEADER yuvInfo;
};

class YuvImageListWidget : public QWidget
{
    Q_OBJECT
public:
    explicit YuvImageListWidget(QWidget *parent = nullptr);
    void addTableItem(YuvImageListItem *item);
    QList<YuvImageListItem *> getYuvImageList();
    QList<YuvImageListItem *> getSelYuvImageList();
    void setCheckState(Qt::CheckState state);
    void clearOtherItemDisplay(YuvImageListItem *item);

public slots:
    void moveup_sel();
    void movedown_sel();
    void del_selected();
    void del_all();

private:
    void initUI();
    void retranslateUi();
    void moveRow(int from, int to);
    void printImageList();

private:
     QVBoxLayout *main_layout;
     QTableWidget *image_table_widget;
     QList<YuvImageListItem *> yuv_image_list;
};

#endif // YUV_IMAGE_LIST_WIDGET_H
