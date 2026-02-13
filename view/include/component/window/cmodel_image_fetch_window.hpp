#ifndef CMODEL_IMAGE_FETCH_WINDOW_HPP
#define CMODEL_IMAGE_FETCH_WINDOW_HPP

#include <QMainWindow>
#include <QGridLayout>
#include <QLabel>
#include <QPixmap>
#include <QScrollArea>
#include <QPlainTextEdit>
#include "image_viewer_widget.hpp"

class CModelImageFetchWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit CModelImageFetchWindow(QWidget *parent = nullptr);
    void initUI();
    void initEvent();
    void initMenu();

protected:
    void closeEvent(QCloseEvent *event) override;
    void changeEvent(QEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

private:
    void retranslateUi();

private slots:
    void imageRawDataUpdate(QPixmap *image);
    void setExePath();
    void start();
    void stop();
    void handleChooseImageFile();
    void handleProcessLog();

private:
    QWidget *main_widget;
    QVBoxLayout *main_layout;
    QLabel *image_label;
    QScrollArea *image_area;
    QPixmap *image;
    QPlainTextEdit* log_text;
    QAction *choose_file_action;
    QAction *start_action;
    QAction *stop_action;
    ImageViewerWidget *img_viewer;
};

#endif // CMODEL_IMAGE_FETCH_WINDOW_HPP
