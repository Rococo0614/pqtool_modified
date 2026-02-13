#ifndef IMAGE_LABEL_HPP
#define IMAGE_LABEL_HPP

#include <QLabel>
#include "global_data.hpp"

const int GRID_AF_ROW = 15;
const int GRID_AF_COL = 17;
const int GRID_AE_ROW = 15;
const int GRID_AE_COL = 17;
const int GRID_AWB_ROW = 32;
const int GRID_AWB_COL = 64;

enum AutoAnaState {
    STATE_NONE,
    STATE_AE,
    STATE_AF,
    STATE_AWB
};

class ImageLabel : public QLabel
{
    Q_OBJECT

public:
    enum MODE {
        NORMAL,
        GRID,
        SINGLE_RECT,
        COLOR_CHECKER,
        GRID_3A,
        GRID_FOCUS_ASSIST
    };
    int awb_win_num_x = 64;
    int awb_win_num_y = 32;
public:
    explicit ImageLabel(QWidget *parent, MODE mode);

    void setPixmap(const QPixmap &p);
    QImage getImage();
    /* color checker rects */
    void reset();
    void up();
    void down();
    void left();
    void right();
    void large();
    void small();
    void hIntervalLarge();
    void vIntervalLarge();
    void hIntervalSmall();
    void vIntervalSmall();
    void hspaceLarge();
    void hspaceSmall();
    void vspaceLarge();
    void vspaceSmall();
    void setScaleFactor(double scale_factor);
    QList<QList<QColor>> getColorCheckerPixels(int rect_index);
    QVector<QRectF> getColorCheckerRect();
    void clearColorCheckerRect();
    void setFvTable(QVector<QVector<int>> &);
    void drawSingleRect();
    void updateSingleRect();
    void updateSingleRect(int x, int y, int w, int h);
    QRectF getSingleRect();
    void setRepaintEvent(bool state);

    /* Focus Assistan */
    void setShowFvText(bool enabled);
    void setFvNowTable(int table[][GRID_AF_COL]);
    void setFvMaxTable(int table[][GRID_AF_COL]);
    void getFvSelectedTable(bool table[][GRID_AF_COL]);
    void setFvSelectedTable(bool value);
    void setWindowInfo(int h, int v);

    void setEnGridMode(int value);

    void setHasFvTable(bool value);

    bool getHasFvTable() const;
    void enableGrid(bool en);
    void enableRect(bool en);
    void setGridSize(int row, int col);

    void setMode(MODE mode);
    MODE getMode();

protected:
    void resizeEvent(QResizeEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    void drawGrid();
    void drawRect();
    void drawColorCheckerRect();
    void updateColorCheckerRect();
    void drawGrid3A();
    void drawGridText();
    void drawGridCell();
    void drawGridFocusAssist();
    void drawFvText();
    void drawSelectedCircle();

signals:
    void sigGridCellSel(int x, int y);
    void sigPositionChange(QString text);
    void sigUpdateSingleRect(QRectF rect, double factor);

private:
    MODE mode;
    bool grid_enabled;
    bool repaint_event{false};
    bool mouse_press{false};
    QPoint p1;
    QPoint p2;

    /* color checker rects */
    QVector<QRectF> color_checker_rects;
    QVector<QPointF> color_checker_corner_points;
    double size_offset;
    double h_interval;
    double v_interval;
    double h_spacing;
    double v_spacing;
    double grid_width;
    double grid_height;
    double x_offset;
    double y_offset;
    QRectF drag_color_rect;
    double scale_factor{1.0};
    double pre_scale_factor{1.0};
    QImage image;
    int enGridMode {STATE_NONE};
    bool has_fv_table {true};
    QVector<QVector<int>> fv_table;
    int row {-1};
    int col {-1};
    int grid_row {45};
    int grid_col {80};
    bool enGrid {true};
    bool enRect {true};

    /* Focus Assistan */
    bool show_fv_text_enabled{true};
    bool fv_selected_table[GRID_AF_ROW][GRID_AF_COL] = {};
    int fv_now_table[GRID_AF_ROW][GRID_AF_COL] = {};
    int fv_max_table[GRID_AF_ROW][GRID_AF_COL] = {};
    int h_wnd = GRID_AF_COL, v_wnd = GRID_AF_ROW;
    int width_wnd = 0, height_wnd = 0;
    QRectF single_rect;
};

#endif // IMAGE_LABEL_HPP
