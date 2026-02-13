#ifndef CLUT_ONLINE_ITEM_WIDGET_HPP
#define CLUT_ONLINE_ITEM_WIDGET_HPP

#include "file_dialog.hpp"
#include "group_widget.hpp"
#include "base_item_widget.hpp"
#include "spinner_item_widget.hpp"
#include "universal_matrix_item_widget.hpp"

#define SCALE_UNIT 50

enum ONLINE_TYPE {
    HBYH,
    SBYH,
    LBYH,
    HBYS,
    SBYS,
    LBYS,
    HBYL,
    SBYL,
    LBYL
};

enum Gradient_Band_Type {
    HUE,
    SAT,
    LUM
};

class GradientWidget : public QWidget {
    Q_OBJECT
public:
    explicit GradientWidget(Gradient_Band_Type type, QWidget *parent = nullptr);

    void setColorAt(int position, QColor color);
    virtual void clearColors();
    virtual void initDefaultColors();
    void updateGradient();
    void setAjustHue(int hue) { ajust_hue = hue; }
    void setAjustSat(int sat) { ajust_sat = sat; }
    void setAjustLum(int lum) { ajust_lum = lum; }
    int getAjustHue() { return ajust_hue; }
    int getAjustSat() { return ajust_sat; }
    int getAjustLum() { return ajust_lum; }
    void updateColorPoints();

protected:
    int getMaxValue();
    void paintEvent(QPaintEvent *) override;
    virtual void mousePressEvent(QMouseEvent *) override;

protected:
    QMap<int, QColor> color_points;
    Gradient_Band_Type g_type;
    QLinearGradient gradient;
    int ajust_hue;
    int ajust_sat;
    int ajust_lum;
};


class CLUTCurveWidget : public GradientWidget
{
    Q_OBJECT

public:
    explicit CLUTCurveWidget(Gradient_Band_Type g_type, Gradient_Band_Type a_type,
                                                                                                QWidget *parent = nullptr);

    void setPoints(const QVector<QPoint>& points);
    QVector<QPoint> getPoints() const { return coord_points; }
    void setXRange(qreal min, qreal max);
    void setYRange(qreal min, qreal max);
    void setGridSize(int xSize, int ySize);
    void setGridVisible(bool visible) { is_show_grid = visible; update(); }
    void setDisplayY(bool visible) { display_value_y = visible; update(); }
    QVariant getData() const;
    void setHSLInColorPoints(int hsl, Gradient_Band_Type band_type);
    void updateCurve() { update(); }
    void resetPoints(int y);

    void clearColors() override;
    void initDefaultColors() override;
    void updateColorPointsRef();
    void saveJsonFile(QString filePath);
    void saveTxtFile(QString filePath);
    void loadJsonFile(QString filePath);
    void loadTxtFile(QString file_path);

private:
    QRect getPlotArea() const;
    QPoint toScreen(const QPoint& point) const;
    QPoint fromScreen(const QPoint& point) const;
    int findNearestPoint(const QPoint& pos, int maxDistance = 10) const;
    void drawGrid(QPainter& painter);
    void drawAxes(QPainter& painter);
    void drawCurve(QPainter& painter);
    void drawPoints(QPainter& painter);
    void reColorGradientAtPos(int);
    void interpolateGradientColorAtPos(int);

signals:
    void pointYChanged(int index, qreal newY);
    void curveChanged(const QVector<QPoint>& points);
    void sigUpdateMousePos(double x, double y);

protected:
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;

public:
    static const int MARGIN_LEFT = 50;
    static const int MARGIN_RIGHT = 30;
    static const int MARGIN_TOP = 90;
    static const int MARGIN_BOTTOM = 30;

private:
    QVector<QPoint> coord_points;
    int selected_point{-1};
    bool is_show_grid{true};
    int grid_size_x;
    int grid_size_y;
    bool display_value_y{true};
    QMap<int, QColor> color_points_ref;

    qreal xMin{0}, xMax{0};
    qreal yMin{0}, yMax{0};
    Gradient_Band_Type a_type;
    int angle{0}, s_scale{SCALE_UNIT}, l_scale{SCALE_UNIT};
};


class CLUTOnlineItemWidget : public BaseItemWidget
{
    Q_OBJECT
public:
    explicit CLUTOnlineItemWidget(QString module, QString title,
                                  UniversalMatrixItemWidget *hsl_table, QWidget *parent = nullptr);
    void slotRefreshChart();
    void setHSLTable(UniversalMatrixItemWidget *);

private slots:
    void updateMousePos(double x, double y);
    void openLoadDialog();
    void openSaveDialog();
    void handleResetCurve();
    void slotRefreshTable(const QVector<QPoint>& points);
    void handleSelectFile(QString format, QString file_path, MODE mode);
    void hueSpinnerChanged(QVariant);
    void slotUpdateCurve(QVariant);

signals:
    void sigUpdateMousePos(double x, double y);
    void sigPressedScatterPt(QPoint p);
    void sigValueChanged();

protected:
    virtual void retranslateUi() override;
    void paintEvent(QPaintEvent *) override;

private:
    void initChart();
    void initGroupWidget();
    int getHSLPointNums();
    int getHSLPointMaxX();
    int getHSLPointMaxY();
    QVector<QPoint> getHSLPoints() ;
    Gradient_Band_Type getGradientBandType(ONLINE_TYPE online_type);
    Gradient_Band_Type getAjustType(ONLINE_TYPE online_type);
    QLayout *initButtons();
    static ONLINE_TYPE getOnlineTypeByStr(QString str);
    SpinnerItemWidget *initSpinner();

private:
    CLUTCurveWidget *line_chart_widget;
    GroupItemWidget *group_widget;
    ONLINE_TYPE online_type;
    Gradient_Band_Type band_type;
    Gradient_Band_Type band_type_ajust;
    Gradient_Band_Type band_type_spinner;
    QLabel *pos_label;
    UniversalMatrixItemWidget *hsl_table;
    QPushButton *load_button;
    QPushButton *save_button;
    QPushButton *reset_button;
    FileDialog *file_select_dialog;
};

#endif // CLUT_ONLINE_ITEM_WIDGET_HPP
