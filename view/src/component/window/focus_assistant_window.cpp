#include <QMessageBox>

#include "focus_assistant_window.hpp"
#include "network_client_helper.hpp"
#include "utils.hpp"
#include "global_data.hpp"

const int MIN_WINDOW_WIDTH = 1468;
const int MIN_WINDOW_HEIGHT = 988;
const int MAX_RECENT_DATA = 30;
const int MIN_CROP_WIDTH = 272;
const int MIN_CROP_HEIGHT = 240;

// Tool already supports H2 statistics, but H2 statiscs have some problems.
// static const QList<QString> fv_item_list{"V1", "H1", "H2"};
static const QList<QString> fv_item_list{"V1", "H1"};


FocusAssistantWindow::FocusAssistantWindow(QWidget *parent):
    QMainWindow(parent)
{
    setWindowTitle(tr("Focus Assistant"));
    setWindowFlags(Qt::WindowMaximizeButtonHint|Qt::WindowCloseButtonHint);

    timer = new QTimer(this);

    initUI();
    initEvent();
}

void FocusAssistantWindow::initUI()
{
    main_widget = new QWidget();
    main_widget->setMinimumSize(QSize(MIN_WINDOW_WIDTH, MIN_WINDOW_HEIGHT));
    main_widget->setMaximumSize(QSize(MIN_WINDOW_WIDTH, MIN_WINDOW_HEIGHT));
    main_layout = new QHBoxLayout();

    // Set left control items
    QVBoxLayout *left_layout = new QVBoxLayout;
    capture_btn = new QPushButton(tr("Capture Image"));
    grab_af_statistics = new QCheckBox(tr("Garb AF Statistics"));
    show_zone_fv = new QCheckBox(tr("Show Zone FV"));
    show_zone_fv->setChecked(true);
    select_all_zones_btn = new QPushButton(tr("Select All Zones"));
    deselect_all_zones_btn = new QPushButton(tr("Deselect All Zones"));
    reset_fv_chart_btn = new QPushButton(tr("Reset FV Chart"));

    QHBoxLayout *interval_layout = new QHBoxLayout;
    QLabel *interval_label = new QLabel(tr("interval:"));
    interval_label->setFixedSize(48, 24);
    interval_edit = new QLineEdit;
    interval_edit->setFixedSize(72, 24);
    interval_edit->setText("500");
    QLabel *unit_label = new QLabel(tr("ms"));
    unit_label->setFixedSize(24, 24);
    interval_layout->addWidget(interval_label);
    interval_layout->addWidget(interval_edit);
    interval_layout->addWidget(unit_label);

    QHBoxLayout *fv_item_layout = new QHBoxLayout;
    QLabel *fv_item_label = new QLabel(tr("FV Item:"));
    fv_item_label->setFixedSize(64, 24);
    fv_item_combobox = new QComboBox;
    fv_item_combobox->setFixedSize(80, 24);
    fv_item_combobox->addItems(fv_item_list);
    fv_item_combobox->setCurrentIndex(0);
    fv_item_layout->addWidget(fv_item_label);
    fv_item_layout->addWidget(fv_item_combobox);

    left_layout->addWidget(capture_btn);
    left_layout->addWidget(grab_af_statistics);
    left_layout->addLayout(interval_layout);
    left_layout->addLayout(fv_item_layout);
    left_layout->addWidget(show_zone_fv);
    left_layout->addWidget(select_all_zones_btn);
    left_layout->addWidget(deselect_all_zones_btn);
    left_layout->addWidget(reset_fv_chart_btn);

    // Set right image label and chart
    QVBoxLayout *right_layout = new QVBoxLayout;
    scroll_area = new QScrollArea;
    scroll_area->setFixedSize(1284, 724);
    scroll_area->setBackgroundRole(QPalette::Dark);
    img_label = new ImageLabel(this, ImageLabel::GRID_FOCUS_ASSIST);
    img_label->setScaleFactor(1.0);
    img_label->setHasFvTable(false);
    QPixmap image = QPixmap(1280, 720);
    image.fill(QColor("gray"));
    img_label->setPixmap(image);
    img_label->adjustSize();
    img_label->setScaledContents(true);
    img_label->getFvSelectedTable(fv_selected_table);
    scroll_area->setWidget(img_label);

    // Set series for chart
    line_max_series = new QtCharts::QLineSeries();
    line_max_series->append(1, 0);
    line_max_series->append(30, 0);
    line_series = new QtCharts::QLineSeries();
    scatter_series = new QtCharts::QScatterSeries();
    for (int i = 0; i < MAX_RECENT_DATA; i++) {
        line_series->append(i+1, 0);
        scatter_series->append(i+1, 0);
        fv_v1_recent.push_back(0);
        fv_h1_recent.push_back(0);
        fv_h2_recent.push_back(0);
    }

    // set axes for chart
    chart = new QtCharts::QChart();
    chart->legend()->hide();
    chart->addSeries(line_max_series);
    chart->addSeries(scatter_series);
    chart->addSeries(line_series);
    chart->setTitle("FV Curve");
    axis_x = new QtCharts::QValueAxis();
    axis_y = new QtCharts::QValueAxis();
    axis_x->setLabelFormat("%d");
    axis_y->setLabelFormat("%d");
    axis_x->setTickCount(2);
    axis_y->setTickCount(5);
    axis_x->setRange(1, 30);
    axis_y->setRange(0, 10000);
    axis_x->setTitleText("Recent FV");
    axis_y->setTitleText("FV Value");
    chart->setAxisX(axis_x, line_series);
    chart->setAxisY(axis_y, line_series);
    chart->setAxisX(axis_x, scatter_series);
    chart->setAxisY(axis_y, scatter_series);
    chart->setAxisX(axis_x, line_max_series);
    chart->setAxisY(axis_y, line_max_series);
    chart_view = new QtCharts::QChartView(chart);
    chart_view->setFixedSize(1280,240);
    chart_view->setRenderHint(QPainter::Antialiasing);

    right_layout->addWidget(scroll_area);
    right_layout->addWidget(chart_view);

    main_layout->addLayout(left_layout);
    main_layout->addLayout(right_layout);
    setCentralWidget(main_widget);
    centralWidget()->setLayout(main_layout);
    main_widget->move(160, 40);
}

void FocusAssistantWindow::initEvent()
{
    shared_ptr<NetworkClientHelper> network_helper = NetworkClientHelper::getInstance();
    connect(capture_btn, &QPushButton::clicked, this, &FocusAssistantWindow::slotSendCaptureImageCmd);
    connect(network_helper.get(), &NetworkClientHelper::sigAFYuvStatus, this, &FocusAssistantWindow::handleImageStatus);
    connect(network_helper.get(), &NetworkClientHelper::sigAFYuvData, this, &FocusAssistantWindow::slotUpdateImage);
    connect(timer, &QTimer::timeout, this, &FocusAssistantWindow::slotSendGrabDataCmd);
    connect(network_helper.get(), &NetworkClientHelper::sigAFStatisticsInfo, this, &FocusAssistantWindow::slotUpdateData);
    connect(show_zone_fv, &QCheckBox::clicked, this, &FocusAssistantWindow::slotSetShowFvText);
    connect(fv_item_combobox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &FocusAssistantWindow::slotSetCurrentFvTable);
    connect(select_all_zones_btn, &QPushButton::clicked, this, &FocusAssistantWindow::slotSelectAllZones);
    connect(deselect_all_zones_btn, &QPushButton::clicked, this, &FocusAssistantWindow::slotDeselectAllZones);
    connect(reset_fv_chart_btn, &QPushButton::clicked, this, &FocusAssistantWindow::slotResetFvChart);
    connect(grab_af_statistics, &QCheckBox::clicked, [=]{
        if (grab_af_statistics->isChecked()) {
            if (!NetworkClientHelper::getInstance()->isNormalStatus()) {
                grab_af_statistics->setCheckState(Qt::Unchecked);
                QMessageBox::warning(this, tr("Warning"), tr("Network is not connected!"), QMessageBox::Ok);
                return;
            }

            int interval = interval_edit->text().toInt();
            if (interval < 100 || interval > 1000) {
                grab_af_statistics->setCheckState(Qt::Unchecked);
                QMessageBox::warning(this, tr("Error"), tr("Please input a interval value between 100 to 1000 ms."), QMessageBox::Ok);
                return;
            }
            capture_btn->setEnabled(false);
            interval_edit->setEnabled(false);
            timer->start(interval);
            qDebug("Grab AF Statistics: interval = %d", interval);
        } else {
            interval_edit->setEnabled(true);
            timer->stop();
            capture_btn->setEnabled(true);
        }
    });
}

void FocusAssistantWindow::closeEvent(QCloseEvent *event)
{
    if (timer->isActive()) {
        grab_af_statistics->setChecked(false);
        interval_edit->setEnabled(true);
        timer->stop();
    }
    event->accept();
}

void FocusAssistantWindow::keyReleaseEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape) {
        this->close();
    }
}

inline void FocusAssistantWindow::cropImage(QImage &source, QPixmap &destination)
{
    int x = crop_x, y = crop_y, w = crop_w, h = crop_h;

    if(crop_enable) {
        if ((crop_x + MIN_CROP_WIDTH) > source.width())
            x = source.width() - MIN_CROP_WIDTH;

        if ((crop_y + MIN_CROP_HEIGHT) > source.height())
            y = source.height() - MIN_CROP_HEIGHT;

        if ((x + crop_w) > source.width())
            w = source.width() - x;

        if ((y + crop_h) > source.height()) {
            h = source.height() - y;
        }

        destination = QPixmap::fromImage(source.copy(x, y, w, h));
    } else {
        destination = QPixmap::fromImage(source);
    }
}

inline void FocusAssistantWindow::resetData()
{
    // reset image
    for(int i = 0; i < GRID_AF_ROW; i++) {
        for(int j = 0; j < GRID_AF_COL; j++) {
            fv_v1_now_table[i][j] = 0;
            fv_v1_max_table[i][j] = 0;
            fv_h1_now_table[i][j] = 0;
            fv_h1_max_table[i][j] = 0;
            fv_h2_now_table[i][j] = 0;
            fv_h2_max_table[i][j] = 0;
        }
    }

    img_label->setFvNowTable(fv_v1_now_table);
    img_label->setFvMaxTable(fv_v1_max_table);
    img_label->repaint();

    // rest fv curve
    for (int i = 0; i < MAX_RECENT_DATA; i++) {
        fv_v1_recent.at(i) = 0;
        fv_h1_recent.at(i) = 0;
        fv_h2_recent.at(i) = 0;
        line_series->replace(i, i+1, 0);
        scatter_series->replace(i, i+1, 0);
    }
}

void FocusAssistantWindow::handleImageStatus(int id)
{
    QJsonObject obj = NetworkClientHelper::getInstance()->popCmdDataJsonrpc(id);
    QJsonObject result_obj = obj[MSG_KEY_RESULT].toObject();

    if (result_obj.isEmpty()) {
        QMessageBox::warning(this, tr("Error"), "AF get single Image failed!", QMessageBox::Ok);
        return;
    }
    QJsonObject params_obj = result_obj[MSG_KEY_PARAMS].toObject();
    int size = params_obj["Data size"].toInt();
    NetworkClientHelper::getInstance()->setCallerInfo(size, CMD_AF_GET_SINGLE_YUV);
    qDebug("Set receive size:%d\n", size);

    int temp_id = GlobalData::getInstance()->getJsonrpcId();
    QJsonObject send_obj;
    send_obj[MSG_KEY_METHOD] = METHOD_GET_IMAGE_DATA;
    send_obj[MSG_KEY_ID] = temp_id;
    send_obj[MSG_KEY_JSONRPC] = MSG_JSONRPC_VERSION;
    NetworkClientHelper::getInstance()->sendCmd(std::move(send_obj));
}

void FocusAssistantWindow::slotUpdateImage(QString cmd)
{
    qDebug("Receive focus assitant image");
    NetworkClientHelper::getInstance()->setCallerInfo(0, NULL);
    QByteArray data = NetworkClientHelper::getInstance()->popCmdData(cmd);

    QByteArray header = data.left(sizeof(YUV_HEADER));
    YUV_HEADER *pstHeader = (YUV_HEADER *) header.data();
    data.remove(0, sizeof(YUV_HEADER));

    qDebug("numframe:%d, curframe:%d, width:%d, height:%d, stride:%d,%d,%d, format::%d\n", pstHeader->numFrame, pstHeader->curFrame,
        pstHeader->width, pstHeader->height, pstHeader->stride[0], pstHeader->stride[1], pstHeader->stride[2], pstHeader->pixelFormat);
    qDebug("data.size:%d, pstHeader->size:%d\n", data.size(), pstHeader->size);

    if (data.size() != (int)(pstHeader->size)) {
        QMessageBox::warning(this, tr("Error"), tr("Focus Assistant, The received yuv data size is wrong!"), QMessageBox::Ok);
        return;
    }

    if (!data.isEmpty()) {
        if (img_label)
        {
            image = utils::convertYUV2RGBImage(data, pstHeader);
            cropImage(image, crop_image);
            img_label->setPixmap(crop_image.scaled(1280, 720, Qt::KeepAspectRatio));
        }
    }
}

void FocusAssistantWindow::slotUpdateData(int id)
{
    qDebug("Receive focus assistant data");

    QJsonObject obj = NetworkClientHelper::getInstance()->popCmdDataJsonrpc(id);
    QJsonObject result_obj = obj[MSG_KEY_RESULT].toObject();

    if (result_obj.isEmpty()) {
        QMessageBox::warning(this, tr("Error"), "Get af statistics info failed!", QMessageBox::Ok);
        return;
    }
    QJsonObject params_obj = result_obj[MSG_KEY_PARAMS].toObject();

    // set crop info, windown info and json array
    bool reset_flag = false;
    bool af_enable = true, enable = crop_enable;
    int x = crop_x, y = crop_y, w = crop_w, h = crop_h;
    int hw = h_wnd, vw = v_wnd;
    QJsonArray v1_array, h1_array, h2_array;

    if (params_obj.contains(KEY_ATTR_AF_ENABLE)) {
        af_enable = params_obj[KEY_ATTR_AF_ENABLE].toInt();
    }
    h_wnd = params_obj[KEY_ARRT_H_WND].toInt();
    v_wnd = params_obj[KEY_ARRT_V_WND].toInt();
    crop_enable = params_obj[KEY_ARRT_CROP_ENABLE].toBool();
    crop_x = params_obj[KEY_ARRT_CROP_X].toInt();
    crop_y = params_obj[KEY_ARRT_CROP_Y].toInt();
    crop_w = params_obj[KEY_ARRT_CROP_W].toInt();
    crop_h = params_obj[KEY_ARRT_CROP_H].toInt();
    v1_array = params_obj[KEY_ATTR_AF_ZONE_V1].toArray();
    h1_array = params_obj[KEY_ATTR_AF_ZONE_H1].toArray();
    h2_array = params_obj[KEY_ATTR_AF_ZONE_H2].toArray();

    qDebug("af_enable = %d, Hwnd = %d, Vwnd = %d, crop_enable = %d, cropX = %d, cropY = %d, cropW = %d, cropH = %d",
           af_enable, h_wnd, v_wnd, crop_enable, crop_x, crop_y, crop_w, crop_h);

    if (!af_enable) {
        return;
    }

    if ((h_wnd != 0) && (v_wnd != 0) && (!v1_array.isEmpty()) && (!h1_array.isEmpty()) && (!h2_array.isEmpty())) {
        int count = 0;
        fv_v1_avg_selected = 0;
        fv_h1_avg_selected = 0;
        fv_h2_avg_selected = 0;
        img_label->getFvSelectedTable(fv_selected_table);

        if ((enable != crop_enable) || (hw != h_wnd) || (vw != v_wnd)) {
            reset_flag = true;
        } else if ((crop_enable) && ((x != crop_x) || (y != crop_y) || (w != crop_w) || (h != crop_h))) {
            reset_flag = true;
        }

        if (reset_flag) {
            cropImage(image, crop_image);
            img_label->setPixmap(crop_image.scaled(1280, 720, Qt::KeepAspectRatio));
            img_label->setWindowInfo(h_wnd, v_wnd);
            img_label->setFvSelectedTable(false);
            resetData();
            return;
        }

        // set tables from json array
        for (int i = 0; i < v_wnd; i++) {
            QJsonArray v1_row = v1_array[i].toArray();
            QJsonArray h1_row = h1_array[i].toArray();
            QJsonArray h2_row = h2_array[i].toArray();
            for (int j = 0; j < h_wnd; j++) {
                fv_v1_now_table[i][j] = v1_row[j].toInt();
                fv_h1_now_table[i][j] = h1_row[j].toInt();
                fv_h2_now_table[i][j] = h2_row[j].toInt();
                if (fv_v1_max_table[i][j] < v1_row[j].toInt()) {
                    fv_v1_max_table[i][j] = v1_row[j].toInt();
                }

                if (fv_h1_max_table[i][j] < h1_row[j].toInt()) {
                    fv_h1_max_table[i][j] = h1_row[j].toInt();
                }

                if (fv_h2_max_table[i][j] < h2_row[j].toInt()) {
                    fv_h2_max_table[i][j] = h2_row[j].toInt();
                }

                if (fv_selected_table[i][j] == true) {
                    count++;
                    fv_v1_avg_selected += v1_row[j].toInt();
                    fv_h1_avg_selected += h1_row[j].toInt();
                    fv_h2_avg_selected += h2_row[j].toInt();
                }
            }
        }

        // reset max value of selected zones
        if (zone_num_selected != count) {
            fv_v1_max_selected = 0;
            fv_h1_max_selected = 0;
            fv_h2_max_selected = 0;
            zone_num_selected = count;
        }

        // set avg and max values of selected zones
        if (count != 0) {
            fv_v1_avg_selected /= count;
            fv_h1_avg_selected /= count;
            fv_h2_avg_selected /= count;
            fv_v1_recent.pop_back();
            fv_v1_recent.insert(fv_v1_recent.begin(), fv_v1_avg_selected);
            fv_h1_recent.pop_back();
            fv_h1_recent.insert(fv_h1_recent.begin(), fv_h1_avg_selected);
            fv_h2_recent.pop_back();
            fv_h2_recent.insert(fv_h2_recent.begin(), fv_h2_avg_selected);

            if (fv_v1_max_selected < fv_v1_avg_selected) {
                fv_v1_max_selected = fv_v1_avg_selected;
            }

            if (fv_h1_max_selected < fv_h1_avg_selected) {
                fv_h1_max_selected = fv_h1_avg_selected;
            }

            if (fv_h2_max_selected < fv_h2_avg_selected) {
                fv_h2_max_selected = fv_h2_avg_selected;
            }
        }
    }
    else {
        qDebug("Received focus assistant data is wrong");
        return;
    }

    if (fv_item_combobox->currentIndex() == 0) {
        img_label->setFvNowTable(fv_v1_now_table);
        img_label->setFvMaxTable(fv_v1_max_table);
    } else if (fv_item_combobox->currentIndex() == 1) {
        img_label->setFvNowTable(fv_h1_now_table);
        img_label->setFvMaxTable(fv_h1_max_table);
    } else if (fv_item_combobox->currentIndex() == 2) {
        img_label->setFvNowTable(fv_h2_now_table);
        img_label->setFvMaxTable(fv_h2_max_table);
    }

    img_label->setWindowInfo(h_wnd, v_wnd);
    img_label->repaint();
    updateFvCurve();
}

void FocusAssistantWindow::slotSendCaptureImageCmd()
{
    if (!NetworkClientHelper::getInstance()->isNormalStatus()) {
        QMessageBox::warning(this, tr("Warning"), tr("Network is not connected!"), QMessageBox::Ok);
        return;
    }

    NetworkClientHelper::getInstance()->setCallerInfo(0, NULL);

    QJsonObject id_info;
    id_info[MSG_KEY_CMD] = CMD_AF_GET_SINGLE_YUV;
    int id = GlobalData::getInstance()->getJsonrpcId();
    NetworkClientHelper::getInstance()->pushIdInfoJsonrpc(id, id_info);

    QJsonObject send_obj;
    send_obj[MSG_KEY_METHOD] = METHOD_GET_SINGLE_YUV;
    send_obj[MSG_KEY_ID] = id;
    send_obj[MSG_KEY_JSONRPC] = MSG_JSONRPC_VERSION;
    NetworkClientHelper::getInstance()->sendCmd(std::move(send_obj));
}

void FocusAssistantWindow::slotSendGrabDataCmd()
{
    if (!NetworkClientHelper::getInstance()->isNormalStatus()) {
        timer->stop();
        capture_btn->setEnabled(true);
        grab_af_statistics->setCheckState(Qt::Unchecked);
        QMessageBox::warning(this, tr("Warning"), tr("Network is not connected!"), QMessageBox::Ok);
        return;
    }

    QJsonObject id_info;
    id_info[MSG_KEY_CMD] = CMD_AF_GET_STATISTICS_INFO;
    int id = GlobalData::getInstance()->getJsonrpcId();
    NetworkClientHelper::getInstance()->pushIdInfoJsonrpc(id, id_info);

    QJsonObject send_obj;
    send_obj[MSG_KEY_METHOD] = METHOD_GET_AF_STATISTICS_INFO;
    send_obj[MSG_KEY_ID] = id;
    send_obj[MSG_KEY_JSONRPC] = MSG_JSONRPC_VERSION;
    NetworkClientHelper::getInstance()->sendCmd(std::move(send_obj));
}

void FocusAssistantWindow::slotSetShowFvText()
{
    if (show_zone_fv->isChecked()) {
        img_label->setShowFvText(true);
    } else {
        img_label->setShowFvText(false);
    }

    img_label->repaint();
}

void FocusAssistantWindow::slotSetCurrentFvTable(int index)
{
    if (index == 0) {
        img_label->setFvNowTable(fv_v1_now_table);
        img_label->setFvMaxTable(fv_v1_max_table);
    } else if (index == 1) {
        img_label->setFvNowTable(fv_h1_now_table);
        img_label->setFvMaxTable(fv_h1_max_table);
    } else if (index == 2) {
        img_label->setFvNowTable(fv_h2_now_table);
        img_label->setFvMaxTable(fv_h2_max_table);
    }

    img_label->repaint();

    if (!grab_af_statistics->isChecked()) {
        updateFvCurve();
    }
}

void FocusAssistantWindow::slotSelectAllZones()
{
    img_label->setFvSelectedTable(true);
    img_label->repaint();
}

void FocusAssistantWindow::slotDeselectAllZones()
{
    img_label->setFvSelectedTable(false);
    img_label->repaint();
}

void FocusAssistantWindow::slotResetFvChart()
{
    // stop grab af statistics to reset all tables
    if (timer->isActive()) {
        timer->stop();
    }

    resetData();

    if (grab_af_statistics->isChecked()) {
        timer->start(interval_edit->text().toInt());
    }
}

void FocusAssistantWindow::updateFvCurve()
{
    int fv_max_recent = 0;

    if (fv_item_combobox->currentIndex() == 0) {
        line_max_series->replace(0, 1, fv_v1_max_selected);
        line_max_series->replace(1, 30, fv_v1_max_selected);
        for (int i = 0; i < MAX_RECENT_DATA; i++) {
            line_series->replace(i, i+1, fv_v1_recent.at(i));
            scatter_series->replace(i, i+1, fv_v1_recent.at(i));
            if (fv_max_recent < fv_v1_recent.at(i))
                fv_max_recent = fv_v1_recent.at(i);
        }
    } else if (fv_item_combobox->currentIndex() == 1) {
        line_max_series->replace(0, 1, fv_h1_max_selected);
        line_max_series->replace(1, 30, fv_h1_max_selected);
        for (int i = 0; i < MAX_RECENT_DATA; i++) {
            line_series->replace(i, i+1, fv_h1_recent.at(i));
            scatter_series->replace(i, i+1, fv_h1_recent.at(i));
            if (fv_max_recent < fv_h1_recent.at(i))
                fv_max_recent = fv_h1_recent.at(i);
        }
    } else if (fv_item_combobox->currentIndex() == 2) {
        line_max_series->replace(0, 1, fv_h2_max_selected);
        line_max_series->replace(1, 30, fv_h2_max_selected);
        for (int i = 0; i < MAX_RECENT_DATA; i++) {
            line_series->replace(i, i+1, fv_h2_recent.at(i));
            scatter_series->replace(i, i+1, fv_h2_recent.at(i));
            if (fv_max_recent < fv_h2_recent.at(i))
                fv_max_recent = fv_h2_recent.at(i);
        }
    }

    axis_y->setRange(0, fv_max_recent * 1.2);
}

void FocusAssistantWindow::handleUpdateTheme(bool dark_theme_enabled)
{
    if (dark_theme_enabled) {
        chart->setBackgroundBrush(QColor(25, 35, 45));
        axis_x->setLabelsColor(QColor(224, 225, 226));
        axis_y->setLabelsColor(QColor(224, 225, 226));
    } else {
        chart->setBackgroundBrush(QColor(255, 255, 255));
        axis_x->setLabelsColor(QColor(0, 0, 0));
        axis_y->setLabelsColor(QColor(0, 0, 0));
    }
}
