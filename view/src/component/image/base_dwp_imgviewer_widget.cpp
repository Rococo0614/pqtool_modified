#include "base_dwp_imgviewer_widget.hpp"

BaseDewarpImgViewerWidget::BaseDewarpImgViewerWidget(QWidget *parent):
    QWidget (parent)
{
    initUI();
}

void BaseDewarpImgViewerWidget::initUI()
{

}

void BaseDewarpImgViewerWidget::updateRgnView(int idx, QImage *img)
{

}

int BaseDewarpImgViewerWidget::getActiveRegion()
{
    return -1;
}
