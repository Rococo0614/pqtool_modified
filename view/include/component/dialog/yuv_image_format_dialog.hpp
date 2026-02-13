#ifndef YUV_IMAGE_FORMAT_DIALOG_H
#define YUV_IMAGE_FORMAT_DIALOG_H

#include <QDialog>
#include <QComboBox>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QKeyEvent>
#include <QGroupBox>


class YuvImageFormatDialog : public QDialog
{
    Q_OBJECT

public:
    explicit YuvImageFormatDialog(QWidget *parent = nullptr);
    void initUI();
    std::tuple<QString, QString> getSize();
    QString getFormat();
    QString getAlign();
    void setFilepath(QString path);

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
    QLabel *format_label;
    QComboBox *format_combobox;
    QLabel *align_label;
    QLineEdit *align_line_edit;
    QPushButton *ok_btn;
    QPushButton *cancel_btn;
};

#endif
