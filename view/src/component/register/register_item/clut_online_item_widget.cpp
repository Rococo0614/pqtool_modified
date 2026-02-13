#include "clut_online_item_widget.hpp"
#include "global_data.hpp"
#include "main_window.hpp"
#include "utils.hpp"
#include <QMessageBox>
#include <QToolTip>
#include <QPainter>


GradientWidget::GradientWidget(Gradient_Band_Type type, QWidget *parent) :
   QWidget(parent), g_type(type)
{
    ajust_hue = 0;
    ajust_sat = 50;
    ajust_lum = 50;
    clearColors();
    initDefaultColors();
}

void GradientWidget::initDefaultColors()
{
    int steps = getMaxValue();

    for (int i = 0; i <= steps; i++) {
        int hue = ajust_hue;
        int sat = qCeil(ajust_sat * 255 /100);
        int lum = qCeil(ajust_lum * 255 /100);
        int pos = qBound(0, i, steps);

        if (g_type == HUE) {
            hue = i % steps;
        } else if (g_type == SAT) {
            sat = i;
        } else if (g_type == LUM) {
            lum = i;
        }
        QColor hsl_color = QColor::fromHsl(hue, sat, lum);
        setColorAt(pos, hsl_color);
    }
}

void GradientWidget::updateColorPoints()
{
    for (auto it = color_points.begin(); it != color_points.end(); it++) {
        QColor color = it.value();
        int h, s, l;
        color.getHsl(&h, &s, &l);

        int hue = ajust_hue;
        int sat = qCeil(ajust_sat * 255 /100);
        int lum = qCeil(ajust_lum * 255 /100);

        if (g_type == HUE) {
            hue = h % 360;
        } else if (g_type == SAT) {
            sat = s;
        } else if (g_type == LUM) {
            lum = l;
        }
        color = QColor::fromHsl(hue, sat, lum);
        *it = color;
    }
}

void GradientWidget::setColorAt(int position, QColor color)
{
    int steps = getMaxValue();
    color_points[qBound(0, position, steps)] = color;
}

void GradientWidget::clearColors()
{
    color_points.clear();
}

void GradientWidget::updateGradient()
{
    update();
}

void GradientWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    gradient = QLinearGradient(0, 0, width(), 0);
    int steps = getMaxValue();

    for (auto it = color_points.begin(); it != color_points.end(); it++) {
        gradient.setColorAt(qreal(it.key()) / steps, it.value());
    }

    painter.fillRect(rect(), gradient);
}

int GradientWidget::getMaxValue()
{
    switch (g_type) {
        case HUE: return 360;
        case SAT: return 255;
        case LUM: return 255;
        default: return 0;
    }
}

void GradientWidget::mousePressEvent(QMouseEvent *event)
{
    QPoint pos = event->pos();
    int h, s, l;
    int r, g, b;

    QPixmap pixmap = grab(QRect(pos, QSize(1, 1)));
    QImage image = pixmap.toImage();
    QColor color = QColor(image.pixel(0, 0));

    color.getHsl(&h, &s, &l);
    color.getRgb(&r, &g, &b);

    QString hsl_str = QString("HSL(%1, %2, %3), ").arg(h).arg(s).arg(l);
    QString rgb_str = QString("RGB(%1, %2, %3)").arg(r).arg(g).arg(b);

    QToolTip::showText(event->globalPos() - QPoint(100, 40), hsl_str + rgb_str, this);
}


CLUTCurveWidget::CLUTCurveWidget(Gradient_Band_Type g_type, Gradient_Band_Type a_type, QWidget *parent) :
    GradientWidget(g_type, parent), a_type(a_type)
{
    setMouseTracking(true);
    setMinimumSize(800, 200);

    clearColors();
    initDefaultColors();
}

void CLUTCurveWidget::clearColors()
{
    color_points.clear();
    color_points_ref.clear();
}

void CLUTCurveWidget::initDefaultColors()
{
    int steps = getMaxValue();

    for (int i = 0; i <= steps; i++) {
        int hue = ajust_hue;
        int sat = qCeil(ajust_sat * 255 /100);
        int lum = qCeil(ajust_lum * 255 /100);
        int pos = qBound(0, i, steps);

        if (g_type == HUE) {
            hue = i % steps;
        } else if (g_type == SAT) {
            sat = i;
        } else if (g_type == LUM) {
            lum = i;
        }
        QColor hsl_color = QColor::fromHsl(hue, sat, lum);
        setColorAt(pos, hsl_color);
        color_points_ref[pos] = hsl_color;
    }
}

void CLUTCurveWidget::updateColorPointsRef()
{
    for (auto it = color_points_ref.begin(); it != color_points_ref.end(); it++) {
        QColor color = it.value();
        int h, s, l;
        color.getHsl(&h, &s, &l);

        int hue = ajust_hue;
        int sat = qCeil(ajust_sat * 255 /100);
        int lum = qCeil(ajust_lum * 255 /100);

        if (g_type == HUE) {
            hue = h % 360;
        } else if (g_type == SAT) {
            sat = s;
        } else if (g_type == LUM) {
            lum = l;
        }
        color = QColor::fromHsl(hue, sat, lum);
        *it = color;
    }
}

void CLUTCurveWidget::setPoints(const QVector<QPoint>& points)
{
    this->coord_points.clear();
    this->coord_points += points;
    update();
}

void CLUTCurveWidget::setXRange(qreal min, qreal max)
{
    xMin = min;
    xMax = max;
    update();
}

void CLUTCurveWidget::setYRange(qreal min, qreal max)
{
    yMin = min;
    yMax = max;
    update();
}

void CLUTCurveWidget::setGridSize(int xSize, int ySize)
{
   grid_size_x = xSize;
   grid_size_y = ySize;
   update();
}

QRect CLUTCurveWidget::getPlotArea() const
{
    return QRect(MARGIN_LEFT,
                        MARGIN_TOP,
                        width() - MARGIN_LEFT - MARGIN_RIGHT,
                        height() - MARGIN_TOP - MARGIN_BOTTOM);
}

void CLUTCurveWidget::interpolateGradientColorAtPos(int pos)
{
    int point_nums = coord_points.size();
    int x_max = 255;

    if (g_type == HUE) {
        x_max = 360;
    }
    double stride = (double)x_max / (point_nums - 1);
    quint16 idx0 = pos / stride;
    quint16 idx1;

    if  (idx0 >= point_nums - 1) {
        idx1 = idx0 = point_nums - 1;
    } else {
        idx1 = idx0 + 1;
    }

    if (idx0 == idx1) {
        if (a_type == HUE) {
            angle = coord_points[idx0].y();
        } else if (a_type == SAT) {
            s_scale = coord_points[idx0].y();
        } else if (a_type == LUM) {
            l_scale = coord_points[idx0].y();
        }
    } else {
        double x = pos;
        double x0 = idx0 * stride;
        double y0 = coord_points[idx0].y();
        double y1 = coord_points[idx1].y();

        if (a_type == HUE) {
            angle = ((y1 - y0) * (x - x0) / stride + y0) + 0.5;
        } else if (a_type == SAT) {
            s_scale = (y1 - y0) * (x - x0) / stride + y0 + 0.5;
        } else if (a_type == LUM) {
            l_scale = (y1 - y0) * (x - x0) / stride + y0 + 0.5;
        }
    }
}

void CLUTCurveWidget::reColorGradientAtPos(int pos)
{
    int steps = getMaxValue();
    int h, s, l;
    QColor color_point = color_points[qBound(0, pos, steps)];

    color_point.getHsl(&h, &s, &l);
    interpolateGradientColorAtPos(pos);

    h += angle;
    s = s * s_scale / SCALE_UNIT;
    l = l * l_scale / SCALE_UNIT;

    h = qBound(0, h, 360) % 360;
    s = qBound(0, s, 255);
    l = qBound(0, l, 255);

    color_points[qBound(0, pos, steps)] = QColor::fromHsl(h, s, l);
}

void CLUTCurveWidget::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.fillRect(rect(), Qt::white);

    int steps = getMaxValue();
    QLinearGradient gradient = QLinearGradient(MARGIN_LEFT, 0, getPlotArea().width() + MARGIN_LEFT, 0);

    GradientWidget::initDefaultColors();  // use update maybe overlay history data
    for (auto it = color_points.begin(); it != color_points.end(); it++) {
        reColorGradientAtPos(it.key());
        gradient.setColorAt(qreal(it.key()) / steps, it.value());
    }
    painter.fillRect(getPlotArea(), gradient);

    QLinearGradient gradient_ref = QLinearGradient(MARGIN_LEFT, 0, getPlotArea().width() + MARGIN_LEFT, 0);
    QRect plot_area_ref = QRect(MARGIN_LEFT,
                                                10,
                                                width() - MARGIN_LEFT - MARGIN_RIGHT,
                                                60);

    for (auto it = color_points_ref.begin(); it != color_points_ref.end(); it++) {
        gradient_ref.setColorAt(qreal(it.key()) / steps, it.value());
    }
    painter.fillRect(plot_area_ref, gradient_ref);

    painter.setFont(QFont("Arial", 9));
    painter.setPen(QPen(Qt::darkGray, 1));
    QString text = QString("ref:");
    QFontMetrics fm(painter.font());
    int textWidth = fm.horizontalAdvance(text);
    painter.drawText(plot_area_ref.left() - textWidth - 10, (plot_area_ref.bottom() + fm.height())/2, text);

    if (is_show_grid) {
        drawGrid(painter);
    }

    drawAxes(painter);
    drawCurve(painter);
    drawPoints(painter);
}

void CLUTCurveWidget::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) {
        selected_point = findNearestPoint(event->pos());
    }
    update();

    if (selected_point == -1){
        QPoint pos = event->pos();
        int h, s, l;
        int r, g, b;

        QPixmap pixmap = grab(QRect(pos, QSize(1, 1)));
        QImage image = pixmap.toImage();
        QColor color = QColor(image.pixel(0, 0));

        color.getHsl(&h, &s, &l);
        color.getRgb(&r, &g, &b);

        s = s *100 / 255;
        l = l * 100 / 255;

        QString hsl_str = QString("HSL(%1, %2, %3), ").arg(h).arg(s).arg(l);
        QString rgb_str = QString("RGB(%1, %2, %3)").arg(r).arg(g).arg(b);

        QToolTip::showText(event->globalPos() - QPoint(100, 40), hsl_str + rgb_str, this);
    }
}

void CLUTCurveWidget::mouseMoveEvent(QMouseEvent* event)
{
    QPoint screenPoint = fromScreen(event->pos());

    if (selected_point >= 0 && selected_point < coord_points.size()) {

        QPoint currentPoint = coord_points[selected_point];

        qreal newY = qBound((int)yMin, screenPoint.y(), (int)yMax);

        if (currentPoint.y() != newY) {
            coord_points[selected_point].setY(newY);
            emit pointYChanged(selected_point, newY);
            emit curveChanged(coord_points);
            update();
        }
    }
    double pos_x = screenPoint.x();
    double pos_y = screenPoint.y();
    emit sigUpdateMousePos(pos_x, pos_y);
}

void CLUTCurveWidget::mouseReleaseEvent(QMouseEvent*)
{
    selected_point = -1;
}

QPoint CLUTCurveWidget::toScreen(const QPoint& point) const
{
    QRect plotArea = getPlotArea();

    int x = plotArea.left() + qRound((point.x() - xMin) * plotArea.width() / (xMax - xMin));
    int y = plotArea.bottom() - qRound((point.y() - yMin) * plotArea.height() / (yMax - yMin));

    return QPoint(x, y);
}

QPoint CLUTCurveWidget::fromScreen(const QPoint& point) const
{
    QRect plotArea = getPlotArea();

    qreal x = selected_point >= 0 ?
                coord_points[selected_point].x() :
                xMin + ((point.x() - plotArea.left()) * (xMax -xMin)) / plotArea.width();
    qreal y = yMin + ((plotArea.bottom() - point.y()) * (yMax - yMin)) / plotArea.height();

    return QPoint(x, y);
}

int CLUTCurveWidget::findNearestPoint(const QPoint& pos, int maxDistance) const
{
    int nearest = -1;
    int minDist = maxDistance + 1;

    for (int i = 0; i < coord_points.size(); ++i) {
        QPoint screenPos = toScreen(coord_points[i]);
        int dist = QLineF(pos, screenPos).length();
        if (dist < minDist) {
            minDist = dist;
            nearest = i;
        }
    }
    return nearest;
}

void CLUTCurveWidget::drawGrid(QPainter& painter)
{
    QRect plotArea = getPlotArea();

    painter.setPen(QPen(QColor(200, 200, 200), 1, Qt::DotLine));

    for (int i = 1; i <=grid_size_x; ++i) {
        qreal xValue =xMin + (i * (xMax -xMin) /grid_size_x);
        int x = plotArea.left() + qRound((xValue -xMin) * plotArea.width() / (xMax -xMin));
        painter.drawLine(x, plotArea.top(), x, plotArea.bottom());
    }

    for (int i = 1; i <= grid_size_y; ++i) {
        qreal yValue =yMin + (i * (yMax -yMin) /grid_size_y);
        int y = plotArea.bottom() - qRound((yValue -yMin) * plotArea.height() / (yMax -yMin));
        painter.drawLine(plotArea.left(), y, plotArea.right(), y);
    }
}

void CLUTCurveWidget::drawAxes(QPainter& painter)
{
    QRect plotArea = getPlotArea();

    painter.setPen(QPen(Qt::darkGray, 1));

    // X Axes
    // int zeroY = plotArea.bottom() - qRound((0 - yMin) * plotArea.height() / (yMax - yMin));
    int zeroY = plotArea.bottom();
    painter.drawLine(plotArea.left(), zeroY, plotArea.right(), zeroY);

    // Y Axes
    painter.drawLine(plotArea.left(), plotArea.top(), plotArea.left(), plotArea.bottom());

    // X Axes Scale
    painter.setFont(QFont("Arial", 8));
    for (int i = 0; i <=grid_size_x; ++i) {
        qreal xValue =xMin + (i * (xMax -xMin) /grid_size_x);
        int x = plotArea.left() + qRound((xValue -xMin) * plotArea.width() / (xMax -xMin));

        painter.drawLine(x, zeroY, x, zeroY + 5);

        QString text = QString::number(xValue, 'f', 0);
        QFontMetrics fm(painter.font());
        int textWidth = fm.horizontalAdvance(text);
        painter.drawText(x - textWidth/2, zeroY + 20, text);
    }

    // Y Axes Scale
    for (int i = 0; i <=grid_size_y; ++i) {
        qreal yValue =yMin + (i * (yMax -yMin) /grid_size_y);
        int y = plotArea.bottom() - qRound((yValue -yMin) * plotArea.height() / (yMax -yMin));

        painter.drawLine(plotArea.left() - 5, y, plotArea.left(), y);

        QString text = QString::number(yValue, 'f', 0);
        QFontMetrics fm(painter.font());
        int textWidth = fm.horizontalAdvance(text);
        painter.drawText(plotArea.left() - textWidth - 10, y + fm.height()/2 - 2, text);
    }
}

void CLUTCurveWidget::drawCurve(QPainter& painter)
{
    if (coord_points.size() < 2) return;

    painter.setPen(QPen(Qt::blue, 2));

    QPoint prevPoint = toScreen(coord_points[0]);
    for (int i = 1; i < coord_points.size(); ++i) {
        QPoint currentPoint = toScreen(coord_points[i]);
        painter.drawLine(prevPoint, currentPoint);
        prevPoint = currentPoint;
    }
}

void CLUTCurveWidget::drawPoints(QPainter& painter)
{
    for (int i = 0; i < coord_points.size(); ++i) {
        QPoint screenPos = toScreen(coord_points[i]);

        if (i == selected_point) {
            painter.setPen(QPen(Qt::red, 2));
            painter.setBrush(Qt::red);
        } else {
            painter.setPen(QPen(Qt::blue, 2));
            painter.setBrush(Qt::white);
        }

        painter.drawEllipse(screenPos, 5, 5);

        // Display Y value
        if (display_value_y) {
            QString valueText = QString::number(coord_points[i].y(), 'f', 1);
            QFontMetrics fm(painter.font());
            int textWidth = fm.horizontalAdvance(valueText);
            int textX = screenPos.x() - textWidth / 2;
            int textY = screenPos.y() - 10;

            painter.drawText(textX, textY, valueText);
        }
    }
}

QVariant CLUTCurveWidget::getData() const
{
    QVariantList data;
    if (coord_points.size() <= 0) return data;

    int data_x_pre = coord_points[0].x();

    for (QPoint point : coord_points) {
        int data_x = point.x();
        if (data_x < data_x_pre) {
            data.clear();
            break;
        } else {
            data_x_pre = data_x;
        }
        data.append(point.y());
    }

    return data;
}

void CLUTCurveWidget::setHSLInColorPoints(int hsl, Gradient_Band_Type band_type)
{

    if (band_type == HUE) {
        ajust_hue = hsl % 360;
    } else if (band_type == SAT) {
        ajust_sat = hsl;
    } else if (band_type == LUM) {
        ajust_lum = hsl;
    }

    // only for color ref points
    updateColorPointsRef();

    update();
}

void CLUTCurveWidget::resetPoints(int y)
{
    for (auto it = coord_points.begin(); it != coord_points.end(); it++) {
        it->setY(y);
    }
    update();
    emit curveChanged(coord_points);
}

void CLUTCurveWidget::loadJsonFile(QString)
{

}

void CLUTCurveWidget::saveJsonFile(QString)
{

}

void CLUTCurveWidget::loadTxtFile(QString)
{

}

void CLUTCurveWidget::saveTxtFile(QString)
{

}


CLUTOnlineItemWidget::CLUTOnlineItemWidget(QString module, QString title,
                                           UniversalMatrixItemWidget *hsl_table, QWidget *parent) :
    BaseItemWidget(module, title, parent), hsl_table(hsl_table)
{
    this->value_type = TYPE_ARRAY;
    layout->removeWidget(icon_widget);
    layout->removeWidget(title_label);
    layout->removeWidget(value_widget);

    online_type = getOnlineTypeByStr(title);
    band_type = getGradientBandType(online_type);
    band_type_ajust = getAjustType(online_type);

    initGroupWidget();
    retranslateUi();
}

void CLUTOnlineItemWidget::initGroupWidget()
{
    initChart();

    SpinnerItemWidget *hue_spinner = initSpinner();
    pos_label = new QLabel("Out of chart");
    pos_label->setFixedWidth(300);
    pos_label->setFixedHeight(40);
    pos_label->setMargin(5);
    pos_label->setAlignment(Qt::AlignCenter);

    QHBoxLayout *label_layout = new QHBoxLayout;
    label_layout->addWidget(pos_label, Qt::AlignCenter);

    QLayout * button_layout = initButtons();
    QHBoxLayout *opt_layout = new QHBoxLayout();
    opt_layout->addWidget(hue_spinner);
    opt_layout->addLayout(label_layout);
    opt_layout->addLayout(button_layout);

    GroupItemWidget *group_widget = new GroupItemWidget(title);
    group_widget->addWidget(line_chart_widget);
    group_widget->addLayout(opt_layout);
    layout->addWidget(group_widget);

    file_select_dialog = new FileDialog;

    connect(line_chart_widget, &CLUTCurveWidget::sigUpdateMousePos, this, &CLUTOnlineItemWidget::updateMousePos);
}

void CLUTOnlineItemWidget::initChart()
{
    line_chart_widget = new CLUTCurveWidget(band_type, band_type_ajust);
    if (line_chart_widget != nullptr) {
        int x_min = 0;
        int x_max = getHSLPointMaxX();
        int y_min = 0;
        int y_max = getHSLPointMaxY();
        int y_grid = 5;
        if (band_type_ajust == HUE) {
            y_min = -30;
            y_grid = 6;
        }
        line_chart_widget->setXRange(x_min, x_max);
        line_chart_widget->setYRange(y_min, y_max);
        line_chart_widget->setGridSize(4, y_grid);
    }
    line_chart_widget->setPoints(getHSLPoints());
    line_chart_widget->setMinimumSize(1400, 300);
    connect(line_chart_widget, &CLUTCurveWidget::curveChanged, this, &CLUTOnlineItemWidget::slotRefreshTable);
    connect(hsl_table, &UniversalMatrixItemWidget::sigTableValueChanged, this, &CLUTOnlineItemWidget::slotUpdateCurve);
}

SpinnerItemWidget *CLUTOnlineItemWidget::initSpinner()
{
    if (line_chart_widget == nullptr)
        return nullptr;

    QString hsl_tittle;
    int data_default = 0;
    int data_max = 0;

    switch (online_type) {
    case HBYH:
    case SBYH:
    case SBYL:
        hsl_tittle = QString("Sat");
        data_default = line_chart_widget->getAjustSat();
        data_max = 100;
        band_type_spinner = SAT;
        break;
    case LBYH:
    case LBYS:
        hsl_tittle = QString("Lum");
        data_default = line_chart_widget->getAjustLum();
        data_max = 100;
        band_type_spinner = LUM;
        break;
    case HBYS:
    case HBYL:
    case SBYS:
    case LBYL:
        hsl_tittle = QString("Hue");
        data_default = line_chart_widget->getAjustHue();
        data_max = 360;
        band_type_spinner = HUE;
        break;
    default:
        break;
    }

    SpinnerItemWidget *hsl_spinner = new SpinnerItemWidget(module, hsl_tittle);
    hsl_spinner->setData(data_default);
    hsl_spinner->setRange(0, data_max);
    hsl_spinner->setLabelWidth(100);
    hsl_spinner->setFixedHeight(50);
    hsl_spinner->setFixedWidth(400);
    connect(hsl_spinner, &SpinnerItemWidget::sigValueChanged, this, &CLUTOnlineItemWidget::hueSpinnerChanged);
    return hsl_spinner;
}

void CLUTOnlineItemWidget::slotUpdateCurve(QVariant)
{
    if (line_chart_widget != nullptr) {
        line_chart_widget->setPoints(getHSLPoints());
        line_chart_widget->updateCurve();
    }
}

void CLUTOnlineItemWidget::slotRefreshTable(const QVector<QPoint>& points)
{
    QVariantList data;

    if (points.size() != getHSLPointNums()) return;

    disconnect(hsl_table, &UniversalMatrixItemWidget::sigTableValueChanged, this, &CLUTOnlineItemWidget::slotUpdateCurve);

    int data_x_pre = points[0].x();

    for (QPoint point : points) {
        int data_x = point.x();
        if (data_x < data_x_pre) {
            data.clear();
            QString message = module + QString(" Page: set data to table failed!");
            emit MainWindow::getInstance()->sigStatusLog(message, LOG_LEVEL::LOG_ERROR);
            return;
        } else {
            data_x_pre = data_x;
        }
        data.append(point.y());
    }

    hsl_table->setData(data, true);
    connect(hsl_table, &UniversalMatrixItemWidget::sigTableValueChanged, this, &CLUTOnlineItemWidget::slotUpdateCurve);
}

QVector<QPoint> CLUTOnlineItemWidget::getHSLPoints()
{
    QVector<QPoint> points;
    QVariant data = hsl_table->getData();
    QVariantList data_array = data.toList();

    for (int i = 0; i < getHSLPointNums(); i++) {
        int axis_x = i * getHSLPointMaxX() / (getHSLPointNums() - 1);

        points.append(QPoint(axis_x,  data_array[i].toInt()));
    }
    return points;
}

QLayout *CLUTOnlineItemWidget::initButtons()
{
    QHBoxLayout *button_layout = new QHBoxLayout();
    load_button = new QPushButton();
    save_button = new QPushButton();
    reset_button = new QPushButton();
    load_button->setFixedHeight(40);
    save_button->setFixedHeight(40);
    reset_button->setFixedHeight(40);

    connect(load_button, &QPushButton::clicked, this, &CLUTOnlineItemWidget::openLoadDialog);
    connect(save_button, &QPushButton::clicked, this, &CLUTOnlineItemWidget::openSaveDialog);
    connect(reset_button, &QPushButton::clicked, this, &CLUTOnlineItemWidget::handleResetCurve);

    button_layout->setAlignment(Qt::AlignCenter);
    button_layout->setSpacing(10);
    button_layout->addWidget(reset_button);
    button_layout->addWidget(load_button);
    button_layout->addWidget(save_button);

    return button_layout;
}

void CLUTOnlineItemWidget::slotRefreshChart()
{
    if (line_chart_widget != nullptr) {
        line_chart_widget->setPoints(getHSLPoints());
        line_chart_widget->updateCurve();
    }
}

void CLUTOnlineItemWidget::openLoadDialog()
{
    file_select_dialog->setMode(MODE_LOAD);
    file_select_dialog->show();

    connect(file_select_dialog, &FileDialog::sigSelectFile, this, &CLUTOnlineItemWidget::handleSelectFile);
}

void CLUTOnlineItemWidget::openSaveDialog()
{
    file_select_dialog->setMode(MODE_SAVE);
    file_select_dialog->show();

    connect(file_select_dialog, &FileDialog::sigSelectFile, this, &CLUTOnlineItemWidget::handleSelectFile);
}

void CLUTOnlineItemWidget::handleResetCurve()
{
    int value_y = 0;
    if (band_type_ajust == HUE) {
        value_y = 0;
    } else if (band_type_ajust == SAT) {
        value_y = 50;
    } else if (band_type_ajust == LUM) {
        value_y = 50;
    }
    if (line_chart_widget)
        line_chart_widget->resetPoints(value_y);
}

void CLUTOnlineItemWidget::handleSelectFile(QString format, QString file_path, MODE mode)
{
    if (format == CVITEK_FORMAT_STR) {
        if (mode == MODE_LOAD) {
            line_chart_widget->loadJsonFile(file_path);
        } else {
            line_chart_widget->saveJsonFile(file_path);
        }
    } else {
        if (mode == MODE_LOAD) {
            line_chart_widget->loadTxtFile(file_path);
        } else {
            line_chart_widget->saveTxtFile(file_path);
        }
    }

    disconnect(file_select_dialog, &FileDialog::sigSelectFile, this, &CLUTOnlineItemWidget::handleSelectFile);
    file_select_dialog->close();
}

void CLUTOnlineItemWidget::hueSpinnerChanged(QVariant v)
{
    int x = v.toInt();
    if (line_chart_widget){
        line_chart_widget->setHSLInColorPoints(x, band_type_spinner);
    }
}

void CLUTOnlineItemWidget::paintEvent(QPaintEvent *)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}


void CLUTOnlineItemWidget::updateMousePos(double x, double y)
{
    int x_min = 0;
    int x_max = getHSLPointMaxX();
    int y_min = 0;
    int y_max = getHSLPointMaxY();

    if (online_type == HBYH || online_type == HBYS || online_type == HBYL)
        y_min = -30;

    if (x < x_min || x > x_max || y < y_min || y > y_max) {
        pos_label->setText("Out of Chart!");
    } else {
        pos_label->setText(QString("Position:X = %1, Y = %2").arg(int(x)).arg(int(y)));
    }
}

void CLUTOnlineItemWidget::retranslateUi()
{
    load_button->setText(tr("Load"));
    save_button->setText(tr("Save"));
    reset_button->setText(tr("Reset"));
}

int CLUTOnlineItemWidget::getHSLPointNums()
{
    switch (online_type) {
    case HBYH:
    case SBYH:
    case LBYH:
        return 37;
    case HBYS:
    case SBYS:
    case LBYS:
    case HBYL:
    case SBYL:
    case LBYL:
        return 21;
    default:
        return 0;
    }
    return 0;
}

int CLUTOnlineItemWidget::getHSLPointMaxX()
{
    switch (online_type) {
    case HBYH:
    case SBYH:
    case LBYH:
        return 360;
    case HBYS:
    case SBYS:
    case LBYS:
    case HBYL:
    case SBYL:
    case LBYL:
        return 100;
    default:
        return 0;
    }
    return 0;
}

int CLUTOnlineItemWidget::getHSLPointMaxY()
{
    switch (online_type) {
    case HBYH:
    case HBYS:
    case HBYL:
        return 30;
    case SBYH:
    case SBYS:
    case SBYL:
    case LBYH:
    case LBYS:
    case LBYL:
        return 100;
    default:
        return 0;
    }
    return 0;
}

Gradient_Band_Type CLUTOnlineItemWidget::getGradientBandType(ONLINE_TYPE online_type)
{
    switch (online_type) {
    case HBYH:
    case SBYH:
    case LBYH:
        return HUE;
    case HBYS:
    case SBYS:
    case LBYS:
        return SAT;
    case HBYL:
    case SBYL:
    case LBYL:
        return LUM;
    default:
        return HUE;
    }
    return HUE;
}

Gradient_Band_Type CLUTOnlineItemWidget::getAjustType(ONLINE_TYPE online_type)
{
    switch (online_type) {
    case HBYH:
    case HBYS:
    case HBYL:
        return HUE;
    case SBYH:
    case SBYS:
    case SBYL:
        return SAT;
    case LBYH:
    case LBYS:
    case LBYL:
        return LUM;
    default:
        return HUE;
    }
    return HUE;
}

ONLINE_TYPE CLUTOnlineItemWidget::getOnlineTypeByStr(QString str)
{
    ONLINE_TYPE type = HBYH;

    if (str == "HByH") {
        type = HBYH;
    } else if (str == "SByH") {
        type = SBYH;
    } else if (str == "LByH") {
        type = LBYH;
    } else if (str == "HByS") {
        type = HBYS;
    } else if (str == "SByS") {
        type = SBYS;
    } else if (str == "LByS") {
        type = LBYS;
    } else if (str == "HByL") {
        type = HBYL;
    } else if (str == "SByL") {
        type = SBYL;
    } else if (str == "LByL") {
        type = LBYL;
    }

    return type;
}
