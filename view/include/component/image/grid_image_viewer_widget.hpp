#ifndef GRID_IMAGE_VIEWER_WIDGET_H
#define GRID_IMAGE_VIEWER_WIDGET_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QScrollArea>
#include "image_label.hpp"
#include "calibration_utils.hpp"

enum IMAGE_VIEWER_MODE {
    IMAGE_VIEWER_4R,
    IMAGE_VIEWER_MODE_END
};

class GridImageViewerWidget : public QWidget
{
    Q_OBJECT

public:
    explicit GridImageViewerWidget(IMAGE_VIEWER_MODE m = IMAGE_VIEWER_4R, QWidget *parent = nullptr);

    void setImg(QPixmap img, int idx);
    void setImageLabel(QList<ImageLabel *> &label_list);
    double getScaleFactor();
    void selectImage(int idx);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    void initUI();
    void retranslateUi();

signals:
    void sigSetImage(const QPixmap *image);

private:
    double scale_factor;
    QList<ImageLabel *> image_list;
    int select_label_idx{-1};
    IMAGE_VIEWER_MODE mode{IMAGE_VIEWER_4R};
};

#endif // GRID_IMAGE_VIEWER_WIDGET_H
