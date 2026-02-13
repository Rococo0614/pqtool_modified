#include "drc_tone_viewer_window.hpp"
#include "network_client_helper.hpp"
#include "main_window.hpp"
#include "global_data.hpp"

const int MIN_WINDOW_WIDTH = 1200;
const int MIN_WINDOW_HEIGHT = 600;
const int LINEAR_BIN_NUM = 256;
const int WDR_BIN_NUM = 769;
const int PIXEL_MAX_VALUE = 255;

DrcToneViewerWindow::DrcToneViewerWindow(QWidget *parent):
    QMainWindow(parent)
{
    setWindowTitle(tr("DRC Tone Viewer"));
    setWindowFlags(Qt::WindowMinMaxButtonsHint|Qt::WindowCloseButtonHint);

    initUI();
    initEvent();
}

void DrcToneViewerWindow::initUI()
{
    main_widget = new QWidget();
    main_widget->setMinimumSize(QSize(MIN_WINDOW_WIDTH, MIN_WINDOW_HEIGHT));
    main_layout = new QVBoxLayout();

    initTopArea();
    initCenterArea();

    main_layout->setStretch(0, 1);
    main_layout->setStretch(1, 12);
    setCentralWidget(main_widget);
    centralWidget()->setLayout(main_layout);
}

void DrcToneViewerWindow::initTopArea()
{
    initItems();
    QHBoxLayout *top_layout = new QHBoxLayout();
    top_layout->setContentsMargins(100, 0, 100, 0);

    if (GlobalData::getInstance()->getCvipqToolSoc() == CV184X_SOC) {
        QHBoxLayout *choose_layout = new QHBoxLayout;
        QLabel *color_title_label = new QLabel(tr("Choose Histogram"));
        choose_layout->setAlignment(Qt::AlignRight);
        choose_layout->addWidget(color_title_label);
        choose_layout->addWidget(color_combobox);
        top_layout->addWidget(update_btn);
        top_layout->addLayout(choose_layout);
        step_edit->hide();
        expRatio_edit->hide();
        connect(color_combobox, &QComboBox::currentTextChanged, this, &DrcToneViewerWindow::slotHandleSelectColor);
    } else {
        QHBoxLayout *step_layout = new QHBoxLayout;
        QLabel *step_label = new QLabel("Global Tone Bin SE Step:");
        step_layout->addWidget(step_label);
        step_layout->addWidget(step_edit, 1, Qt::AlignLeft);
        top_layout->addLayout(step_layout);
        QHBoxLayout *exp_layout = new QHBoxLayout;
        QLabel *exp_label = new QLabel("Exposure Ratio:");
        exp_layout->addWidget(exp_label);
        exp_layout->addWidget(expRatio_edit, 1, Qt::AlignLeft);
        top_layout->addLayout(exp_layout);
        color_combobox->hide();
        top_layout->addWidget(update_btn);
    }
    main_layout->addLayout(top_layout);
}

void DrcToneViewerWindow::initCenterArea()
{
    QString soc = GlobalData::getInstance()->getCvipqToolSoc();
    if (soc == CV182X_SOC) {
        initCharts();
        QWidget *scroll_widget = new QWidget;
        QHBoxLayout *scroll_layout = new QHBoxLayout(scroll_widget);
        QGridLayout *grid = new QGridLayout;
        grid->addWidget(global_tone_curve_widget, 0, 0);
        grid->addWidget(dark_tone_curve_widget, 1, 0);
        grid->addWidget(bright_tone_curve_widget, 0, 1);
        scroll_layout->addLayout(grid);

        QScrollArea *scroll_area = new QScrollArea();
        scroll_area->setWidget(scroll_widget);
        main_layout->addWidget(scroll_area);
    } else if (soc == CV184X_SOC) {
        initChartsMars3();
    } else {
        initChartsNew();
    }
}

void DrcToneViewerWindow::initItems()
{
    QString module = "DRC Tone Viewer";

    update_btn = new QPushButton("Update All Charts");
    update_btn->setMaximumWidth(200);
    connect(update_btn, &QPushButton::clicked, this, &DrcToneViewerWindow::slotSendGetDrcHistogramInfoCmd);

    step_edit = new QLineEdit("0");
    step_edit->setFixedWidth(80);
    step_edit->setReadOnly(true);

    expRatio_edit = new QLineEdit("0");
    expRatio_edit->setFixedWidth(80);
    expRatio_edit->setReadOnly(true);

    color_combobox = new QComboBox;
    color_list.append(tr("ALL"));
    color_list.append(tr("DRC"));
    color_list.append(tr("DCI"));
    color_list.append(tr("RLSC R,G,B"));
    color_list.append(tr("RLSC R"));
    color_list.append(tr("RLSC G"));
    color_list.append(tr("RLSC B"));
    color_list.append(tr("AE"));
    for (auto &color_name : color_list) {
        color_combobox->addItem(color_name);
    }
}

void DrcToneViewerWindow::initChartsNew()
{
    g_series = new QtCharts::QLineSeries();
    b_series = new QtCharts::QLineSeries();
    d_series = new QtCharts::QLineSeries();
    l_series = new QtCharts::QLineSeries();

    QPen gpen, bpen, dpen, lpen;
    gpen.setColor(QColor(255, 0, 0));
    gpen.setWidth(3);
    g_series->setPen(gpen);
    bpen.setColor(QColor(255, 255, 0));
    bpen.setWidth(3);
    b_series->setPen(bpen);
    dpen.setColor(QColor(0, 255, 255));
    dpen.setWidth(3);
    d_series->setPen(dpen);
    lpen.setColor(QColor(0, 0, 255));
    lpen.setWidth(2);
    l_series->setPen(lpen);

    g_series->setName("global tone");
    b_series->setName("bright tone");
    d_series->setName("dark tone");
    l_series->setName("linear tone");

    chart = new QtCharts::QChart();
    chart->addSeries(g_series);
    chart->addSeries(b_series);
    chart->addSeries(d_series);
    chart->addSeries(l_series);
    chart->setTitle("drc tone curve");

    axis_x = new QtCharts::QValueAxis();
    axis_x->setRange(0, 65535);
    axis_x->setTickCount(9);
    axis_y = new QtCharts::QValueAxis();
    axis_y->setRange(0, 65535);
    axis_y->setTickCount(9);
    chart->setAxisX(axis_x);
    chart->setAxisY(axis_y);
    g_series->attachAxis(axis_x);
    g_series->attachAxis(axis_y);
    b_series->attachAxis(axis_x);
    b_series->attachAxis(axis_y);
    d_series->attachAxis(axis_x);
    d_series->attachAxis(axis_y);
    l_series->attachAxis(axis_x);
    l_series->attachAxis(axis_y);
    QtCharts::QChartView *chartView = new QtCharts::QChartView(chart);

    main_layout->addWidget(chartView);
}

void DrcToneViewerWindow::initChartsMars3()
{
    d_series = new QtCharts::QLineSeries();
    l_series = new QtCharts::QLineSeries();

    QPen dpen, lpen;
    dpen.setColor(QColor(0, 255, 255));
    dpen.setWidth(2);
    d_series->setPen(dpen);
    lpen.setColor(QColor(0, 0, 255));
    lpen.setWidth(2);
    l_series->setPen(lpen);

    d_series->setName("outbld curve");
    l_series->setName("linear curve");

    chart = new QtCharts::QChart();
    chart->addSeries(l_series);
    chart->addSeries(d_series);
    chart->setTitle("drc tone curve");

    axis_x = new QtCharts::QValueAxis();
    axis_x->setRange(0, 65536);
    axis_x->setTickCount(9);
    axis_y = new QtCharts::QValueAxis();
    axis_y->setRange(0, 65536);
    axis_y->setTickCount(9);
    chart->setAxisX(axis_x);
    chart->setAxisY(axis_y);
    d_series->attachAxis(axis_x);
    d_series->attachAxis(axis_y);
    l_series->attachAxis(axis_x);
    l_series->attachAxis(axis_y);
    QtCharts::QChartView *chartView = new QtCharts::QChartView(chart);

    drc_series = new QtCharts::QLineSeries();
    drc_area_series = new QtCharts::QAreaSeries(drc_series);
    drc_area_series->setPen(QColor(255, 0, 255));
    drc_area_series->setBrush(QColor(255, 0, 255, 50));
    drc_area_series->setName("DRC");

    dci_series = new QtCharts::QLineSeries();
    dci_area_series = new QtCharts::QAreaSeries(dci_series);
    dci_area_series->setPen(QColor(255, 255, 0));
    dci_area_series->setBrush(QColor(255, 255, 0, 50));
    dci_area_series->setName("DCI");

    rlsc_r_series = new QtCharts::QLineSeries();
    rlsc_r_area_series = new QtCharts::QAreaSeries(rlsc_r_series);
    rlsc_r_area_series->setPen(QColor(255, 0, 0));
    rlsc_r_area_series->setBrush(QColor(255, 0, 0, 50));
    rlsc_r_area_series->setName("RLSC R");

    rlsc_g_series = new QtCharts::QLineSeries();
    rlsc_g_area_series = new QtCharts::QAreaSeries(rlsc_g_series);
    rlsc_g_area_series->setPen(QColor(0, 255, 0));
    rlsc_g_area_series->setBrush(QColor(0, 255, 0, 50));
    rlsc_g_area_series->setName("RLSC G");

    rlsc_b_series = new QtCharts::QLineSeries();
    rlsc_b_area_series = new QtCharts::QAreaSeries(rlsc_b_series);
    rlsc_b_area_series->setPen(QColor(0, 0, 255));
    rlsc_b_area_series->setBrush(QColor(0, 0, 255, 50));
    rlsc_b_area_series->setName("RLSC B");

    ae_series = new QtCharts::QLineSeries();
    ae_area_series = new QtCharts::QAreaSeries(ae_series);
    ae_area_series->setPen(QColor(0, 255, 255));
    ae_area_series->setBrush(QColor(0, 255, 255, 50));
    ae_area_series->setName("AE");

    h_chart = new QtCharts::QChart();
    h_chart->addSeries(drc_area_series);
    h_chart->addSeries(dci_area_series);
    h_chart->addSeries(rlsc_r_area_series);
    h_chart->addSeries(rlsc_g_area_series);
    h_chart->addSeries(rlsc_b_area_series);
    h_chart->addSeries(ae_area_series);
    h_chart->setTitle("Histogram");
    h_chart->createDefaultAxes();
    h_chart->axes(Qt::Horizontal).first()->setRange(0, PIXEL_MAX_VALUE + 1);
    QtCharts::QChartView *h_chartView = new QtCharts::QChartView(h_chart);

    QHBoxLayout *chart_layout = new QHBoxLayout;
    chart_layout->addWidget(chartView);
    chart_layout->addWidget(h_chartView);

    main_layout->addLayout(chart_layout);

    ae_hist_matrix = nullptr;
    ae_info_page = MainWindow::getInstance()->getPageWidget(QString("Exposure Info"));
    QList<BaseItemWidget *> base_item_list = ae_info_page->getBaseItemList();
    for (int i = 0; i < base_item_list.size(); i++) {
        bool is_find = false;
        BaseItemWidget *item = base_item_list[i];
        QStringList path_list = item->getPathList();
        for (int j = 0; j < path_list.count(); j++) {
            if (path_list[j] == "ISP_EXP_INFO_S.au32AE_Hist256Value") {
                ae_hist_matrix = (UniversalMatrixItemWidget *) item;
                is_find = true;
                break;
            }
        }
        if (is_find) {
            break;
        }
    }
}

void DrcToneViewerWindow::initCharts()
{
    QString module = "DRC Tone Viewer";

    global_tone_curve_widget = new HistItemWidget(module, "WDR Curve", GLOBAL_TONE_CURVE);
    global_tone_curve_widget->addRegSectionKey("DRC", "reg_wdr_tone_curve");
    global_tone_curve_widget->hideSeries();

    dark_tone_curve_widget = new HistItemWidget(module, "Dark Tone Curve", DARK_TONE_CURVE);
    dark_tone_curve_widget->addRegSectionKey("DRC", "reg_dark_tone_curve");
    dark_tone_curve_widget->hideSeries();

    bright_tone_curve_widget = new HistItemWidget(module, "Bright Tone Curve", BRIGHT_TONE_CURVE);
    bright_tone_curve_widget->addRegSectionKey("DRC", "reg_bright_tone_curve");
    bright_tone_curve_widget->hideSeries();
}

void DrcToneViewerWindow::initEvent()
{
    shared_ptr<NetworkClientHelper> network_helper = NetworkClientHelper::getInstance();
    connect(network_helper.get(), &NetworkClientHelper::sigRecDrcHistogramInfo, this, &DrcToneViewerWindow::slotUpdateAllCharts);
}

void DrcToneViewerWindow::slotSendGetDrcHistogramInfoCmd()
{
    if (!NetworkClientHelper::getInstance()->isNormalStatus()) {
        QMessageBox::warning(this, tr("Warning"), tr("Network is not connected!"), QMessageBox::Ok);
        return;
    }

    NetworkClientHelper::getInstance()->setCallerInfo(0, NULL);

    QJsonObject id_info;
    id_info[MSG_KEY_CMD] = CMD_GET_DRC_HISTOGRAM_INFO;
    int id = GlobalData::getInstance()->getJsonrpcId();
    NetworkClientHelper::getInstance()->pushIdInfoJsonrpc(id, id_info);

    QJsonObject send_obj;
    send_obj[MSG_KEY_METHOD] = METHOD_GET_DRC_HISTOGRAM_INFO;
    send_obj[MSG_KEY_ID] = id;
    send_obj[MSG_KEY_JSONRPC] = MSG_JSONRPC_VERSION;
    NetworkClientHelper::getInstance()->sendCmd(std::move(send_obj));
}

void DrcToneViewerWindow::slotUpdateAllCharts(int id)
{
    QJsonObject obj = NetworkClientHelper::getInstance()->popCmdDataJsonrpc(id);
    QJsonObject result_obj = obj[MSG_KEY_RESULT].toObject();
    if (result_obj.isEmpty()) {
        QMessageBox::warning(this, tr("Error"), "Failed to update all charts.", QMessageBox::Ok);
        return;
    }

    params_obj = result_obj[MSG_KEY_PARAMS].toObject();
    int bin_num = 0;
    if (params_obj.contains("GlobalToneBinNum")) {
        bin_num = params_obj["GlobalToneBinNum"].toInt();
    }

    int bin_se_step = 0;
    if (params_obj.contains("GlobalToneBinSEStep")) {
        bin_se_step = params_obj["GlobalToneBinSEStep"].toInt();
    }
    step_edit->setText(QString("%1").arg(bin_se_step));

    int exp_ratio = 0;
    if (params_obj.contains("Exposure Ratio")) {
        exp_ratio = params_obj["Exposure Ratio"].toInt();
    }
    expRatio_edit->setText(QString("%1").arg(exp_ratio));

    QString soc = GlobalData::getInstance()->getCvipqToolSoc();
    if (soc == CV182X_SOC) {
        int y_max = 4095;
        if (params_obj.contains("GlobalToneCurve") && bin_num != 0) {
            QList<QVariant> list;
            QJsonArray value_array = params_obj["GlobalToneCurve"].toArray();
            if (value_array.count() == WDR_BIN_NUM) {
                for (int i = 0, x = 0; i < WDR_BIN_NUM; i++) {
                    if (i > 256) {
                        x = 256 + (i - 256) * bin_se_step;
                    } else {
                        x = i;
                    }

                    list.append(QPointF(x, value_array.at(i).toDouble()));
                    if (value_array.at(i).toInt() >= y_max) {
                        break;
                    }
                }
            } else if (value_array.count() == LINEAR_BIN_NUM) {
                for (int i = 0; i < LINEAR_BIN_NUM; i++) {
                    list.append(QPointF(i, value_array.at(i).toDouble()));
                }
            }

            global_tone_curve_widget->setData(list);
            HistChartView * curve = global_tone_curve_widget->getHisChartView();
            curve->setDisplayRange(0, list[list.count()-1].toPointF().x());
            global_tone_curve_widget->showSeries();
        } else {
            global_tone_curve_widget->hideSeries();
        }

        if (params_obj.contains("DarkToneCurve")) {
            QList<QVariant> list;
            QJsonArray value_array = params_obj["DarkToneCurve"].toArray();
            for (int i = 0; i < value_array.count(); i++)
                list.append(QPointF(i, value_array.at(i).toDouble()));

            dark_tone_curve_widget->setData(list);
            dark_tone_curve_widget->showSeries();
        } else {
            dark_tone_curve_widget->hideSeries();
        }

        if (params_obj.contains("BrightToneCurve")) {
            QList<QVariant> list;
            QJsonArray value_array = params_obj["BrightToneCurve"].toArray();
            for (int i = 0; i < value_array.count(); i++)
                list.append(QPointF(i, value_array.at(i).toDouble()));

            bright_tone_curve_widget->setData(list);
            bright_tone_curve_widget->showSeries();
        } else {
            bright_tone_curve_widget->hideSeries();
        }

    } else if (soc == CV184X_SOC) {
        d_series->clear();
        l_series->clear();
        int y_max = 0, x_max = 0;

        if (params_obj.contains("DrcOutbldCurveL")) {
            QJsonArray value_array = params_obj["DrcOutbldCurveL"].toArray();
            for (int i = 0, x = 0; i < value_array.count(); i++) {
                x = i << 5;
                *d_series << QPointF(x, value_array.at(i).toDouble());
                y_max = (value_array.at(i).toInt() > y_max) ? value_array.at(i).toInt() : y_max;
                x_max = (x > x_max) ? x : x_max;
                if (value_array.at(i).toInt() >= 65535) {
                    break;
                }
            }
        }
        if (params_obj.contains("DrcOutbldCurveR")) {
            QJsonArray value_array = params_obj["DrcOutbldCurveR"].toArray();
            for (int i = 0, x = 0; i < value_array.count(); i++) {
                x = (i << 7) + 2048;
                *d_series << QPointF(x, value_array.at(i).toDouble());
                y_max = (value_array.at(i).toInt() > y_max) ? value_array.at(i).toInt() : y_max;
                x_max = (x > x_max) ? x : x_max;
                if (value_array.at(i).toInt() >= 65535) {
                    break;
                }
            }
        }
        if ((y_max > 0) && (x_max > 0)) {
            *l_series << QPointF(0, 0);
            *l_series << QPointF(x_max, y_max);
            axis_x->setRange(0, x_max);
            axis_y->setRange(0, y_max);
        }
        updateHistCharts(color_combobox->currentIndex());
    } else {
        g_series->clear();
        d_series->clear();
        b_series->clear();
        l_series->clear();
        int y_max = 0, x_max = 0;
        if (params_obj.contains("GlobalToneCurve")) {
            QJsonArray value_array = params_obj["GlobalToneCurve"].toArray();
            for (int i = 0, x = 0; i < value_array.count(); i++) {
                if (i > 256) {
                    x = 4096 + (i - 256) * bin_se_step * 16;
                } else {
                    x = i * 16;
                }

                *g_series << QPointF(x, value_array.at(i).toDouble());
                y_max = (value_array.at(i).toInt() > y_max) ? value_array.at(i).toInt() : y_max;
                x_max = (x > x_max) ? x : x_max;
                if (value_array.at(i).toInt() >= 65535) {
                    break;
                }
            }
        }

        if (params_obj.contains("DarkToneCurve")) {
            QJsonArray value_array = params_obj["DarkToneCurve"].toArray();
            for (int i = 0; i < value_array.count(); i++) {
                int x = i * 16;
                *d_series << QPointF(x, value_array.at(i).toDouble());
                y_max = (value_array.at(i).toInt() > y_max) ? value_array.at(i).toInt() : y_max;
                x_max = (x > x_max) ? x : x_max;
                if (value_array.at(i).toInt() >= 65535) {
                    break;
                }
            }
        }

        if (params_obj.contains("BrightToneCurve")) {
            QJsonArray value_array = params_obj["BrightToneCurve"].toArray();
            for (int i = 0; i < value_array.count(); i++) {
                int x = i * bin_se_step * 16;
                *b_series << QPointF(x, value_array.at(i).toDouble());
                y_max = (value_array.at(i).toInt() > y_max) ? value_array.at(i).toInt() : y_max;
                x_max = (x > x_max) ? x : x_max;
                if (value_array.at(i).toInt() >= 65535) {
                    break;
                }
            }
        }

        if ((y_max > 0) && (x_max > 0)) {
            *l_series << QPointF(0, 0);
            *l_series << QPointF(x_max, y_max);
            axis_x->setRange(0, x_max);
            axis_y->setRange(0, y_max);
        }
    }
}

void DrcToneViewerWindow::updateHistCharts(int idx)
{
    drc_series->clear();
    dci_series->clear();
    rlsc_r_series->clear();
    rlsc_g_series->clear();
    rlsc_b_series->clear();
    ae_series->clear();

    uint u_max = 0;
    if (params_obj.contains("DrcHist") && (idx == 0 || idx == 1)) {
        QJsonArray value_array = params_obj["DrcHist"].toArray();
        for (int i = 0; i < value_array.count(); i++) {
            *drc_series << QPointF(i * 2, value_array.at(i).toVariant().toUInt());
            u_max = (value_array.at(i).toVariant().toUInt() > u_max) ? value_array.at(i).toVariant().toUInt() : u_max;
        }
    }
    if (params_obj.contains("DciHist") && (idx == 0 || idx == 2)) {
        QJsonArray value_array = params_obj["DciHist"].toArray();
        for (int i = 0; i < value_array.count(); i++) {
            *dci_series << QPointF(i, value_array.at(i).toVariant().toUInt());
            u_max = (value_array.at(i).toVariant().toUInt() > u_max) ? value_array.at(i).toVariant().toUInt() : u_max;
        }
    }
    if (params_obj.contains("RlscHistR") && (idx == 0 || idx == 3 || idx == 4)) {
        QJsonArray value_array = params_obj["RlscHistR"].toArray();
        for (int i = 0; i < value_array.count(); i++) {
            *rlsc_r_series << QPointF(i, value_array.at(i).toVariant().toUInt());
            u_max = (value_array.at(i).toVariant().toUInt() > u_max) ? value_array.at(i).toVariant().toUInt() : u_max;
        }
    }
    if (params_obj.contains("RlscHistG") && (idx == 0 || idx == 3 || idx == 5)) {
        QJsonArray value_array = params_obj["RlscHistG"].toArray();
        for (int i = 0; i < value_array.count(); i++) {
            *rlsc_g_series << QPointF(i, value_array.at(i).toVariant().toUInt());
            u_max = (value_array.at(i).toVariant().toUInt() > u_max) ? value_array.at(i).toVariant().toUInt() : u_max;
        }
    }
    if (params_obj.contains("RlscHistB") && (idx == 0 || idx == 3 || idx == 6)) {
        QJsonArray value_array = params_obj["RlscHistB"].toArray();
        for (int i = 0; i < value_array.count(); i++) {
            *rlsc_b_series << QPointF(i, value_array.at(i).toVariant().toUInt());
            u_max = (value_array.at(i).toVariant().toUInt() > u_max) ? value_array.at(i).toVariant().toUInt() : u_max;
        }
    }
    if (ae_hist_matrix && (idx == 0 || idx == 7)) {
        QVariant data = ae_hist_matrix->getData();
        QVariantList value_array = data.toList();
        for (int i = 0; i < value_array.size(); i++) {
            *ae_series << QPointF(i, value_array.at(i).toUInt());
            u_max = (value_array.at(i).toUInt() > u_max) ? value_array.at(i).toUInt() : u_max;
        }
    }
    h_chart->axes(Qt::Vertical).first()->setRange(0, u_max);
}

void DrcToneViewerWindow::slotHandleSelectColor(const QString &text)
{
    int color_idx = color_list.indexOf(text);
    if (color_idx == 1) {
        drc_area_series->show();
        dci_area_series->hide();
        rlsc_r_area_series->hide();
        rlsc_g_area_series->hide();
        rlsc_b_area_series->hide();
        ae_area_series->hide();
    } else if (color_idx == 2) {
        drc_area_series->hide();
        dci_area_series->show();
        rlsc_r_area_series->hide();
        rlsc_g_area_series->hide();
        rlsc_b_area_series->hide();
        ae_area_series->hide();
    } else if (color_idx == 3) {
        drc_area_series->hide();
        dci_area_series->hide();
        rlsc_r_area_series->show();
        rlsc_g_area_series->show();
        rlsc_b_area_series->show();
        ae_area_series->hide();
    } else if (color_idx == 4) {
        drc_area_series->hide();
        dci_area_series->hide();
        rlsc_r_area_series->show();
        rlsc_g_area_series->hide();
        rlsc_b_area_series->hide();
        ae_area_series->hide();
    } else if (color_idx == 5) {
        drc_area_series->hide();
        dci_area_series->hide();
        rlsc_r_area_series->hide();
        rlsc_g_area_series->show();
        rlsc_b_area_series->hide();
        ae_area_series->hide();
    } else if (color_idx == 6) {
        drc_area_series->hide();
        dci_area_series->hide();
        rlsc_r_area_series->hide();
        rlsc_g_area_series->hide();
        rlsc_b_area_series->show();
        ae_area_series->hide();
    } else if (color_idx == 7) {
        drc_area_series->hide();
        dci_area_series->hide();
        rlsc_r_area_series->hide();
        rlsc_g_area_series->hide();
        rlsc_b_area_series->hide();
        ae_area_series->show();
    } else {
        drc_area_series->show();
        dci_area_series->show();
        rlsc_r_area_series->show();
        rlsc_g_area_series->show();
        rlsc_b_area_series->show();
        ae_area_series->show();
    }
    updateHistCharts(color_idx);
}
