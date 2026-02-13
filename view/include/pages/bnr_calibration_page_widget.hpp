#ifndef BNR_CALIBRATION_PAGE_WIDGET_HPP
#define BNR_CALIBRATION_PAGE_WIDGET_HPP

#include "base_calibration_page_widget.hpp"
#include <QPushButton>

class BNRCalibrationPageWidget : public BaseCalibrationPageWidget
{
    Q_OBJECT

public:
    explicit BNRCalibrationPageWidget(QWidget *parent = nullptr);

protected:
    void initLeftArea() override;
    void retranslateUi() override;

private:
    QPushButton *adjust_calibration_data_btn;

};

#endif // BNR_CALIBRATION_PAGE_WIDGET_HPP
