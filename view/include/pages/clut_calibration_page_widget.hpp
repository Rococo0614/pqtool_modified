#ifndef CLUT_CALIBRATION_PAGE_WIDGET_HPP
#define CLUT_CALIBRATION_PAGE_WIDGET_HPP

#include "base_calibration_page_widget.hpp"
#include "combobox_calibration_item_widget.hpp"
#include "lineedit_calibration_item_widget.hpp"
#include "otp_pdaf_cali.h"
#include <QPushButton>
#include <QMessageBox>
#include <QGroupBox>

#define CLUT_SIZE_183X 3276
#define CLUT_SIZE 4913
#define MAX_CLUT_VALUE 1023
#define MAX_RGB_VALUE 4096
#define MAX_HUE_VALUE 5760
#define MAX_SATURATION_VALUE 4096
#define MAX_LIGHTNESS_VALUE 4096
#define SCALE_UNIT 50
#define HUE_LENGTH (37)
#define SAT_LENGTH (21)
#define KERNEL_SIZE 5

typedef struct _HSL_ATTR_S {
	qint16 HByH[HUE_LENGTH];
	quint16 SByH[HUE_LENGTH];
	quint16 LByH[HUE_LENGTH];
	quint16 SByS[SAT_LENGTH];
} HSL_ATTR_S;

typedef struct {
    quint16 r;
    quint16 g;
    quint16 b;
} RGB;

typedef struct {
    int h;
    int s;
    int l;
} HSL;

class CLUTCalibrationPageWidget : public BaseCalibrationPageWidget
{
    Q_OBJECT

public:
    explicit CLUTCalibrationPageWidget(QWidget *parent = nullptr);
    ~CLUTCalibrationPageWidget();

protected:
    void initLeftArea() override;
    void retranslateUi() override;
    void initEvent();
    void generateRgbPairs();
    bool calibration() override;
    QJsonArray getCalibrationData() override;
    void initClutTable();
    void releaseClutTable();
    void handleImportData(QString name, QJsonObject data);
    virtual QJsonObject GetModuleObjectToNetworkJsonrpc() override;
    QGroupBox *createHslAdjustGroupbox();
private:
    void rgb_to_hsl(RGB *rgb, HSL* hsl);
    void hsl_to_rgb(HSL *hsl, RGB *rgb);
    void interpolate_by_hue(int hue, HSL_ATTR_S *hsl_attr,
            qint16 *angle,  quint16 *s_scale, quint16 *l_scale);
    void interpolate_by_sat(int sat, HSL_ATTR_S *hsl_attr, quint16 *scale);
    void clut_denoise(RGB *in, RGB *out, int *wgt);
    void hsl_by_hue(HSL* in, HSL *out, qint16 angle, quint16 s_scale, quint16 l_scale);
    void saturation_by_saturation(HSL* in, HSL *out, quint16 scale);
    void clut_adjust_by_hsl(RGB *in, RGB *out);
    void cal_gaussian_kernel(int *wgt, float sigma);

private:
    QLabel *ccm_matrix_label;
    QPushButton *clut_import_ccm_cali_btn;
    const int MATRIX_ROW = 3;
    const int MATRIX_COL = 3;
    QTableWidget *clut_matrix_table;
    QCheckBox   *luminance_invariant_check;
    QCheckBox   *gray_scale_invariant_check;
    ComboboxCaliItemWidget *color_space_item;
    ComboboxCaliItemWidget *isp_gamma_item;
    ComboboxCaliItemWidget *target_gamma_item;
    ComboboxCaliItemWidget *mode_item;
    LineEditCaliItemWidget *smoothness_item;
    QPushButton *color_check_free_btn;
    QLabel *input_label;
    QLabel *rgb_generate_label;
    QLabel *lut_operation_label;
    QPushButton *edit_source_roi_btn;
    QPushButton *edit_target_roi_btn;
    QWidget *source_roi_control_widget{nullptr};
    QWidget *target_roi_control_widget{nullptr};
    BinaryItemWidget *hsl_enable;
    DoubleSpinnerItemWidget *sigma_spinner;
    UniversalMatrixItemWidget *hbyh_matrix;
    UniversalMatrixItemWidget *sbyh_matrix;
    UniversalMatrixItemWidget *lbyh_matrix;
    UniversalMatrixItemWidget *sbys_matrix;
    int *h_lut{nullptr};
    int *s_lut{nullptr};
    int *v_lut{nullptr};
    int *r_lut{nullptr};
    int *g_lut{nullptr};
    int *b_lut{nullptr};
};

#endif // CLUT_CALIBRATION_PAGE_WIDGET_HPP
