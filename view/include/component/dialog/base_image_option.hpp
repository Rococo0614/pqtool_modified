#ifndef BASE_IMG_OPTION_DIALOG_H
#define BASE_IMG_OPTION_DIALOG_H

#include <QDialog>
#include <QComboBox>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QPlainTextEdit>
#include <QLineEdit>
#include <QGroupBox>

class BaseImageOptionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BaseImageOptionDialog(QWidget *parent = nullptr);
    void initUI();
private:
    void retranslateUi();

private slots:
    void handleOk();
    void handleCancel();
    void slotQuickSelect(int);

protected:
    void addOptionItem(QLayout *item_layout);
    void addOptionItem(QWidget *item);
    void setResolution(QString rse_x, QString res_y);

protected:
    QLineEdit *file_path;
    QVBoxLayout *main_layout;
    QPushButton *ok_btn;
    QPushButton *cancel_btn;
    QGroupBox *option_group;
    QLineEdit *w_res_line;
    QLineEdit *h_res_line;
    QComboBox *res_list_box;
    QVBoxLayout * group_layout;
};

#endif // BASE_IMG_OPTION_DIALOG_H
