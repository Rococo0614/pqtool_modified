#ifndef SIMPLE_LINE_CHART_VIEW_CPP
#define SIMPLE_LINE_CHART_VIEW_CPP

#include "simple_line_chart_view.hpp"
#include <QMessageBox>
#include <QToolTip>
#include <QPainter>
#include <QMouseEvent>


SimpleLineChartView::SimpleLineChartView(QWidget *parent) :
    QWidget(parent)
{
    setMouseTracking(true);
    setMinimumSize(400, 100);
}

void SimpleLineChartView::setPoints( QList<QPoint>& points)
{
    this->coord_points.clear();
    this->coord_points += points;
    update();
}

void SimpleLineChartView::setXRange(qreal min, qreal max)
{
    xMin = min;
    xMax = max;
    update();
}

void SimpleLineChartView::setYRange(qreal min, qreal max)
{
    yMin = min;
    yMax = max;
    update();
}

void SimpleLineChartView::setGridSize(int xSize, int ySize)
{
   grid_size_x = xSize;
   grid_size_y = ySize;
   update();
}

QRect SimpleLineChartView::getPlotArea() const
{
    return QRect(MARGIN_LEFT,
                        MARGIN_TOP,
                        width() - MARGIN_LEFT - MARGIN_RIGHT,
                        height() - MARGIN_TOP - MARGIN_BOTTOM);
}

void SimpleLineChartView::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.fillRect(rect(), Qt::white);

    if (is_show_grid) {
        drawGrid(painter);
    }

    drawAxes(painter);
    drawCurve(painter);
    drawPoints(painter);
}

void SimpleLineChartView::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) {
        selected_point = findNearestPoint(event->pos());
    }
    update();

    if (selected_point != -1){

        QString x_str = QString("X: %1, ").arg(coord_points[selected_point].x());
        QString y_str = QString("Y: %1").arg(coord_points[selected_point].y());

        QToolTip::showText(event->globalPos() - QPoint(100, 40), x_str + y_str, this);

        emit sigPointSelected(selected_point);
    }
}

void SimpleLineChartView::mouseMoveEvent(QMouseEvent* event)
{
    QPoint screenPoint = fromScreen(event->pos());

    if (selected_point >= 0 && selected_point < coord_points.size()) {

        QPoint currentPoint = coord_points[selected_point];

        qreal newY = qBound((int)yMin, screenPoint.y(), (int)yMax);

        if (currentPoint.y() != newY) {
            coord_points[selected_point].setY(newY);
            emit sigPointYChanged(selected_point, newY);
            emit sigCurveChanged(coord_points);
            emit sigValueChanged();
            update();
        }
    }
    double pos_x = screenPoint.x();
    double pos_y = screenPoint.y();
    emit sigUpdateMousePos(pos_x, pos_y);
}

void SimpleLineChartView::mouseReleaseEvent(QMouseEvent*)
{
    selected_point = -1;
}

QPoint SimpleLineChartView::toScreen(const QPoint& point) const
{
    QRect plotArea = getPlotArea();

    int x = plotArea.left() + qRound((point.x() - xMin) * plotArea.width() / (xMax - xMin));
    int y = plotArea.bottom() - qRound((point.y() - yMin) * plotArea.height() / (yMax - yMin));

    return QPoint(x, y);
}

QPoint SimpleLineChartView::fromScreen(const QPoint& point) const
{
    QRect plotArea = getPlotArea();

    qreal x = selected_point >= 0 ?
                coord_points[selected_point].x() :
                xMin + ((point.x() - plotArea.left()) * (xMax -xMin)) / plotArea.width();
    qreal y = yMin + ((plotArea.bottom() - point.y()) * (yMax - yMin)) / plotArea.height();

    return QPoint(qRound(x), qRound(y));
}

int SimpleLineChartView::findNearestPoint(const QPoint& pos, int maxDistance) const
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

void SimpleLineChartView::drawGrid(QPainter& painter)
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

void SimpleLineChartView::drawAxes(QPainter& painter)
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
    for (int i = 0; i <= grid_size_x; ++i) {
        qreal xValue =xMin + (i * (xMax -xMin) /grid_size_x);
        int x = plotArea.left() + qRound((xValue -xMin) * plotArea.width() / (xMax -xMin));

        painter.drawLine(x, zeroY, x, zeroY + 5);

        QString text;
        if (axis_x_str_list.count() == grid_size_x + 1) {
            text = axis_x_str_list[i];
        } else {
            text = QString::number(xValue, 'f', 0);
        }
        QFontMetrics fm(painter.font());
        int textWidth = fm.horizontalAdvance(text);
        painter.drawText(x - textWidth/2, zeroY + 20, text);
    }

    // Y Axes Scale
    for (int i = 0; i <= grid_size_y; ++i) {
        qreal yValue =yMin + (i * (yMax -yMin) /grid_size_y);
        int y = plotArea.bottom() - qRound((yValue -yMin) * plotArea.height() / (yMax -yMin));

        painter.drawLine(plotArea.left() - 5, y, plotArea.left(), y);

        QString text = QString::number(yValue, 'f', 0);
        QFontMetrics fm(painter.font());
        int textWidth = fm.horizontalAdvance(text);
        painter.drawText(plotArea.left() - textWidth - 10, y + fm.height()/2 - 2, text);
    }
}

void SimpleLineChartView::drawCurve(QPainter& painter)
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

void SimpleLineChartView::drawPoints(QPainter& painter)
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
            QString valueText = QString::number(coord_points[i].y());
            QFontMetrics fm(painter.font());
            int textWidth = fm.horizontalAdvance(valueText);
            int textX = screenPos.x() - textWidth / 2;
            int textY = screenPos.y() - 10;

            painter.drawText(textX, textY, valueText);
        }
    }
}

QVariant SimpleLineChartView::getData() const
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

void SimpleLineChartView::resetPoints(int y)
{
    for (auto it = coord_points.begin(); it != coord_points.end(); it++) {
        it->setY(y);
    }
    update();
    emit sigCurveChanged(coord_points);
}

void SimpleLineChartView::setAxisXLabelList(QStringList label_list)
{
    axis_x_str_list.clear();
    for (QString label : label_list) {
        axis_x_str_list.append(label);
    }
}

#endif // SIMPLE_LINE_CHART_VIEW_CPP
