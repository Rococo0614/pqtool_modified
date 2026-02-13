#include "dpc_calibration_page_widget.hpp"

DPCCalibrationPageWidget::DPCCalibrationPageWidget(QWidget *parent) : BaseCalibrationPageWidget(static_cast<int>(CALIBRATION_MODULE_ID::DPC), parent)
{
    initLeftArea();
    retranslateUi();
}

void DPCCalibrationPageWidget::initLeftArea()
{
    initImportRawUI();

    bright_defeat_cali_item = new CheckboxCaliItemWidget("Bright defeat calibration");
    bright_defeat_cali_item->setKey("bright_defeat_cali");
    param_layout->addWidget(bright_defeat_cali_item);
    bright_threshold_item = new LineEditCaliItemWidget("Bright threshold");
    bright_threshold_item->setKey("bright_threshold");
    bright_threshold_item->setData(200);
    param_layout->addWidget(bright_threshold_item);
    dark_defeat_cali_item = new CheckboxCaliItemWidget("Dark defeat calibration");
    dark_defeat_cali_item->setKey("dark_defeat_cali");
    param_layout->addWidget(dark_defeat_cali_item);
    dark_threshold_item = new LineEditCaliItemWidget("Dark threshold");
    dark_threshold_item->setKey("dark_threshold");
    dark_threshold_item->setData(200);
    param_layout->addWidget(dark_threshold_item);
    static_defeat_pixel_cali_btn = new QPushButton;
    param_layout->addWidget(static_defeat_pixel_cali_btn);
    QHBoxLayout *coord_layout = new QHBoxLayout;
    export_defeat_coord_btn = new QPushButton;
    import_defeat_coord_btn = new QPushButton;
    coord_layout->addWidget(export_defeat_coord_btn);
    coord_layout->addWidget(import_defeat_coord_btn);
    param_layout->addLayout(coord_layout);

    initCaliUI();
    initWriteDataUI();
    initExportDataUI();
}

void DPCCalibrationPageWidget::retranslateUi()
{
    BaseCalibrationPageWidget::retranslateUi();

    static_defeat_pixel_cali_btn->setText(tr("Static defeat pixel calibration"));
    export_defeat_coord_btn->setText(tr("Export defeat coord."));
    import_defeat_coord_btn->setText(tr("Import defeat coord."));
}
