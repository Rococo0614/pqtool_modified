#include "utils.hpp"
#include "grid_image_viewer_widget.hpp"

#include <QFileDialog>
#include <QHBoxLayout>
#include <QImageReader>
#include <QScrollArea>
#include <QtCore>
#include <QPainter>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QtGlobal>

#define MAX_WIDTH (1280)
#define MAX_HEIGHT (720)

const QString label_highlight_style = "QLabel{border:2px solid rgb(255, 0, 0);}";

GridImageViewerWidget::GridImageViewerWidget(IMAGE_VIEWER_MODE m, QWidget *parent) :
    QWidget(parent),
    scale_factor(1.0),
    mode(m)
{
    initUI();
    retranslateUi();
    setAcceptDrops(true);
}

double GridImageViewerWidget::getScaleFactor()
{
    return this->scale_factor;
}

void GridImageViewerWidget::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);

    if (mode == IMAGE_VIEWER_4R && image_list.size() == 4) {
        if (select_label_idx >= 0 && select_label_idx <= 4)
        {
            image_list[select_label_idx]->setStyleSheet(label_highlight_style);
        }
    }
}

void GridImageViewerWidget::initUI()
{
    this->setMaximumSize(QSize(MAX_WIDTH, MAX_HEIGHT));
    if (mode == IMAGE_VIEWER_4R) {
        QGridLayout *layout = new QGridLayout;

        this->setLayout(layout);
    }
}

void GridImageViewerWidget::retranslateUi()
{

}

void GridImageViewerWidget::setImg(QPixmap img, int idx)
{
    if (mode == IMAGE_VIEWER_4R && image_list.size() == 4) {
        if (idx < 0 || idx > 4)
            return;

        if (image_list[idx]->pixmap() && *image_list[idx]->pixmap() == img)
            return;

        image_list[idx]->setPixmap(img);
    }
}

void GridImageViewerWidget::setImageLabel(QList<ImageLabel *> &label_list)
{
    image_list = label_list;

    if (mode == IMAGE_VIEWER_4R && image_list.size() == 4) {
        QGridLayout *layout = qobject_cast<QGridLayout*>(this->layout());
        if (layout != nullptr) {
            layout->addWidget(image_list[0], 0, 0);
            layout->addWidget(image_list[1], 0, 1);
            layout->addWidget(image_list[2], 1, 0);
            layout->addWidget(image_list[3], 1, 1);
        }
    }
}

void GridImageViewerWidget::selectImage(int idx)
{
    if (idx > 4 || idx < 0) return;

    if (mode == IMAGE_VIEWER_4R) {
        image_list[idx]->setStyleSheet(label_highlight_style);
        if (select_label_idx >= 0 && select_label_idx <= 4)
            image_list[select_label_idx]->setStyleSheet("QLabel{}");
    }
}
