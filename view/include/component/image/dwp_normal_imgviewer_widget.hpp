#ifndef BASE_DEWARP_IMGVIEWER_1O4R_WIDGET_H
#define BASE_DEWARP_IMGVIEWER_1O4R_WIDGET_H

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>
#include <QVariant>
#include <QPushButton>
#include <QScrollArea>

#include "base_dwp_imgviewer_widget.hpp"
#include "rgn_label.hpp"

class DewarpNormalImgViewerWidget : public BaseDewarpImgViewerWidget
{
    Q_OBJECT

public:
    DewarpNormalImgViewerWidget(QWidget *parent = nullptr);
    virtual void updateRgnView(int idx, QImage *img) override;
    virtual int getActiveRegion() override;
    void initMode(FISH_EYE_MODE mode);

private:
    void initUiLayout();
    void setRgnFocusOn(int idx);
    void setRgnFocusOff(int idx);

    void initMode01();
    void initMode02();
    void initMode03();
    void initMode04();
    void initMode05();
    void initMode06();
    void initMode07();
    void initModePano360();
    void initModePano180();
    void connectEvent(RegionLabel *label);

protected:
    QList<RegionLabel *> label_list;
    RegionLabel *label_1O;
//    RegionLabel *label_pano0;
//    RegionLabel *label_pano1;
    int active_region{-1};

    QHBoxLayout *main_layout;
    QGridLayout *grid_layout;

signals:
    void sigLabelSel(int idx);
};

#endif
