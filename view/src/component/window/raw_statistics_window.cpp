#include "raw_statistics_window.hpp"
#include <QtCharts/QChart>
#include <QtCharts/QChartView>

const int AXIS_MAX_RANGE = 4095;
#define MIN(a, b)               ((a) < (b) ? (a) : (b))
#define MAX(a, b)               ((a) > (b) ? (a) : (b))

static QColor QColor_R = QColor(250, 128, 114, 255); //r
//static QColor QColor_Gr = QColor(255, 190, 192, 255); //gr
static QColor QColor_Gr = QColor(128, 255, 114, 255); //gr
static QColor QColor_Gb = QColor(173, 216, 230, 255); //gb
static QColor QColor_B = QColor(192, 193, 255, 255); //b
static QColor QColor_G = QColor(128, 255, 114, 255); //g

RawStatisticsWindow::RawStatisticsWindow(QWidget *parent) :
    QMainWindow(parent)
{
    resize(QDesktopWidget().availableGeometry(this).size() * 0.75);

    initUI();
    retranslateUi();
}

RawStatisticsWindow::~RawStatisticsWindow() {}

void RawStatisticsWindow::initUI()
{
    QWidget *main_widget = new QWidget;
    QVBoxLayout *main_layout = new QVBoxLayout;
    QHBoxLayout *aixs_layout = new QHBoxLayout;
    QHBoxLayout *raw_info_layout = new QHBoxLayout;
    raw_info_label = new QLabel("Max: 0(0/0 Pts) Mid: 0 Avr: 0");

    //top layout
    QCustomPlot *customPlot = createChartView();
    QGroupBox *control_info = createControlShowGroupBox();
    aixs_layout->addWidget(customPlot);
    aixs_layout->addWidget(control_info);

    raw_info_layout->addStretch(1);
    raw_info_layout->addWidget(raw_info_label);
    raw_info_layout->addStretch(1);

    main_layout->addLayout(aixs_layout);
    main_layout->addLayout(raw_info_layout);

    setCentralWidget(main_widget);
    centralWidget()->setLayout(main_layout);

    //status bar
    QFont font;
    font.setPointSize(11);
    status_label = new QLabel("waiting data...");
    status_label->setFont(font);
    statusBar()->setStyleSheet(QString("background-color:") + "rgb(220,220,220)");
    statusBar()->addPermanentWidget(status_label);
}

void RawStatisticsWindow::setStatusBarState(QString text)
{
    status_label->setText(text);
}

QCustomPlot *RawStatisticsWindow::createChartView()
{
    customPlot = new QCustomPlot(this);
    customPlot->setMinimumSize(this->frameGeometry().width() * 0.9, this->frameGeometry().height() * 0.9);

    // create empty bar chart objects:
    QCPBarsGroup *group = new QCPBarsGroup(customPlot);
    QList<QCPBars *> bar_list;
    r_bar = new QCPBars(customPlot->xAxis, customPlot->yAxis);
    gr_bar = new QCPBars(customPlot->xAxis, customPlot->yAxis);
    gb_bar = new QCPBars(customPlot->xAxis, customPlot->yAxis);
    b_bar = new QCPBars(customPlot->xAxis, customPlot->yAxis);
    bar_list << r_bar << gr_bar << gb_bar << b_bar;
    foreach (QCPBars *bar, bar_list) {
        bar->setAntialiased(false);
        bar->setWidthType(QCPBars::wtPlotCoords);
        bar->setWidth(bar->width() / bar_list.size());
        group->append(bar);
    }

    group->setSpacingType(QCPBarsGroup::stAbsolute);
    group->setSpacing(0);

    // set names and colors
    QColor color;
    r_bar->setName("r bar");
    color = QColor_R;
    r_bar->setPen(QPen(color));
    r_bar->setBrush(color);
    gr_bar->setName("gr bar");
    color = QColor_Gr;
    gr_bar->setPen(QPen(color));
    gr_bar->setBrush(color);
    gb_bar->setName("gb bar");
    color = QColor_Gb;
    gb_bar->setPen(QPen(color));
    gb_bar->setBrush(color);
    b_bar->setName("b bar");
    color = QColor_B;
    b_bar->setPen(QPen(color));
    b_bar->setBrush(color);

    QVector<double> brightness;
    for(int i = 0; i < AXIS_MAX_RANGE + 1; i++) {
        brightness.append(i);
    }
    customPlot->xAxis->setAutoTickStep(true);
	customPlot->xAxis->setRange(-1, AXIS_MAX_RANGE + 1);
	customPlot->xAxis->grid()->setPen(QPen(QColor(130, 130, 130), 0, Qt::DotLine));
    customPlot->xAxis->setLabel("Brightness");
    customPlot->xAxis->setTickVector(brightness);
    customPlot->xAxis->setSubTickCount(0);
    customPlot->xAxis->setTickStep(1);
    customPlot->xAxis->setAutoTickStep(true);
    customPlot->xAxis->setTickLength(0, 0);
    customPlot->xAxis->setTickLabelSide(QCPAxis::lsOutside);
    customPlot->xAxis->setTickLabelPadding(1);

	// prepare y axis:
	customPlot->yAxis->setRange(0, 12);
	customPlot->yAxis->setLabel("Count");
    customPlot->yAxis->setAutoTickStep(true);
    customPlot->yAxis->setNumberFormat("f");
    customPlot->yAxis->setNumberPrecision(0);

    QPen pen;
    pen.setColor(Qt::black);
    pen.setWidth(2);

    customPlot->addGraph();
    pen.setStyle(Qt::DotLine);
    customPlot->graph(0)->setPen(pen);
    customPlot->addGraph();
    pen.setStyle(Qt::SolidLine);
    customPlot->graph(1)->setPen(pen);

	// setup legend:
	customPlot->legend->setVisible(false);

    return customPlot;
}

void RawStatisticsWindow::setPixelChannelShowState(QCPBars *ch_bar, int color_type, int status)
{
    QColor color;

    if (color_type == 0) {
        color = QColor(250, 128, 114, status); //r
    } else if (color_type == 1) {
        color = QColor(192, 193, 255, status); //b
    } else if (color_type == 2) {
        color = QColor(128, 255, 114, status); // gr or g(yuv/jpeg)
    } else if (color_type == 3) {
        color = QColor(173, 216, 230, status); //gb
    }

    ch_bar->setPen(QPen(color));
	ch_bar->setBrush(color);
    ch_bar->setBrush(QBrush(color));
}

QGroupBox *RawStatisticsWindow::createControlShowGroupBox()
{
    QGroupBox *control_info_groupbox = new QGroupBox(this);
    QVBoxLayout *main_v_layout = new QVBoxLayout;
    control_info_groupbox->setMaximumHeight(200);
    control_info_groupbox->setMaximumWidth(130);
    control_info_groupbox->setMinimumWidth(130);

    //color type layout
    color_type = new QComboBox;
    color_type->addItems({"All", "R", "B", "Gr", "Gb"});
    color_type->setCurrentIndex(0);
    connect(color_type, QOverload<int>::of(QComboBox::currentIndexChanged), this, &RawStatisticsWindow::slotColorShowChangeEvent);

    // color label
    color_label_r = new QLabel("R");
    color_label_gr = new QLabel("Gr");
    color_label_gb = new QLabel("Gb");
    color_label_b = new QLabel("B");

    color_block_r = new QLabel();
    color_block_gr = new QLabel();
    color_block_gb = new QLabel();
    color_block_b = new QLabel();

    color_block_r->setFixedSize(16, 16);
    color_block_r->setStyleSheet(QString(
        "background-color: %1; "
        "border: 1px solid #666666; "
        "border-radius: 2px;"
    ).arg(QColor_R.name()));

    color_block_gr->setFixedSize(16, 16);
    color_block_gr->setStyleSheet(QString(
        "background-color: %1; "
        "border: 1px solid #666666; "
        "border-radius: 2px;"
    ).arg(QColor_Gr.name()));

    color_block_gb->setFixedSize(16, 16);
    color_block_gb->setStyleSheet(QString(
        "background-color: %1; "
        "border: 1px solid #666666; "
        "border-radius: 2px;"
    ).arg(QColor_Gb.name()));

    color_block_b->setFixedSize(16, 16);
    color_block_b->setStyleSheet(QString(
        "background-color: %1; "
        "border: 1px solid #666666; "
        "border-radius: 2px;"
    ).arg(QColor_B.name()));

    QHBoxLayout *color_r_layout = new QHBoxLayout();
    QHBoxLayout *color_gr_layout = new QHBoxLayout();
    QHBoxLayout *color_gb_layout = new QHBoxLayout();
    QHBoxLayout *color_b_layout = new QHBoxLayout();

    //color_r_layout->setSpacing(4);
    color_r_layout->addWidget(color_label_r);
    color_r_layout->addWidget(color_block_r);

    //color_gr_layout->setSpacing(4);
    color_gr_layout->addWidget(color_label_gr);
    color_gr_layout->addWidget(color_block_gr);

    //color_gb_layout->setSpacing(4);
    color_gb_layout->addWidget(color_label_gb);
    color_gb_layout->addWidget(color_block_gb);

    //color_b_layout->setSpacing(4);
    color_b_layout->addWidget(color_label_b);
    color_b_layout->addWidget(color_block_b);

    //mid value
    QHBoxLayout *mid_value_layout = new QHBoxLayout;
    QLabel *mid_label = new QLabel("Mid");
    QLabel *mid_pic = new QLabel("--");
    mid_pic->setAlignment(Qt::AlignHCenter);
    mid_label->setMinimumWidth(45);
    mid_label->setMaximumWidth(50);
    mid_label->setMaximumHeight(20);
    mid_label->setAlignment(Qt::AlignHCenter);

    mid_value_layout->addWidget(mid_pic);
    mid_value_layout->addWidget(mid_label);

    //max value
    QHBoxLayout *max_value_layout = new QHBoxLayout;
    QLabel *max_label = new QLabel("Max");
    QLabel *max_pic = new QLabel("-");
    QFont font;
    font.setPointSize(19);
    max_pic->setFont(font);
    max_pic->setAlignment(Qt::AlignHCenter);
    max_label->setMinimumWidth(45);
    max_label->setMaximumWidth(50);
    max_label->setMaximumHeight(20);
    max_label->setAlignment(Qt::AlignHCenter);

    max_value_layout->addStretch(1);
    max_value_layout->addWidget(max_pic);
    max_value_layout->addStretch(1);
    max_value_layout->addWidget(max_label);

    //block layout
    main_v_layout->addStretch(1);
    main_v_layout->addWidget(color_type);
    main_v_layout->addStretch(2);
    main_v_layout->addLayout(color_r_layout);
    main_v_layout->addLayout(color_b_layout);
    main_v_layout->addLayout(color_gr_layout);
    main_v_layout->addLayout(color_gb_layout);
    main_v_layout->addStretch(1);
    main_v_layout->addLayout(mid_value_layout);
    main_v_layout->addLayout(max_value_layout);
    main_v_layout->addStretch(1);

    control_info_groupbox->setLayout(main_v_layout);

	return control_info_groupbox;
}

void RawStatisticsWindow::changeEvent(QEvent *event)
{
    switch(event->type()) {
        case QEvent::LanguageChange:
            retranslateUi();
            break;
        default:
            break;
    }

    QMainWindow::changeEvent(event);
}

void RawStatisticsWindow::retranslateUi()
{
    setWindowTitle(tr("Roi Histogram"));
}

void RawStatisticsWindow::slotColorShowChangeEvent(int index)
{
    if (!this->is_image_loaded || !this->is_statistic_ready) {
        return;
    }
    if (index == 0) {
        setPixelChannelShowState(r_bar, 0, 255);
        setPixelChannelShowState(b_bar, 1, 255);
        setPixelChannelShowState(gr_bar, 2, 255);
        setPixelChannelShowState(gb_bar, 3, 255);
    } else if (index == 1) { // r
        setPixelChannelShowState(r_bar, 0, 255);
        setPixelChannelShowState(b_bar, 1, 0);
        setPixelChannelShowState(gr_bar, 2, 0);
        setPixelChannelShowState(gb_bar, 3, 0);
    } else if (index == 2) { // b
        setPixelChannelShowState(r_bar, 0, 0);
        setPixelChannelShowState(b_bar, 1, 255);
        setPixelChannelShowState(gr_bar, 2, 0);
        setPixelChannelShowState(gb_bar, 3, 0);
    } else if (index == 3) { // gr
        setPixelChannelShowState(r_bar, 0, 0);
        setPixelChannelShowState(b_bar, 1, 0);
        setPixelChannelShowState(gr_bar, 2, 255);
        setPixelChannelShowState(gb_bar, 3, 0);
    } else if (index == 4) { // gb
        setPixelChannelShowState(r_bar, 0, 0);
        setPixelChannelShowState(b_bar, 1, 0);
        setPixelChannelShowState(gr_bar, 2, 0);
        setPixelChannelShowState(gb_bar, 3, 255);
    }
   updateStatisticsShow(overall_x_min, overall_x_max, overall_y_max);
}

int RawStatisticsWindow::getMidValueIndex(QMap<int, int> channel_map, int min_pixel, int max_pixel)
{
    QMap<MyInt, int> pixel_map;
    pixel_map.clear();
    for (int i = min_pixel; i < max_pixel + 1; i++) {
        if (channel_map[i] != 0) {
            pixel_map.insert(MyInt(i), channel_map[i]);
        }
    }

    QMap<MyInt, int>::const_iterator iter = pixel_map.constBegin();
    int index = pixel_map.size() / 2;
    int i = 0, mid_pixel;

    if (pixel_map.size() % 2 == 0) {
        while (iter != pixel_map.constEnd()) {
            if (i == (index - 1)) {
                mid_pixel = iter.key().value;
                break;
            }
            i++;
            ++iter;
        }
    } else {
        while (iter != pixel_map.constEnd()) {
            if (i == index) {
                mid_pixel = iter.key().value;
                break;
            }
            i++;
            ++iter;
        }
    }

    return mid_pixel;
}

void RawStatisticsWindow::calculateMidValue(QMap<int, int> r_map, QMap<int, int> gr_map, QMap<int, int> gb_map, QMap<int, int> b_map, int x_min, int x_max)
{
    //get r mid value
    mid_r_index = getMidValueIndex(r_map, min_R, max_R);

    //get gr mid value
    mid_gr_index = getMidValueIndex(gr_map, min_Gr, max_Gr);

    //get gb mid value
    mid_gb_index = getMidValueIndex(gb_map, min_Gb, max_Gb);

    //get b mid value
    mid_b_index = getMidValueIndex(b_map, min_B, max_B);

    //get rgb mid value
    QMap<int, int> rgb_map;
    for (int i = min_R; i <= max_R; i++) {
        rgb_map.insert(i, r_map[i]);
    }
    for (int i = min_Gr; i <= max_Gr; i++) {
        rgb_map.insert(i, gr_map[i]);
    }
    for (int i = min_Gb; i <= max_Gb; i++) {
        rgb_map.insert(i, gb_map[i]);
    }
    for (int i = min_B; i <= max_B; i++) {
        rgb_map.insert(i, b_map[i]);
    }

    mid_rgb_index = getMidValueIndex(rgb_map, x_min, x_max);
}

int RawStatisticsWindow::getCurMidValue()
{
    int mid_value;

    if (color_type->currentIndex() == 0) {
        mid_value = mid_rgb_index;
    } else if (color_type->currentIndex() == 1) {
        mid_value = mid_r_index;
    } else if (color_type->currentIndex() == 2) {
        mid_value = mid_b_index;
    } else if (color_type->currentIndex() == 3) {
        mid_value = mid_gr_index;
    } else if (color_type->currentIndex() == 4) {
        mid_value = mid_gb_index;
    }

    return mid_value;
}

std::tuple<int, int, int> RawStatisticsWindow::getMaxMinValue(int x_min, int x_max, int y_max)
{
    int min, max;
    int y_max_value;

    if (color_type->currentIndex() == 0) {
        min = x_min;
        max = x_max;
        y_max_value = y_max;
    } else if (color_type->currentIndex() == 1) { // r
        min = min_R;
        max = max_R;
        y_max_value = y_r_max;
    } else if (color_type->currentIndex() == 2) { // b
        min = min_B;
        max = max_B;
        y_max_value = y_b_max;
    } else if (color_type->currentIndex() == 3) { // gr
        min = min_Gr;
        max = max_Gr;
        y_max_value = y_gr_max;
    } else if (color_type->currentIndex() == 4) { // gb
        min = min_Gb;
        max = max_Gb;
        y_max_value = y_gb_max;
    }

    std::tuple<int, int, int> max_min = std::make_tuple(min, max, y_max_value);

    return max_min;
}

int RawStatisticsWindow::getMeanValue(QMap<int, int> &r_count, QMap<int, int> &gr_count, QMap<int, int> &gb_count, QMap<int, int> &b_count, int x_min, int x_max)
{
    int cnt = 0;
    int mean_value = 0;
    unsigned long long int total_pixel = 0;

    if (color_type->currentIndex() == 0) {
        for (int i = x_min; i < x_max + 1; i++) {
            if (r_count[i] != 0) {
                total_pixel += i * r_count[i];
                cnt += r_count[i];
            }

            if (gr_count[i] != 0) {
                total_pixel += i * gr_count[i];
                cnt += gr_count[i];
            }

            if (gb_count[i] != 0) {
                total_pixel += i * gb_count[i];
                cnt += gb_count[i];
            }

            if (b_count[i] != 0) {
                total_pixel += i * b_count[i];
                cnt += b_count[i];
            }
        }
    } else if (color_type->currentIndex() == 1) { // r
        for (int i = min_R; i < max_R + 1; i++) {
            if (r_count[i] != 0) {
                total_pixel += i * r_count[i];
                cnt += r_count[i];
            }
        }
    } else if (color_type->currentIndex() == 2) { // b
        for (int i = min_B; i < max_B + 1; i++) {
            if (b_count[i] != 0) {
                total_pixel += i * b_count[i];
                cnt += b_count[i];
            }
        }
    } else if (color_type->currentIndex() == 3) { // gr
        for (int i = min_Gr; i < max_Gr + 1; i++) {
            if (gr_count[i] != 0) {
                total_pixel += i * gr_count[i];
                cnt += gr_count[i];
            }
        }
    } else if (color_type->currentIndex() == 4) { // gb
        for (int i = min_Gb; i < max_Gb + 1; i++) {
            if (gb_count[i] != 0) {
                total_pixel += i * gb_count[i];
                cnt += gb_count[i];
            }
        }
    }
    mean_value = total_pixel / cnt;

    return mean_value;
}

std::tuple<int, int, int> RawStatisticsWindow::findMaxPixelCnt(QMap<int, int> &r_count, QMap<int, int> &gr_count, QMap<int, int> &gb_count, QMap<int, int> &b_count, int x_min, int x_max)
{
    int total_pixel_cnt;
    int max_pixel_cnt, cur_pixel;

    if (color_type->currentIndex() == 0) {
        max_pixel_cnt = r_count[x_min];
        cur_pixel = x_min;
        for (int i = x_min; i < x_max + 1; i++) {
            if (r_count[i] > max_pixel_cnt) {
                cur_pixel = i;
                max_pixel_cnt= r_count[i];
            }

            if (gr_count[i] > max_pixel_cnt) {
                cur_pixel = i;
                max_pixel_cnt= gr_count[i];
            }

            if (gb_count[i] > max_pixel_cnt) {
                cur_pixel = i;
                max_pixel_cnt= gb_count[i];
            }

            if (b_count[i] > max_pixel_cnt) {
                cur_pixel = i;
                max_pixel_cnt = b_count[i];
            }
        }
        total_pixel_cnt = crop_width * crop_height;
    } else if (color_type->currentIndex() == 1) { // r
        max_pixel_cnt = r_count[min_R];
        cur_pixel = min_R;
        for (int i = min_R; i < max_R + 1; i++) {
            if (r_count[i] > max_pixel_cnt) {
                cur_pixel = i;
                max_pixel_cnt= r_count[i];
            }
        }
        total_pixel_cnt = total_r_pixel_cnt;
    } else if (color_type->currentIndex() == 2) { // b
        max_pixel_cnt = b_count[min_B];
        cur_pixel = min_B;
        for (int i = min_B; i < max_B + 1; i++) {
            if (b_count[i] > max_pixel_cnt) {
                cur_pixel = i;
                max_pixel_cnt= b_count[i];
            }
        }
        total_pixel_cnt = total_b_pixel_cnt;
    } else if (color_type->currentIndex() == 3) { // gr
        max_pixel_cnt = gr_count[min_Gr];
        cur_pixel = min_Gr;
        for (int i = min_Gr; i < max_Gr + 1; i++) {
            if (gr_count[i] > max_pixel_cnt) {
                cur_pixel = i;
                max_pixel_cnt= gr_count[i];
            }
        }
        total_pixel_cnt = total_gr_pixel_cnt;
    } else if (color_type->currentIndex() == 4) { // gb
        max_pixel_cnt = gb_count[min_Gb];
        cur_pixel = min_Gb;
        for (int i = min_Gb; i < max_Gb + 1; i++) {
            if (gb_count[i] > max_pixel_cnt) {
                cur_pixel = i;
                max_pixel_cnt= gb_count[i];
            }
        }
        total_pixel_cnt = total_gb_pixel_cnt;
    }

    std::tuple<int, int, int> result = std::make_tuple(cur_pixel, max_pixel_cnt, total_pixel_cnt);

    return result;
}

void RawStatisticsWindow::showMidMaxLine(int mean_value_pixel, int max_cnt_pixel, int number)
{
    QVector<double> mean_pixel;
    QVector<double> max_pixel;
    QVector<double> value;

    mean_pixel << mean_value_pixel << mean_value_pixel;
    max_pixel << max_cnt_pixel << max_cnt_pixel;
    value << 0 << number;

    customPlot->graph(0)->setData(mean_pixel, value);
    customPlot->graph(1)->setData(max_pixel, value);
}

void RawStatisticsWindow::updateStatisticsShow(int x_min, int x_max, int y_max)
{
    int mid_value;

    /*chart table show*/
    std::tuple<int, int, int> max_min = getMaxMinValue(x_min, x_max, y_max);
    customPlot->xAxis->setRange(std::get<0>(max_min) - 1, std::get<1>(max_min) + 1);
    customPlot->xAxis->setLabel(QString("Brightness (Range: %1 - %2)").arg(QString::number(std::get<0>(max_min)),
        QString::number(std::get<1>(max_min))));
    customPlot->yAxis->setRange(0, std::get<2>(max_min));
    customPlot->yAxis->setLabel(QString("Count (Max: %1)").arg(QString::number(std::get<2>(max_min))));

    /*get mid value*/
    mid_value = getCurMidValue();

    /*find max value info*/
    std::tuple<int, int, int> max_result = findMaxPixelCnt(r_count_map, gr_count_map, gb_count_map, b_count_map, x_min, x_max);

    /*find avr value*/
    int avr_value = getMeanValue(r_count_map, gr_count_map, gb_count_map, b_count_map, x_min, x_max);

    QString max_cnt_pixel_s = QString::number(std::get<0>(max_result));
    QString pixel_cnt_s = QString::number(std::get<1>(max_result));
    QString total_pixel_s = QString::number(std::get<2>(max_result));
    QString mid_value_s = QString::number(mid_value);
    QString avr_value_s = QString::number(avr_value);

    /*mid and max pixel number picture show*/
    showMidMaxLine(mid_value, std::get<0>(max_result), std::get<1>(max_result));

    /*show label info*/
    raw_info_label->setText(QString("Max: %1(%2/%3 Pts) Mid: %4 Avr: %5").arg(max_cnt_pixel_s, pixel_cnt_s,
        total_pixel_s, mid_value_s, avr_value_s));

    customPlot->replot(QCustomPlot::rpImmediate);
}

void RawStatisticsWindow::handleUpdatePointsInfo(const QPixmap *image, QRectF rect, bool is_update_colortype)
{
    if (image->isNull()) {
        return;
    } else {
        img_type = utils::IMAGE_TYPE::IMAGE_YUV;
        is_image_loaded = true;
    }

    // update color combobox for yuv/jpeg
    if (is_update_colortype) {
        is_statistic_ready = false;
        int pre_index = color_type->currentIndex();
        // raw -> yuv/rgb, gb color -> gr color
        if (pre_index == 4) {
            setPixelChannelShowState(gr_bar, 2, 255);
        }
        pre_index = qBound(0, pre_index, 3);
        color_type->clear();
        color_type->addItems({"All", "R", "B", "G"});
        color_type->setCurrentIndex(pre_index);
        // update color label and color block
        color_label_gr->setText("G");
        color_block_gr->setStyleSheet(QString(
        "background-color: %1; "
        "border: 1px solid #666666; "
        "border-radius: 2px;"
        ).arg(QColor_G.name()));
        if (!color_block_gb->isHidden()) {
            color_block_gb->hide();
        }
        if (!color_label_gb->isHidden()) {
            color_label_gb->hide();
        }
    }

    int start_x = rect.x();
    int start_y = rect.y();
    unsigned int tempR, tempG, tempB;
    unsigned short *start = nullptr;
    int x_max = 0, x_min = 0, y_max = 0;
    crop_width = rect.width();
    crop_height = rect.height();

    max_R = 0;
    max_Gr = 0;
    max_Gb = 0;
    max_B = 0;
    min_R = 0;
    min_Gr = 0;
    min_Gb = 0;
    min_B = 0;
    y_r_max = 0;
    y_gr_max = 0;
    y_gb_max = 0;
    y_b_max = 0;
    total_r_pixel_cnt = 0;
    total_gr_pixel_cnt = 0;
    total_gb_pixel_cnt = 0;
    total_b_pixel_cnt = 0;

    r_count_map.clear();
    gr_count_map.clear();
    gb_count_map.clear();
    b_count_map.clear();

    QImage img = image->toImage();

    const int width = img.width();
    const int height = img.height();

    int roi_x_s = rect.x();
    int roi_x_e = rect.x() + rect.width();
    int roi_y_s = rect.y();
    int roi_y_e = rect.y() + rect.height();

    roi_x_s = qBound(0, roi_x_s, width - 1);
    roi_x_e = qBound(0, roi_x_e, width - 1);
    roi_y_s = qBound(0, roi_y_s, height - 1);
    roi_y_e = qBound(0, roi_y_e, height - 1);

    for (int y = roi_y_s; y < roi_y_e; ++y) {
        for (int x = roi_x_s; x < roi_x_e; ++x) {
            QRgb pixel = img.pixel(x, y);
            int r = qRed(pixel);
            int g = qGreen(pixel);
            int b = qBlue(pixel);
            r_count_map[r]++;
            gr_count_map[g]++;
            gb_count_map[g]++;
            b_count_map[b]++;
        }
    }

    QVector<double> range_key;
    QVector<double> r_data, gr_data, gb_data, b_data;

    QMap<int, int> r_count_map_temp;
    QMap<int, int> gr_count_map_temp;
    QMap<int, int> gb_count_map_temp;
    QMap<int, int> b_count_map_temp;

    for (int i = 0; i < 256; i++) {
        if (r_count_map[i] != 0) {
            r_count_map_temp[i] = r_count_map[i];
        }

        if (gr_count_map[i] != 0) {
            gr_count_map_temp[i] = gr_count_map[i];
        }

        if (gb_count_map[i] != 0) {
            gb_count_map_temp[i] = gb_count_map[i];
        }

        if (b_count_map[i] != 0) {
            b_count_map_temp[i] = b_count_map[i];
        }
    }

    QMap<int, int>::const_iterator k;
    int index = 0;
    for (k = r_count_map_temp.constBegin(); k != r_count_map_temp.constEnd(); ++k) {
        index++;
        if (index == 1) {
            min_R = k.key();
            max_R = k.key();
            y_r_max = k.value();
        } else {
            min_R = MIN(k.key(), min_R);
            max_R = MAX(k.key(), max_R);
            if (k.value() > y_r_max) {
                y_r_max = k.value();
            }
        }
    }

    index = 0;
    for (k = gr_count_map_temp.constBegin(); k != gr_count_map_temp.constEnd(); ++k) {
        index++;
        if (index == 1) {
            min_Gr = k.key();
            max_Gr = k.key();
            y_gr_max = k.value();
        } else {
            min_Gr = MIN(k.key(), min_Gr);
            max_Gr = MAX(k.key(), max_Gr);
            if (k.value() > y_gr_max) {
                y_gr_max = k.value();
            }
        }
    }

    index = 0;
    for (k = gb_count_map_temp.constBegin(); k != gb_count_map_temp.constEnd(); ++k) {
        index++;
        if (index == 1) {
            min_Gb = k.key();
            max_Gb = k.key();
            y_gb_max = k.value();
        } else {
            min_Gb = MIN(k.key(), min_Gb);
            max_Gb = MAX(k.key(), max_Gb);
            if (k.value() > y_gb_max) {
                y_gb_max = k.value();
            }
        }
    }

    index = 0;
    for (k = b_count_map_temp.constBegin(); k != b_count_map_temp.constEnd(); ++k) {
        index++;
        if (index == 1) {
            min_B = k.key();
            max_B = k.key();
            y_b_max = k.value();
        } else {
            min_B = MIN(k.key(), min_B);
            max_B = MAX(k.key(), max_B);
            if (k.value() > y_b_max) {
                y_b_max = k.value();
            }
        }
    }

    x_max = max_R;
    x_min = min_R;
    y_max = y_r_max;

    x_max = MAX(max_Gr, x_max);
    x_min = MIN(min_Gr, x_min);
    y_max = MAX(y_gr_max, y_max);

    x_max = MAX(max_Gb, x_max);
    x_min = MIN(min_Gb, x_min);
    y_max = MAX(y_gb_max, y_max);

    x_max = MAX(max_B, x_max);
    x_min = MIN(min_B, x_min);
    y_max = MAX(y_b_max, y_max);

    for (int i = x_min; i < x_max + 1; i++) {
        range_key.append(i);
        r_data.append(r_count_map[i]);
        gr_data.append(gr_count_map[i]);
        gb_data.append(gb_count_map[i]);
        b_data.append(b_count_map[i]);
    }
    r_bar->setData(range_key, r_data);
    gr_bar->setData(range_key, gr_data);
    gb_bar->setData(range_key, gb_data);
    b_bar->setData(range_key, b_data);

    /*calculate mid value*/
    calculateMidValue(r_count_map, gr_count_map, gb_count_map, b_count_map, x_min, x_max);

    /*show statistics picture*/
    updateStatisticsShow(x_min, x_max, y_max);
    overall_x_min = x_min;
    overall_x_max = x_max;
    overall_y_max = y_max;

    is_statistic_ready = true;
}

void RawStatisticsWindow::handleUpdatePointsInfo(unsigned char *praw, unsigned int width, unsigned int height,
                                                 utils::RAW_BAYER_ID bayer, utils::RAW_BITS bits, QRectF rect, bool is_update_colortype)
{
    if (praw != nullptr) {
        is_image_loaded = true;
        img_type = utils::IMAGE_TYPE::IMAGE_RAW;
    } else {
        return;
    }
    // update color combobox for raw
    if (is_update_colortype) {
        is_statistic_ready = false;
        int pre_index = color_type->currentIndex();
        pre_index = qBound(0, pre_index, 4);
        color_type->clear();
        color_type->addItems({"All", "R", "B", "Gr", "Gb"});
        color_type->setCurrentIndex(pre_index); // preserve the index
        // update color label and color block
        color_label_gr->setText("Gr");
        color_block_gr->setStyleSheet(QString(
        "background-color: %1; "
        "border: 1px solid #666666; "
        "border-radius: 2px;"
        ).arg(QColor_Gr.name()));
        if (color_block_gb->isHidden()) {
            color_block_gb->show();
        }
        if (color_label_gb->isHidden()) {
            color_label_gb->show();
        }
    }

    unsigned short *source = reinterpret_cast<unsigned short *>(praw);
    unsigned short *s = nullptr;
    int start_x = rect.x();
    int start_y = rect.y();
    unsigned int tempR, tempG, tempB;
    unsigned short *start = nullptr;
    int x_max = 0, x_min = 0, y_max = 0;
    crop_width = rect.width();
    crop_height = rect.height();

    max_R = 0;
    max_Gr = 0;
    max_Gb = 0;
    max_B = 0;
    min_R = 0;
    min_Gr = 0;
    min_Gb = 0;
    min_B = 0;
    y_r_max = 0;
    y_gr_max = 0;
    y_gb_max = 0;
    y_b_max = 0;
    total_r_pixel_cnt = 0;
    total_gr_pixel_cnt = 0;
    total_gb_pixel_cnt = 0;
    total_b_pixel_cnt = 0;

    r_count_map.clear();
    gr_count_map.clear();
    gb_count_map.clear();
    b_count_map.clear();

    QVector<double> range_key;
    QVector<double> r_data, gr_data, gb_data, b_data;

    if (start_y == 0) {
        s = source;
    } else {
        s = source + (start_y - 1) * width;
        start_y = start_y - 1;
    }

    if (start_x == 0) {
        s = s + start_x;
    } else {
        s = s + start_x - 1;
    }

    start = s;
    int pattern_idx = bayer;
    QList<QStringList> bayer_patterns = {{"bg", "gr"}, {"gb", "rg"}, {"gr", "bg"}, {"rg", "gb"}};
    QStringList bayer_pattern = bayer_patterns[pattern_idx];

    unsigned int j = 0;
    unsigned int i = 0;
    unsigned int cnt = 0;
    start_x = start_x -1;
    for (j = start_y; j < start_y + crop_height; j++) {
        QString pattern = bayer_pattern[j % 2];

        cnt = 0;
        for (i = start_x; i < start_x + crop_width; i++) {
            int pattern_index = i % 2;
            tempR = 0;
            tempG = 0;
            tempB = 0;
            if((i > 0) && (i < (width - 1)) && (j > 0) && (j < (height - 1))) {
                if (pattern[pattern_index] == "g") {
                    tempG = *s;
                    if(pattern[(pattern_index + 1) % 2] == "b") {
                        gb_count_map[tempG]++;
                        total_gb_pixel_cnt++;
                    } else {
                        gr_count_map[tempG]++;
                        total_gr_pixel_cnt++;
                    }
                } else if (pattern[pattern_index] == "r") {
                    tempR = *s;
                    total_r_pixel_cnt++;
                    r_count_map[tempR]++;
                } else if (pattern[pattern_index] == "b") {
                    tempB = *s;
                    total_b_pixel_cnt++;
                    b_count_map[tempB]++;
                }
            }
            if (i < (start_x + crop_width -1)) {
                cnt++;
            }
            s = start + (j - start_y) * width + cnt;
        }
    }

    QMap<int, int> r_count_map_temp;
    QMap<int, int> gr_count_map_temp;
    QMap<int, int> gb_count_map_temp;
    QMap<int, int> b_count_map_temp;
    for (int i = 0; i < 4096; i++) {
        if (r_count_map[i] != 0) {
            r_count_map_temp[i] = r_count_map[i];
        }

        if (gr_count_map[i] != 0) {
            gr_count_map_temp[i] = gr_count_map[i];
        }

        if (gb_count_map[i] != 0) {
            gb_count_map_temp[i] = gb_count_map[i];
        }

        if (b_count_map[i] != 0) {
            b_count_map_temp[i] = b_count_map[i];
        }
    }

    QMap<int, int>::const_iterator k;
    int index = 0;
    for (k = r_count_map_temp.constBegin(); k != r_count_map_temp.constEnd(); ++k) {
        index++;
        if (index == 1) {
            min_R = k.key();
            max_R = k.key();
            y_r_max = k.value();
        } else {
            min_R = MIN(k.key(), min_R);
            max_R = MAX(k.key(), max_R);
            if (k.value() > y_r_max) {
                y_r_max = k.value();
            }
        }
    }

    index = 0;
    for (k = gr_count_map_temp.constBegin(); k != gr_count_map_temp.constEnd(); ++k) {
        index++;
        if (index == 1) {
            min_Gr = k.key();
            max_Gr = k.key();
            y_gr_max = k.value();
        } else {
            min_Gr = MIN(k.key(), min_Gr);
            max_Gr = MAX(k.key(), max_Gr);
            if (k.value() > y_gr_max) {
                y_gr_max = k.value();
            }
        }
    }

    index = 0;
    for (k = gb_count_map_temp.constBegin(); k != gb_count_map_temp.constEnd(); ++k) {
        index++;
        if (index == 1) {
            min_Gb = k.key();
            max_Gb = k.key();
            y_gb_max = k.value();
        } else {
            min_Gb = MIN(k.key(), min_Gb);
            max_Gb = MAX(k.key(), max_Gb);
            if (k.value() > y_gb_max) {
                y_gb_max = k.value();
            }
        }
    }

    index = 0;
    for (k = b_count_map_temp.constBegin(); k != b_count_map_temp.constEnd(); ++k) {
        index++;
        if (index == 1) {
            min_B = k.key();
            max_B = k.key();
            y_b_max = k.value();
        } else {
            min_B = MIN(k.key(), min_B);
            max_B = MAX(k.key(), max_B);
            if (k.value() > y_b_max) {
                y_b_max = k.value();
            }
        }
    }

    x_max = max_R;
    x_min = min_R;
    y_max = y_r_max;

    x_max = MAX(max_Gr, x_max);
    x_min = MIN(min_Gr, x_min);
    y_max = MAX(y_gr_max, y_max);

    x_max = MAX(max_Gb, x_max);
    x_min = MIN(min_Gb, x_min);
    y_max = MAX(y_gb_max, y_max);

    x_max = MAX(max_B, x_max);
    x_min = MIN(min_B, x_min);
    y_max = MAX(y_b_max, y_max);

    for (int i = x_min; i < x_max + 1; i++) {
        range_key.append(i);
        r_data.append(r_count_map[i]);
        gr_data.append(gr_count_map[i]);
        gb_data.append(gb_count_map[i]);
        b_data.append(b_count_map[i]);
    }
    r_bar->setData(range_key, r_data);
    gr_bar->setData(range_key, gr_data);
    gb_bar->setData(range_key, gb_data);
    b_bar->setData(range_key, b_data);

    /*calculate mid value*/
    calculateMidValue(r_count_map, gr_count_map, gb_count_map, b_count_map, x_min, x_max);

    /*show statistics picture*/
    updateStatisticsShow(x_min, x_max, y_max);
    overall_x_min = x_min;
    overall_x_max = x_max;
    overall_y_max = y_max;

    is_statistic_ready = true;
}

void RawStatisticsWindow::keyReleaseEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape) {
        this->close();
    }
}

void RawStatisticsWindow::closeEvent(QCloseEvent *event)
{
    event->accept();
    this->close();
    emit sigChildWindowEvent(1);
}

void RawStatisticsWindow::closeWindow()
{
    this->close();
}
