#include "awb_attr_page_widget.hpp"
#include "module_define.hpp"
#include "global_data.hpp"
#include "image_viewer_widget.hpp"

AWBAttrPageWidget::AWBAttrPageWidget(QWidget *parent) : BasePageWidget(MODULE_NAME_AWB_ATTR, parent)
{
    initData();
    initUI();
    main_layout->setStretch(0, 1);
    main_layout->setStretch(1, 1);
    main_layout->setStretch(2, 2);
}

void AWBAttrPageWidget::initLeftArea()
{
    ImageViewerWidget *img = new ImageViewerWidget();
    ImageLabel *image_label = new ImageLabel(this, ImageLabel::GRID);
    img->setImageLabel(image_label);
    main_layout->addWidget(img);

    QWidget *cal_widget = initCalibrationWidget();
    main_layout->addWidget(cal_widget);
}

void AWBAttrPageWidget::initCenterArea()
{
    cv = new LineChartView(4, this);
    cv->initAWB();

    QVBoxLayout *v_layout = new QVBoxLayout();
    cv->setPoints(0, initPoints(1.0), false);

    v_layout->addWidget(cv);
    main_layout->addLayout(v_layout);
}

QWidget *AWBAttrPageWidget::initCalibrationWidget()
{
    QWidget *widget = new QWidget();

    QVBoxLayout *button_layout = new QVBoxLayout();
    QWidget *cal_widget = new QWidget();

    QPushButton *RSum_button = new QPushButton("RSum");
    QPushButton *GSum_button = new QPushButton("GSum");
    QPushButton *BSum_button = new QPushButton("BSum");
    QPushButton *RNum_button = new QPushButton("RNum");
    QPushButton *GNum_button = new QPushButton("GNum");
    QPushButton *BNum_button = new QPushButton("BNum");
    QPushButton *RGain_button = new QPushButton("Rgain");
    QPushButton *BGain_button = new QPushButton("Bgain");
    button_layout->setAlignment(Qt::AlignCenter);
    button_layout->addWidget(RSum_button);
    button_layout->addWidget(GSum_button);
    button_layout->addWidget(BSum_button);
    button_layout->addWidget(RNum_button);
    button_layout->addWidget(GNum_button);
    button_layout->addWidget(BNum_button);
    button_layout->addWidget(RGain_button);
    button_layout->addWidget(BGain_button);

    QHBoxLayout *h_layout = new QHBoxLayout(widget);

    h_layout->addLayout(button_layout);
    h_layout->addWidget(cal_widget);

    return widget;
}

QList<QPointF> AWBAttrPageWidget::initPoints(double coefficient)
{
    QList<QPointF> points;
    int point_num = 4;
    int max_x = 15000;
    int max_y = 15000;
    int step_x = max_x / point_num;

    for (int i = 0; i < point_num + 1; i++) {
        int x = i * step_x;
        double norm_x = static_cast<double>(x) / max_x;
        double norm_y = qPow(norm_x, coefficient);
        int y = static_cast<int>(norm_y * max_y);

        points.append(QPointF(x, y));
    }

    return points;
}
