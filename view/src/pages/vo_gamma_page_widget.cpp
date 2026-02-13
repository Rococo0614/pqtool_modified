#include "vo_gamma_page_widget.hpp"
#include "module_define.hpp"
#include "global_data.hpp"
#include "network_client_helper.hpp"

#include <QMessageBox>

constexpr int MAX_X = 256;
constexpr double MAX_Y = 255;
constexpr int MAX_POINT_NUM = 256;

VOGammaPageWidget::VOGammaPageWidget(QWidget *parent) : BasePageWidget(MODULE_NAME_VO_GAMMA, parent)
{
    initData();
    initUI();
    initEvent();
}

void VOGammaPageWidget::initLeftArea()
{
    QString id = property("module_id").toString();
    QJsonObject obj = GlobalData::getInstance()->getPageInfo(id);
    if (obj.contains("ready")) {
        setReadyFlag(obj["ready"].toBool());
    }

    initItemWidgets();
    initGammaWidgets();

    // Gamma Group
    GroupItemWidget *gamma_group_widget = new GroupItemWidget("Gamma");
    gamma_group_widget->setRegSection("vo_gamma");
    gamma_group_widget->addPageWidget(gamma_enable, this);
    gamma_group_widget->addPageWidget(osd_enable, this);
    gamma_group_widget->addWidget(coeff_spinner);
    gamma_group_widget->addWidget(slope_spinner);
    gamma_group_widget->addWidget(control_points_spinner);
    QHBoxLayout *label_layout = new QHBoxLayout;
    pos_label->setFixedWidth(230);
    pos_label->setMargin(5);
    label_layout->addWidget(pos_label, Qt::AlignCenter);
    gamma_group_widget->addLayout(label_layout);

    // Reference Set Group
    GroupItemWidget *reference_group_widget = new GroupItemWidget("Reference Set");
    reference_group_widget->setConfigSetHidden();
    QHBoxLayout *reference_layout = new QHBoxLayout;
    reference_layout->addWidget(reference_set_combo);
    reference_layout->addWidget(reference_save_button);
    reference_layout->addWidget(reference_apply_button);
    reference_group_widget->addLayout(reference_layout);

    // Reset Group
    GroupItemWidget *reset_group_widget = new GroupItemWidget("Reset");
    reset_group_widget->setConfigSetHidden();
    reset_group_widget->addWidget(reset_button);

    // Auto Gamma Group
    GroupItemWidget *gamma_table_group_widget = new GroupItemWidget("Gamma Table");
    gamma_table_group_widget->setConfigSetHidden();
    gamma_table_group_widget->addPageWidget(vo_gamma_table, this);

    QVBoxLayout *wraper_layout = new QVBoxLayout();
    wraper_layout->setAlignment(Qt::AlignTop);
    wraper_layout->addWidget(gamma_group_widget);
    // wraper_layout->addWidget(gamma_table_group_widget);
    wraper_layout->addWidget(reference_group_widget);
    wraper_layout->addWidget(reset_group_widget);

    QWidget *widget = new QWidget;
    widget->setLayout(wraper_layout);
    scroll_area = new QScrollArea();
    scroll_area->setWidget(widget);
    main_layout->addWidget(scroll_area);
}

void VOGammaPageWidget::initCenterArea()
{
    initChart();

    QLayout * button_layout = initButtons();
    QVBoxLayout *v_layout = new QVBoxLayout();

    v_layout->addLayout(button_layout);
    v_layout->addWidget(line_chart_widget);
    main_layout->addLayout(v_layout);

    file_select_dialog = new FileDialog;
    connect(line_chart_widget, &GammaChartItemWidget::sigUpdateMousePos, this, &VOGammaPageWidget::updateMousePos);
}

void VOGammaPageWidget::initEvent()
{
    shared_ptr<NetworkClientHelper> network_helper = NetworkClientHelper::getInstance();
    connect(network_helper.get(), &NetworkClientHelper::sigReceiveGamma, this, &VOGammaPageWidget::slotRefreshChart);
    connect(GlobalData::getInstance().get(), &GlobalData::sigReceiveGamma, this, &VOGammaPageWidget::slotRefreshChart);
}

void VOGammaPageWidget::paintEvent(QPaintEvent *)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void VOGammaPageWidget::retranslateUi()
{
    load_button->setText(tr("load"));
    save_button->setText(tr("save"));

    reset_button->setText(tr("Reset All Curves"));

    reference_save_button->setText(tr("Save"));
    reference_apply_button->setText(tr("Use"));

    BasePageWidget::retranslateUi();
}

void VOGammaPageWidget::updateUI()
{
}

void VOGammaPageWidget::slotRefreshChart()
{
    line_chart_widget->setPoints(0, getVOGammaPoints());
    line_chart_widget->setControlPointsNum(control_points_spinner->getData().toInt());
}

void VOGammaPageWidget::slotParamsChanged(QVariant)
{
    double coeff = this->coeff_spinner->getData().toDouble();
    double slope = this->slope_spinner->getData().toDouble();
    line_chart_widget->setPoints(0, initPoints(coeff, slope));
    line_chart_widget->setControlPointsNum(control_points_spinner->getData().toInt());
}

void VOGammaPageWidget::slotControlPointsNumChanged(QVariant num)
{
    this->control_points_num = num.toInt();
    line_chart_widget->setUserDefineCtrlPt(false);
    line_chart_widget->setControlPointsNum(control_points_num);
}

QList<QPointF> VOGammaPageWidget::getVOGammaPoints()
{
    QList<QPointF> points;
    QVariant data = vo_gamma_table->getData();
    QVariantList data_array = data.toList();

    for (int i = 0, j = 0; i < data_array.count() - 1; i++, j+=4) {
        float step = (data_array[i+1].toInt() - data_array[i].toInt()) / 4.0;
        points.append(QPointF(j,  data_array[i].toInt()));
        points.append(QPointF(j+1,  data_array[i].toInt() + step + 0.5));
        points.append(QPointF(j+2,  data_array[i].toInt() + 2*step + 0.5));
        points.append(QPointF(j+3,  data_array[i].toInt() + 3*step + 0.5));
    }
    return points;
}

QList<QPointF> VOGammaPageWidget::initPoints(double coefficient, double slope_at_zero)
{
    QList<QPointF> points;
    int point_num = MAX_POINT_NUM + 1;
    qreal step_x = qreal(MAX_X) / (point_num - 1);
    vector<double> slopes_array;
    vector<double> y_array;

    if (coefficient < 1) {
        if (slope_at_zero > 1) {
            for (int i = 0; i < point_num; i++) {
                qreal x1 = i * step_x;
                double norm_x1 = static_cast<double>(x1) / MAX_X;
                double y1 = qPow(norm_x1, coefficient);

                qreal x2 = (i + 1) * step_x;
                if (i == point_num - 1) x2 = MAX_X;
                double norm_x2 = static_cast<double>(x2) / MAX_X;
                double y2 = qPow(norm_x2, coefficient);

                double slope_i = (y2 - y1) / (norm_x2 - norm_x1);
                slope_i = qMin(slope_i, slope_at_zero);

                if (i == 0) {
                    slopes_array.emplace_back(slope_i);
                    y_array.emplace_back(y1);
                    double y2_new = y1 + slope_i * (norm_x2 - norm_x1);
                    y_array.emplace_back(y2_new);
                } else {
                    double smooth_slope = ((0.1 / coefficient) * slope_i) + slopes_array[static_cast<unsigned int>(i - 1)];
                    slope_i = slope_i * slope_at_zero + smooth_slope * (20 - slope_at_zero);
                    slope_i = slope_i / 20;

                    slope_i = qMin(slope_i, slopes_array[static_cast<unsigned int>(i - 1)]);
                    double y2_new = y_array[static_cast<unsigned int>(i)] + slope_i * (norm_x2 - norm_x1);

                    slopes_array.emplace_back(slope_i);
                    y_array.emplace_back(y2_new);
                }
            }
        }
    }

    for (int i = 0; i < point_num; i++) {
        qreal x = i * step_x;
        if (i == (point_num-1)) x = MAX_X;
        double norm_x = static_cast<double>(x) / MAX_X;
        double y = qPow(norm_x, coefficient);

        if (coefficient < 1) {
            if (slope_at_zero > 1) {
                y = y_array[static_cast<unsigned int>(i)] / (*max_element(y_array.begin(), y_array.end()));
            } else {
                y = norm_x * slope_at_zero;
            }
        }

        // output y
        y = y * MAX_Y;
        points.append(QPointF(x, y));
    }

    return points;
}

void VOGammaPageWidget::initGammaWidgets()
{
    QString module = property("module_id").toString();
    QList<BaseItemWidget *> base_item_list = getBaseItemList();

    for (int i = 0; i < base_item_list.size(); i++) {
        BaseItemWidget *item = base_item_list[i];
        QStringList path_list = item->getPathList();
        for (int j = 0; j < path_list.count(); j++) {
            if (path_list[j] == "VO_GAMMA_INFO_S.enable") {
                gamma_enable = (BinaryItemWidget *) item;
            } else if (path_list[j] == "VO_GAMMA_INFO_S.osd_apply") {
                osd_enable = (BinaryItemWidget *) item;
            } else if (path_list[j] == "VO_GAMMA_INFO_S.value") {
                vo_gamma_table = (UniversalMatrixItemWidget *) item;
                vo_gamma_table->hide();
            }
        }
    }

    // Gamma Group
    coeff_spinner = new DoubleSpinnerItemWidget(module, tr("GammaCOEFFI"));
    coeff_spinner->setData(1.0);
    coeff_spinner->setRange(0.01, 20);
    setBaseItemList(coeff_spinner);
    connect(coeff_spinner, &DoubleSpinnerItemWidget::sigValueChanged, this, &VOGammaPageWidget::slotParamsChanged);

    slope_spinner = new DoubleSpinnerItemWidget(module, tr("SlopeAtZero"));
    slope_spinner->setData(1.0);
    slope_spinner->setRange(0.01, 20);
    setBaseItemList(slope_spinner);
    connect(slope_spinner, &DoubleSpinnerItemWidget::sigValueChanged, this, &VOGammaPageWidget::slotParamsChanged);

    control_points_spinner = new SpinnerItemWidget(module, tr("Control Points Num"));
    control_points_spinner->setData(this->control_points_num);
    control_points_spinner->setRange(MIN_CONTROL_PT_NUM, MAX_CONTROL_PT_NUM);
    setBaseItemList(control_points_spinner);
    connect(control_points_spinner, &SpinnerItemWidget::sigValueChanged, this, &VOGammaPageWidget::slotControlPointsNumChanged);

    pos_label = new QLabel("Out of chart");
    pos_label->setAlignment(Qt::AlignCenter);

    // Reference Set Group
    reference_set_combo = new QComboBox();
    reference_set_combo->addItem("Set1");
    reference_set_combo->addItem("Set2");
    reference_set_combo->addItem("Set3");
    connect(reference_set_combo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &VOGammaPageWidget::slotReferSetSelect);

    refer_set[0] << QVariantList();
    refer_set[1] << QVariantList();
    refer_set[2] << QVariantList();

    reference_save_button = new QPushButton();
    connect(reference_save_button, &QPushButton::clicked, this, &VOGammaPageWidget::slotReferSetSave);

    reference_apply_button = new QPushButton();
    reference_apply_button->setEnabled(false);
    connect(reference_apply_button, &QPushButton::clicked, this, &VOGammaPageWidget::slotReferSetApply);

    // Reset Group
    reset_button = new QPushButton();
    connect(reset_button, &QPushButton::clicked, this, &VOGammaPageWidget::resetCurve);
}

void VOGammaPageWidget::initChart()
{
    QString module = property("module_id").toString();
    line_chart_widget = new GammaChartItemWidget(module, "VO Gamma Table");
    CubicCurveChartView *gamma_line_chart = line_chart_widget->getLineChart();
    if (gamma_line_chart != nullptr) {
        gamma_line_chart->initGamma(MAX_X, MAX_Y);
    }
    line_chart_widget->addRegSectionKey("vo_gamma", "vo_gamma_table");
    line_chart_widget->setSampleInterval(1);
    line_chart_widget->setDisplayInterval(1);
    line_chart_widget->setPoints(0, getVOGammaPoints(), false);
    line_chart_widget->setControlPointsNum(control_points_spinner->getData().toInt());
    line_chart_widget->setUserDefineMode(true);
    connect(line_chart_widget, &GammaChartItemWidget::sigValueChanged, this, &VOGammaPageWidget::slotRefreshTable);
}

QLayout *VOGammaPageWidget::initButtons()
{
    QHBoxLayout *button_layout = new QHBoxLayout();
    load_button = new QPushButton();
    save_button = new QPushButton();
    load_button->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    save_button->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);

    connect(load_button, &QPushButton::clicked, this, &VOGammaPageWidget::openLoadDialog);
    connect(save_button, &QPushButton::clicked, this, &VOGammaPageWidget::openSaveDialog);

    button_layout->setAlignment(Qt::AlignCenter);
    button_layout->addWidget(load_button);
    button_layout->addWidget(save_button);

    return button_layout;
}

void VOGammaPageWidget::openLoadDialog()
{
    file_select_dialog->setMode(MODE_LOAD);
    file_select_dialog->show();

    slotRefreshChart();
    connect(file_select_dialog, &FileDialog::sigSelectFile, this, &VOGammaPageWidget::handleSelectFile);
}

void VOGammaPageWidget::openSaveDialog()
{
    file_select_dialog->setMode(MODE_SAVE);
    file_select_dialog->show();

    connect(file_select_dialog, &FileDialog::sigSelectFile, this, &VOGammaPageWidget::handleSelectFile);
}

void VOGammaPageWidget::handleSelectFile(QString format, QString file_path, MODE mode)
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

    disconnect(file_select_dialog, &FileDialog::sigSelectFile, this, &VOGammaPageWidget::handleSelectFile);
    file_select_dialog->close();
}

void VOGammaPageWidget::updateMousePos(double x, double y)
{
    if (x < 0 || x > 256 || y < 0 || y > 4095) {
        pos_label->setText("Out of Chart!");
    } else {
        pos_label->setText(QString("Position:X = %1, Y = %2").arg(int(x)).arg(int(y)));
    }
}

void VOGammaPageWidget::resetCurve()
{
    line_chart_widget->setPoints(0, initPoints(1.0, 1.0));
    line_chart_widget->setControlPointsNum(control_points_num);
}

void VOGammaPageWidget::slotReferSetSelect(QVariant v)
{
    int index = v.toInt();
    if (refer_set[index].isEmpty()) {
        if (reference_apply_button->isEnabled())
            reference_apply_button->setEnabled(false);
    } else {
        if (!reference_apply_button->isEnabled())
            reference_apply_button->setEnabled(true);
    }
}

void VOGammaPageWidget::slotReferSetSave()
{
    int index = reference_set_combo->currentIndex();
    refer_set[index] << line_chart_widget->getData().toList();

    if (!reference_apply_button->isEnabled())
        reference_apply_button->setEnabled(true);
}

void VOGammaPageWidget::slotReferSetApply()
{
    int index = reference_set_combo->currentIndex();
    line_chart_widget->setData(refer_set[index]);
}

void VOGammaPageWidget::paegUpdateTheme(bool dark_theme_enabled)
{
    BasePageWidget::paegUpdateTheme(dark_theme_enabled);
    coeff_spinner->handleUpdateTheme(dark_theme_enabled);
    slope_spinner->handleUpdateTheme(dark_theme_enabled);
    control_points_spinner->handleUpdateTheme(dark_theme_enabled);
    line_chart_widget->handleUpdateTheme(dark_theme_enabled);
}

void VOGammaPageWidget::slotRefreshTable()
{
    QVariantList data256_list = line_chart_widget->getData().toList();
    QVariantList data65_list;
    for(int i = 0 ; i < 256; i+=4) {
        data65_list.append(data256_list[i].toInt());
    }

    data65_list.append(255);
    vo_gamma_table->setData(data65_list, true);
}
