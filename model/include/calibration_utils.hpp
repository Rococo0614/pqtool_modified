#ifndef CALIBRATION_UTILS_H
#define CALIBRATION_UTILS_H

#include <string>
#include <map>
#include <vector>
#include <QString>
#include <QVariant>
#include <QFileInfo>
#include "global_data.hpp"
#include "utils.hpp"

using namespace std;

enum class SCENE
{
    BEGIN,
    DARK_FRAME = BEGIN,
    UNIFORM_COLOR,
    COLOR24,
    COLOR124,
    END,
};

enum class FRAME_MODE
{
    LINEAR,
    WDR,
};

inline QString getSceneName(SCENE scene_id)
{
    QString scene_name;
    switch(scene_id) {
        case SCENE::DARK_FRAME:
            scene_name = "Dark frame";
            break;
        case SCENE::UNIFORM_COLOR:
            scene_name = "Uniform color";
            break;
        case SCENE::COLOR24:
            scene_name = "24 Colors";
            break;
        case SCENE::COLOR124:
            scene_name = "124 Colors";
            break;
        default:
            scene_name = "";
            break;
    }

    return scene_name;
}

class RawImageItem: public QObject
{
    Q_OBJECT

public:
    RawImageItem(QString file_path, SCENE scene) {
        this->file_path = file_path;
        this->scene = scene;
    }

    void setImgSize(int width, int height) {
        this->width = width;
        this->height = height;
    }

    void setISO(int iso) {
        this->iso = iso;
    }

    void setBayer(QVariant bayer) {
        this->bayer = bayer;
    }

    void setIrPosition(QVariant ir_pst) {
        this->ir_pst = ir_pst;
    }

    void setPacked(utils::RAW_PACK_TYPE packed) {
        this->packed = packed;
    }

    void setBits(QVariant bits) {
        this->bits = bits;
    }

    void setFramesCnt(QVariant cnt) {
        this->frames_cnt = cnt;
    }

    void setFrameMode(FRAME_MODE frame_mode) {
        this->frame_mode = frame_mode;
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

    SCENE getScene() {
        return this->scene;
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

    RECT_S getRect() {
        return this->rect;
    }

    void setRect(RECT_S rect) {
        this->rect = rect;
    }

    int getISO() {
        return this->iso;
    }

    QVariant getBayer() {
        return bayer;
    }

    utils::RAW_PACK_TYPE getPacked() {
        return packed;
    }

    QVariant getBits() {
        return bits;
    }
     QVariant getIrPosition() {
        return this->ir_pst;
    }
    QVariant getFramesCnt() {
        return frames_cnt;
    }
    FRAME_MODE getFrameMode() {
        return this->frame_mode;
    }

signals:
    void displayStateChanged(Qt::CheckState display);

private:
    QString file_path;
    SCENE scene;
    Qt::CheckState state;
    int width{1920};
    int height{1080};
    int iso{100};
    QVariant bayer;
    utils::RAW_PACK_TYPE packed;
    QVariant bits;
    QVariant frames_cnt;
    QVariant ir_pst;
    RECT_S rect{0,0,0,0};
    FRAME_MODE frame_mode;
};

class RawConfigItem
{
public:
    void setConfig(QString width, QString height, QVariant bits, QVariant bayer, utils::RAW_PACK_TYPE packed,
                   QVariant ir_pst, SCENE scene = SCENE::DARK_FRAME, FRAME_MODE frame_mode = FRAME_MODE::LINEAR) {
        this->width = width;
        this->height = height;
        this->bits = bits;
        this->bayer = bayer;
        this->packed = packed;
        this->ir_pst = ir_pst;
        this->scene = scene;
        this->frame_mode = frame_mode;
    }

    void setWidth(QString width) {
        this->width = width;
    }

    void setHeight(QString height) {
        this->height = height;
    }

    void setBits(QVariant bits) {
        this->bits = bits;
    }

    void setBayer(QVariant bayer) {
        this->bayer = bayer;
    }

    void setPacked(utils::RAW_PACK_TYPE packed) {
        this->packed = packed;
    }

    void setIrPosition(QVariant ir_pst) {
        this->ir_pst = ir_pst;
    }

    void setScene(SCENE scene) {
        this->scene = scene;
    }

    void setFrameMode(FRAME_MODE frame_mode) {
        this->frame_mode = frame_mode;
    }

    QString getWidth() {
        return this->width;
    }

    QString getHeight() {
        return this->height;
    }

    QVariant getBits() {
        return this->bits;
    }

    QVariant getBayer() {
        return this->bayer;
    }

    utils::RAW_PACK_TYPE getPacked() {
        return this->packed;
    }

    QVariant getIrPosition() {
        return this->ir_pst;
    }

    SCENE getScene() {
        return this->scene;
    }

    FRAME_MODE getFrameMode() {
        return this->frame_mode;
    }

private:
    QString width;
    QString height;
    QVariant bits;
    QVariant bayer;
    utils::RAW_PACK_TYPE packed;
    QVariant ir_pst;
    SCENE scene;
    FRAME_MODE frame_mode;
};

enum class CALIBRATION_STATUS
{
    READY,
    CALIBRATION,
    CAL_ERROR,
    WARNING
};

#endif // CALIBRATION_UTILS_H
