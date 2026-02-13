#include "gamma_page_widget.hpp"
#include "module_define.hpp"
#include "global_data.hpp"
#include "network_client_helper.hpp"

#include <QMessageBox>

constexpr int MAX_X = 256;
constexpr double MAX_Y = 4095;
constexpr int MAX_POINT_NUM = 256;

#define ADJUST_ADD_MODE (1)
#define ADJUST_SET_MODE (0)

//python genarate script
//def generate_srgb_gamma_table():
//    srgb_table = []
//    for i in range(256):
//        f = i / 255.0
//        if f <= 0.0031308:
//            v = 12.92 * f
//        else:
//            v = 1.055 * (f ** (1 / 2.2)) - 0.055
//        srgb_table.append(round(v * 4095))
//    return srgb_table
const quint16 gamma_table_sRGB[MAX_POINT_NUM] = {
          0,   204,   348,   453,   540,   614,   681,   741,   796,   847,   895,   941,   984, 1025, 1064, 1102,
    1138, 1173, 1206, 1239, 1271, 1301, 1331, 1360, 1389, 1416, 1443, 1470, 1496, 1521, 1546, 1570,
    1594, 1618, 1641, 1663, 1686, 1708, 1729, 1750, 1771, 1792, 1812, 1833, 1852, 1872, 1891, 1910,
    1929, 1948, 1966, 1984, 2002, 2020, 2037, 2055, 2072, 2089, 2106, 2122, 2139, 2155, 2171, 2187,
    2203, 2219, 2235, 2250, 2266, 2281, 2296, 2311, 2326, 2340, 2355, 2369, 2384, 2398, 2412, 2426,
    2440, 2454, 2468, 2481, 2495, 2508, 2522, 2535, 2548, 2561, 2574, 2587, 2600, 2613, 2625, 2638,
    2650, 2663, 2675, 2687, 2700, 2712, 2724, 2736, 2748, 2760, 2772, 2783, 2795, 2807, 2818, 2830,
    2841, 2853, 2864, 2875, 2886, 2897, 2909, 2920, 2931, 2941, 2952, 2963, 2974, 2985, 2995, 3006,
    3017, 3027, 3038, 3048, 3058, 3069, 3079, 3089, 3100, 3110, 3120, 3130, 3140, 3150, 3160, 3170,
    3180, 3189, 3199, 3209, 3219, 3228, 3238, 3248, 3257, 3267, 3276, 3286, 3295, 3304, 3314, 3323,
    3332, 3342, 3351, 3360, 3369, 3378, 3387, 3396, 3406, 3414, 3423, 3432, 3441, 3450, 3459, 3468,
    3477, 3485, 3494, 3503, 3511, 3520, 3529, 3537, 3546, 3554, 3563, 3571, 3580, 3588, 3597, 3605,
    3613, 3622, 3630, 3638, 3646, 3655, 3663, 3671, 3679, 3687, 3695, 3703, 3711, 3719, 3727, 3735,
    3743, 3751, 3759, 3767, 3775, 3783, 3791, 3799, 3806, 3814, 3822, 3830, 3837, 3845, 3853, 3860,
    3868, 3875, 3883, 3891, 3898, 3906, 3913, 3921, 3928, 3936, 3943, 3950, 3958, 3965, 3973, 3980,
    3987, 3995, 4002, 4009, 4016, 4024, 4031, 4038, 4045, 4052, 4060, 4067, 4074, 4081, 4088, 4095
 };

//python genarate script
//def generate_srgb_gamma_table():
//    linear_values = np.linspace(0, 1, 256)
//    nonlinear_lut = np.where(
//        linear_values < 0.018,
//        4.5 * linear_values,
//        1.099 * (linear_values ** 0.45) - 0.099)
//    return [round(i * 4095) for i in nonlinear_lut]
const quint16 gamma_table_bt709[MAX_POINT_NUM] = {
          0,     72,   145,   217,   289,   362,   427,   487,   542,   594,   642,   688,   732,   774,   814,   852,
      889,   925,   960,   993, 1026, 1058, 1089, 1119, 1148, 1177, 1205, 1233, 1260, 1287, 1313, 1338,
    1363, 1388, 1412, 1436, 1459, 1483, 1505, 1528, 1550, 1572, 1593, 1615, 1636, 1656, 1677, 1697,
    1717, 1737, 1757, 1776, 1795, 1814, 1833, 1851, 1870, 1888, 1906, 1924, 1941, 1959, 1976, 1993,
    2011, 2027, 2044, 2061, 2077, 2094, 2110, 2126, 2142, 2158, 2174, 2189, 2205, 2220, 2235, 2251,
    2266, 2281, 2296, 2310, 2325, 2340, 2354, 2369, 2383, 2397, 2411, 2425, 2439, 2453, 2467, 2481,
    2494, 2508, 2521, 2535, 2548, 2561, 2574, 2587, 2600, 2613, 2626, 2639, 2652, 2665, 2677, 2690,
    2702, 2715, 2727, 2740, 2752, 2764, 2776, 2788, 2800, 2812, 2824, 2836, 2848, 2860, 2872, 2883,
    2895, 2906, 2918, 2929, 2941, 2952, 2964, 2975, 2986, 2997, 3009, 3020, 3031, 3042, 3053, 3064,
    3075, 3085, 3096, 3107, 3118, 3128, 3139, 3150, 3160, 3171, 3181, 3192, 3202, 3213, 3223, 3233,
    3243, 3254, 3264, 3274, 3284, 3294, 3304, 3314, 3324, 3334, 3344, 3354, 3364, 3374, 3384, 3394,
    3403, 3413, 3423, 3432, 3442, 3452, 3461, 3471, 3480, 3490, 3499, 3509, 3518, 3528, 3537, 3546,
    3555, 3565, 3574, 3583, 3592, 3602, 3611, 3620, 3629, 3638, 3647, 3656, 3665, 3674, 3683, 3692,
    3701, 3710, 3718, 3727, 3736, 3745, 3754, 3762, 3771, 3780, 3788, 3797, 3806, 3814, 3823, 3831,
    3840, 3849, 3857, 3866, 3874, 3882, 3891, 3899, 3908, 3916, 3924, 3933, 3941, 3949, 3957, 3966,
    3974, 3982, 3990, 3998, 4007, 4015, 4023, 4031, 4039, 4047, 4055, 4063, 4071, 4079, 4087, 4095
};

GammaPageWidget::GammaPageWidget(QWidget *parent) : BasePageWidget(MODULE_NAME_GAMMA, parent)
{
    initData();
    initUI();
    initEvent();
}

void GammaPageWidget::initLeftArea()
{
    initItemWidgets();
    initGammaWidgets();

    // Gamma Group
    GroupItemWidget *gamma_group_widget = new GroupItemWidget("Gamma");
    gamma_group_widget->setRegSection("isp_gamma");
    gamma_group_widget->addPageWidget(gamma_enable, this);
    gamma_group_widget->addPageWidget(gamma_mode, this);
    gamma_group_widget->addPageWidget(UpdateInterval_spinner, this);
    gamma_group_widget->addPageWidget(coeff_spinner, this);
    gamma_group_widget->addPageWidget(slope_spinner, this);
    gamma_group_widget->addWidget(control_points_spinner);

    // Adjust Group
    GroupItemWidget *adjust_group_widget = new GroupItemWidget("Adjust");
    adjust_group_widget->setConfigSetHidden();
    adjust_group_widget->addWidget(x_adjust_spinner);
    adjust_group_widget->addWidget(y_adjust_spinner);
    QHBoxLayout *btn_layout = new QHBoxLayout;
    btn_layout->addWidget(add_ctrlpoint_btn);
    btn_layout->addWidget(set_btn);
    adjust_group_widget->addLayout(btn_layout);
    QHBoxLayout *label_layout = new QHBoxLayout;
    pos_label->setFixedWidth(230);
    pos_label->setMargin(5);
    label_layout->addWidget(pos_label, Qt::AlignCenter);
    gamma_group_widget->addLayout(label_layout);
    // adjust_group_widget->addLayout(label_layout);

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
    GroupItemWidget *auto_gamma_group_widget = new GroupItemWidget("Auto Gamma");
    auto_gamma_group_widget->setConfigSetHidden();
    auto_gamma_group_widget->addPageWidget(temp_table, this);
    auto_gamma_group_widget->addPageWidget(default_table, this);
    auto_gamma_group_widget->addPageWidget(srgb_table, this);
    auto_gamma_group_widget->addPageWidget(user_define_table, this);
    auto_gamma_group_widget->addPageWidget(auto_gamma_table0, this);
    auto_gamma_group_widget->addPageWidget(auto_gamma_table1, this);
    auto_gamma_group_widget->addPageWidget(auto_gamma_table2, this);
    auto_gamma_group_widget->addPageWidget(auto_gamma_table3, this);
    auto_gamma_group_widget->addPageWidget(auto_gamma_table4, this);
    auto_gamma_group_widget->addPageWidget(table_choice, this);
    auto_gamma_group_widget->addPageWidget(gamma_table_number, this);
    auto_gamma_group_widget->addPageWidget(auto_gamma_lv0, this);
    auto_gamma_group_widget->addPageWidget(auto_gamma_lv1, this);
    auto_gamma_group_widget->addPageWidget(auto_gamma_lv2, this);
    auto_gamma_group_widget->addPageWidget(auto_gamma_lv3, this);
    auto_gamma_group_widget->addPageWidget(auto_gamma_lv4, this);
    setBaseItemList(default_table);
    setBaseItemList(srgb_table);

    QVBoxLayout *wraper_layout = new QVBoxLayout();
    wraper_layout->setAlignment(Qt::AlignTop);
    wraper_layout->addWidget(gamma_group_widget);
    wraper_layout->addWidget(auto_gamma_group_widget);
    // wraper_layout->addWidget(adjust_group_widget);
    wraper_layout->addWidget(reference_group_widget);
    wraper_layout->addWidget(reset_group_widget);

    QWidget *widget = new QWidget;
    widget->setLayout(wraper_layout);
    scroll_area = new QScrollArea();
    scroll_area->setWidget(widget);
    main_layout->addWidget(scroll_area);
}

void GammaPageWidget::initCenterArea()
{
    initChart();

    QLayout * button_layout = initButtons();
    QLayout * combobo_layout = initCombobox();
    QVBoxLayout *v_layout = new QVBoxLayout();
    QHBoxLayout *func_layout = new QHBoxLayout();

    func_layout->addLayout(combobo_layout);
    func_layout->addLayout(button_layout);
    func_layout->setContentsMargins(100, 0, 100, 0);
    v_layout->addLayout(func_layout);
    v_layout->addWidget(line_chart_widget);
    main_layout->addLayout(v_layout);

    file_select_dialog = new FileDialog;
    connect(line_chart_widget, &GammaChartItemWidget::sigUpdateMousePos, this, &GammaPageWidget::updateMousePos);
    connect(line_combobox, &QComboBox::currentTextChanged, this, &GammaPageWidget::slotHandleSelectLine);
}

void GammaPageWidget::initEvent()
{
    shared_ptr<NetworkClientHelper> network_helper = NetworkClientHelper::getInstance();
    connect(network_helper.get(), &NetworkClientHelper::sigReceiveGamma, this, &GammaPageWidget::slotRefreshChart);
}

void GammaPageWidget::paintEvent(QPaintEvent *)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void GammaPageWidget::retranslateUi()
{
    load_button->setText(tr("load"));
    save_button->setText(tr("save"));

    reset_button->setText(tr("Reset All Curves"));

    reference_save_button->setText(tr("Save"));
    reference_apply_button->setText(tr("Use"));

    BasePageWidget::retranslateUi();
}

void GammaPageWidget::updateUI()
{
    UpdateInterval_spinner->updateVisible();
}

void GammaPageWidget::slotRefreshChart()
{
    int mode = gamma_mode->getData().toInt();
    int choice = table_choice->getData().toInt();

    switch (mode) {
        case 0:
            line_chart_widget->setUserDefineMode(false);
            line_chart_widget->setPoints(0, getDefaultPoints(), false);
            line_chart_widget->setControlPointsNum(2);
            break;
        case 1:
            line_chart_widget->setUserDefineMode(false);
            line_chart_widget->setPoints(0, getSRGBPoints(), false);
            line_chart_widget->setControlPointsNum(2);
            break;
        case 2:
            line_chart_widget->setUserDefineMode(true);
            line_chart_widget->setPoints(0, getUserDefinePoints());
            line_chart_widget->setControlPointsNum(control_points_spinner->getData().toInt());
            break;
        case 3:
            line_chart_widget->setUserDefineMode(true);
            line_chart_widget->setPoints(0, getAutoGammaPoints(choice));
            line_chart_widget->setControlPointsNum(control_points_spinner->getData().toInt());
            break;
        default:
            break;
    }

    if (mode != 3) {
        table_choice->setDisabled(true);
        gamma_table_number->setDisabled(true);
        auto_gamma_lv0->setDisabled(true);
        auto_gamma_lv1->setDisabled(true);
        auto_gamma_lv2->setDisabled(true);
        auto_gamma_lv3->setDisabled(true);
        auto_gamma_lv4->setDisabled(true);
    } else {
        table_choice->setDisabled(false);
        gamma_table_number->setDisabled(false);
        auto_gamma_lv0->setDisabled(false);
        auto_gamma_lv1->setDisabled(false);
        auto_gamma_lv2->setDisabled(false);
        auto_gamma_lv3->setDisabled(false);
        auto_gamma_lv4->setDisabled(false);
    }
}

void GammaPageWidget::slotParamsChanged(QVariant)
{
    if (gamma_mode->getData().toInt() >= 2) {
        double coeff = this->coeff_spinner->getData().toDouble();
        double slope = this->slope_spinner->getData().toDouble();
        line_chart_widget->setPoints(0, initPoints(coeff, slope));
        line_chart_widget->setControlPointsNum(control_points_spinner->getData().toInt());
    }
}

void GammaPageWidget::slotControlPointsNumChanged(QVariant num)
{
    if (gamma_mode->getData().toInt() >= 2) {
        this->control_points_num = num.toInt();
        line_chart_widget->setUserDefineCtrlPt(false);
        line_chart_widget->setControlPointsNum(control_points_num);
    } else {
        control_points_spinner->setData(2);
        control_points_num = 2;
        line_chart_widget->setUserDefineCtrlPt(false);
        line_chart_widget->setControlPointsNum(control_points_num);
    }
}

QList<QPointF> GammaPageWidget::getDefaultPoints()
{
    QList<QPointF> points;
    QVariant data = default_table->getData();
    QVariantList data_array = data.toList();

    for (int i = 0; i < MAX_POINT_NUM; i++) {
        points.append(QPointF(i,  data_array[i].toInt()));
    }
    return points;
}

QList<QPointF> GammaPageWidget::getSRGBPoints()
{
    QList<QPointF> points;
    QVariant data = srgb_table->getData();
    QVariantList data_array = data.toList();

    for (int i = 0; i < MAX_POINT_NUM; i++) {
        points.append(QPointF(i, data_array[i].toInt()));
    }
    return points;
}

QList<QPointF> GammaPageWidget::getUserDefinePoints()
{
    QList<QPointF> points;
    QVariant data = user_define_table->getData();
    QVariantList data_array = data.toList();

    for (int i = 0; i < MAX_POINT_NUM; i++) {
        points.append(QPointF(i,  data_array[i].toInt()));
    }
    return points;
}

QList<QPointF> GammaPageWidget::getAutoGammaPoints(int index)
{
    QList<QPointF> points;
    QVariant data;

    switch (index) {
        case 0:
            data = auto_gamma_table0->getData();
            break;
        case 1:
            data = auto_gamma_table1->getData();
            break;
        case 2:
            data = auto_gamma_table2->getData();
            break;
        case 3:
            data = auto_gamma_table3->getData();
            break;
        case 4:
            data = auto_gamma_table4->getData();
            break;
        default:
            break;
    }

    QVariantList data_array = data.toList();

    for (int i = 0; i < MAX_POINT_NUM; i++) {
        points.append(QPointF(i,  data_array[i].toInt()));
    }
    return points;
}

QList<QPointF> GammaPageWidget::initPoints(double coefficient, double slope_at_zero)
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

void GammaPageWidget::initGammaWidgets()
{
    QString module = property("module_id").toString();
    QList<BaseItemWidget *> base_item_list = getBaseItemList();

    for (int i = 0; i < base_item_list.size(); i++) {
        BaseItemWidget *item = base_item_list[i];
        QStringList path_list = item->getPathList();
        for (int j = 0; j < path_list.count(); j++) {
            if (path_list[j] == "ISP_GAMMA_ATTR_S.Enable") {
                gamma_enable = (BinaryItemWidget *) item;
            } else if (path_list[j] == "ISP_GAMMA_ATTR_S.UpdateInterval") {
                UpdateInterval_spinner = (SpinnerItemWidget *) item;
                UpdateInterval_spinner->hide();
            } else if (path_list[j] == "ISP_GAMMA_ATTR_S.Table") {
                user_define_table = (UniversalMatrixItemWidget *) item;
                user_define_table->hide();
            } else if (path_list[j] == "ISP_GAMMA_ATTR_S.enCurveType") {
                gamma_mode = (ComboBoxItemWidget *) item;
                connect(gamma_mode, &ComboBoxItemWidget::sigValueChanged, this, &GammaPageWidget::slotRefreshChart);
            } else if (path_list[j] == "ISP_AUTO_GAMMA_ATTR_S.GammaTabNum") {
                gamma_table_number = (SpinnerItemWidget *) item;
                gamma_table_number->setDisabled(true);
            } else if (path_list[j] == "ISP_AUTO_GAMMA_ATTR_S.GammaTab[0].Lv") {
                auto_gamma_lv0 = (SpinnerItemWidget *) item;
                auto_gamma_lv0->setDisabled(true);
            } else if (path_list[j] == "ISP_AUTO_GAMMA_ATTR_S.GammaTab[0].Tbl") {
                auto_gamma_table0 = (UniversalMatrixItemWidget *) item;
                auto_gamma_table0->hide();
            } else if (path_list[j] == "ISP_AUTO_GAMMA_ATTR_S.GammaTab[1].Lv") {
                auto_gamma_lv1 = (SpinnerItemWidget *) item;
                auto_gamma_lv1->setDisabled(true);
            } else if (path_list[j] == "ISP_AUTO_GAMMA_ATTR_S.GammaTab[1].Tbl") {
                auto_gamma_table1 = (UniversalMatrixItemWidget *) item;
                auto_gamma_table1->hide();
            } else if (path_list[j] == "ISP_AUTO_GAMMA_ATTR_S.GammaTab[2].Lv") {
                auto_gamma_lv2 = (SpinnerItemWidget *) item;
                auto_gamma_lv2->setDisabled(true);
            } else if (path_list[j] == "ISP_AUTO_GAMMA_ATTR_S.GammaTab[2].Tbl") {
                auto_gamma_table2 = (UniversalMatrixItemWidget *) item;
                auto_gamma_table2->hide();
            } else if (path_list[j] == "ISP_AUTO_GAMMA_ATTR_S.GammaTab[3].Lv") {
                auto_gamma_lv3 = (SpinnerItemWidget *) item;
                auto_gamma_lv3->setDisabled(true);
            } else if (path_list[j] == "ISP_AUTO_GAMMA_ATTR_S.GammaTab[3].Tbl") {
                auto_gamma_table3 = (UniversalMatrixItemWidget *) item;
                auto_gamma_table3->hide();
            } else if (path_list[j] == "ISP_AUTO_GAMMA_ATTR_S.GammaTab[4].Lv") {
                auto_gamma_lv4 = (SpinnerItemWidget *) item;
                auto_gamma_lv4->setDisabled(true);
            } else if (path_list[j] == "ISP_AUTO_GAMMA_ATTR_S.GammaTab[4].Tbl") {
                auto_gamma_table4 = (UniversalMatrixItemWidget *) item;
                auto_gamma_table4->hide();
            }
        }
    }

    // Gamma Group
    coeff_spinner = new DoubleSpinnerItemWidget(module, tr("GammaCOEFFI"));
    coeff_spinner->addRegSectionKey("isp_gamma", "reg_gamma_coeffi");
    coeff_spinner->setData(1.0);
    coeff_spinner->setRange(0.01, 20);
    connect(coeff_spinner, &DoubleSpinnerItemWidget::sigValueChanged, this, &GammaPageWidget::slotParamsChanged);
    insertBaseItemList(2, coeff_spinner);

    slope_spinner = new DoubleSpinnerItemWidget(module, tr("SlopeAtZero"));
    slope_spinner->addRegSectionKey("isp_gamma", "reg_gamma_slopeatzero");
    slope_spinner->setData(1.0);
    slope_spinner->setRange(0.01, 20);
    connect(slope_spinner, &DoubleSpinnerItemWidget::sigValueChanged, this, &GammaPageWidget::slotParamsChanged);
    insertBaseItemList(2, slope_spinner);

    control_points_spinner = new SpinnerItemWidget(module, tr("Control Points Num"));
    control_points_spinner->addRegSectionKey("isp_gamma", "reg_gamma_pointnum");
    control_points_spinner->setData(this->control_points_num);
    control_points_spinner->setRange(MIN_CONTROL_PT_NUM, MAX_CONTROL_PT_NUM);
    connect(control_points_spinner, &SpinnerItemWidget::sigValueChanged, this, &GammaPageWidget::slotControlPointsNumChanged);
    insertBaseItemList(2, control_points_spinner);

    // Adjust Group
    x_adjust_spinner = new SpinnerItemWidget(module, tr("X"));
    x_adjust_spinner->setData(0);
    x_adjust_spinner->setRange(0, 255);
    connect(x_adjust_spinner, &SpinnerItemWidget::sigValueChanged, this, &GammaPageWidget::slotXAdjustChanged);

    y_adjust_spinner = new SpinnerItemWidget(module, tr("Y"));
    y_adjust_spinner->setData(0);
    y_adjust_spinner->setRange(0, 4095);
    connect(y_adjust_spinner, &SpinnerItemWidget::sigValueChanged, this, &GammaPageWidget::slotYAdjustChanged);

    add_ctrlpoint_btn = new QPushButton(tr("Add Ctrl Point"));
    connect(add_ctrlpoint_btn, &QPushButton::clicked, this, &GammaPageWidget::handleAddCtrlPt);
    set_btn = new QPushButton(tr("Set Ctrl Point"));
    connect(set_btn, &QPushButton::clicked, this, &GammaPageWidget::handleSetEvent);
    setAjustButtons(ADJUST_SET_MODE);

    pos_label = new QLabel("Out of chart");
    pos_label->setAlignment(Qt::AlignCenter);

    // Reference Set Group
    reference_set_combo = new QComboBox();
    reference_set_combo->addItem("Set1");
    reference_set_combo->addItem("Set2");
    reference_set_combo->addItem("Set3");
    connect(reference_set_combo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &GammaPageWidget::slotReferSetSelect);

    refer_set[0] << QVariantList();
    refer_set[1] << QVariantList();
    refer_set[2] << QVariantList();

    reference_save_button = new QPushButton();
    connect(reference_save_button, &QPushButton::clicked, this, &GammaPageWidget::slotReferSetSave);

    reference_apply_button = new QPushButton();
    reference_apply_button->setEnabled(false);
    connect(reference_apply_button, &QPushButton::clicked, this, &GammaPageWidget::slotReferSetApply);

    // Reset Group
    reset_button = new QPushButton();
    connect(reset_button, &QPushButton::clicked, this, &GammaPageWidget::resetCurve);

    // Auto Gamma Group
    // Todo: Get count, max, min from tool json
    QJsonArray count_array = {1, 256};
    QJsonObject gamma_table_param
    {
        {"count", count_array},
        {"max", 4095},
        {"min", 0}
    };

    temp_table = new UniversalMatrixItemWidget(module, "Temp Table");
    temp_table->setParam(gamma_table_param);
    temp_table->addRegSectionKey("isp_gamma", "temp_gamma");
    temp_table->hide();

    default_table = new UniversalMatrixItemWidget(module, "Default Table");
    default_table->setParam(gamma_table_param);
    default_table->addRegSectionKey("isp_gamma", "default_gamma");
    default_table->addPath("PQT_GAMMA.DeaultTable");
    GlobalData::getInstance()->subscribeItemJsonrpc("PQT_GAMMA.DeaultTable", default_table);
    default_table->hide();

    srgb_table = new UniversalMatrixItemWidget(module, "sRGB Table");
    srgb_table->setParam(gamma_table_param);
    srgb_table->addRegSectionKey("isp_gamma", "srgb_gamma");
    srgb_table->addPath("PQT_GAMMA.sRGBTable");
    GlobalData::getInstance()->subscribeItemJsonrpc("PQT_GAMMA.sRGBTable", srgb_table);
    srgb_table->hide();

    QVariantList items;
    items.append(QVariantMap({{"label", "Table0"}, {"value", 0}}));
    items.append(QVariantMap({{"label", "Table1"}, {"value", 1}}));
    items.append(QVariantMap({{"label", "Table2"}, {"value", 2}}));
    items.append(QVariantMap({{"label", "Table3"}, {"value", 3}}));
    items.append(QVariantMap({{"label", "Table4"}, {"value", 4}}));
    table_choice = new ComboBoxItemWidget(module, tr("Table Choice"));
    table_choice->setParam(items);
    table_choice->setValue(0);
    table_choice->setDisabled(true);
    connect(table_choice, &ComboBoxItemWidget::sigValueChanged, this, &GammaPageWidget::slotRefreshChart);
}

void GammaPageWidget::initChart()
{
    QString module = property("module_id").toString();
    line_chart_widget = new GammaChartItemWidget(module, "Temp Table");
    CubicCurveChartView *gamma_line_chart = line_chart_widget->getLineChart();
    if (gamma_line_chart != nullptr) {
        gamma_line_chart->initGamma(MAX_X, MAX_Y);
        QList<QPointF> points_sRGB;
        QList<QPointF> points_bt709;
        for (int i = 0; i < MAX_POINT_NUM; i++) {
            points_sRGB.append(QPointF(i, gamma_table_sRGB[i]));
            points_bt709.append(QPointF(i, gamma_table_bt709[i]));
        }
        series_sRGB = createLineSeries("sRGB", QColor(0, 255, 0), points_sRGB);
        series_bt709 = createLineSeries("BT709", QColor(0, 0, 255), points_bt709);
        gamma_line_chart->addLine(series_sRGB);
        gamma_line_chart->addLine(series_bt709);
    }
    line_chart_widget->addRegSectionKey("isp_gamma", "temp_gamma");
    line_chart_widget->setSampleInterval(1);
    line_chart_widget->setDisplayInterval(1);
    line_chart_widget->setPoints(0, getDefaultPoints(), false);
    line_chart_widget->setControlPointsNum(control_points_spinner->getData().toInt());
    connect(line_chart_widget, &GammaChartItemWidget::sigPressedScatterPt, this, &GammaPageWidget::slotAjustUpdate);
    connect(line_chart_widget, &GammaChartItemWidget::sigValueChanged, this, &GammaPageWidget::slotRefreshTable);
}

QLayout *GammaPageWidget::initButtons()
{
    QHBoxLayout *button_layout = new QHBoxLayout();
    load_button = new QPushButton();
    save_button = new QPushButton();
    load_button->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    save_button->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);

    connect(load_button, &QPushButton::clicked, this, &GammaPageWidget::openLoadDialog);
    connect(save_button, &QPushButton::clicked, this, &GammaPageWidget::openSaveDialog);

    button_layout->setAlignment(Qt::AlignCenter);
    button_layout->addWidget(load_button);
    button_layout->addWidget(save_button);

    return button_layout;
}

QLayout *GammaPageWidget::initCombobox()
{
    QHBoxLayout *choose_layout = new QHBoxLayout();
    QLabel *line_title_label = new QLabel(tr("Choose Line Type"));
    line_combobox = new QComboBox();
    line_list.append(tr("ALL"));
    line_list.append(tr("User + sRGB"));
    line_list.append(tr("User + BT709"));
    line_list.append(tr("Only User"));

    for (auto &line_name : line_list) {
        line_combobox->addItem(line_name);
    }

    choose_layout->setAlignment(Qt::AlignLeft);
    choose_layout->addWidget(line_title_label);
    choose_layout->addWidget(line_combobox);

    return choose_layout;
}

void GammaPageWidget::openLoadDialog()
{
    file_select_dialog->setMode(MODE_LOAD);
    file_select_dialog->show();

    connect(file_select_dialog, &FileDialog::sigSelectFile, this, &GammaPageWidget::handleSelectFile);
}

void GammaPageWidget::openSaveDialog()
{
    file_select_dialog->setMode(MODE_SAVE);
    file_select_dialog->show();

    connect(file_select_dialog, &FileDialog::sigSelectFile, this, &GammaPageWidget::handleSelectFile);
}

void GammaPageWidget::handleSelectFile(QString format, QString file_path, MODE mode)
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

    disconnect(file_select_dialog, &FileDialog::sigSelectFile, this, &GammaPageWidget::handleSelectFile);
    file_select_dialog->close();
}

void GammaPageWidget::setAjustButtons(int mode)
{
    adjust_mode = mode;
    if (mode == ADJUST_SET_MODE) {
        add_ctrlpoint_btn->setEnabled(false);
        set_btn->setEnabled(true);
    } else if (mode == ADJUST_ADD_MODE){
        add_ctrlpoint_btn->setEnabled(true);
        set_btn->setEnabled(false);
    }
}

void GammaPageWidget::slotXAdjustChanged(QVariant v)
{
    int x = v.toInt();
    if (x == 0 || x == 255)
    {
        setAjustButtons(ADJUST_SET_MODE);
        return;
    } else {
        setAjustButtons(ADJUST_ADD_MODE);
        double y = line_chart_widget->getY(x);
        y_adjust_spinner->setData(y);
    }
}

void GammaPageWidget::slotYAdjustChanged(QVariant v)
{

}

void GammaPageWidget::handleAddCtrlPt()
{
    if (adjust_mode == ADJUST_ADD_MODE) {
        int x = x_adjust_spinner->getData().toInt();
        int y = y_adjust_spinner->getData().toInt();

        line_chart_widget->addControlPt(x, y);
    }
}

void GammaPageWidget::handleSetEvent()
{
    if (adjust_mode == ADJUST_SET_MODE) {
        int x = x_adjust_spinner->getData().toInt();
        int y = y_adjust_spinner->getData().toInt();

        line_chart_widget->addControlPt(x, y);
    }
}

void GammaPageWidget::updateMousePos(double x, double y)
{
    if (x < 0 || x > 256 || y < 0 || y > 4095) {
        pos_label->setText("Out of Chart!");
    } else {
        pos_label->setText(QString("Position:X = %1, Y = %2").arg(int(x)).arg(int(y)));
    }
}

void GammaPageWidget::resetCurve()
{
    line_chart_widget->setPoints(0, initPoints(1.0, 1.0));
    line_chart_widget->setControlPointsNum(control_points_num);
    QVariant data = temp_table->getData();
    user_define_table->setData(data, true);
    auto_gamma_table0->setData(data, true);
    auto_gamma_table1->setData(data, true);
    auto_gamma_table2->setData(data, true);
    auto_gamma_table3->setData(data, true);
    auto_gamma_table4->setData(data, true);
}

void GammaPageWidget::slotReferSetSelect(QVariant v)
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

void GammaPageWidget::slotReferSetSave()
{
    int index = reference_set_combo->currentIndex();
    mode[index] = gamma_mode->getData().toInt();
    choice[index] = table_choice->getData().toInt();
    refer_set[index].clear();
    refer_set[index] << line_chart_widget->getData().toList();
    //refer_set.insert(index, line_chart_widget->getData().toList());

    if (!reference_apply_button->isEnabled())
        reference_apply_button->setEnabled(true);
}

void GammaPageWidget::slotReferSetApply()
{
    int index = reference_set_combo->currentIndex();
    gamma_mode->setValue(mode[index]);
    table_choice->setValue(choice[index]);
    line_chart_widget->setData(refer_set[index], true);
}


void GammaPageWidget::slotAjustUpdate(QPointF p)
{
    this->setAjustButtons(ADJUST_SET_MODE);
    x_adjust_spinner->setData(p.x());
    y_adjust_spinner->setData(p.y());
}

void GammaPageWidget::slotRefreshTable()
{
    int mode = gamma_mode->getData().toInt();
    int choice = table_choice->getData().toInt();
    QVariant data = line_chart_widget->getData();

    if (mode == 2) {
        user_define_table->setData(data, true);
    } else if (mode == 3) {
        switch (choice) {
            case 0:
                auto_gamma_table0->setData(data, true);
                break;
            case 1:
                auto_gamma_table1->setData(data, true);
                break;
            case 2:
                auto_gamma_table2->setData(data, true);
                break;
            case 3:
                auto_gamma_table3->setData(data, true);
                break;
            case 4:
                auto_gamma_table4->setData(data, true);
                break;
            default:
                break;
        }
    }
}

void GammaPageWidget::paegUpdateTheme(bool dark_theme_enabled)
{
    BasePageWidget::paegUpdateTheme(dark_theme_enabled);
    coeff_spinner->handleUpdateTheme(dark_theme_enabled);
    slope_spinner->handleUpdateTheme(dark_theme_enabled);
    control_points_spinner->handleUpdateTheme(dark_theme_enabled);
    table_choice->handleUpdateTheme(dark_theme_enabled);
    line_chart_widget->handleUpdateTheme(dark_theme_enabled);
}

void GammaPageWidget::readModuleFromNetworkJsonrpc()
{
    QList<BaseItemWidget *> base_item_list = getBaseItemList();
    foreach (BaseItemWidget *base_item_widget, base_item_list) {
        base_item_widget->uiReset();
    }

    QStringList struct_list = getStructList();
    QMap<QString, QString> struct_get_api_map = getStructGetApiMap();
    QMap<QString, QString> struct_set_api_map = getStructSetApiMap();
    NetworkClientHelper::getInstance()->clearIdInfoJsonrpc();
    for (int i = -3; i < struct_list.count(); i++) {
        QJsonObject send_obj;
        QJsonObject id_info;
        QString struct_id;
        int jsonrpc_id = GlobalData::getInstance()->getJsonrpcId();

        if (i < 0) {
            struct_id = struct_list[0];
            send_obj[MSG_KEY_METHOD] = "CVI_ISP_GetGammaCurveByType";
            id_info[MSG_KEY_CMD] = CMD_READ_GAMMA;

            // Add curve type param to send obj and id info
            QJsonObject curve_type_obj;
            curve_type_obj["curveType"] = i + 3;
            send_obj["params"] = curve_type_obj;
            id_info["curve_type"] = i + 3;
        } else {
            struct_id = struct_list[i];
            send_obj[MSG_KEY_METHOD] = struct_get_api_map[struct_id];
            id_info[MSG_KEY_CMD] = CMD_READ_MODULE;
        }

        send_obj[MSG_KEY_JSONRPC] = MSG_JSONRPC_VERSION;
        send_obj[MSG_KEY_ID] = jsonrpc_id;
        id_info[MSG_KEY_MODULE] = property("module_id").toString();
        id_info[MSG_KEY_STRUCT] = struct_id;

        NetworkClientHelper::getInstance()->pushIdInfoJsonrpc(jsonrpc_id, id_info);
        NetworkClientHelper::getInstance()->sendCmd(std::move(send_obj));

        bool gotResponse = false;
        QTime stopTime = QTime::currentTime().addSecs(READ_WRITE_TIMEOUT);
        while (QTime::currentTime() < stopTime) {
            QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
            if (!GlobalData::getInstance()->getWaitForResponse()) {
                gotResponse = true;
                break;
            }
        }

        if (!gotResponse) {
            NetworkClientHelper::getInstance()->removeIdInfoJsonrpc(jsonrpc_id);
            GlobalData::getInstance()->setWaitForResponse(false);
            QString message = getTitle() + QString(" Page: Failed to read %1. Timeout: Can't get reponse from board within %2 seconds.").arg(struct_id).arg(READ_WRITE_TIMEOUT);
            emit sigStatusLog(message, LOG_LEVEL::LOG_ERROR);
            break;
        }
    }

    slotRefreshChart();
}

void GammaPageWidget::readData()
{
    setStatusBusy(true);

    GlobalData::getInstance()->setRestoreFlag(false);
    if (NetworkClientHelper::getInstance()->isNormalStatus()) {
        readModuleFromNetworkJsonrpc();
    } else {
        QMessageBox::warning(this, tr("Warning"), tr("Network is not connected!"), QMessageBox::Ok);
    }

    setStatusBusy(false);
}

QtCharts::QLineSeries *GammaPageWidget::createLineSeries(QString name, QColor color, QList<QPointF> points)
{
    QtCharts::QLineSeries *series = new QtCharts::QLineSeries();
    QPen pen = series->pen();
    pen.setColor(color);
    pen.setWidth(2);

    series->setPen(pen);
    series->setName(name);
    series->setPointsVisible(false);
    series->setUseOpenGL(true);

    for (int i = 0; i < points.size(); i++) {
        *series << points[i];
    }

    return series;
}

void GammaPageWidget::slotHandleSelectLine(const QString &text)
{
    int idx = line_list.indexOf(text);
    if (idx == 1) {
        series_sRGB->show();
        series_bt709->hide();
    } else if (idx == 2) {
        series_sRGB->hide();
        series_bt709->show();
    } else if (idx == 3) {
        series_sRGB->hide();
        series_bt709->hide();
    } else {
        series_sRGB->show();
        series_bt709->show();
    }
}
