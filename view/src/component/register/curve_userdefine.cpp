#include "global_data.hpp"
#include "curve_userdefine.hpp"
#include "utils.hpp"
#include "group_widget.hpp"
#include "network_client_helper.hpp"

#include <QLineEdit>
#include <QFile>
#include <QTextStream>
#include <QIntValidator>
#include <QMessageBox>

#define MAX_POINT_NUM (1024)

CurveUserDefine::CurveUserDefine()
{
    main_layout = new QHBoxLayout(this);
    this->setLayout(main_layout);
}

QVariant CurveUserDefine::getValue(const SecKeyType &reg_section_key)
{
    QVariant value = 0;
    value = GlobalData::getInstance()->getRegValue(reg_section_key.first, reg_section_key.second);
    return value;
}

QVariant CurveUserDefine::setValue(const SecKeyType &reg_section_key, QVariant value)
{
    GlobalData::getInstance()->setRegValue(reg_section_key.first, reg_section_key.second, value);
}

QVariant CurveUserDefine::validateValue(QVariant value)
{
    if (value < this->value_min) return this->value_min;
    if (value > this->value_max) return this->value_max;

    return value;
}

void CurveUserDefine::setRange(int min, int max)
{
    this->value_min = min;
    this->value_max = max;
}

void CurveUserDefine::handleImportTable(QString file_path)
{
    QFile inut_file(file_path);
    if(!inut_file.open(QIODevice::ReadOnly)) {
        qDebug("Import file :%s error", qUtf8Printable(file_path));
        return;
    }

    QString data = "";
    int reg_idx = 0;
    QTextStream in(&inut_file);
    while (!in.atEnd())
    {
       data = in.readLine();
       QList<QVariant> data_list = utils::stringToList<QVariant>(data);
       if (reg_idx > reg_section_key_list.size() - 1) break;
       QList<QVariant> data_array;
       for (int i = 0; i < data_list.size(); i++) {
           data_array.append(data_list[i].toDouble());
       }

       GlobalData::getInstance()->setRegValue(reg_section_key_list[reg_idx].first, reg_section_key_list[reg_idx].second, data_array);
       reg_idx++;
    }
    inut_file.close();
}

void CurveUserDefine::handleExportTable(QString file_path)
{
    QFile output_file(file_path);
    if(!output_file.open(QIODevice::WriteOnly)) {
        qDebug("Export file :%s error", qUtf8Printable(file_path));
        return;
    }

    QTextStream out(&output_file);
    QVariantList value_list = getValue(reg_section_key_list[0]).toList();
    for (int i = 0; i < value_list.size(); i++) {
        out << value_list[i].toString();
        if (i < value_list.size() - 1) {
          out << ",";
        }
    }
}

void CurveUserDefine::refreshData(const SecKeyType &reg_section_key)
{
    //int idx = reg_section_key_list.indexOf(reg_section_key);
}

QVariant CurveUserDefine::getData()
{
    QVariantList data;
//    QList<qreal> list = chart->getData(0);
//    for (int i = 0; i < list.size(); i++) {
//        data.append(list[i]);
//    }

    data.append(chart->getGlobalTone());
    // data.append(chart->getDarkTone());
    // data.append(chart->getBritTone());
    return data;
}

void CurveUserDefine::setData(QVariant data)
{
    chart->setData(data);
//    QVariantList value_list = data.toList();
//    chart->setData(value_list);
}

void CurveUserDefine::initUI()
{
    initLeftArea();
    initCenterArea();
    initEvent();
}

void CurveUserDefine::init_chart()
{
    QString module = "DRC";

    chart = new CubicCurveChartView;
    chart->initToneCurve();
    chart->setUserDefineMode(true);
    chart->addRegSectionKey("DRC", "reg_CurveUserDefine");
    chart->setPoints(initDefaultPoints(), false);
    chart->setControlPointsNum(control_points_spinner->getData().toInt());
    chart->updateToneCurve();

//    wdr_hist_widget = new HistItemWidget(module, "WDR Histogram", WDR_HIST);
//    wdr_hist_widget->addRegSectionKey("DRC", "reg_wdr_hist");
}

void CurveUserDefine::initLeftArea()
{
    initItemWidgets();

    GroupItemWidget *exposure_ratio = new GroupItemWidget("User Define Exposure Ratio");
    exposure_ratio->setConfigSetHidden();
    exposure_ratio->addWidget(exp_ratio);

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
    // adjust_group_widget->addLayout(label_layout);

    // control points area
    GroupItemWidget *control_group_widget = new GroupItemWidget("Control Points");
    control_group_widget->setConfigSetHidden();
    control_group_widget->addWidget(control_points_spinner);
    control_group_widget->addLayout(label_layout);

    QHBoxLayout *reference_layout = new QHBoxLayout;
    reference_layout->addWidget(reference_set_combo);
    reference_layout->addWidget(reference_save_button);
    reference_layout->addWidget(reference_apply_button);

    GroupItemWidget *reference_group_widget = new GroupItemWidget("Reference Set");
    reference_group_widget->setConfigSetHidden();
    reference_group_widget->addLayout(reference_layout);

    GroupItemWidget *reset_group_widget = new GroupItemWidget("Reset");
    reset_group_widget->setConfigSetHidden();
    reset_group_widget->addWidget(reset_button);

//    GroupItemWidget *table_group_widget = new GroupItemWidget("Manual Generation");
//    table_group_widget->setConfigSetHidden();
//    table_group_widget->addWidget(edit_matrix_button);

    QVBoxLayout *wraper_layout = new QVBoxLayout();
    wraper_layout->setAlignment(Qt::AlignTop);
    wraper_layout->addWidget(exposure_ratio);
    wraper_layout->addWidget(control_group_widget);
    // wraper_layout->addWidget(adjust_group_widget);
    wraper_layout->addWidget(reference_group_widget);
    wraper_layout->addWidget(reset_group_widget);
//    wraper_layout->addWidget(table_group_widget);

    QWidget *widget = new QWidget;
    widget->setLayout(wraper_layout);
    QScrollArea *scroll_area = new QScrollArea();
    scroll_area->setMaximumWidth(470);
    scroll_area->setWidget(widget);
    main_layout->addWidget(scroll_area);
}

void CurveUserDefine::initCenterArea()
{
    init_chart();
//    QWidget *scroll_widget = new QWidget;
//    QHBoxLayout *scroll_layout = new QHBoxLayout(scroll_widget);
//    scroll_layout->setSpacing(8);
//    scroll_layout->setMargin(8);

//    scroll_layout->addWidget(chart);
    connect(chart, &CubicCurveChartView::sigMouseMove, this, &CurveUserDefine::updateMousePos);
//    connect(chart, &CubicCurveChartView::sigUpdateWdrHistogram, this, &CurveUserDefine::slotUpdateWdrHistogram);

//    scroll_layout->addWidget(wdr_hist_widget);

//    QScrollArea *scroll_area = new QScrollArea();
//    scroll_area->setWidget(scroll_widget);
    connect(chart, &CubicCurveChartView::sigPageWriteData, this, &CurveUserDefine::handlePageWriteData);
    connect(chart, &CubicCurveChartView::sigValueChanged, this, &CurveUserDefine::handleValueChanged);
    main_layout->addWidget(chart);
}

void CurveUserDefine::initEvent()
{
    shared_ptr<NetworkClientHelper> network_helper = NetworkClientHelper::getInstance();
}

void CurveUserDefine::initItemWidgets()
{
    QString module = "DRC";

    exp_ratio = new SpinnerItemWidget(module, "UserDefine Exposure Ratio");
    exp_ratio->setData(4);
    exp_ratio->setRange(4, 255);
    exp_ratio->setChmod(MOD_RO);
//    connect(exp_ratio, &SpinnerItemWidget::sigValueChanged, this, &CurveUserDefine::slotUpdateExpRatio);
    connect(exp_ratio, &SpinnerItemWidget::sigValueChanged, [=](QVariant value){
        chart->setExpRatio(value.toInt());
    });

    x_adjust_spinner = new SpinnerItemWidget(module, "X");
    x_adjust_spinner->setData(0);
    x_adjust_spinner->setRange(0, 768);
    y_adjust_spinner = new SpinnerItemWidget(module, "Y");
    y_adjust_spinner->setData(0);
    y_adjust_spinner->setRange(0, 4095);

    add_ctrlpoint_btn = new QPushButton(tr("Add CtrlPt"));
    connect(add_ctrlpoint_btn, &QPushButton::clicked, this, &CurveUserDefine::handleAddCtrlPt);
    set_btn = new QPushButton(tr("Set"));
    connect(set_btn, &QPushButton::clicked, this, &CurveUserDefine::handleSetEvent);
    pos_label = new QLabel("Out of chart");
    pos_label->setAlignment(Qt::AlignCenter);

    control_points_spinner = new SpinnerItemWidget(module, "Control Points");
    control_points_spinner->setData(this->control_points_num);
    control_points_spinner->setRange(MIN_CONTROL_PT_NUM, MAX_CONTROL_PT_NUM);

    reference_set_combo = new QComboBox();
    reference_set_combo->addItem("Set1");
    reference_set_combo->addItem("Set2");
    reference_set_combo->addItem("Set3");
    refer_set[0] << QVariantList();
    refer_set[1] << QVariantList();
    refer_set[2] << QVariantList();
    reference_save_button = new QPushButton(tr("Save"));
    reference_apply_button = new QPushButton(tr("Use"));
    reference_apply_button->setEnabled(false);

    reset_button = new QPushButton(tr("Reset"));
//    edit_matrix_button = new MatrixItemWidget(module, "CurveUserDefine");
//    edit_matrix_button->setRegValueType(TYPE_ARRAY);
//    edit_matrix_button->addRegSectionKey(module, "reg_CurveUserDefine");

    connect(control_points_spinner, &SpinnerItemWidget::sigValueChanged,
            this, &CurveUserDefine::slotControlPointsNumChanged);

    connect(reference_set_combo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &CurveUserDefine::slotReferSetSelect);

    connect(reference_save_button, &QPushButton::clicked,
            this, &CurveUserDefine::slotReferSetSave);

    connect(reference_apply_button, &QPushButton::clicked,
            this, &CurveUserDefine::slotReferSetApply);

    connect(reset_button, &QPushButton::clicked,
            this, &CurveUserDefine::resetCurve);
}

void CurveUserDefine::slotControlPointsNumChanged(QVariant num)
{
    this->control_points_num = num.toInt();
    chart->setControlPointsNum(control_points_num);
}

void CurveUserDefine::handleAddCtrlPt()
{

}

void CurveUserDefine::handleSetEvent()
{

}

void CurveUserDefine::updateMousePos(double x, double y)
{
    if (x < 0 || x > 4096 || y < 0 || y > 4096) {
        pos_label->setText("Out of Chart!");
    } else {
        pos_label->setText(QString("Position:X = %1, Y = %2").arg(int(x)).arg(int(y)));
    }
}

void CurveUserDefine::resetCurve()
{
    chart->setPoints(initDefaultPoints());
    chart->setControlPointsNum(control_points_num);
}

void CurveUserDefine::slotReferSetSelect(QVariant v)
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

void CurveUserDefine::slotReferSetSave()
{
    int index = reference_set_combo->currentIndex();
    QList<qreal> data = chart->getData(0);
    for (int i=0; i < data.size(); i++) {
        refer_set[index].append(data[i]);
    }

    if (!reference_apply_button->isEnabled())
        reference_apply_button->setEnabled(true);
}

void CurveUserDefine::slotReferSetApply()
{
    int index = reference_set_combo->currentIndex();
    chart->setData(refer_set[index]);
}

QList<QPointF> CurveUserDefine::initDefaultPoints()
{
    QList<QPointF> points;
    int point_num = MAX_POINT_NUM + 1;

    for (int i = 0; i < point_num; i++) {
        points.append(QPointF(i, 4 * i));
    }
    return points;
}

void CurveUserDefine::slotUpdateWdrHistogram(QVariant data)
{
//    wdr_hist_widget->setData(data);

//    HistChartView * chart = wdr_hist_widget->getHisChartView();
//    chart->setDisplayRange(0, data.toList().size());
}

void CurveUserDefine::slotUpdateExpRatio(QVariant value)
{
    chart->setExpRatio(value.toInt());
}

void CurveUserDefine::handlePageWriteData()
{
    emit sigPageWriteData();
}

void CurveUserDefine::handleValueChanged(QVariant value)
{
    emit sigValueChanged(value);
}

void CurveUserDefine::handleUpdateTheme(bool dark_theme_enabled)
{
    exp_ratio->handleUpdateTheme(dark_theme_enabled);
    control_points_spinner->handleUpdateTheme(dark_theme_enabled);
    chart->handleUpdateTheme(dark_theme_enabled);
}
