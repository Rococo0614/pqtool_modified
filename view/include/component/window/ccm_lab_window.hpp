#ifndef CCM_LAB_WINDOW_HPP
#define CCM_LAB_WINDOW_HPP

#include "multi_image_viewer_widget.hpp"
#include "double_slider_calibration_item_widget.hpp"
#include "combobox_calibration_item_widget.hpp"
#include "lineedit_calibration_item_widget.hpp"
#include "network_client_helper.hpp"
#include "ccm_lab_img_window.hpp"
#include <QMainWindow>
#include <QGridLayout>
#include <QLabel>
#include <QComboBox>
#include <QPixmap>
#include <QGroupBox>
#include <QCheckBox>
#include <QTableWidget>

class CCMLabWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit CCMLabWindow(QWidget *parent = nullptr);
    void initUI();
    void initEvent();
    void GetLab(double *l, double *a, double *b);

protected:
    void keyReleaseEvent(QKeyEvent *event) override;
    void HandleLabChange(int row, int col);
    void SetLabTable(double *l, double *a, double *b, double *red, double *green, double *blue);
    void SetLabItem(int row, int col);
private:
    void closeEvent(QCloseEvent *event) override;
    void SetDefaultLab();

private slots:
    void ImportLabFile();
    void ExportLabFile();
    void ImportImage();
    void HandleImgLab();

signals:
    void sigLabChange(int row,int col);
    void sigLabOk();

private:
    QWidget *main_widget;
    QVBoxLayout *main_layout;
    QGroupBox *color_group;
    QTableWidget *lab_table;
    QPushButton *open_btn;
    QLineEdit *file_path;
    CcmLabImageWindow *labImg{nullptr};

    double red[24];
    double green[24];
    double blue[24];
    double lab_ref_l[24];
    double lab_ref_a[24];
    double lab_ref_b[24];
};

#endif
