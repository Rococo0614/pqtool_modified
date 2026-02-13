#ifndef BASE_DEWARP_IMGVIEWER_WIDGET_HPP
#define BASE_DEWARP_IMGVIEWER_WIDGET_HPP

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>
#include <QVariant>
#include <QPushButton>

#include "dewarp_engine_helper.hpp"

class BaseDewarpImgViewerWidget : public QWidget
{
    Q_OBJECT

public:
    BaseDewarpImgViewerWidget(QWidget *parent = nullptr);
    virtual void updateRgnView(int idx, QImage *img);
    virtual int getActiveRegion();

private:
    void initUI();
};

#endif //_BASE_DEWARP_IMGVIEWER_WIDGET_HPP_
