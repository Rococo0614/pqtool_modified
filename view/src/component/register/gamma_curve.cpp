#include "gamma_curve.hpp"
#include "utils.hpp"
#include "module_define.hpp"
#include "register_item_factory.hpp"
#include "base_page_widget.hpp"
#include "network_client_helper.hpp"
#include <QLineEdit>
#include <QFile>
#include <QTextStream>
#include <QIntValidator>
#include <QMessageBox>

#define ADJUST_ADD_MODE (1)
#define ADJUST_SET_MODE (0)


GammaCurve::GammaCurve(QString module, QString title, QWidget *parent)
{
    main_layout = new QHBoxLayout(this);
    this->setLayout(main_layout);
    this->title = title;
    this->module = module;
    this->pageWidget = parent;
}

void GammaCurve::initUI()
{
    initLeftArea();
    initCenterArea();
    retranslateUi();
}

void GammaCurve::initLeftArea()
{
    // Adjust Group
    GroupItemWidget *adjust_group_widget = new GroupItemWidget("Adjust");
    for (int i = 0; i < related_params_json.count(); i++) {
        QJsonObject item_json = related_params_json[i].toObject();
        item_json.insert("section", module);
        BaseItemWidget *item_widget = RegisterItemFactory::createRegisterItemByJson(module, item_json);
        connect(item_widget, &BaseItemWidget::sigValueChanged, this, &GammaCurve::slotParamsChanged);
        adjust_group_widget->addWidget(item_widget);
        related_params_widget.append(item_widget);
        if (!item_json.contains("tool_only")) {
            BasePageWidget *page = dynamic_cast<BasePageWidget *>(this->pageWidget);
            page->setBaseItemList(item_widget);
        }
    }

    control_points_spinner = new SpinnerItemWidget(module, tr("Control Points Num"));
    control_points_spinner->setData(this->control_points_num);
    control_points_spinner->setRange(MIN_CONTROL_PT_NUM, MAX_CONTROL_PT_NUM);
    connect(control_points_spinner, &SpinnerItemWidget::sigValueChanged, this, &GammaCurve::slotControlPointsNumChanged);
    adjust_group_widget->addWidget(control_points_spinner);

    QHBoxLayout *label_layout = new QHBoxLayout;
    pos_label = new QLabel("Out of chart");
    pos_label->setAlignment(Qt::AlignCenter);
    pos_label->setFixedWidth(230);
    pos_label->setMargin(5);
    label_layout->addWidget(pos_label, Qt::AlignCenter);
    adjust_group_widget->addLayout(label_layout);

    // Reference Set Group
    GroupItemWidget *reference_group_widget = new GroupItemWidget("Reference Set");
    reference_group_widget->setConfigSetHidden();
    QHBoxLayout *reference_layout = new QHBoxLayout;
    reference_set_combo = new QComboBox();
    reference_set_combo->addItem("Set1");
    reference_set_combo->addItem("Set2");
    reference_set_combo->addItem("Set3");
    connect(reference_set_combo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &GammaCurve::slotReferSetSelect);

    refer_set[0] << QVariantList();
    refer_set[1] << QVariantList();
    refer_set[2] << QVariantList();

    reference_save_button = new QPushButton();
    connect(reference_save_button, &QPushButton::clicked, this, &GammaCurve::slotReferSetSave);

    reference_apply_button = new QPushButton();
    reference_apply_button->setEnabled(false);
    connect(reference_apply_button, &QPushButton::clicked, this, &GammaCurve::slotReferSetApply);
    reference_layout->addWidget(reference_set_combo);
    reference_layout->addWidget(reference_save_button);
    reference_layout->addWidget(reference_apply_button);
    reference_group_widget->addLayout(reference_layout);

    // Reset Group
    GroupItemWidget *reset_group_widget = new GroupItemWidget("Reset");
    reset_group_widget->setConfigSetHidden();
    reset_button = new QPushButton();
    connect(reset_button, &QPushButton::clicked, this, &GammaCurve::resetCurve);
    reset_group_widget->addWidget(reset_button);

    QVBoxLayout *wraper_layout = new QVBoxLayout();
    wraper_layout->setAlignment(Qt::AlignTop);
    wraper_layout->addWidget(adjust_group_widget);
    wraper_layout->addWidget(reference_group_widget);
    wraper_layout->addWidget(reset_group_widget);

    QWidget *widget = new QWidget;
    widget->setLayout(wraper_layout);
    scroll_area = new QScrollArea();
    scroll_area->setWidget(widget);
    main_layout->addWidget(scroll_area,3);
}

void GammaCurve::initCenterArea()
{
    initChart();

    QLayout * button_layout = initButtons();
    QVBoxLayout *v_layout = new QVBoxLayout();

    v_layout->addLayout(button_layout);
    v_layout->addWidget(line_chart_widget);
    main_layout->addLayout(v_layout,7);

    file_select_dialog = new FileDialog;
    connect(line_chart_widget, &GammaChartItemWidget::sigUpdateMousePos, this, &GammaCurve::updateMousePos);
}

void GammaCurve::initChart()
{
    line_chart_widget = new GammaChartItemWidget(module, title);
    line_chart_widget->setUserDefineMode(true);
    CubicCurveChartView *gamma_line_chart = line_chart_widget->getLineChart();
    if (gamma_line_chart != nullptr) {
        gamma_line_chart->initGamma(MAX_X, MAX_Y);
    }
    line_chart_widget->addRegSectionKey(module, title);
    line_chart_widget->setSampleInterval(1);
    line_chart_widget->setDisplayInterval(1);
    line_chart_widget->setPoints(0, getDefaultPoints(), false);
    line_chart_widget->setUserDefineCtrlPt(false);
    line_chart_widget->setControlPointsNum(control_points_spinner->getData().toInt());
    connect(line_chart_widget, &GammaChartItemWidget::sigValueChanged, this, &GammaCurve::slotChartChange);
}

QLayout *GammaCurve::initButtons()
{
    QHBoxLayout *button_layout = new QHBoxLayout();
    load_button = new QPushButton();
    save_button = new QPushButton();
    load_button->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    save_button->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);

    connect(load_button, &QPushButton::clicked, this, &GammaCurve::openLoadDialog);
    connect(save_button, &QPushButton::clicked, this, &GammaCurve::openSaveDialog);

    button_layout->setAlignment(Qt::AlignCenter);
    button_layout->addWidget(load_button);
    button_layout->addWidget(save_button);

    return button_layout;
}

void GammaCurve::setAxis(double x, double y)
{
    this->MAX_X = x;
    this->MAX_Y = y;
}

void GammaCurve::setParamArr(QJsonArray arr)
{
    this->related_params_json = arr;
}

void GammaCurve::setParam(QVariant param)
{
    QJsonObject param_obj = param.toJsonObject();
    double x = 256, y = 255;

    if (param_obj.contains("max")) {
        y = param_obj["max"].toArray()[0].toDouble();
    }

    if (param_obj.contains("count")) {
        x = param_obj["count"].toArray()[1].toInt();
    }

    setAxis(x, y);

    if (param_obj.contains("related_params")) {
        setParamArr(param_obj["related_params"].toArray());
    }
}
void GammaCurve::paintEvent(QPaintEvent *)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void GammaCurve::retranslateUi()
{
    load_button->setText(tr("load"));
    save_button->setText(tr("save"));

    reset_button->setText(tr("Reset Curve"));

    reference_save_button->setText(tr("Save"));
    reference_apply_button->setText(tr("Use"));
}

void GammaCurve::setData(QVariant data)
{
    QList<QPointF> points;
    QJsonArray value_array = data.toJsonArray();

    for (int i = 0; i < value_array.count(); i++) {
        int value = value_array[i].toInt();
        points.append(QPointF(i, value));
    }

    updatePoints(points, false);
}

void GammaCurve::updatePoints(QList<QPointF> points, bool sync)
{
    line_chart_widget->setPoints(0, points, sync);
    line_chart_widget->setControlPointsNum(control_points_spinner->getData().toInt());
}

QVariant GammaCurve::getData()
{
    return line_chart_widget->getData();
}

void GammaCurve::slotParamsChanged(QVariant)
{
    ItemValueType itemsValue;

    for (int i = 0; i < related_params_widget.size(); i++) {
        QString name = related_params_widget[i]->getTitle();
        QVariant value = related_params_widget[i]->getData();
        itemsValue.insert(name, value);
    }

    if (gamma_call_list.contains(this->title)) {
        GenGammaCall fun = gamma_call_list[this->title];
        if (fun != nullptr) {
            QList<QPointF> points;
            QVariantList data = fun(MAX_X, MAX_Y, itemsValue).toList();
            for (int i = 0; i < data.size(); i++) {
                points.append(QPointF(i, data[i].toDouble()));
            }
            if (!points.isEmpty()) {
                updatePoints(points, true);
            }
        }
    }
}

void GammaCurve::slotControlPointsNumChanged(QVariant num)
{
    this->control_points_num = num.toInt();
    line_chart_widget->setControlPointsNum(control_points_num);
}

QList<QPointF> GammaCurve::getDefaultPoints()
{
    QList<QPointF> points;
    int y = 0;

    for (int i = 0; i < MAX_X; i++) {
        y = i * MAX_Y / MAX_X;
        points.append(QPointF(i, y));
    }

    return points;
}

void GammaCurve::openLoadDialog()
{
    file_select_dialog->setMode(MODE_LOAD);
    file_select_dialog->show();

    connect(file_select_dialog, &FileDialog::sigSelectFile, this, &GammaCurve::handleSelectFile);
}

void GammaCurve::openSaveDialog()
{
    file_select_dialog->setMode(MODE_SAVE);
    file_select_dialog->show();

    connect(file_select_dialog, &FileDialog::sigSelectFile, this, &GammaCurve::handleSelectFile);
}

void GammaCurve::handleSelectFile(QString format, QString file_path, MODE mode)
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

    disconnect(file_select_dialog, &FileDialog::sigSelectFile, this, &GammaCurve::handleSelectFile);
    file_select_dialog->close();
}

void GammaCurve::updateMousePos(double x, double y)
{
    if (x < 0 || x > MAX_X || y < 0 || y > MAX_Y) {
        pos_label->setText("Out of Chart!");
    } else {
        pos_label->setText(QString("Position:X = %1, Y = %2").arg(int(x)).arg(int(y)));
    }
}

void GammaCurve::resetCurve()
{
    QList<QPointF> points = getDefaultPoints();

    updatePoints(points, true);
}

void GammaCurve::slotReferSetSelect(QVariant v)
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

void GammaCurve::slotReferSetSave()
{
    int index = reference_set_combo->currentIndex();
    ItemValueType itemsValue;

    for (int i = 0; i < related_params_widget.size(); i++) {
        QVariant data =  related_params_widget[i]->getData();
        QString name = related_params_widget[i]->getTitle();
        itemsValue.insert(name, data);
    }
    related_params_value.insert(index, itemsValue);
    refer_set[index].clear();
    refer_set[index] << line_chart_widget->getData().toList();

    if (!reference_apply_button->isEnabled())
        reference_apply_button->setEnabled(true);
}

void GammaCurve::slotReferSetApply()
{
    QList<QPointF> points;
    int index = reference_set_combo->currentIndex();
    ItemValueType itemsValue = related_params_value[index];

    for (int i = 0; i < related_params_widget.size(); i++) {
        QString name = related_params_widget[i]->getTitle();
        related_params_widget[i]->setData(itemsValue[name], false);
    }

    QVariantList py = refer_set[index];
    for (int i = 0; i < py.size(); i++) {
        points.append(QPointF(i, py[i].toDouble()));
    }
    updatePoints(points, true);
}

void GammaCurve::slotChartChange()
{
    emit sigValueChanged();
    if (GlobalData::getInstance()->getSettings(SETTINGS_SECTION, KEY_AUTO_WRITE).toInt()) {
        emit sigWritePageData();
    }
}


GammaCurveLV::GammaCurveLV(QString module, QString title, QWidget *parent):
    GammaCurve(module, title, parent)
{
}

void GammaCurveLV::initUI()
{
    initLeftArea();
    initCenterArea();
    retranslateUi();
}

void GammaCurveLV::initLeftArea()
{
    // DCI Manual Gamma Group
    GroupItemWidget *adjust_group_widget = new GroupItemWidget("Gamma By LV");
    QVariantList items;
    items.append(QVariantMap({{"label", "Table0"}, {"value", 0}}));
    items.append(QVariantMap({{"label", "Table1"}, {"value", 1}}));
    items.append(QVariantMap({{"label", "Table2"}, {"value", 2}}));
    items.append(QVariantMap({{"label", "Table3"}, {"value", 3}}));
    items.append(QVariantMap({{"label", "Table4"}, {"value", 4}}));
    table_choice = new ComboBoxItemWidget(module, tr("Table Choice"));
    table_choice->setParam(items);
    table_choice->setValue(0);
    connect(table_choice, &ComboBoxItemWidget::sigValueChanged, this, &GammaCurveLV::slotRefreshChart);
    adjust_group_widget->addWidget(table_choice);

    for (int i = 0; i < related_params_json.count(); i++) {
        QJsonObject item_json = related_params_json[i].toObject();
        item_json.insert("section", module);
        BaseItemWidget *item_widget = RegisterItemFactory::createRegisterItemByJson(module, item_json);
        QString item_title = item_widget->getTitle();
        if (item_title.contains("GammaTab")) {
            if (item_title.contains("Tbl")) {
                item_widget->setLevel(3);
                gamma_table_list.append(item_widget);
                connect((UniversalMatrixItemWidget *)item_widget, &UniversalMatrixItemWidget::sigTableValueChanged, this, &GammaCurveLV::slotRefreshChart);
            } else if (item_title.contains("Lv")) {
                item_widget->setLevel(3);
                gamma_lv_list.append(item_widget);
                //connect((SpinnerItemWidget *)item_widget, &SpinnerItemWidget::sigValueChangedCpt, this, &GammaCurveLV::slotCheckLvValue);
            } else if (item_title.contains("GammaTabNum")) {
                gamma_table_num = item_widget;
            }
            adjust_group_widget->addWidget(item_widget);
        } else {
            QJsonObject param_json = item_json["param"].toObject();
            if (item_title.contains("DciGamma")) {
                dci_gamma = item_widget;
                dci_gamma->setDisabled(true);
                dci_gamma->setLevel(3);
                dci_gamma->hide();
                dci_gamma_spinner = new SpinnerItemWidget(module, QString("DciGamma"));
                dci_gamma_spinner->setRange(param_json["min"].toInt(), param_json["max"].toInt());
                related_params_widget.append(dci_gamma_spinner);
                adjust_group_widget->addWidget(dci_gamma_spinner);
                connect(dci_gamma_spinner, &BaseItemWidget::sigValueChanged, this, &GammaCurveLV::slotRelatedChanged);
            }
            if (item_title.contains("DciOffset")) {
                dci_offset = item_widget;
                dci_offset->setLevel(3);
                dci_offset->hide();
                dci_offset_spinner = new SpinnerItemWidget(module, QString("DciOffset"));
                dci_offset_spinner->setRange(param_json["min"].toInt(), param_json["max"].toInt());
                related_params_widget.append(dci_offset_spinner);
                adjust_group_widget->addWidget(dci_offset_spinner);
                connect(dci_offset_spinner, &BaseItemWidget::sigValueChanged, this, &GammaCurveLV::slotRelatedChanged);
            }
            if (item_title.contains("DciContrast")) {
                dci_contrast = item_widget;
                dci_contrast->setLevel(3);
                dci_contrast->hide();
                dci_contrast_spinner = new SpinnerItemWidget(module, QString("DciContrast"));
                dci_contrast_spinner->setRange(param_json["min"].toInt(), param_json["max"].toInt());
                related_params_widget.append(dci_contrast_spinner);
                adjust_group_widget->addWidget(dci_contrast_spinner);
                connect(dci_contrast_spinner, &BaseItemWidget::sigValueChanged, this, &GammaCurveLV::slotRelatedChanged);
            }
        }
        if (!item_json.contains("tool_only")) {
            BasePageWidget *page = dynamic_cast<BasePageWidget *>(this->pageWidget);
            page->setBaseItemList(item_widget);
        }
    }

    control_points_spinner = new SpinnerItemWidget(module, tr("Control Points Num"));
    control_points_spinner->setData(this->control_points_num);
    control_points_spinner->setRange(MIN_CONTROL_PT_NUM, MAX_CONTROL_PT_NUM);
    connect(control_points_spinner, &SpinnerItemWidget::sigValueChanged, this, &GammaCurveLV::slotControlPointsNumChanged);
    adjust_group_widget->addWidget(control_points_spinner);

    QHBoxLayout *label_layout = new QHBoxLayout;
    pos_label = new QLabel("Out of chart");
    pos_label->setAlignment(Qt::AlignCenter);
    pos_label->setFixedWidth(230);
    pos_label->setMargin(5);
    label_layout->addWidget(pos_label, Qt::AlignCenter);
    adjust_group_widget->addLayout(label_layout);

    // Reference Set Group
    GroupItemWidget *reference_group_widget = new GroupItemWidget("Reference Set");
    reference_group_widget->setConfigSetHidden();
    QHBoxLayout *reference_layout = new QHBoxLayout;
    reference_set_combo = new QComboBox();
    reference_set_combo->addItem("Set1");
    reference_set_combo->addItem("Set2");
    reference_set_combo->addItem("Set3");
    connect(reference_set_combo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &GammaCurveLV::slotReferSetSelect);

    refer_set[0] << QVariantList();
    refer_set[1] << QVariantList();
    refer_set[2] << QVariantList();

    reference_save_button = new QPushButton();
    connect(reference_save_button, &QPushButton::clicked, this, &GammaCurveLV::slotReferSetSave);

    reference_apply_button = new QPushButton();
    reference_apply_button->setEnabled(false);
    connect(reference_apply_button, &QPushButton::clicked, this, &GammaCurveLV::slotReferSetApply);
    reference_layout->addWidget(reference_set_combo);
    reference_layout->addWidget(reference_save_button);
    reference_layout->addWidget(reference_apply_button);
    reference_group_widget->addLayout(reference_layout);

    // Reset Group
    GroupItemWidget *reset_group_widget = new GroupItemWidget("Reset");
    reset_group_widget->setConfigSetHidden();
    reset_button = new QPushButton();
    connect(reset_button, &QPushButton::clicked, this, &GammaCurveLV::resetCurve);
    reset_group_widget->addWidget(reset_button);

    QVBoxLayout *wraper_layout = new QVBoxLayout();
    wraper_layout->setAlignment(Qt::AlignTop);
    wraper_layout->addWidget(adjust_group_widget);
    wraper_layout->addWidget(reference_group_widget);
    wraper_layout->addWidget(reset_group_widget);

    QWidget *widget = new QWidget;
    widget->setLayout(wraper_layout);
    scroll_area = new QScrollArea();
    scroll_area->setWidget(widget);
    main_layout->addWidget(scroll_area,3);

    updateRelatedWidget();
}


void GammaCurveLV::slotChartChange()
{
    emit sigValueChanged();
    if (GlobalData::getInstance()->getSettings(SETTINGS_SECTION, KEY_AUTO_WRITE).toInt()) {
        emit sigWritePageData();
    }
    int choice = table_choice->getData().toInt();
    BaseItemWidget *item = gamma_table_list[choice];
    item->setData(line_chart_widget->getData());
}

void GammaCurveLV::slotRefreshChart()
{
    QList<QPointF> points;
    int choice = table_choice->getData().toInt();
    BaseItemWidget *item = gamma_table_list[choice];
    QJsonArray value_array = item->getData().toJsonArray();

    for (int i = 0; i < value_array.count(); i++) {
        int value = value_array[i].toInt();
        points.append(QPointF(i, value));
    }

    updatePoints(points, false);
    updateRelatedWidget();
}

void GammaCurveLV::setData(QVariant data)
{
    QList<QPointF> points;
    int choice = table_choice->getData().toInt();
    BaseItemWidget *item = gamma_table_list[choice];
    data = item->getData();
    QJsonArray value_array = data.toJsonArray();

    for (int i = 0; i < value_array.count(); i++) {
        int value = value_array[i].toInt();
        points.append(QPointF(i, value));
    }

    updatePoints(points, false);
}

QVariant GammaCurveLV::getData()
{
    return dci_gamma->getData();
}

void GammaCurveLV::updateRelatedWidget()
{
    int choice = table_choice->getData().toInt();
    dci_gamma_spinner->setData(dci_gamma->getData().toList()[choice]);
    dci_offset_spinner->setData(dci_offset->getData().toList()[choice]);
    dci_contrast_spinner->setData(dci_contrast->getData().toList()[choice]);

    for (int i = 0; i < gamma_table_list.size(); i++) {
        if (i == choice) {
            gamma_table_list[i]->setVisible(true);
            gamma_table_list[i]->setLevel(0);
            gamma_lv_list[i]->setVisible(true);
            gamma_lv_list[i]->setLevel(0);
        } else {
            gamma_table_list[i]->hide();
            gamma_table_list[i]->setLevel(3);
            gamma_lv_list[i]->hide();
            gamma_lv_list[i]->setLevel(3);
        }
    }
}

void GammaCurveLV::slotRelatedChanged(QVariant v)
{
    slotParamsChanged(v);
    int choice = table_choice->getData().toInt();

    QList<QVariant> dci_gamma_variants = dci_gamma->getData().toList();
    dci_gamma_variants[choice] = dci_gamma_spinner->getData();
    dci_gamma->setData(dci_gamma_variants);

    QList<QVariant> dci_offset_variants = dci_offset->getData().toList();
    dci_offset_variants[choice] = dci_offset_spinner->getData();
    dci_offset->setData(dci_offset_variants);

    QList<QVariant> dci_contrast_variants = dci_contrast->getData().toList();
    dci_contrast_variants[choice] = dci_contrast_spinner->getData();
    dci_contrast->setData(dci_contrast_variants);
}

void GammaCurveLV::slotCheckLvValue(QVariant)
{
    int choice = table_choice->getData().toInt();
    int table_num = gamma_table_num->getData().toInt();

    if (choice == 0 || table_num ==1 || choice >= table_num)
        return;

    int current_v = gamma_lv_list[choice]->getData().toInt();
    int previous_v = gamma_lv_list[choice - 1]->getData().toInt();

    if (previous_v > current_v) {
        QString message = QString("Lv of table[%1] must be bigger than table[%2]!").arg(choice).arg(choice - 1);
        QMessageBox::warning(this, tr("Warning"), message, QMessageBox::Ok);
        gamma_lv_list[choice]->setData(gamma_lv_list[choice - 1]->getData());
    }
}

void GammaCurveLV::setParam(QVariant param)
{
    QJsonObject param_obj = param.toJsonObject();
    double x = 256, y = 255;

    setAxis(x, y);

    if (param_obj.contains("related_params")) {
        setParamArr(param_obj["related_params"].toArray());
    }
}


#define MAX_POINT_NUM (256)
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

GammaCurveRGB::GammaCurveRGB(QString module, QString title, QWidget *parent):
    GammaCurve(module, title, parent)
{
}

void GammaCurveRGB::initUI()
{
    initLeftArea();
    initCenterArea();
    retranslateUi();
    addLine2Chart();
}

void GammaCurveRGB::initLeftArea()
{
    // Adjust Group
    GroupItemWidget *adjust_group_widget = new GroupItemWidget("Adjust");
    coeff_spinner = new DoubleSpinnerItemWidget(module, tr("GammaCOEFFI"));
    coeff_spinner->addRegSectionKey("isp_gamma", "reg_gamma_coeffi");
    coeff_spinner->setData(1.0);
    coeff_spinner->setRange(0.01, 20);
    connect(coeff_spinner, &DoubleSpinnerItemWidget::sigValueChanged, this, &GammaCurveRGB::slotParamsChanged);
    adjust_group_widget->addWidget(coeff_spinner);

    slope_spinner = new DoubleSpinnerItemWidget(module, tr("SlopeAtZero"));
    slope_spinner->addRegSectionKey("isp_gamma", "reg_gamma_slopeatzero");
    slope_spinner->setData(1.0);
    slope_spinner->setRange(0.01, 20);
    connect(slope_spinner, &DoubleSpinnerItemWidget::sigValueChanged, this, &GammaCurveRGB::slotParamsChanged);
    adjust_group_widget->addWidget(slope_spinner);

    control_points_spinner = new SpinnerItemWidget(module, tr("Control Points Num"));
    control_points_spinner->setData(this->control_points_num);
    control_points_spinner->setRange(MIN_CONTROL_PT_NUM, MAX_CONTROL_PT_NUM);
    connect(control_points_spinner, &SpinnerItemWidget::sigValueChanged, this, &GammaCurveRGB::slotControlPointsNumChanged);
    adjust_group_widget->addWidget(control_points_spinner);

    QHBoxLayout *label_layout = new QHBoxLayout;
    pos_label = new QLabel("Out of chart");
    pos_label->setAlignment(Qt::AlignCenter);
    pos_label->setFixedWidth(230);
    pos_label->setMargin(5);
    label_layout->addWidget(pos_label, Qt::AlignCenter);
    adjust_group_widget->addLayout(label_layout);

    // Reference Set Group
    GroupItemWidget *reference_group_widget = new GroupItemWidget("Reference Set");
    reference_group_widget->setConfigSetHidden();
    QHBoxLayout *reference_layout = new QHBoxLayout;
    reference_set_combo = new QComboBox();
    reference_set_combo->addItem("Set1");
    reference_set_combo->addItem("Set2");
    reference_set_combo->addItem("Set3");
    connect(reference_set_combo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &GammaCurveRGB::slotReferSetSelect);

    refer_set[0] << QVariantList();
    refer_set[1] << QVariantList();
    refer_set[2] << QVariantList();

    reference_save_button = new QPushButton();
    connect(reference_save_button, &QPushButton::clicked, this, &GammaCurveRGB::slotReferSetSave);

    reference_apply_button = new QPushButton();
    reference_apply_button->setEnabled(false);
    connect(reference_apply_button, &QPushButton::clicked, this, &GammaCurveRGB::slotReferSetApply);
    reference_layout->addWidget(reference_set_combo);
    reference_layout->addWidget(reference_save_button);
    reference_layout->addWidget(reference_apply_button);
    reference_group_widget->addLayout(reference_layout);

    // Reset Group
    GroupItemWidget *reset_group_widget = new GroupItemWidget("Reset");
    reset_group_widget->setConfigSetHidden();
    reset_button = new QPushButton();
    connect(reset_button, &QPushButton::clicked, this, &GammaCurveRGB::resetCurve);
    reset_group_widget->addWidget(reset_button);

    QVBoxLayout *wraper_layout = new QVBoxLayout();
    wraper_layout->setAlignment(Qt::AlignTop);
    wraper_layout->addWidget(adjust_group_widget);
    wraper_layout->addWidget(reference_group_widget);
    wraper_layout->addWidget(reset_group_widget);

    QWidget *widget = new QWidget;
    widget->setLayout(wraper_layout);
    scroll_area = new QScrollArea();
    scroll_area->setWidget(widget);
    main_layout->addWidget(scroll_area,3);
}

void GammaCurveRGB::slotParamsChanged(QVariant)
{
    double coeff = this->coeff_spinner->getData().toDouble();
    double slope = this->slope_spinner->getData().toDouble();

    updatePoints(initPoints(coeff, slope), true);
}

QList<QPointF> GammaCurveRGB::initPoints(double coefficient, double slope_at_zero)
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

void GammaCurveRGB::addLine2Chart()
{
    CubicCurveChartView *gamma_line_chart = line_chart_widget->getLineChart();
    if (gamma_line_chart != nullptr) {
        QList<QPointF> points_sRGB;
        QList<QPointF> points_bt709;
        for (int i = 0; i < MAX_POINT_NUM; i++) {
            points_sRGB.append(QPointF(i, gamma_table_sRGB[i]));
            points_bt709.append(QPointF(i, gamma_table_bt709[i]));
        }
        QtCharts::QLineSeries *series_sRGB = createLineSeries("sRGB", QColor(0, 255, 0), points_sRGB);
        QtCharts::QLineSeries *series_bt709 = createLineSeries("BT709", QColor(0, 0, 255), points_bt709);
        gamma_line_chart->addLine(series_sRGB);
        gamma_line_chart->addLine(series_bt709);
    }
}

QtCharts::QLineSeries *GammaCurveRGB::createLineSeries(QString name, QColor color, QList<QPointF> points)
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


MapCurve::MapCurve(QString module, QString title, QWidget *parent):
    GammaCurve(module, title, parent)
{
}

void MapCurve::initUI()
{
    initCenterArea();
    retranslateUi();
}

void MapCurve::initCenterArea()
{
    initChart();

    QLayout * button_layout = initCenterTop();
    QVBoxLayout *v_layout = new QVBoxLayout();

    v_layout->addLayout(button_layout);
    v_layout->addWidget(map_curve_chart);
    main_layout->addLayout(v_layout);
}

void MapCurve::initChart()
{
    MAX_X = 96;
    MAX_Y = 1048576;
    map_curve_chart  = new CubicCurveChartView(this);
    map_curve_chart->initMapCurve(MAX_X, MAX_Y);
    series_l = createLineSeries("map_curve_l", QColor(255, 0, 0));
    series_r = createLineSeries("map_curve_r", QColor(0, 255, 0));
    series_end = createLineSeries("map_curve_end", QColor(0, 0, 255));
    map_curve_chart->addLine(series_l);
    map_curve_chart->addLine(series_r);
    map_curve_chart->addLine(series_end);
    connect(map_curve_chart, &CubicCurveChartView::sigMouseMove, this, &MapCurve::updateMousePos);
    initEvent();
}

QLayout *MapCurve::initCenterTop()
{
    QHBoxLayout *button_layout = new QHBoxLayout();
    pos_label = new QLabel("Out of chart");
    pos_label->setAlignment(Qt::AlignCenter);
    pos_label->setMargin(5);
    refresh_button = new QPushButton();
    refresh_button->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);

    connect(refresh_button, &QPushButton::clicked, this, &MapCurve::slotSendGetRecFshdrMapcurveInfoCmd);

    button_layout->setAlignment(Qt::AlignCenter);
    button_layout->addWidget(pos_label, 8);
    button_layout->addWidget(refresh_button, 2, Qt::AlignCenter);

    return button_layout;
}

QtCharts::QLineSeries *MapCurve::createLineSeries(QString name, QColor color)
{
    QtCharts::QLineSeries *series = new QtCharts::QLineSeries();
    QPen pen = series->pen();
    pen.setColor(color);
    pen.setWidth(3);

    series->setPen(pen);
    series->setName(name);
    series->setPointsVisible(false);
    series->setUseOpenGL(true);

    return series;
}

void MapCurve::setParam(QVariant)
{
}

void MapCurve::retranslateUi()
{
    refresh_button->setText(tr("refresh curve"));
}

void MapCurve::setData(QVariant)
{

}

QVariant MapCurve::getData()
{
    return QVariant();
}

void MapCurve::slotSendGetRecFshdrMapcurveInfoCmd()
{
    if (!NetworkClientHelper::getInstance()->isNormalStatus()) {
        QMessageBox::warning(this, tr("Warning"), tr("Network is not connected!"), QMessageBox::Ok);
        return;
    }

    NetworkClientHelper::getInstance()->setCallerInfo(0, NULL);

    QJsonObject id_info;
    id_info[MSG_KEY_CMD] = CMD_GET_FSHDR_MAPCURVE_INFO;
    int id = GlobalData::getInstance()->getJsonrpcId();
    NetworkClientHelper::getInstance()->pushIdInfoJsonrpc(id, id_info);

    QJsonObject send_obj;
    send_obj[MSG_KEY_METHOD] = METHOD_GET_FSHDR_MAPCURVE_INFO;
    send_obj[MSG_KEY_ID] = id;
    send_obj[MSG_KEY_JSONRPC] = MSG_JSONRPC_VERSION;
    NetworkClientHelper::getInstance()->sendCmd(std::move(send_obj));
}

void MapCurve::initEvent()
{
    shared_ptr<NetworkClientHelper> network_helper = NetworkClientHelper::getInstance();
    connect(network_helper.get(), &NetworkClientHelper::sigRecFshdrMapcurveInfo, this, &MapCurve::slotRefreshCurve);
}

void MapCurve::onOpenWindow()
{
    slotSendGetRecFshdrMapcurveInfoCmd();
}

void MapCurve::slotRefreshCurve(int id)
{
    QJsonObject obj = NetworkClientHelper::getInstance()->popCmdDataJsonrpc(id);
    QJsonObject result_obj = obj[MSG_KEY_RESULT].toObject();
    if (result_obj.isEmpty()) {
        QMessageBox::warning(this, tr("Error"), "Failed to update all charts.", QMessageBox::Ok);
        return;
    }

    series_l->clear();
    series_r->clear();
    series_end->clear();

    int x = 0;
    int y_max = 0;
    QJsonObject params_obj = result_obj[MSG_KEY_PARAMS].toObject();

    if (params_obj.contains("EV_FMT")) {
        ev_fmt = params_obj["EV_FMT"].toInt();
    }
    MAX_X = 0;
    knee_by_ev = (1 << (11 + ev_fmt));
    max_value_by_ev = (1 << (12 + ev_fmt)) - 1;

    if (params_obj.contains("MapCurveL")) {
        QJsonArray value_array = params_obj["MapCurveL"].toArray();
        for (int i = 0; i < value_array.count(); i++) {
            *series_l << QPointF(getLumaByX(x), value_array.at(i).toInt());
            y_max = (value_array.at(i).toInt() > y_max) ? value_array.at(i).toInt() : y_max;
            x++;
        }
    }
    x--;
    if (params_obj.contains("MapCurveR")) {
        QJsonArray value_array = params_obj["MapCurveR"].toArray();
        for (int i = 0; i < value_array.count(); i++) {
            *series_r << QPointF(getLumaByX(x), value_array.at(i).toInt());
            y_max = (value_array.at(i).toInt() > y_max) ? value_array.at(i).toInt() : y_max;
            x++;
        }
    }
    x--;
    if (params_obj.contains("MapCurveEnd")) {
        QJsonArray value_array = params_obj["MapCurveEnd"].toArray();
        for (int i = 0; i < value_array.count(); i++) {
            *series_end << QPointF(getLumaByX(x), value_array.at(i).toInt());
            y_max = (value_array.at(i).toInt() > y_max) ? value_array.at(i).toInt() : y_max;
            x++;
        }
    }
    MAX_Y = y_max;
    map_curve_chart->setRange(MAX_X, MAX_Y);
}

int MapCurve::getLumaByX(int x)
{
    int luma = 0;
    if (max_value_by_ev <= 65536) {
        if (x < 65) {
            luma = x << (5  + ev_fmt);
        } else if (x < 81) {
            x -= 64;
            luma = MIN((x << (7 + ev_fmt)) + knee_by_ev, max_value_by_ev);
        } else {
            luma = MAX_X;
        }
    } else {
        if (x < 65) {
            luma = x << 9;
        } else if (x < 81) {
            x -= 64;
            luma = MIN((x << 11) + (1 << 15), (1 << 16));
        } else if  (x < 97) {
            x -= 80;
            luma = MIN(((max_value_by_ev - (1 << 16)) * x + 8) / 16 + (1 << 16), max_value_by_ev);
        } else {
            luma = MAX_X;
        }
    }
    MAX_X = luma > MAX_X ? luma : MAX_X;
    return luma;
}
