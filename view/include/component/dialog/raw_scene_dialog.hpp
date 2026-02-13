#ifndef RAW_SCENE_DIALOG_H
#define RAW_SCENE_DIALOG_H

#include "calibration_utils.hpp"

#include <QDialog>
#include <QComboBox>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QPlainTextEdit>

class RawSceneDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RawSceneDialog(QWidget *parent = nullptr);
    void initUI();
    SCENE getScene();
    void setFileList(const QStringList &file_list);

private:
    void retranslateUi();

private slots:
    void handleOk();
    void handleCancel();

private:
    QPlainTextEdit *text_edit;
    QVBoxLayout *main_layout;
    QLabel *scene_label;
    QComboBox *scene_combobox;
    QPushButton *ok_btn;
    QPushButton *cancel_btn;
};

#endif // RAW_SCENE_DIALOG_H
