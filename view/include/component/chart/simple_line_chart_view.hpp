#ifndef SIMPLE_LINE_CHART_VIEW_HPP
#define SIMPLE_LINE_CHART_VIEW_HPP

#include "base_item_widget.hpp"


class SimpleLineChartView : public QWidget
{
    Q_OBJECT

public:
    explicit SimpleLineChartView(QWidget *parent = nullptr);

    void setPoints(QList<QPoint>& points);
    QList<QPoint> getPoints() const { return coord_points; }
    void setXRange(qreal min, qreal max);
    void setYRange(qreal min, qreal max);
    void setGridSize(int xSize, int ySize);
    void setGridVisible(bool visible) { is_show_grid = visible; update(); }
    void setDisplayY(bool visible) { display_value_y = visible; update(); }
    QVariant getData() const;
    void updateCurve() { update(); }
    void resetPoints(int y);
    void setAxisXLabelList(QStringList);

private:
    QRect getPlotArea() const;
    QPoint toScreen(const QPoint& point) const;
    QPoint fromScreen(const QPoint& point) const;
    int findNearestPoint(const QPoint& pos, int maxDistance = 10) const;
    void drawGrid(QPainter& painter);
    void drawAxes(QPainter& painter);
    void drawCurve(QPainter& painter);
    void drawPoints(QPainter& painter);

signals:
    void sigPointYChanged(int index, qreal newY);
    void sigCurveChanged(const QList<QPoint>& points);
    void sigUpdateMousePos(double x, double y);
    void sigValueChanged();
    void sigPointSelected(int);

protected:
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;

public:
    static const int MARGIN_LEFT = 60;
    static const int MARGIN_RIGHT = 50;
    static const int MARGIN_TOP = 30;
    static const int MARGIN_BOTTOM = 30;

private:
    QList<QPoint> coord_points;
    int selected_point{-1};
    bool is_show_grid{true};
    int grid_size_x;
    int grid_size_y;
    bool display_value_y{true};
    QStringList axis_x_str_list;

    qreal xMin{0}, xMax{0};
    qreal yMin{0}, yMax{0};
};

#endif // SIMPLE_LINE_CHART_VIEW_HPP
