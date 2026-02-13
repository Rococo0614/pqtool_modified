#ifndef FILE_DIALOG_H
#define FILE_DIALOG_H

#include <QDialog>
#include <QComboBox>
#include <QVBoxLayout>
#include <QPushButton>

const QString CVITEK_FORMAT_STR = "cvitek json file";
const QString HI_FORMAT_STR = "text file";
const QString CVS_FORMAT_STR = "csv file";

typedef enum {
    MODE_LOAD = 0,
    MODE_SAVE,
} MODE;

class FileDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FileDialog(QWidget *parent = nullptr, MODE mode = MODE_LOAD);
    void initUI();
    void setMode(MODE mode);
    QPushButton getOkBtn();
    QPushButton getCancelBtn();

signals:
    void sigSelectFile(QString format, QString file_path, MODE mode);

private slots:
    void handleOpenFile();

private:
    void retranslateUi();

private:
    QVBoxLayout *main_layout;
    QComboBox *file_format_combobox;
    QPushButton *ok_btn;
    QPushButton *cancel_btn;
    MODE mode;
};

#endif // FILE_DIALOG_H
