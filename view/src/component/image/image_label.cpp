#include "image_label.hpp"
#include <QPainter>
#include <QtCore>
#include <QMouseEvent>
#include <QResizeEvent>

const QColor LINE_COLOR = QColor(50, 139, 255, 200);
const QColor GRID_COLOR = QColor(80, 150, 255, 150);
const QColor RECT_COLOR = QColor(255, 28, 28);
const QColor CYAN_COLOR = QColor(0, 255, 255);
const QColor YELLOW_COLOR = QColor(255, 255, 0);
const QColor GREEN_COLOR = QColor(0, 255, 0);
const QColor BLUE_COLOR = QColor(0, 0, 255);
const QColor GRID_3A_LINE_COLOR = QColor(255, 255, 0, 200); //yellow
const QColor GRID_3A_DATA_COLOR = QColor(0, 0, 0xFF, 200); //blue
const QColor GRID_3A_RECT_COLOR = QColor(0, 0, 0xFF, 170); //blue with more transparent

ImageLabel::ImageLabel(QWidget *parent, MODE mode) :
    QLabel(parent),
    mode(mode),
    size_offset(0.0),
    h_interval(0),
    v_interval(0),
    h_spacing(20.0),
    v_spacing(20.0),
    x_offset(0.0),
    y_offset(0.0)
{
    has_fv_table = false;
    updateColorCheckerRect();
    updateSingleRect();
    setMouseTracking(true);
}

void ImageLabel::setPixmap(const QPixmap &p)
{
    QLabel::setPixmap(p);
    this->image = p.toImage();
}

QImage ImageLabel::getImage()
{
    return this->image;
}

void ImageLabel::reset()
{
    size_offset = 0.0;
    h_spacing = 20.0;
    v_spacing = 20.0;
    h_interval = 0.0;
    v_interval = 0.0;
    x_offset = 0.0;
    y_offset = 0.0;
    updateColorCheckerRect();
    repaint();
}

void ImageLabel::up()
{
    y_offset -= 5;
    updateColorCheckerRect();
    repaint();
}

void ImageLabel::down()
{
    y_offset += 5;
    updateColorCheckerRect();
    repaint();
}

void ImageLabel::left()
{
    x_offset -= 5;
    updateColorCheckerRect();
    repaint();
}

void ImageLabel::right()
{
    x_offset += 5;
    updateColorCheckerRect();
    repaint();
}

void ImageLabel::large()
{
    size_offset += 5;
    updateColorCheckerRect();
    repaint();
}

void ImageLabel::small()
{
    size_offset -= 5;
    updateColorCheckerRect();
    repaint();
}

void ImageLabel::hIntervalSmall()
{
    h_interval -= 1;
    updateColorCheckerRect();
    repaint();
}

void ImageLabel::vIntervalSmall()
{
    v_interval -= 1;
    updateColorCheckerRect();
    repaint();
}

void ImageLabel::hIntervalLarge()
{
    h_interval += 1;
    updateColorCheckerRect();
    repaint();
}

void ImageLabel::vIntervalLarge()
{
    v_interval += 1;
    updateColorCheckerRect();
    repaint();
}

void ImageLabel::hspaceLarge()
{
    h_spacing -= 5;
    updateColorCheckerRect();
    repaint();
}

void ImageLabel::hspaceSmall()
{
    h_spacing += 5;
    updateColorCheckerRect();
    repaint();
}

void ImageLabel::vspaceLarge()
{
    v_spacing -= 5;
    updateColorCheckerRect();
    repaint();
}

void ImageLabel::setScaleFactor(double scale_factor)
{
    this->pre_scale_factor = this->scale_factor;
    this->scale_factor = scale_factor;

    double fact = this->scale_factor / this->pre_scale_factor;
    double width = drag_color_rect.width() * fact;
    double height = drag_color_rect.height() * fact;
    double center_x = drag_color_rect.x() * fact;
    double center_y = drag_color_rect.y() * fact;
    drag_color_rect.setRect(center_x, center_y, width, height);

    size_offset *= fact;
    h_interval *= fact;
    v_interval *= fact;
    h_spacing *= fact;
    v_spacing *= fact;
    x_offset *= fact;
    y_offset *= fact;
}

void ImageLabel::vspaceSmall()
{
    v_spacing += 5;
    updateColorCheckerRect();
    repaint();
}

QList<QList<QColor> > ImageLabel::getColorCheckerPixels(int rect_index)
{
    QList<QList<QColor>> pixels;
    if (!pixmap()) {
        return pixels;
    }

    QImage img = pixmap()->toImage();
    double factor = double(img.size().width()) / size().width();
    QRectF rect = color_checker_rects[rect_index];
    for (int i = 0; i < rect.height() * factor; i++) {
        pixels.append(QList<QColor>());
        for (int j = 0; j < rect.width() * factor; j++) {
            int x = qCeil(rect.left() * factor) + j;
            int y = qCeil(rect.top() * factor) + i;
            QColor c = img.pixelColor(x, y);
            pixels[i].append(c);
        }
    }

    return pixels;
}

QVector<QRectF> ImageLabel::getColorCheckerRect() {
    return this->color_checker_rects;
}

void ImageLabel::clearColorCheckerRect() {
    this->color_checker_rects.clear();
    this->color_checker_corner_points.clear();
}

void ImageLabel::resizeEvent(QResizeEvent *event)
{
    QLabel::resizeEvent(event);

    grid_width = event->size().width() / grid_col;
    grid_height = event->size().height() / grid_row;
    updateColorCheckerRect();
    updateSingleRect();
    if (mode == NORMAL) {
        p1 = p1 * (double(event->size().width()) / event->oldSize().width());
        p2 = p2 * (double(event->size().width()) / event->oldSize().width());
    }
}

void ImageLabel::paintEvent(QPaintEvent *event)
{
    QLabel::paintEvent(event);

    if (!this->pixmap()) {
        return;
    }

    if (mode == SINGLE_RECT) {
        drawSingleRect();
        repaint_event = false;
    }

    if (mode == COLOR_CHECKER) {
        drawColorCheckerRect();
    }

    if (mode == GRID && enGrid) {
        drawGrid();
    }

    if (mode == GRID_3A && enGridMode != STATE_NONE) {
        drawGrid3A();
        drawGridCell();
    }

    if (mode == GRID_FOCUS_ASSIST) {
        drawGridFocusAssist();
    }

    if (!p2.isNull() && enRect){
        drawRect();
    }
}

void ImageLabel::mousePressEvent(QMouseEvent *event)
{
    QLabel::mousePressEvent(event);

    mouse_press = true;
    // right click reset drag
    if (((mode == COLOR_CHECKER) || (mode == SINGLE_RECT)) && (event->buttons() == Qt::RightButton)) {
        drag_color_rect.setRect(0, 0, 0, 0);
    }

    p1 = event->pos();
    if (mode == GRID_3A && enGridMode != STATE_NONE) {
            int row_num = GRID_AF_ROW;
            int col_num = GRID_AF_COL;
            if (enGridMode == STATE_AF) {
                row_num = GRID_AF_ROW;
                col_num = GRID_AF_COL;
            }
            else if (enGridMode == STATE_AE) {
                row_num = GRID_AE_ROW;
                col_num = GRID_AE_COL;
            } else if (enGridMode == STATE_AWB) {
                row_num = awb_win_num_y;
                col_num = awb_win_num_x;
            }

            double cell_width = width() / (col_num * 1.0);
            double cell_height = height() / (row_num * 1.0);
            col = ceil( p1.x() / cell_width) - 1;
            row = ceil( p1.y() / cell_height) - 1;

            emit sigGridCellSel(row, col);
    }

    if (mode == GRID) {
        p1.setX(qFloor(p1.x() / grid_width));
        p1.setY(qFloor(p1.y() / grid_height));
    }

    if (mode == GRID_FOCUS_ASSIST) {
        col = p1.x() / width_wnd;
        row = p1.y() / height_wnd;
        fv_selected_table[row][col] = !fv_selected_table[row][col];
    }

    p2.setX(0);
    p2.setY(0);
    repaint();
}

void ImageLabel::mouseMoveEvent(QMouseEvent *event)
{
    QLabel::mouseMoveEvent(event);

    if (this->pixmap() != nullptr) {
        QPoint pos = event->pos();
        QColor c = this->image.pixelColor(
                    static_cast<int>(pos.x() / this->scale_factor),
                    static_cast<int>(pos.y()/this->scale_factor));
        QString tool_tip("(" + QString::number(c.red()) + "," + QString::number(c.green()) + "," + QString::number(c.blue()) + ")");
        this->setToolTip(tool_tip);

        QString info("X=" + QString::number(static_cast<int>(pos.x() / this->scale_factor)) + ","
            + "Y=" + QString::number(static_cast<int>(pos.y() / this->scale_factor)) + ","
            + "R=" + QString::number(c.red()) + ","
            + "G=" + QString::number(c.green()) + ","
            + "B="+ QString::number(c.blue()));
        // Convert RGB to YUV
        int Y = static_cast<int>(0.299 * c.red() + 0.587 * c.green() + 0.114 * c.blue());
        int U = static_cast<int>((c.blue() - Y) * 0.493) + 128;
        int V = static_cast<int>((c.red() - Y) * 0.877) + 128;
        // Convert RGB to HSV
        QColor hsvColor = c.toHsv();
        int H = hsvColor.hue() + 1;             // -1~359 to 0~360
        int S = hsvColor.saturation();
        int V_hsv = hsvColor.value();
        // Add YUV & HSV to info
        info += "\nY=" + QString::number(Y) + ","
            + "U=" + QString::number(U) + ","
            + "V=" + QString::number(V) + ","
            + "H=" + QString::number(H) + ","
            + "S=" + QString::number(S) + ","
            + "V=" + QString::number(V_hsv);
        emit sigPositionChange(info);
    }

    if (((mode == COLOR_CHECKER) || (mode == SINGLE_RECT)) && mouse_press) {
        p2 = event->pos();
        repaint();
    } else if ((mode == GRID) && mouse_press) {
        p2 = event->pos();
        p2.setX(qCeil(p2.x() / grid_width));
        p2.setY(qCeil(p2.y() / grid_height));
        repaint();
    }
}

void ImageLabel::mouseReleaseEvent(QMouseEvent *event)
{
    QLabel::mouseReleaseEvent(event);

    mouse_press = false;

    p2 = event->pos();
    if (mode == GRID) {
        p2.setX(qCeil(p2.x() / grid_width));
        p2.setY(qCeil(p2.y() / grid_height));
        repaint();
    }

    if (((mode == COLOR_CHECKER) || (mode == SINGLE_RECT)) && (!p2.isNull())) {
        // check drag area is reset
        if (drag_color_rect.isEmpty()) {
            if (mode == SINGLE_RECT) {
                updateSingleRect();
                drawSingleRect();
            } else {
                updateColorCheckerRect();
                drawColorCheckerRect();
            }
        }
        // check has drag area or not
        if ((p1.x() != p2.x()) && (p1.y() != p2.y())) {
            double width = abs(p2.x() - p1.x());
            double height = abs(p2.y() - p1.y());
            double center_x = p1.x() + (width / 2);
            double center_y = p1.y() + (height / 2);
            drag_color_rect.setRect(center_x, center_y, width, height);
            if (mode == SINGLE_RECT) {
                repaint_event = true;
                updateSingleRect();
                drawSingleRect();
                repaint_event = false;
            } else {
                updateColorCheckerRect();
                drawColorCheckerRect();
            }
            p2.setX(0);
            p2.setY(0);
        }
        repaint();
    }
}

void ImageLabel::setEnGridMode(int value)
{
    enGridMode = value;
}

void ImageLabel::drawGridCell()
{
    int row_num = GRID_AF_ROW;
    int col_num = GRID_AF_COL;
    if (enGridMode == STATE_AF) {
        row_num = GRID_AF_ROW;
        col_num = GRID_AF_COL;
    }
    else if (enGridMode == STATE_AE) {
        row_num = GRID_AE_ROW;
        col_num = GRID_AE_COL;
    } else if (enGridMode == STATE_AWB) {
        row_num = awb_win_num_y;
        col_num = awb_win_num_x;
    }

    if (row < 0 || row >= row_num || col < 0 || col >= col_num) {
        return;
    }

    double cell_width = width() / (col_num * 1.0);
    double cell_height = height() / (row_num * 1.0);

    QPainter painter(this);
    painter.setPen(QPen(GRID_3A_RECT_COLOR, 2));
    painter.drawLine(col * cell_width, row *cell_height, (col + 1) * cell_width, row *cell_height);
    painter.drawLine(col * cell_width, (row + 1) *cell_height, (col + 1) * cell_width, (row + 1) *cell_height);

    painter.drawLine(col * cell_width, row *cell_height, col * cell_width, (row + 1) *cell_height);
    painter.drawLine((col + 1) * cell_width, row *cell_height, (col + 1) * cell_width, (row + 1) *cell_height);
}

bool ImageLabel::getHasFvTable() const
{
    return has_fv_table;
}

void ImageLabel::setHasFvTable(bool value)
{
    has_fv_table = value;
}

void ImageLabel::drawGrid3A()
{
    if (enGridMode == STATE_NONE)
    {
        return;
    }

    QPainter painter(this);
    painter.setPen(QPen(GRID_3A_LINE_COLOR, 2));

    //default AF
    int row_num = GRID_AF_ROW;
    int col_num = GRID_AF_COL;
    if (enGridMode == STATE_AF) {
        row_num = GRID_AF_ROW;
        col_num = GRID_AF_COL;
    }
    else if (enGridMode == STATE_AE) {
        row_num = GRID_AE_ROW;
        col_num = GRID_AE_COL;
    } else if (enGridMode == STATE_AWB) {
        row_num = awb_win_num_y;
        col_num = awb_win_num_x;
    }

    painter.drawLine(0, 0, 0, height());
    painter.drawLine(width(), 0, width(), height());
    double cell_width = this->width() / (col_num * 1.0);
    for (int idx = 0; idx < col_num-1; idx++) {
        painter.drawLine((idx+1)*cell_width, 0, (idx+1)*cell_width, height());
    }

    painter.drawLine(0, 0, width(), 0);
    painter.drawLine(0, height(), width(), height());
    double cell_height = height() / (row_num * 1.0);
    for (int idx = 0; idx < row_num-1; idx++) {
        painter.drawLine(0, (idx+1)*cell_height, width(), (idx+1)*cell_height);
    }

    //text font baseline
    const int x = 2;
    const int y = 10;
    for (int idx = 0; idx < col_num; idx++) {
        painter.drawText(x + cell_width*idx, y, QString("%1").arg(idx));
    }

    for (int idx = 1; idx < row_num; idx++) {
        painter.drawText(x, y + cell_height*idx, QString("%1").arg(idx));
    }

    if (enGridMode == STATE_AF && has_fv_table) drawGridText();
}

void ImageLabel::drawGridFocusAssist()
{
    QPainter painter(this);
    width_wnd = (width() / h_wnd) / 2 * 2;
    height_wnd = (height() / v_wnd) / 2 * 2;

    painter.setPen(QPen(YELLOW_COLOR, 2));
    painter.drawLine(0, 0, 0, height());
    painter.drawLine(width(), 0, width(), height());
    for (int idx = 1; idx < h_wnd; idx++) {
        painter.drawLine(idx * width_wnd, 0, idx * width_wnd, height());
    }

    painter.drawLine(0, 0, width(), 0);
    painter.drawLine(0, height(), width(), height());
    for (int idx = 1; idx < v_wnd; idx++) {
        painter.drawLine(0, idx * height_wnd, width(), idx * height_wnd);
    }

    if (show_fv_text_enabled == true) {
        drawFvText();
        drawSelectedCircle();
    }
}

void ImageLabel::drawFvText()
{
    QPainter painter(this);
    painter.setPen(QPen(YELLOW_COLOR, 2));
    QFont font("宋体", 9);
    painter.setFont(font);

    const int x = width_wnd - 48;
    const int y_now = height_wnd - 32;
    const int y_max = height_wnd - 16;

    for(int i = 0; i < GRID_AF_COL; i++) {
        for(int j = 0; j < GRID_AF_ROW; j++) {
                painter.drawText(x + i*width_wnd, y_now + j*height_wnd, QString("%1").arg(fv_now_table[j][i]));
        }
    }

    painter.setPen(QPen(CYAN_COLOR, 2));
    for(int i = 0; i < GRID_AF_COL; i++) {
        for(int j = 0; j < GRID_AF_ROW; j++) {
                painter.drawText(x + i*width_wnd, y_max + j*height_wnd, QString("%1").arg(fv_max_table[j][i]));
        }
    }
}

void ImageLabel::drawSelectedCircle()
{
    QPainter painter(this);
    painter.setPen(GREEN_COLOR);
    painter.setBrush(GREEN_COLOR);

    const int x = 8;
    const int y = 8;

    for(int i = 0; i < GRID_AF_COL; i++) {
        for(int j = 0; j < GRID_AF_ROW; j++) {
            if (fv_selected_table[j][i]) {
                painter.drawEllipse(x + i*width_wnd, y + j*height_wnd, 9, 9);
            }
        }
    }
}

void ImageLabel::setShowFvText(bool enabled)
{
    show_fv_text_enabled = enabled;
}

void ImageLabel::setFvNowTable(int table[][GRID_AF_COL])
{
    for(int i = 0; i < GRID_AF_ROW; i++) {
        for(int j = 0; j < GRID_AF_COL; j++) {
            fv_now_table[i][j] = table[i][j];
        }
    }
}

void ImageLabel::setFvMaxTable(int table[][GRID_AF_COL])
{
     for(int i = 0; i < GRID_AF_ROW; i++) {
        for(int j = 0; j < GRID_AF_COL; j++) {
            fv_max_table[i][j] = table[i][j];
        }
    }
}

void ImageLabel::getFvSelectedTable(bool table[][GRID_AF_COL])
{
    for(int i = 0; i < GRID_AF_ROW; i++) {
        for(int j = 0; j < GRID_AF_COL; j++) {
            table[i][j] = fv_selected_table[i][j];
        }
    }
}

void ImageLabel::setFvSelectedTable(bool value)
{
    for(int i = 0; i < GRID_AF_ROW; i++) {
        for(int j = 0; j < GRID_AF_COL; j++) {
            fv_selected_table[i][j] = value;
        }
    }
}

void ImageLabel::setWindowInfo(int h, int v)
{
    h_wnd = h;
    v_wnd = v;
}

void ImageLabel::setFvTable(QVector<QVector<int>> &fv)
{
    fv_table = fv;
}

void ImageLabel::drawGridText()
{
    if (fv_table.isEmpty()) return;

    QPainter painter(this);
    painter.setPen(QPen(GRID_3A_DATA_COLOR, 2));
    int font_size = scale_factor * 2 + 9;
    QFont font("宋体", font_size);
    painter.setFont(font);

    int cell_width = width() / GRID_AF_COL;
    int cell_height = height() / GRID_AF_ROW;
    const int x = cell_width - 30;
    const int y = cell_height - 5;

    for(int i = 0; i < GRID_AF_COL; i++) {
        for(int j = 0; j < GRID_AF_ROW; j++) {
            if (j > fv_table.size() || i > fv_table.first().size()) {
                painter.drawText(x + i*cell_width, y + j*cell_height, QString("0"));
            }else {
                painter.drawText(x + i*cell_width, y + j*cell_height, QString("%1").arg(fv_table[j][i]));
            }
        }
    }
}

void ImageLabel::enableGrid(bool en)
{
    this->enGrid = en;
}

void ImageLabel::enableRect(bool en)
{
    this->enRect = en;
}

void ImageLabel::setGridSize(int row, int col)
{
    this->grid_row = row;
    this->grid_col = col;
}

void ImageLabel::drawGrid()
{
    QPainter painter(this);
    painter.setPen(QPen(LINE_COLOR, 2));

    for (int i = 0; i < this->width(); i += this->width() / grid_col) {
        painter.drawLine(i, 0, i, height());
    }
    for (int i = 0; i < this->height(); i += this->height() / grid_row) {
        painter.drawLine(0, i, width(), i);
    }
}

void ImageLabel::drawRect()
{
    QPainter painter(this);
    painter.setPen(QPen(GRID_COLOR, 2));
    painter.setBrush(QBrush(GRID_COLOR));

    QPointF pf1, pf2;
    if (mode == GRID) {
        pf1 = QPointF(p1.x() * grid_width, p1.y() * grid_height);
        pf2 = QPointF(p2.x() * grid_width, p2.y() * grid_height);
    } else {
        pf1 = p1;
        pf2 = p2;
    }
    painter.drawRect(QRectF(pf1, pf2));
}

void ImageLabel::drawColorCheckerRect()
{
    QPainter painter(this);
    painter.setPen(QPen(RECT_COLOR, 2));

    painter.drawRects(color_checker_rects);
    painter.drawPoints(color_checker_corner_points);
}

void ImageLabel::updateColorCheckerRect()
{
    bool hasDragRect = !drag_color_rect.isEmpty();

    color_checker_rects.clear();
    color_checker_corner_points.clear();

    double rect_w, rect_h, center_x, center_y;
    if (hasDragRect) {
        // drag_color_rect should size should update with zoom
        rect_w = ((drag_color_rect.width()) / 6.0) - h_spacing + size_offset;
        rect_h = ((drag_color_rect.height()) / 4.0) - v_spacing + size_offset;

        center_x = drag_color_rect.x();
        center_y = drag_color_rect.y();
    } else {
        rect_w = ((width() * 0.7) / 6.0) - h_spacing + size_offset;
        rect_h = ((width() * 0.7) / 6.0) - v_spacing + size_offset;

        center_x = width() / 2.0;
        center_y = height() / 2.0;
    }

    for (int j = 0; j < 4; j++) {
        for (int i = 0; i < 6; i++) {
            double x = center_x - (3 - i) * (rect_w + h_spacing + h_interval) + x_offset;
            double y = center_y - (2 - j) * (rect_h + v_spacing + v_interval) + y_offset;

            if ((j == 0 && i == 0) || (j == 0 && i == 5) || (j == 3 && i == 0) || (j == 3 && i == 5)) {
                color_checker_corner_points.push_back(QPointF(x + rect_w/2, y + rect_h/2));
            }
            color_checker_rects.push_back(QRectF(x, y, rect_w, rect_h));
        }
    }
}

void ImageLabel::drawSingleRect()
{
    QPainter painter(this);
    painter.setPen(QPen(RECT_COLOR, 2));

    painter.drawRect(single_rect);
    if (!mouse_press && repaint_event) {
        emit sigUpdateSingleRect(single_rect, scale_factor);
    }
}

void ImageLabel::updateSingleRect()
{
    bool hasDragRect = !drag_color_rect.isEmpty();

    double rect_w, rect_h, center_x, center_y;
    double w, h, x, y;
    if (hasDragRect) {
        rect_w = drag_color_rect.width();
        rect_h = drag_color_rect.height();
        center_x = drag_color_rect.x();
        center_y = drag_color_rect.y();
    } else {
        if (!pixmap()) {
            return;
        }
        rect_w = width();
        rect_h = height() / 6.0;
        center_x = width() / 2.0;
        center_y = height() / 2.0  - height() / 12.0 + rect_h / 2.0;
    }

    w = rect_w;
    h = rect_h;
    x = center_x - w/2;
    y = center_y - h/2;
    single_rect.setRect(x, y, w, h);
}

void ImageLabel::updateSingleRect(int x, int y, int w, int h)
{
    double rect_x, rect_y, rect_w, rect_h, center_x, center_y;

    rect_x = x * scale_factor;
    rect_y = y * scale_factor;
    rect_w = w * scale_factor;
    rect_h = h * scale_factor;
    center_x = rect_x + rect_w/2;
    center_y = rect_y + rect_h/2;;

    drag_color_rect.setRect(center_x, center_y, rect_w, rect_h);
    single_rect.setRect(rect_x, rect_y, rect_w, rect_h);
    repaint();
}

QRectF ImageLabel::getSingleRect()
{
    return this->single_rect;
}

void ImageLabel::setRepaintEvent(bool state)
{
    this->repaint_event = state;
}

void ImageLabel::setMode(MODE mode)
{
    this->mode = mode;
}

ImageLabel::MODE ImageLabel::getMode()
{
    return this->mode;
}
