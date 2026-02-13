#include "table_curve.hpp"
#include "utils.hpp"
#include "register_item_factory.hpp"
#include <QHeaderView>

#define BNR_BASE_LUT_NUM 6
#define BNR_LUMA_LUT_NUM 17

#ifdef M_PI
#undef M_PI
#endif
#define M_PI   3.14159265358979323846264338327950288

#ifndef ABS
#define ABS(a) (((a) < 0) ? -(a) : (a))
#endif


SimpleTableCurve::SimpleTableCurve(QString module, QString title, QWidget *parent)
{
    main_layout = new QHBoxLayout(this);
    this->setLayout(main_layout);
    this->title = title;
    this->module = module;
    this->pageWidget = parent;
}

void SimpleTableCurve::initUI()
{
    initLeftArea();
    initCenterArea();
    retranslateUi();
}

void SimpleTableCurve::initLeftArea()
{
}

void SimpleTableCurve::initCenterArea()
{
    initChart();

    QVBoxLayout *v_layout = new QVBoxLayout();
    QHBoxLayout *label_layout = new QHBoxLayout;
    pos_label = new QLabel("Out of chart");
    pos_label->setAlignment(Qt::AlignCenter);
    pos_label->setMargin(5);
    label_layout->addWidget(pos_label, Qt::AlignCenter);

    splitter = new QSplitter(Qt::Vertical, this);
    splitter->addWidget(univ_table);
    splitter->addWidget(simple_line_char);

    splitter->setStretchFactor(0, 5);
    splitter->setStretchFactor(1, 5);
    splitter->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    //splitter->setCollapsible(1, false);

    v_layout->addWidget(splitter);
    v_layout->addLayout(label_layout);
    main_layout->addLayout(v_layout);


    connect(simple_line_char, &SimpleLineChartView::sigUpdateMousePos, this, &SimpleTableCurve::updateMousePos);
}

void SimpleTableCurve::initChart()
{
    simple_line_char = new SimpleLineChartView();
    simple_line_char->setYRange(MIN_Y, MAX_Y);
    int grid_x = 1;
    if (is_multRow) {
        simple_line_char->setXRange(0, row - 1);
        grid_x = row - 1;
    } else {
        simple_line_char->setXRange(0, col - 1);
        grid_x = col - 1;
    }
    if (grid_x % 4 == 0) {
        grid_x = 4;
    } else if (grid_x % 5 == 0) {
        grid_x = 5;
    } else if (grid_x % 3 == 0) {
        grid_x = 3;
    } else {
        if (grid_x > 3)
            grid_x = 4;
    }
    if (MAX_Y < 8) {
        simple_line_char->setGridSize(grid_x, MAX_Y);
    } else {
        simple_line_char->setGridSize(grid_x, 8);
    }

    if ((module == "CCM") && (utils::isMatchCCMSumTitle(title))) {
        QStringList label_list = { "R", "G", "B" };
        simple_line_char->setGridSize(2, 8);
        simple_line_char->setAxisXLabelList(label_list);
    }

    slotRefreshChart();
    connect(simple_line_char, &SimpleLineChartView::sigValueChanged, this, &SimpleTableCurve::slotChartChange);
    connect(simple_line_char, &SimpleLineChartView::sigPointSelected, this, &SimpleTableCurve::slotPointSelected);
}

void SimpleTableCurve::slotSwitchChart()
{
    if (!simple_line_char) {
        return;
    }

    QList<int> newSizes;

    if (pos_label->isVisible()) {
        newSizes << splitter->height();
        newSizes << 0;
        pos_label->hide();
    } else {
        if (isMultRow()) {
            newSizes << splitter->height() / 2;
            newSizes << (splitter->height() - newSizes.at(0));
        } else {
            newSizes << splitter->height() / 3;
            newSizes << (splitter->height() - newSizes.at(0));
        }
        pos_label->show();
    }

    QTimer::singleShot(10, [=]() {
        splitter->setSizes(newSizes);
    });

}

void SimpleTableCurve::setUnivTable(UniversalTable *univ_table)
{
    if (!univ_table)
        return;

    this->univ_table = univ_table;
    connect(univ_table, &UniversalTable::sigValueChanged,
                this, &SimpleTableCurve::slotRefreshChart);
    connect(univ_table->horizontalHeader(), &QHeaderView::sectionClicked,
                this, &SimpleTableCurve::slotTableClicked);
}

void SimpleTableCurve::setAxis(double x0, double x1, double y0, double y1)
{
    this->MIN_X = x0;
    this->MAX_X = x1 - 1;
    this->MIN_Y = y0;
    this->MAX_Y = y1;
    //if (int(y1 + 1) % 4 == 0) {
    //    this->MAX_Y++;
    //}
}

void SimpleTableCurve::getAxis(double *x0, double *x1, double *y0, double *y1)
{
    *x0 = this->MIN_X;
    *x1 = this->MAX_X;
    *y0 = this->MIN_Y;
    *y1 = this->MAX_Y;
}

void SimpleTableCurve::setParam(QVariant param)
{
    double x0 = 0, x1 = 256, y0 = 0, y1 = 255;
    param_obj = param.toJsonObject();

    if (param_obj.contains("min") && param_obj.contains("max")) {
        QJsonValue min_val = param_obj["min"];
        QJsonValue max_val = param_obj["max"];

        if (min_val.isDouble() && max_val.isDouble()) {
            y0 = min_val.toInt();
            y1 = max_val.toInt();
        } else if(min_val.isArray() && max_val.isArray()) {
            QJsonArray min_array = min_val.toArray();
            QJsonArray max_array = max_val.toArray();
            if (min_array.count() == max_array.count()) {
                y0 = min_val[0].toInt();
                y1 = max_val[0].toInt();
            } else {
                 qDebug("[Error] UniversalTable::setRange --> Counts are not equal.");
            }
        } else {
            qDebug("[Error] UniversalTable::setRange --> Undefined data type.");
        }
    }

    if (param_obj.contains("count")) {
        QJsonValue count_val = param_obj["count"];
        if (count_val.isDouble()) {
            col = count_val.toInt();
        } else if (count_val.isArray()){
            row = count_val.toArray()[0].toInt();
            col = count_val.toArray()[1].toInt();
        }
        if ((module == "CCM") && (utils::isMatchCCMSumTitle(title))) {
            col--;
        }
        x1 = col;
    }
    if (row > 1) {
        is_multRow = true;
    }

    setAxis(x0, x1, y0, y1);
}

void SimpleTableCurve::retranslateUi()
{
}

void SimpleTableCurve::updateMousePos(double x, double y)
{
    if (x < 0 || x > MAX_X || y < MIN_Y || y > MAX_Y) {
        pos_label->setText("Out of Chart!");
    } else {
        pos_label->setText(QString("Position:X = %1, Y = %2").arg(int(x)).arg(int(y)));
    }
}

void SimpleTableCurve::updatePoints(QList<QPoint> points)
{
    simple_line_char->setPoints(points);
}


QList<QPoint> SimpleTableCurve::getDefaultPoints()
{
    QList<QPoint> points;
    int y = 0;

    for (int i = 0; i < MAX_X; i++) {
        y = i * MAX_Y / MAX_X;
        points.append(QPoint(i, y));
    }

    return points;
}

void SimpleTableCurve::refreshChart()
{
    QList<QPoint> points;
    QVariantList value_array = univ_table->getData("").toList();

    if (is_multRow) {
        for (int i = 0; i < row; i++) {
            int value = value_array[i * col + select_col].toInt();
            points.append(QPoint(i, value));
        }
    } else {
        for (int i = 0; i < col; i++) {
            int value = value_array[i].toInt();
            points.append(QPoint(i, value));
        }
    }

    updatePoints(points);
}

void SimpleTableCurve::setAxisXLabelList(QStringList label_list)
{
    simple_line_char->setAxisXLabelList(label_list);
}

void SimpleTableCurve::slotTableClicked()
{
    if (univ_table->currentColumn() < col) {
        select_col = univ_table->currentColumn();
        slotRefreshChart();
    }
}

void SimpleTableCurve::slotChartChange()
{
    emit sigValueChanged();

    if (is_multRow) {
        QVariantList data_table = univ_table->getData("").toList();
        QVariantList data_line = simple_line_char->getData().toList();
        for (int i =0; i < row; i++) {
            data_table[i * col + select_col] = data_line[i];
        }
        univ_table->setData("", data_table);
    } else {
        univ_table->setData("", simple_line_char->getData());
    }
}

void SimpleTableCurve::slotRefreshChart()
{
    refreshChart();
}

void SimpleTableCurve::slotPointSelected(int selected_point)
{
    if (selected_point < 0)
        return;
    if (!univ_table)
        return;
    if (is_multRow)
        return;

    univ_table->selectColumn(selected_point);
}

void SimpleTableCurve::slotUpdateLineCurve(double line_slope, double line_intercept)
{
    qDebug() << "line slope: " << line_slope << ", intercept: " << line_intercept;

    QVariantList data_list;
    double output_y = 0.0;

    for (int i = 0; i < this->col; ++i) {
        output_y = i * line_slope + line_intercept;
        output_y = qBound(MIN_Y, output_y, MAX_Y);
        // TODO: double to int
        data_list.append(static_cast<int>(output_y));
    }

    if (is_multRow) {
        QVariantList data = univ_table->getData("").toList();
        for (int i =0; i < row; i++) {
            data[i * col + select_col] = data_list[i];
        }
        univ_table->setData("", data);
    } else {
        univ_table->setData("", data_list);
    }
    slotRefreshChart();
}

void SimpleTableCurve::slotUpdateGaussianCurve(double gaussian_mean, double gaussian_std, double gaussian_gain)
{
    qDebug() << "gaussian mean: " << gaussian_mean << ", gaussian std: " << gaussian_std;

    QVariantList data_list;
    double output_y = 0.0;

    if (gaussian_std <= 1e-8) {
        gaussian_std = 1e-8;
    }

    for (int i = 0; i < this->col; ++i) {
        double coefficient = 1.0 / (gaussian_std * sqrt(2 * M_PI));
        double exponent = -pow(i - gaussian_mean, 2) / (2 * pow(gaussian_std, 2));
        output_y = MAX_Y * coefficient * exp(exponent);
        output_y *= gaussian_gain;
        output_y = qBound(MIN_Y, output_y, MAX_Y);
        data_list.append(static_cast<int>(output_y));
    }

    if (is_multRow) {
        QVariantList data = univ_table->getData("").toList();
        for (int i =0; i < row; i++) {
            data[i * col + select_col] = data_list[i];
        }
        univ_table->setData("", data);
    } else {
        univ_table->setData("", data_list);
    }
    slotRefreshChart();
}

void SimpleTableCurve::slotUpdateGammaCurve(double gamma)
{
    qDebug() << "gamma: " << gamma;

    QVariantList data_list;
    double output_y = 0.0;

    if (gamma <= 1e-8 && gamma >= -1e-8) {
        gamma = 1e-8;
    }

    for (int i = 0; i < this->col; ++i) {
        output_y = pow(((double)i / MAX_X), 1.0f / gamma);
        output_y = MAX_Y * output_y;
        output_y = qBound(MIN_Y, output_y, MAX_Y);
        data_list.append(static_cast<int>(output_y));
    }

    if (is_multRow) {
        QVariantList data = univ_table->getData("").toList();
        for (int i =0; i < row; i++) {
            data[i * col + select_col] = data_list[i];
        }
        univ_table->setData("", data);
    } else {
        univ_table->setData("", data_list);
    }
    slotRefreshChart();
}


MethodTableCurve::MethodTableCurve(QString module, QString title, QWidget *parent) :
    SimpleTableCurve(module, title, parent)
{
}

void MethodTableCurve::initUI()
{
    initLeftArea();
    initCenterArea();
    retranslateUi();
}

void MethodTableCurve::initCenterArea()
{
    initChart();

    QVBoxLayout *v_layout = new QVBoxLayout();
    QHBoxLayout *label_layout = new QHBoxLayout;

    if (curve_method == "gaussian_curve") {
        param_adjust_spinner = new SpinnerItemWidget(module, "Sigma");
        param_adjust_spinner->setData(10);
        param_adjust_spinner->setRange(0, 511);
        param_adjust_spinner->setLabelWidth(80, true);
    } else if (curve_method == "sqrt_curve") {
        param_adjust_spinner = new SpinnerItemWidget(module, "Alpha");
        param_adjust_spinner->setData(5);
        param_adjust_spinner->setRange(0, 1033);
        param_adjust_spinner->setLabelWidth(80, true);
    }
    param_adjust_spinner->setMaximumHeight(45);
    label_layout->addWidget(param_adjust_spinner, 3, Qt::AlignCenter);
    connect(param_adjust_spinner, &SpinnerItemWidget::sigValueChanged, this, &MethodTableCurve::slotCurveParamChanged);

    pos_label = new QLabel("Out of chart");
    pos_label->setAlignment(Qt::AlignCenter);
    pos_label->setMargin(5);
    label_layout->addWidget(pos_label, 3, Qt::AlignCenter);

    reset_button = new QPushButton();
    connect(reset_button, &QPushButton::clicked, this, &MethodTableCurve::resetCurve);
    label_layout->addWidget(reset_button, 3, Qt::AlignCenter);

    splitter = new QSplitter(Qt::Vertical, this);
    splitter->addWidget(univ_table);
    splitter->addWidget(simple_line_char);

    splitter->setStretchFactor(0, 5);
    splitter->setStretchFactor(1, 5);
    splitter->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    //splitter->setCollapsible(1, false);

    v_layout->addWidget(splitter, 4);
    v_layout->addLayout(label_layout);
    main_layout->addLayout(v_layout);

    connect(simple_line_char, &SimpleLineChartView::sigUpdateMousePos, this, &MethodTableCurve::updateMousePos);
}

void MethodTableCurve::slotSwitchChart()
{
    if (!simple_line_char) {
        return;
    }

    QList<int> newSizes;

    if (pos_label->isVisible()) {
        newSizes << splitter->height();
        newSizes << 0;
        pos_label->hide();
        reset_button->hide();
        param_adjust_spinner->hide();
    } else {
        newSizes << splitter->height() / 2;
        newSizes << (splitter->height() - newSizes.at(0));
        pos_label->show();
        reset_button->show();
        param_adjust_spinner->show();
    }

    QTimer::singleShot(10, [=]() {
        splitter->setSizes(newSizes);
    });

}

void MethodTableCurve::retranslateUi()
{
    reset_button->setText(tr("Reset Curve"));
}

void MethodTableCurve::setParam(QVariant param)
{
    SimpleTableCurve::setParam(param);
    curve_method = param_obj["curve_method"].toString();
}

double MethodTableCurve::gaussian(double x, double stddev)
{
    double exponent = -(pow(x, 2) / (2 * pow(stddev, 2)));

    return 31 * exp(exponent);
}

int MethodTableCurve::findNearestX(double target, double stddev)
{
    double minDiff = 1e-2;
    double lastminDiff = -1.0;
    int nearestX = 0;

    for (int x = 0; x < 1024; x++) {
        double y = gaussian(x, stddev);
        double diff = ABS(y - target);

        if (lastminDiff < diff && lastminDiff > 0) {
            minDiff = diff;
            nearestX = x;
            break;
        }

        lastminDiff = diff;
        if (diff < minDiff) {
            minDiff = diff;
            nearestX = x;
            break;
        }
    }

    return nearestX;
}

void MethodTableCurve::generate_gaussian_curve(double sigma, int *nearestX)
{
    double targetValues[BNR_BASE_LUT_NUM] = { 31, 16, 8, 4, 2, 0 };

    for (int i = 0; i < BNR_BASE_LUT_NUM; i++) {
        nearestX[i] = findNearestX(targetValues[i], sigma);
    }
}

void MethodTableCurve::generate_sqrt_curve(double alpha, double beta, int *delta)
{
    for (int i = 0; i < BNR_LUMA_LUT_NUM; i++) {
        int x = i * 16;
        double y = sqrt(alpha * x + beta);

        delta[i] = roundf(y);
    }
}

void MethodTableCurve::generate_curve()
{
    QVariantList data_list;
    if (curve_method == "gaussian_curve") {
        double sigma = param_adjust_spinner->getData().toDouble() / 2.0;
        int nearestX[BNR_BASE_LUT_NUM] = { 0, 1, 2, 3, 4, 5 };
        generate_gaussian_curve(sigma, nearestX);
        for (int i : nearestX) {
            data_list.append(QVariant(i));
        }
    } else if (curve_method == "sqrt_curve") {
        double alpha = param_adjust_spinner->getData().toDouble();
        int luma_delta[BNR_LUMA_LUT_NUM] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 };
        alpha = alpha > 10 ? alpha - 10 : alpha / 10.0;
        generate_sqrt_curve(alpha, 0, luma_delta);
        for (int i : luma_delta) {
            data_list.append(QVariant(i));
        }
    }

    if (is_multRow) {
        QVariantList data = univ_table->getData("").toList();
        for (int i =0; i < row; i++) {
            data[i * col + select_col] = data_list[i];
        }
        univ_table->setData("", data);
    } else {
        univ_table->setData("", data_list);
    }
}

void MethodTableCurve::resetCurve()
{
    generate_curve();
    slotRefreshChart();
}

void MethodTableCurve::slotCurveParamChanged(QVariant)
{
    resetCurve();
}
