#include "bnr_calibration_page_widget.hpp"

BNRCalibrationPageWidget::BNRCalibrationPageWidget(QWidget *parent) : BaseCalibrationPageWidget(static_cast<int>(CALIBRATION_MODULE_ID::BNR), parent)
{
    initLeftArea();
    retranslateUi();
}

void BNRCalibrationPageWidget::initLeftArea()
{
    initImportRawUI();
    initImportBlcUI();
    initImportLscUI();
    initCaliUI();

    adjust_calibration_data_btn = new QPushButton;
    param_layout->addWidget(adjust_calibration_data_btn);

    initWriteDataUI();
    initExportDataUI();
}

void BNRCalibrationPageWidget::retranslateUi()
{
    BaseCalibrationPageWidget::retranslateUi();

    adjust_calibration_data_btn->setText(tr("Adjust calibration data"));
}
