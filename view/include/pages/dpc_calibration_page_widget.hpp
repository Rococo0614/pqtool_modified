#ifndef DPC_CALIBRATION_PAGE_WIDGET_HPP
#define DPC_CALIBRATION_PAGE_WIDGET_HPP

#include "base_calibration_page_widget.hpp"
#include "checkbox_calibration_item_widget.hpp"
#include "lineedit_calibration_item_widget.hpp"
#include <QPushButton>

class DPCCalibrationPageWidget : public BaseCalibrationPageWidget
{
    Q_OBJECT

public:
    explicit DPCCalibrationPageWidget(QWidget *parent = nullptr);

protected:
    void initLeftArea() override;
    void retranslateUi() override;

private:
    CheckboxCaliItemWidget *bright_defeat_cali_item;
    CheckboxCaliItemWidget *dark_defeat_cali_item;
    LineEditCaliItemWidget *bright_threshold_item;
    LineEditCaliItemWidget *dark_threshold_item;
    QPushButton *static_defeat_pixel_cali_btn;
    QPushButton *export_defeat_coord_btn;
    QPushButton *import_defeat_coord_btn;
};

#endif // DPC_CALIBRATION_PAGE_WIDGET_HPP
