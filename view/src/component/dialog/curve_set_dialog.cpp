#include "curve_set_dialog.hpp"
#include "utils.hpp"
#include <QFileDialog>

const int ITEM_WIDTH = 100;

using utils::RAW_BITS;
using utils::RAW_PACK_TYPE;

QStringList function_type_name = {
    "Line",
    "Gaussian",
    "Gamma"
};

CurveSetDialog::CurveSetDialog(QWidget *parent) :
    QDialog(parent)
{
    setWindowTitle(tr("Curve Set Dialog"));
    setWindowFlags(Qt::WindowCloseButtonHint);
    this->setMinimumSize(400, 200);

    initUI();
    retranslateUi();
}

CurveSetDialog::CurveSetDialog(double min_x, double max_x, double min_y, double max_y, QWidget *parent) :
    QDialog(parent)
{
    setWindowTitle(tr("Curve Set Dialog"));
    setWindowFlags(Qt::WindowCloseButtonHint);
    this->setMinimumSize(400, 200);

    this->min_x = min_x;
    this->max_x = max_x;
    this->min_y = min_y;
    this->max_y = max_y;

    initUI();
    retranslateUi();
}

void CurveSetDialog::initUI()
{
    main_layout = new QGridLayout(this);

    function_type_label = new QLabel;
    function_type_combox = new QComboBox;

    for (auto &func_name : function_type_name) {
        function_type_combox->addItem(func_name);
    }

    line_slope =  new SpinnerItemWidget("RegisterTable_CurveSet", "slope");
    line_intercept =  new SpinnerItemWidget("RegisterTable_CurveSet", "intercept");

    gaussian_std =  new DoubleSpinnerItemWidget("RegisterTable_CurveSet", "gaussian_std");
    gaussian_mean =  new SpinnerItemWidget("RegisterTable_CurveSet", "gaussian_mean");
    gaussian_gain =  new SpinnerItemWidget("RegisterTable_CurveSet", "gaussian_gain");
    gaussian_std->hide();
    gaussian_mean->hide();
    gaussian_gain->hide();

    gamma =  new DoubleSpinnerItemWidget("RegisterTable_CurveSet", "gamma");
    gamma->hide();

    apply_btn = new QPushButton;
    apply_btn->setFixedWidth(ITEM_WIDTH);
    ok_btn = new QPushButton;
    ok_btn->setFixedWidth(ITEM_WIDTH);

    QFont titleFont("Arial Black", 10);
    titleFont.setBold(true);

    main_layout->addWidget(function_type_label, 0, 0, Qt::AlignLeft);
    main_layout->addWidget(function_type_combox, 0, 1, Qt::AlignLeft);

    main_layout->addWidget(line_slope, 1, 0, Qt::AlignLeft);
    main_layout->addWidget(line_intercept, 2, 0, Qt::AlignLeft);

    main_layout->addWidget(gaussian_mean, 3, 0, Qt::AlignLeft);
    main_layout->addWidget(gaussian_std, 4, 0, Qt::AlignLeft);
    main_layout->addWidget(gaussian_gain, 5, 0, Qt::AlignLeft);

    main_layout->addWidget(gamma, 6, 0, Qt::AlignLeft);

    main_layout->addWidget(apply_btn, 8, 0, Qt::AlignRight);
    main_layout->addWidget(ok_btn, 8, 1, Qt::AlignRight);

    // combox
    connect(function_type_combox, QOverload<int>::of(&QComboBox::currentIndexChanged),
        this, &CurveSetDialog::slotFunctionChange);
    // btn
    connect(apply_btn, &QPushButton::clicked, this, &CurveSetDialog::handleApply);
    connect(ok_btn, &QPushButton::clicked, this, &CurveSetDialog::handleOk);
    // line
    connect(line_slope, &SpinnerItemWidget::sigValueChanged, this, &CurveSetDialog::slotLineParamUpdate);
    connect(line_intercept, &SpinnerItemWidget::sigValueChanged, this, &CurveSetDialog::slotLineParamUpdate);
    // gaussian
    connect(gaussian_std, &DoubleSpinnerItemWidget::sigValueChanged, this, &CurveSetDialog::slotGaussianParamUpdate);
    connect(gaussian_mean, &SpinnerItemWidget::sigValueChanged, this, &CurveSetDialog::slotGaussianParamUpdate);
    connect(gaussian_gain, &SpinnerItemWidget::sigValueChanged, this, &CurveSetDialog::slotGaussianParamUpdate);
    // gamma
    connect(gamma, &DoubleSpinnerItemWidget::sigValueChanged, this, &CurveSetDialog::slotGammaParamUpdate);
}

void CurveSetDialog::retranslateUi()
{
    function_type_label->setText(tr("CurveType"));
    apply_btn->setText(tr("Apply"));
    ok_btn->setText(tr("OK"));

    /* set range */
    // line
    line_slope->setData(0);
    line_slope->setRange(static_cast<int>(-max_y), static_cast<int>(max_y));
    line_intercept->setData(static_cast<int>(max_y/2));
    line_intercept->setRange(static_cast<int>(-max_y), static_cast<int>(max_y));
    // gaussian
    gaussian_std->setData(1.0);
    gaussian_std->setRange(0.1, 20); // TODO: check
    gaussian_mean->setData(0);
    gaussian_gain->setData(1);
    gaussian_mean->setRange(static_cast<int>(-max_x), static_cast<int>(max_x * 2));
    gaussian_gain->setRange(1, MAX(20, static_cast<int>(max_y / 100)));
    // gamma
    gamma->setData(1.0);
    gamma->setRange(0.1, 20);
}

void CurveSetDialog::handleOk()
{
    this->accept();
}

void CurveSetDialog::handleApply()
{
    QString selectedText = function_type_combox->currentText();

    if (selectedText == "Line") {
        slotLineParamUpdate();
    } else if (selectedText == "Gaussian") {
        slotGaussianParamUpdate();
    } else if (selectedText == "Gamma") {
        slotGammaParamUpdate();
    }
}

void CurveSetDialog::slotLineParamUpdate()
{
    double slope = line_slope->getData().toDouble();
    double intercept = line_intercept->getData().toDouble();

    emit sigLineParamsChanged(slope, intercept);
}

void CurveSetDialog::slotGaussianParamUpdate()
{
    double gaussian_std = this->gaussian_std->getData().toDouble();
    double gaussian_mean = this->gaussian_mean->getData().toDouble();
    double gaussian_gain = this->gaussian_gain->getData().toDouble();

    emit sigGaussianParamsChanged(gaussian_mean, gaussian_std, gaussian_gain);
}

void CurveSetDialog::slotGammaParamUpdate()
{
    double gamma = this->gamma->getData().toDouble();

    emit sigGammaParamsChanged(gamma);
}

void CurveSetDialog::slotFunctionChange()
{
    QString selectedText = function_type_combox->currentText();

    if (selectedText == "Line") {
        line_slope->show();
        line_intercept->show();
        gaussian_std->hide();
        gaussian_mean->hide();
        gaussian_gain->hide();
        gamma->hide();
    } else if (selectedText == "Gaussian") {
        line_slope->hide();
        line_intercept->hide();
        gaussian_std->show();
        gaussian_mean->show();
        gaussian_gain->show();
        gamma->hide();
    } else if (selectedText == "Gamma") {
        line_slope->hide();
        line_intercept->hide();
        gaussian_std->hide();
        gaussian_mean->hide();
        gaussian_gain->hide();
        gamma->show();
    }
}
