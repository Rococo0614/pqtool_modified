#include "level_meter_histogram_window.hpp"
#include <QtCharts/QChart>
#include <QtCharts/QChartView>

const int PIXEL_MAX_VALUE = 255;
#define MIN(a, b)               ((a) < (b) ? (a) : (b))
#define MAX(a, b)               ((a) > (b) ? (a) : (b))

LevelMeterHistogramWindow::LevelMeterHistogramWindow(QWidget *parent) :
    QMainWindow(parent)
{
    resize(QDesktopWidget().availableGeometry(this).size() * 0.75);

    initUI();
    retranslateUi();
}

LevelMeterHistogramWindow::~LevelMeterHistogramWindow() {}

void LevelMeterHistogramWindow::initUI()
{
    QWidget *main_widget = new QWidget;
    QVBoxLayout *main_layout = new QVBoxLayout;
    QHBoxLayout *top_layout = new QHBoxLayout;

    //top layout
    QCustomPlot *customPlot = createChartView();
    QGroupBox *control_info = createControlShowGroupBox();
    top_layout->addWidget(customPlot);
    top_layout->addWidget(control_info);

    //bottom table
    QWidget *bottom_table = createTableMenu();
    main_layout->addLayout(top_layout);
    main_layout->addWidget(bottom_table);

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

void LevelMeterHistogramWindow::setStatusBarState(QString text)
{
    status_label->setText(text);
}

QCustomPlot *LevelMeterHistogramWindow::createChartView()
{
    customPlot = new QCustomPlot(this);
    customPlot->rescaleAxes();
    customPlot->setMinimumSize(this->frameGeometry().width() * 0.9, this->frameGeometry().height() * 0.8);

    customPlot->xAxis->setRange(0, 255);
    customPlot->xAxis->setLabel(QString("pixel column (Max: %1)").arg(QString::number(255)));
    customPlot->xAxis->setAutoTickStep(true);
    customPlot->xAxis->setNumberFormat("f");
    customPlot->xAxis->setNumberPrecision(0);
    customPlot->xAxis->grid()->setPen(QPen(QColor(130, 130, 130), 0, Qt::DotLine));
    customPlot->xAxis->grid()->setVisible(true);

    customPlot->yAxis->grid()->setVisible(true);
	customPlot->yAxis->setRange(0, 255);
    customPlot->yAxis->setLabel(QString("pixel value (Range: %1 - %2)").arg(QString::number(0), QString::number(255)));
    customPlot->yAxis->setAutoTickStep(true);
    customPlot->yAxis->setNumberFormat("f");
    customPlot->yAxis->setNumberPrecision(0);

    //add line
    QPen pen;
    pen.setStyle(Qt::SolidLine);
    pen.setWidth(2);

    pen.setColor(Qt::red);
    r_line = customPlot->addGraph();
    r_line->setPen(pen);//r
    g_line = customPlot->addGraph();
    pen.setColor(Qt::green);
    g_line->setPen(pen);//g
    b_line = customPlot->addGraph();
    pen.setColor(Qt::blue);
    b_line->setPen(pen);//b
    y_line = customPlot->addGraph();
    pen.setColor(QColor(255,204,0));
    y_line->setPen(pen);//y
    y_line->setVisible(false);

    return customPlot;
}

QGroupBox *LevelMeterHistogramWindow::createControlShowGroupBox()
{
    QGroupBox *control_info_groupbox = new QGroupBox(this);
    QVBoxLayout *main_v_layout = new QVBoxLayout;
    control_info_groupbox->setMaximumHeight(160);
    control_info_groupbox->setMaximumWidth(100);

    //r color layout
    QHBoxLayout *r_color_layout = new QHBoxLayout;
    QLabel *r_color_label = new QLabel("R");
    r_check_box = new QCheckBox;
    r_check_box->setChecked(true);
    connect(r_check_box, &QPushButton::clicked, [=]{
        if (r_check_box->isChecked()) {
            bar_r_check_box->setChecked(true);
        } else {
            bar_r_check_box->setChecked(false);
        }
        handleSelectShowColor(r_check_box->isChecked());
    });
    r_color_label->setMinimumWidth(45);
    r_color_label->setMaximumWidth(50);
    r_color_label->setMaximumHeight(20);
    r_color_label->setAlignment(Qt::AlignHCenter);
    r_color_label->setStyleSheet(QString("background-color:") + "rgb(255,190,192)");

	r_color_layout->addWidget(r_check_box);
	r_color_layout->addWidget(r_color_label);

    //g color layout
    QHBoxLayout *g_color_layout = new QHBoxLayout;
    QLabel *g_color_label = new QLabel("G");
    g_check_box = new QCheckBox;
    g_check_box->setChecked(true);
    connect(g_check_box, &QPushButton::clicked, [=]{
        if (g_check_box->isChecked()) {
            bar_g_check_box->setChecked(true);
        } else {
            bar_g_check_box->setChecked(false);
        }
        handleSelectShowColor(g_check_box->isChecked());
    });
    g_color_label->setMinimumWidth(45);
    g_color_label->setMaximumWidth(50);
    g_color_label->setMaximumHeight(20);
    g_color_label->setAlignment(Qt::AlignHCenter);
    g_color_label->setStyleSheet(QString("background-color:") + "rgb(192,255,192)");

    g_color_layout->addWidget(g_check_box);
    g_color_layout->addWidget(g_color_label);

    //b color layout
    QHBoxLayout *b_color_layout = new QHBoxLayout;
    QLabel *b_color_label = new QLabel("B");
    b_check_box = new QCheckBox;
    b_check_box->setChecked(true);
    connect(b_check_box, &QPushButton::clicked, [=]{
        if (b_check_box->isChecked()) {
            bar_b_check_box->setChecked(true);
        } else {
            bar_b_check_box->setChecked(false);
        }
        handleSelectShowColor(b_check_box->isChecked());
    });
    b_color_label->setMinimumWidth(45);
    b_color_label->setMaximumWidth(50);
    b_color_label->setMaximumHeight(20);
    b_color_label->setAlignment(Qt::AlignHCenter);
    b_color_label->setStyleSheet(QString("background-color:") + "rgb(192,193,255)");

    b_color_layout->addWidget(b_check_box);
    b_color_layout->addWidget(b_color_label);

    //y color layout
    QHBoxLayout *y_color_layout = new QHBoxLayout;
    QLabel *y_color_label = new QLabel("Y");
    y_check_box = new QCheckBox;
    connect(y_check_box, &QPushButton::clicked, [=]{
        if (y_check_box->isChecked()) {
            bar_y_check_box->setChecked(true);
        } else {
            bar_y_check_box->setChecked(false);
        }
        handleSelectShowColor(y_check_box->isChecked());
    });
    y_color_label->minimumSize();
    y_color_label->setMinimumWidth(45);
    y_color_label->setMaximumWidth(50);
    y_color_label->setMaximumHeight(20);
    y_color_label->setAlignment(Qt::AlignHCenter);
    y_color_label->setStyleSheet(QString("background-color:") + "rgb(255,255,192)");

    y_color_layout->addWidget(y_check_box);
    y_color_layout->addWidget(y_color_label);

    //block layout
    main_v_layout->addLayout(r_color_layout);
    main_v_layout->addLayout(g_color_layout);
    main_v_layout->addLayout(b_color_layout);
    main_v_layout->addLayout(y_color_layout);
    main_v_layout->addStretch(1);

    control_info_groupbox->setLayout(main_v_layout);

	return control_info_groupbox;
}

QGroupBox *LevelMeterHistogramWindow::createAxisControlBar(QWidget *table_widget)
{
    QGroupBox *axis_control_box = new QGroupBox(table_widget);
    QVBoxLayout *main_v_layout = new QVBoxLayout;
    QHBoxLayout *control_layout = new QHBoxLayout;
    QVBoxLayout *control_v_layout = new QVBoxLayout;

    axis_control_box->setMaximumHeight(240);
    axis_control_box->setMaximumWidth(300);

    //r color layout
    QHBoxLayout *rb_color_layout = new QHBoxLayout;
    QLabel *r_color_label = new QLabel("R");
    QFont font = r_color_label->font();
    font.setPointSize(10);
    r_color_label->setFont(font);
    bar_r_check_box = new QCheckBox;
    bar_r_check_box->setChecked(true);
    connect(bar_r_check_box, &QPushButton::clicked, [=]{
        if (bar_r_check_box->isChecked()) {
            r_check_box->setChecked(true);
        } else {
            r_check_box->setChecked(false);
        }
        handleSelectShowColor(bar_r_check_box->isChecked());
    });
    r_color_label->setMinimumWidth(20);
    r_color_label->setMaximumWidth(25);
    r_color_label->setMaximumHeight(20);
    r_color_label->setAlignment(Qt::AlignHCenter);
    r_color_label->setStyleSheet(QString("background-color:") + "rgb(255,190,192)");

    rb_color_layout->addStretch(1);
    rb_color_layout->addWidget(bar_r_check_box);
    rb_color_layout->addWidget(r_color_label);
    rb_color_layout->addStretch(1);

    //g color layout
    QLabel *b_color_label = new QLabel("B");
    font = b_color_label->font();
    font.setPointSize(10);
    b_color_label->setFont(font);
    bar_b_check_box = new QCheckBox;
    bar_b_check_box->setChecked(true);
    connect(bar_b_check_box, &QPushButton::clicked, [=]{
        if (bar_b_check_box->isChecked()) {
            b_check_box->setChecked(true);
        } else {
            b_check_box->setChecked(false);
        }
        handleSelectShowColor(bar_b_check_box->isChecked());
    });
    b_color_label->setMinimumWidth(20);
    b_color_label->setMaximumWidth(25);
    b_color_label->setMaximumHeight(20);
    b_color_label->setAlignment(Qt::AlignHCenter);
    b_color_label->setStyleSheet(QString("background-color:") + "rgb(192,193,255)");

    rb_color_layout->addWidget(bar_b_check_box);
    rb_color_layout->addWidget(b_color_label);
    rb_color_layout->addStretch(1);

    //g color layout
    QHBoxLayout *gy_color_layout = new QHBoxLayout;
    QLabel *g_color_label = new QLabel("G");
    font = g_color_label->font();
    font.setPointSize(10);
    g_color_label->setFont(font);
    bar_g_check_box = new QCheckBox;
    bar_g_check_box->setChecked(true);
    connect(bar_g_check_box, &QPushButton::clicked, [=]{
        if (bar_g_check_box->isChecked()) {
            g_check_box->setChecked(true);
        } else {
            g_check_box->setChecked(false);
        }
        handleSelectShowColor(bar_g_check_box->isChecked());
    });
    g_color_label->setMinimumWidth(20);
    g_color_label->setMaximumWidth(25);
    g_color_label->setMaximumHeight(20);
    g_color_label->setAlignment(Qt::AlignHCenter);
    g_color_label->setStyleSheet(QString("background-color:") + "rgb(192,255,192)");

    gy_color_layout->addStretch(1);
    gy_color_layout->addWidget(bar_g_check_box);
    gy_color_layout->addWidget(g_color_label);
    gy_color_layout->addStretch(1);

    //y color layout
    QLabel *y_color_label = new QLabel("Y");
    font = y_color_label->font();
    font.setPointSize(10);
    y_color_label->setFont(font);
    bar_y_check_box = new QCheckBox;
    connect(bar_y_check_box, &QPushButton::clicked, [=]{
        if (bar_y_check_box->isChecked()) {
            y_check_box->setChecked(true);
        } else {
            y_check_box->setChecked(false);
        }
        handleSelectShowColor(bar_y_check_box->isChecked());
    });
    y_color_label->setMinimumWidth(20);
    y_color_label->setMaximumWidth(25);
    y_color_label->setMaximumHeight(20);
    y_color_label->setAlignment(Qt::AlignHCenter);
    y_color_label->setStyleSheet(QString("background-color:") + "rgb(255,255,192)");

    gy_color_layout->addWidget(bar_y_check_box);
    gy_color_layout->addWidget(y_color_label);
    gy_color_layout->addStretch(1);

    //Axis max and min
    QHBoxLayout *axis_max_layout = new QHBoxLayout;
    QLabel *axis_max_label = new QLabel("Axis Max:");
    font = axis_max_label->font();
    font.setPointSize(9);
    axis_max_label->setFont(font);
    max_range_edit = new QLineEdit;
    QIntValidator *range_validator = new QIntValidator(0, 300, this);
    max_range_edit->setFixedSize(70, 30);
    max_range_edit->setValidator(range_validator);
    max_range_edit->setText("255");
    connect(max_range_edit, &QLineEdit::textChanged, this, std::bind(&LevelMeterHistogramWindow::cellEdited, this, max_range_edit));
    axis_max_layout->addWidget(axis_max_label);
    axis_max_layout->addWidget(max_range_edit);

    QHBoxLayout *axis_min_layout = new QHBoxLayout;
    QLabel *axis_min_label = new QLabel("Axis Min:");
    font = axis_min_label->font();
    font.setPointSize(9);
    axis_min_label->setFont(font);
    min_range_edit = new QLineEdit;
    min_range_edit->setValidator(range_validator);
    min_range_edit->setFixedSize(70, 30);
    min_range_edit->setText("0");
    connect(min_range_edit, &QLineEdit::textChanged, this, std::bind(&LevelMeterHistogramWindow::cellEdited, this, min_range_edit));
    axis_min_layout->addWidget(axis_min_label);
    axis_min_layout->addWidget(min_range_edit);

    //set button
    QPushButton *set_btn = new QPushButton("Set");
    set_btn->setFixedSize(50,65);
    connect(set_btn, &QPushButton::clicked, this, [=]{
        int min_value = min_range_edit->text().toInt();
        int max_value = max_range_edit->text().toInt();
        if (min_value >= max_value) {
            QMessageBox::warning(this, tr("Error"), tr("The min value is greater or equal than max value!!!"), QMessageBox::Ok);
            return;
        }
        customPlot->yAxis->setRange(min_value, max_value);
        customPlot->yAxis->setLabel(QString("pixel value (Range: %1 - %2)").arg(QString::number(min_value), QString::number(max_value)));
        customPlot->replot(QCustomPlot::rpImmediate);
    });

    control_v_layout->addLayout(axis_max_layout);
    control_v_layout->addLayout(axis_min_layout);
    control_layout->addStretch(1);
    control_layout->addLayout(control_v_layout);
    control_layout->addStretch(1);
    control_layout->addWidget(set_btn);
    control_layout->addStretch(1);

    //block layout
    main_v_layout->addLayout(rb_color_layout);
    main_v_layout->addLayout(gy_color_layout);
    main_v_layout->addStretch(1);
    main_v_layout->addLayout(control_layout);
    main_v_layout->addStretch(1);

    axis_control_box->setLayout(main_v_layout);

    return axis_control_box;
}

void LevelMeterHistogramWindow::cellEdited(QLineEdit *edit)
{
    int value = edit->text().toInt();

    if (value > 255) {
        edit->setText(QString::number(255));
    }
}

QTableWidget *LevelMeterHistogramWindow::createStatisticTable(QWidget *table_widget)
{
    QTableWidgetItem *header_item;
    QStringList header_row = {"R:", "G:", "B:", "Y:"};
    QStringList header_column = {"Max", "Min", "Avg"};
    QTableWidget *stat_data_table = new QTableWidget(header_row.size(), header_column.size());
    stat_data_table->setMaximumHeight(240);

    /*set table header ui*/
    stat_data_table->verticalHeader()->setMinimumWidth(70);
    stat_data_table->horizontalHeader()->setMinimumHeight(45);
    for (int i = 0; i < stat_data_table->rowCount(); i++) {
        header_item = new QTableWidgetItem(header_row.at(i));
        QFont font = header_item->font();
        font.setPointSize(11);
        header_item->setFont(font);
        header_item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        stat_data_table->setVerticalHeaderItem(i, header_item);
    }

    for (int i = 0; i < stat_data_table->columnCount(); i++) {
        header_item = new QTableWidgetItem(header_column.at(i));
        QFont font = header_item->font();
        font.setPointSize(11);
        header_item->setFont(font);
        header_item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        stat_data_table->setHorizontalHeaderItem(i, header_item);
    }

    QString style = "QHeaderView::section{"
                    "border-top:0px solid #E5E5E5;"
                    "border-left:0px solid #E5E5E5;"
                    "border-right:0.5px solid #E5E5E5;"
                    "border-bottom: 0.5px solid #E5E5E5;"
                    "background-color:white;"
                    "padding:4px;"
                "}";
    QString corner_style = "QTableCornerButton::section{"
            "border-top:0px solid #E5E5E5;"
            "border-left:0px solid #E5E5E5;"
            "border-right:0.5px solid #E5E5E5;"
            "border-bottom: 0.5px solid #E5E5E5;"
            "background-color:white;"
            "padding:4px;"
        "}";
    stat_data_table->horizontalHeader()->setStyleSheet(style);
    stat_data_table->verticalHeader()->setStyleSheet(style);
    stat_data_table->setStyleSheet(corner_style);

    /*create table content and format*/
    QColor color;
    QTableWidgetItem *item;
    for (int i = 0; i < header_row.size(); i++) {
        for (int j = 0; j < header_column.size(); j++) {
            if (i == 0) {
                color.setRgb(255,190,192);
            } else if (i == 1) {
                color.setRgb(192,255,192);
            } else if (i == 2) {
                color.setRgb(192,193,255);
            } else if (i == 3) {
                color.setRgb(255,255,192);
            }

            item = new QTableWidgetItem("", j);
            item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            item->setBackgroundColor(color);
            item->setFlags(item->flags() ^ Qt::ItemIsEditable);
            stat_data_table->setItem(i, j, item);
        }
    }

    return stat_data_table;
}

QWidget *LevelMeterHistogramWindow::createTableMenu()
{
    QWidget *table_widget = new QWidget;
    QGroupBox *axisControlBar;
    QHBoxLayout *horizontal_layout = new QHBoxLayout(table_widget);
    table_widget->setMinimumHeight(280);

    axisControlBar = createAxisControlBar(table_widget);
    stat_table = createStatisticTable(table_widget);

    horizontal_layout->addWidget(axisControlBar);
    horizontal_layout->addWidget(stat_table);

    return table_widget;
}

void LevelMeterHistogramWindow::changeEvent(QEvent *event)
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

void LevelMeterHistogramWindow::retranslateUi()
{
    setWindowTitle(tr("Level meter"));
}

void LevelMeterHistogramWindow::handleUpdatePointsInfo(const QPixmap *image, QRectF rect)
{
    if (image->isNull()) {
        return;
    }

    QMap<int, double> r_mean_value_map;
    QMap<int, double> g_mean_value_map;
    QMap<int, double> b_mean_value_map;
    QMap<int, double> y_mean_value_map;
    double r_max_value = 0.0, r_min_value = 0.0, r_avg_value = 0.0;
    double g_max_value = 0.0, g_min_value = 0.0, g_avg_value = 0.0;
    double b_max_value = 0.0, b_min_value = 0.0, b_avg_value = 0.0;
    double y_max_value = 0.0, y_min_value = 0.0, y_avg_value = 0.0;
    double r_total_value = 0.0, g_total_value = 0.0, b_total_value = 0.0, y_total_value = 0.0;

    int image_width = rect.width();
    int image_height = rect.height();
    int start_x = rect.x();
    int start_y = rect.y();
    double r_pixel_value = 0.0, g_pixel_value = 0.0, b_pixel_value = 0.0, y_pixel_value = 0.0;
    QImage img = image->toImage();
    int ori_img_width = img.width();
    int ori_img_height = img.height();
    // check rect size, and update
    if (start_x >= ori_img_width) {
        start_x = ori_img_width - 1;
    }

    if (start_y >= ori_img_height) {
        start_y = ori_img_height - 1;
    }

    if (start_x + image_width >= ori_img_width) {
        image_width = ori_img_width - 1 - start_x;
    }

    if (start_y + image_height >= ori_img_height) {
        image_height = ori_img_height - 1 - start_y;
    }

    r_mean_value_map.clear();
    g_mean_value_map.clear();
    b_mean_value_map.clear();
    y_mean_value_map.clear();

    for (int i = 0; i < image_width; i++) {
        for (int j = 0; j < image_height; j++) {
            QColor color = img.pixelColor(start_x + i, start_y + j);
            int y_pixel;
            r_pixel_value += color.red();
            g_pixel_value += color.green();
            b_pixel_value += color.blue();
            y_pixel = 0.299 * color.red() + 0.587 * color.green() + 0.114 * color.blue();
            y_pixel_value += y_pixel;
        }
        r_mean_value_map[i] = r_pixel_value / image_height;
        g_mean_value_map[i] = g_pixel_value / image_height;
        b_mean_value_map[i] = b_pixel_value / image_height;
        y_mean_value_map[i] = y_pixel_value / image_height;
        r_total_value += r_mean_value_map[i];
        g_total_value += g_mean_value_map[i];
        b_total_value += b_mean_value_map[i];
        y_total_value += y_mean_value_map[i];

        r_max_value = MAX(r_mean_value_map[i], r_max_value);
        g_max_value = MAX(g_mean_value_map[i], g_max_value);
        b_max_value = MAX(b_mean_value_map[i], b_max_value);
        y_max_value = MAX(y_mean_value_map[i], y_max_value);
        if (i == 0) {
            r_min_value = r_mean_value_map[i];
            g_min_value = g_mean_value_map[i];
            b_min_value = b_mean_value_map[i];
            y_min_value = y_mean_value_map[i];
        } else {
            r_min_value = MIN(r_mean_value_map[i], r_min_value);
            g_min_value = MIN(g_mean_value_map[i], g_min_value);
            b_min_value = MIN(b_mean_value_map[i], b_min_value);
            y_min_value = MIN(y_mean_value_map[i], y_min_value);
        }
        r_pixel_value = 0.0;
        g_pixel_value = 0.0;
        b_pixel_value = 0.0;
        y_pixel_value = 0.0;
    }

    r_avg_value = r_total_value / image_width;
    g_avg_value = g_total_value / image_width;
    b_avg_value = b_total_value / image_width;
    y_avg_value = y_total_value / image_width;

    statics_list.append(r_max_value);
    statics_list.append(r_min_value);
    statics_list.append(r_avg_value);

    statics_list.append(g_max_value);
    statics_list.append(g_min_value);
    statics_list.append(g_avg_value);

    statics_list.append(b_max_value);
    statics_list.append(b_min_value);
    statics_list.append(b_avg_value);

    statics_list.append(y_max_value);
    statics_list.append(y_min_value);
    statics_list.append(y_avg_value);

    QVector<double> column;
    QVector<double> r_value, g_value;
    QVector<double> b_value, y_value;

    for (int i = 0; i < image_width; i++) {
        column.append(i);
        r_value.append(r_mean_value_map[i]);
        g_value.append(g_mean_value_map[i]);
        b_value.append(b_mean_value_map[i]);
        y_value.append(y_mean_value_map[i]);
    }
    r_line->setData(column, r_value);
	g_line->setData(column, g_value);
	b_line->setData(column, b_value);
    y_line->setData(column, y_value);

    customPlot->xAxis->setRange(0, image_width - 1);
    customPlot->xAxis->setLabel(QString("pixel column (Max: %1)").arg(QString::number(image_width - 1)));
    customPlot->replot(QCustomPlot::rpImmediate);

    updateTabItemContents();
}

void LevelMeterHistogramWindow::updateTabItemContents()
{
    QTableWidgetItem *item;
    double value;
    QString str_value;
    int column_num = stat_table->columnCount();

    for (int i = 0; i < stat_table->rowCount(); i++) {
        for (int j = 0; j < column_num; j++) {
            item = stat_table->item(i, j);
            value = statics_list.at(i * column_num + j);
            str_value.setNum(value, 'f', 2);
            item->setText(str_value);
        }
    }
    statics_list.clear();
}

void LevelMeterHistogramWindow::handleSelectShowColor(bool checked_state)
{
    if (r_check_box->isChecked()) {
        r_line->setVisible(true);
    } else {
        r_line->setVisible(false);
    }

    if (g_check_box->isChecked()) {
        g_line->setVisible(true);
    } else {
        g_line->setVisible(false);
    }

    if (b_check_box->isChecked()) {
        b_line->setVisible(true);
    } else {
        b_line->setVisible(false);
    }

    if (y_check_box->isChecked()) {
        y_line->setVisible(true);
    } else {
        y_line->setVisible(false);
    }

    customPlot->replot(QCustomPlot::rpImmediate);
}

void LevelMeterHistogramWindow::keyReleaseEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape) {
        this->close();
    }
}

void LevelMeterHistogramWindow::closeEvent(QCloseEvent *event)
{
    event->accept();
    this->close();
    emit sigChildWindowEvent(0);
}

void LevelMeterHistogramWindow::closeWindow()
{
    this->close();
}
