#include <QScrollArea>
#include "dwp_normal_imgviewer_widget.hpp"
#include "image_label.hpp"
#include "dewarp_engine_helper.hpp"

DewarpNormalImgViewerWidget::DewarpNormalImgViewerWidget(QWidget *parent):
    BaseDewarpImgViewerWidget (parent)
{
    initUiLayout();
}

void DewarpNormalImgViewerWidget::initUiLayout()
{
    main_layout = new QHBoxLayout();
    grid_layout = new QGridLayout();

    main_layout->addLayout(grid_layout);
    this->setLayout(main_layout);
}

void DewarpNormalImgViewerWidget::initMode(FISH_EYE_MODE mode)
{
    switch (mode)
    {
    case MODE_01_1O:
        initMode01();
        break;
    case MODE_02_1O4R:
        initMode02();
        break;
    case MODE_03_4R:
        initMode03();
        break;
    case MODE_04_1P2R:
        initMode04();
        break;
    case MODE_05_1P2R:
        initMode05();
        break;
    case MODE_06_1P:
        initMode06();
        break;
    case MODE_07_2P:
        initMode07();
        break;
    case MODE_PANORAMA_180:
        initModePano180();
        break;
    case MODE_PANORAMA_360:
        initModePano360();
        break;
    default:
        break;
    }
}

void DewarpNormalImgViewerWidget::initMode01()
{
    label_1O = new RegionLabel;
    grid_layout->addWidget(label_1O);
}

void DewarpNormalImgViewerWidget::initMode02()
{
    for (int i=0; i<4; i++)
    {
        RegionLabel *label = new RegionLabel;
        label_list.append(label);
        connectEvent(label);
    }

    grid_layout->addWidget(label_list[0], 0, 0);
    grid_layout->addWidget(label_list[1], 0, 1);
    grid_layout->addWidget(label_list[2], 1, 0);
    grid_layout->addWidget(label_list[3], 1, 1);
    grid_layout->setColumnStretch(0, 1);
    grid_layout->setColumnStretch(1, 1);
    grid_layout->setRowStretch(0, 1);
    grid_layout->setRowStretch(1, 1);

    label_1O = new RegionLabel;
    label_1O->setMaximumHeight(320);

    main_layout->addWidget(label_1O);
    main_layout->setStretch(0, 3);
    main_layout->setStretch(1, 1);
}

void DewarpNormalImgViewerWidget::initMode03()
{
    for (int i=0; i<4; i++)
    {
        RegionLabel *label = new RegionLabel;
        label_list.append(label);
        connectEvent(label);
    }

    grid_layout->addWidget(label_list[0], 0, 0);
    grid_layout->addWidget(label_list[1], 0, 1);
    grid_layout->addWidget(label_list[2], 1, 0);
    grid_layout->addWidget(label_list[3], 1, 1);
    grid_layout->setColumnStretch(0, 1);
    grid_layout->setColumnStretch(1, 1);
    grid_layout->setRowStretch(0, 1);
    grid_layout->setRowStretch(1, 1);
}

void DewarpNormalImgViewerWidget::initMode04()
{
    for (int i = 0; i < 3; i++)
    {
        RegionLabel *label = new RegionLabel;
        label_list.append(label);
        connectEvent(label);
    }

    QHBoxLayout * rgn_layout = new QHBoxLayout;
    rgn_layout->addWidget(label_list[1]);
    rgn_layout->addWidget(label_list[2]);
    rgn_layout->setStretch(0, 1);
    rgn_layout->setStretch(1, 1);

    grid_layout->addWidget(label_list[0], 0, 0);
    grid_layout->addLayout(rgn_layout, 1, 0);
    grid_layout->setRowStretch(0, 1);
    grid_layout->setRowStretch(1, 1);
}

void DewarpNormalImgViewerWidget::initMode05()
{
    for (int i = 0; i < 3; i++)
    {
        RegionLabel *label = new RegionLabel;
        label_list.append(label);
        connectEvent(label);
    }

    QHBoxLayout * rgn_layout = new QHBoxLayout;
    rgn_layout->addWidget(label_list[1]);
    rgn_layout->addWidget(label_list[2]);
    rgn_layout->setStretch(0, 1);
    rgn_layout->setStretch(1, 1);

    grid_layout->addWidget(label_list[0], 0, 0);
    grid_layout->addLayout(rgn_layout, 1, 0);
    grid_layout->setRowStretch(0, 1);
    grid_layout->setRowStretch(1, 1);
}

void DewarpNormalImgViewerWidget::initMode06()
{
    RegionLabel *label_pano0 = new RegionLabel;
    label_list.append(label_pano0);
    connectEvent(label_pano0);

    grid_layout->addWidget(label_pano0);
}

void DewarpNormalImgViewerWidget::initMode07()
{
    RegionLabel *label_pano0 = new RegionLabel;
    RegionLabel *label_pano1 = new RegionLabel;
    label_list.append(label_pano0);
    label_list.append(label_pano1);
    connectEvent(label_pano0);
    connectEvent(label_pano1);

    grid_layout->addWidget(label_pano0, 0, 0);
    grid_layout->addWidget(label_pano1, 1, 0);
    grid_layout->setRowStretch(0, 1);
    grid_layout->setRowStretch(1, 1);
}

void DewarpNormalImgViewerWidget::initModePano360()
{
    RegionLabel *label_pano0 = new RegionLabel;
    label_list.append(label_pano0);
    connectEvent(label_pano0);
    grid_layout->addWidget(label_pano0);
}

void DewarpNormalImgViewerWidget::initModePano180()
{
    RegionLabel *label_pano0 = new RegionLabel;
    label_list.append(label_pano0);
    connectEvent(label_pano0);
    grid_layout->addWidget(label_pano0);
}

void DewarpNormalImgViewerWidget::connectEvent(RegionLabel *label)
{
    connect(label, &RegionLabel::clicked, [=]{
        int idx = label_list.indexOf(label);
        if (active_region == idx)
        {
            active_region = -1;
            setRgnFocusOff(idx);
        }
        else
        {
            active_region = idx;
            for (int j=0; j<label_list.size(); j++)
            {
                if (idx == j) {
                    setRgnFocusOn(idx);
                } else {
                    setRgnFocusOff(j);
                }
            }
        }
    });
}

void DewarpNormalImgViewerWidget::setRgnFocusOn(int idx)
{
    label_list[idx]->setFrameShape(QFrame::Box);
    label_list[idx]->setStyleSheet("border-width: 2px;border-style: solid;border-color: rgb(0, 0, 255);");
}

void DewarpNormalImgViewerWidget::setRgnFocusOff(int idx)
{
    label_list[idx]->setFrameShape(QFrame::NoFrame);
    label_list[idx]->setStyleSheet("border-width: 0px");
}

void DewarpNormalImgViewerWidget::updateRgnView(int idx, QImage *img)
{
    QPixmap pixmap = QPixmap::fromImage(*img);

    if (idx >= SECTOR_RGN_0 && idx <= SECTOR_RGN_3)
    {
        pixmap = pixmap.scaled(label_list[idx]->size(), Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
        label_list[idx]->setPixmap(pixmap);
        label_list[idx]->adjustSize();
        label_list[idx]->setScaledContents(true);
    }
    else if (idx == SECTOR_1O)
    {
        pixmap = pixmap.scaled(label_1O->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
        label_1O->setPixmap(pixmap);
        label_1O->adjustSize();
        label_1O->setScaledContents(true);
    }
    else if (idx == SECTOR_PANORAMA_0)
    {
        pixmap = pixmap.scaled(label_list[0]->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
        label_list[0]->setPixmap(pixmap);
        label_list[0]->adjustSize();
        label_list[0]->setScaledContents(true);
    }
    else if (idx == SECTOR_PANORAMA_1)
    {
        pixmap = pixmap.scaled(label_list[1]->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
        label_list[1]->setPixmap(pixmap);
        label_list[1]->adjustSize();
        label_list[1]->setScaledContents(true);
    }

    delete img;
}

int DewarpNormalImgViewerWidget::getActiveRegion()
{
    return active_region;
}
