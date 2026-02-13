#ifndef RAW_IMAGE_FORMAT_DIALOG_H
#define RAW_IMAGE_FORMAT_DIALOG_H

#include "calibration_utils.hpp"
#include <QDialog>
#include <QComboBox>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QKeyEvent>
#include <QGroupBox>
#include "utils.hpp"

const QString RESOLUTION_DELIMETER = "x";

class RawImageFormatDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RawImageFormatDialog(QWidget *parent = nullptr);
    void initUI();
    std::tuple<QString, QString> getSize();
    QVariant getBits();
    QVariant getBayer();
    utils::RAW_PACK_TYPE getPacked();
    QVariant getIrPosition();
    SCENE getScene();
    FRAME_MODE getFrameMode();
    void setScene(SCENE scene);
    void setFilepath(QString path);
    void setData(QString w, QString h, QVariant bits, QVariant bayer, utils::RAW_PACK_TYPE pack,
                 QVariant ir_pst, SCENE scene, FRAME_MODE frame_mode);
    void setSceneLabelState(bool state);
    void bitsComboboxTextChanged(int index);

private:
    void retranslateUi();

private slots:
    void handleOk();
    void handleCancel();

private:
    QVBoxLayout *main_layout;
    QLabel *filepath_label;
    QLineEdit *filepath_edit;
    QLabel *width_label;
    QLineEdit *width_line_edit;
    QLabel *height_label;
    QLineEdit *height_line_edit;
    QLabel *bits_label;
    QComboBox *bits_combobox;
    QLabel *bayer_label;
    QComboBox *bayer_combobox;
    QLabel *ir_position_label;
    QComboBox *ir_position_combobox;
    QLabel *packed_label;
    QComboBox *packed_combobox;
    QPushButton *ok_btn;
    QPushButton *cancel_btn;
    QLabel *scene_label;
    QComboBox *scene_combobox;
    QLabel *frame_mode_label;
    QComboBox *frame_mode_combobox;
};

#endif // RAW_IMAGE_FORMAT_DIALOG_H
