#ifndef RAW_IMAGE_FETCH_WINDOW_HPP
#define RAW_IMAGE_FETCH_WINDOW_HPP

#include "image_viewer_widget.hpp"
#include <QMainWindow>
#include <QGridLayout>
#include <QLabel>
#include <QComboBox>
#include <QPixmap>
#include <QCheckBox>

class RawImageFetchWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit RawImageFetchWindow(QWidget *parent = nullptr);
    void initUI();
    void initEvent();
    void refresh();

signals:
    void fetchCompleted();
    void fetchStart();

protected:
    void keyReleaseEvent(QKeyEvent *event) override;

private:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void handleSingleImageStatus(int id);
    void handleImageUpdate(QString cmd);
    void handleGetSingleImg();
    void handleChangeSensorMode(QString mode);
    void handleFVCheck(bool checked);
    void handleCancelEvent();

private:
    QWidget *main_widget;
    QGridLayout *main_layout;
    QPixmap *image;
    ImageViewerWidget *img_viewer;
    QComboBox *sensor_mode_combobox;
    QPushButton *get_single_img_btn;
    QPushButton *cancel_btn;
};

#endif // RAW_IMAGE_FETCH_WINDOW_HPP
