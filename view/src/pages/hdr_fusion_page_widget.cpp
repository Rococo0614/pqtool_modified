#include "hdr_fusion_page_widget.hpp"
#include "module_define.hpp"
#include "global_data.hpp"
#include "line_chart_view.hpp"
#include <QFileDialog>

HDRFusionPageWidget::HDRFusionPageWidget(QWidget *parent) : BasePageWidget(MODULE_NAME_HDR, parent)
{
    initData();
    initUI();

    main_layout->setStretch(0, 5);
    main_layout->setStretch(1, 5);
    main_layout->setStretch(2, 1);
}

void HDRFusionPageWidget::initLeftArea()
{
    initGroupLeftArea();
}

void HDRFusionPageWidget::retranslateUi()
{
    load_button->setText(tr("load"));
    save_button->setText(tr("save"));

    BasePageWidget::retranslateUi();
}

void HDRFusionPageWidget::initCenterArea()
{
    QLabel *tm_curve_title_label = new QLabel(tr("Choose TM Curve"));
    tm_curve_combobox = new QComboBox;
    tm_curve_layout = new QHBoxLayout;
    cv = new LineChartView(8, this, TYPE_HDR);
    cv->initHDR();

    cv->setPoints(0, initPoints(1.0), false);
    cv->setPoints(1, initPoints(1.2), false);
    cv->setPoints(2, initPoints(0.8), false);
    cv->addRegSectionKey("isp_ltm", "reg_ltm_dark_lut");
    cv->addRegSectionKey("isp_ltm", "reg_ltm_brit_lut");
    cv->addRegSectionKey("isp_ltm", "reg_ltm_deflt_lut");

    QLayout *button_layout = initButtons();
    QVBoxLayout *v_layout = new QVBoxLayout();

    tm_curve_name_list.append(tr("Dark Curve"));
    tm_curve_name_list.append(tr("Bright Curve"));
    tm_curve_name_list.append(tr("Default Curve"));
    for (auto &tm_curve : tm_curve_name_list) {
        tm_curve_combobox->addItem(tm_curve);
    }

    tm_curve_layout->addWidget(tm_curve_title_label);
    tm_curve_layout->addWidget(tm_curve_combobox);

    v_layout->addLayout(tm_curve_layout);
    v_layout->addLayout(button_layout);
    v_layout->addWidget(cv);
    main_layout->addLayout(v_layout);

    connect(tm_curve_combobox, &QComboBox::currentTextChanged, this, &HDRFusionPageWidget::handleSelectCurve);
    connect(cv, &LineChartView::sigSelectCurve, tm_curve_combobox, &QComboBox::setCurrentIndex);
}

QLayout *HDRFusionPageWidget::initButtons()
{
    QHBoxLayout *button_layout = new QHBoxLayout();
    load_button = new QPushButton();
    save_button = new QPushButton();
    load_button->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    save_button->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);

    connect(load_button, &QPushButton::clicked, this, &HDRFusionPageWidget::openFile);
    connect(save_button, &QPushButton::clicked, this, &HDRFusionPageWidget::saveFile);

    button_layout->setAlignment(Qt::AlignCenter);
    button_layout->addWidget(load_button);
    button_layout->addWidget(save_button);

    return button_layout;
}

QList<QPointF> HDRFusionPageWidget::initPoints(double coefficient)
{
    QList<QPointF> points;
    int point_num = 32;
    int max_x = 256;
    int max_y = 4096;
    int step_x = max_x / point_num;

    for (int i = 0; i < point_num + 1; i++) {
        int x = i * step_x;
        double norm_x = static_cast<double>(x) / max_x;
        double norm_y = qPow(norm_x, coefficient);
        int y = static_cast<int>(norm_y * max_y);

        points.append(QPointF(x, y));
    }

    return points;
}

void HDRFusionPageWidget::openFile()
{
    QString filePath = QFileDialog::getOpenFileName(this,
          tr("Open HDR Settings file"), "./", tr("Json files (*.json)"));

    if (filePath.isEmpty()) {
        return;
    }

//    cv->loadJsonFile(filePath);
}

void HDRFusionPageWidget::saveFile()
{
    QString selectedFilter;
    QString filePath = QFileDialog::getSaveFileName(this,
            tr("Save HDR Settings file"), "",
            tr("Json files (*.json)"), &selectedFilter);

    if (filePath.isEmpty()) {
        return;
    }

//    cv->saveJsonFile(filePath);
}

void HDRFusionPageWidget::handleSelectCurve(const QString &text)
{
    int line_idx = tm_curve_name_list.indexOf(text);
    cv->selectLineSeries(line_idx);
}
