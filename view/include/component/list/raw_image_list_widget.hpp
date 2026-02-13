#ifndef RAW_IMAGE_LIST_WIDGET_H
#define RAW_IMAGE_LIST_WIDGET_H

#include "utils.hpp"
#include "raw_image_option_dialog.hpp"
#include "isp_info_dialog.hpp"

#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QTableWidget>


class RawImageListItem: public QObject
{
    Q_OBJECT

public:
    explicit RawImageListItem()
    {
    }

    RawImageListItem(QString file_path) {
        this->file_path = file_path;
    }

    void fromRawImgOption(RawImgOption op)
    {
        this->mode = op.mode;
        this->file_path = op.file_path;
        this->bits = op.bits;
        this->width = op.width;
        this->height = op.height;
        this->bayer = op.bayer;
    }

    void setMode(QVariant mode)
    {
        this->mode = mode;
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

    void setBayer(QVariant bayer) {
        this->bayer = bayer;
    }

    void setBits(QVariant bits) {
        this->bits = bits;
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

    QVariant getBayer() {
        return bayer;
    }

    QVariant getBits() {
        return bits;
    }

    QVariant getMode() {
        return mode;
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

	void setRawInfo(RAW_INFO info)
	{
		this->raw_info = info;
	}

	RAW_INFO getRawInfo()
	{
		return this->raw_info;
	}

    void setRawdata(QByteArray &raw_data) {
        this->raw_data.clear();
        this->raw_data.append(raw_data);
    }

    QByteArray* getRawdata() {
        return &(this->raw_data);
    }

    void setFrameNum(int frame_num) {
        this->frame_num = frame_num;
    }

    int getFrameNum()
    {
        return frame_num;
    }

    void setBlackLvl(QList<int> &&black_lvl)
    {
        this->black_lvl = black_lvl;
    }

    QList<int> getBlackLvl()
    {
        return black_lvl;
    }

    void setRawIspInfo(QList<RawIspInfo> &&info)
    {
        this->raw_data_ispinfo = info;
    }

    QList<RawIspInfo> getRawIspInfo()
    {
        return raw_data_ispinfo;
    }

    void setRoiInfo(RoiInfo roiInfo)
    {
        this->roiInfo = roiInfo;
    }

    RoiInfo getRoiInfo()
    {
        return roiInfo;
    }

    void setExpInfo(QList<EXP_INFO> expInfo)
    {
        this->expInfo = expInfo;
    }

    QList<EXP_INFO> getExpInfo()
    {
        return this->expInfo;
    }

    void setRoiMode(bool roiMode)
    {
        this->roiMode = roiMode;
    }

    bool getRoiMode()
    {
        return roiMode;
    }

    void setRoiPath(QString roi_path)
    {
        this->roi_path = roi_path;
    }

    QString getRoiPath()
    {
        return this->roi_path;
    }

signals:
    void displayStateChanged(Qt::CheckState display);

private:
    Qt::CheckState state;
    QString file_path;
    QVariant mode;
    int width{1920};
    int height{1080};
    QVariant bayer;
    QVariant bits;
    int f_start;
    int f_end;
    int f_gop;
    int frame_num;
    QByteArray raw_data;
    RAW_INFO raw_info = {};
    QList<RawIspInfo> raw_data_ispinfo;
    QList<int> black_lvl;
    QList<EXP_INFO> expInfo;
    RoiInfo roiInfo;
    bool roiMode = false;
    QString roi_path;
};

class RawImageListWidget : public QWidget
{
    Q_OBJECT
public:
    explicit RawImageListWidget(QWidget *parent = nullptr);
    void addTableItem(RawImageListItem *item);
    QList<RawImageListItem *> getRawImageList();
    QList<RawImageListItem *> getSelRawImageList();
    void setCheckState(Qt::CheckState state);
    void clearOtherItemDisplay(RawImageListItem *item);

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
     QList<RawImageListItem *> raw_image_list;
};

#endif // RAW_IMAGE_LIST_WIDGET_H
