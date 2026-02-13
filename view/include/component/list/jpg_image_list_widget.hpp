#ifndef JPG_IMAGE_LIST_WIDGET_HPP
#define JPG_IMAGE_LIST_WIDGET_HPP

#include "cvi_table_widget.hpp"
#include <QWidget>
#include <QVBoxLayout>
#include <QFileInfo>
#include <QComboBox>

typedef enum {
    DEL_ONE,
    DEL_ALL,
    ROW_SWAP,
    ROW_ADD_CAPTURE,
    ROW_ADD_LOCAL,
    LIST_IMPORT_CAPTURE,
    LIST_IMPORT_LOCAL,
} SIGNAL_TYPE;

class JpgImageListItem
{
public:
    explicit JpgImageListItem()
    {
    }

    JpgImageListItem(QString file_path) {
        this->file_path = file_path;
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

    void setRMS(double rms) {
        this->rms = rms;
    }

    double getRMS() {
        return this->rms;
    }

    void setRMSTotal(double rms) {
        this->rms_total = rms;
    }

    double getRMSTotal() {
        return this->rms_total;
    }

private:
    QString file_path = "";
    double rms = 0;
    double rms_total = 0;
};

class JpgImageListWidget : public QWidget
{
    Q_OBJECT
public:
    explicit JpgImageListWidget(QWidget *parent = nullptr);
    void addTableItems(QList<JpgImageListItem *> list);
    QList<JpgImageListItem *> getJpgImageList();
    void del_one();
    void del_all();
    void updateRMS(double rms, QList<double> *list);
signals:
    void imageListChanged(SIGNAL_TYPE type);

protected slots:
    void updateImageList(int from, int to);
    void addTableItem(JpgImageListItem *item);
    void importImages(QString dirPath = "");

private:
    void initUI();
    void initEvent();
    void retranslateUi();

private:
     QVBoxLayout *main_layout;
     CviTableWidget *image_table_widget;
     QList<JpgImageListItem *> image_list;
     QComboBox *image_format_box;
};
#endif // JPG_IMAGE_LIST_WIDGET_HPP
