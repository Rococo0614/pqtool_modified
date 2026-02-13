#ifndef CALIBRATION_PDAF_WINDOW_HPP
#define CALIBRATION_PDAF_WINDOW_HPP

#include "utils.hpp"
#include "multi_image_viewer_widget.hpp"
#include <QMainWindow>
#include <QLabel>
#include <QCheckBox>
#include <QLineEdit>
#include <QComboBox>

class CalibrationPdafWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit CalibrationPdafWindow(QWidget *parent = nullptr);
    void initUI();
    void initEvent();

protected:
    void keyReleaseEvent(QKeyEvent *event) override;

private:
    void closeEvent(QCloseEvent *event) override;
    void showLog(const QString &log, LOG_LEVEL log_level);
    bool creatDir();
    void calibrateGainMap();
    void calibrateDCC();
    int processRawImage(const QString file_path, int width, int height, int black_level_value, uint16_t* pixels);
    int getLRImage(uint16_t* processed_raw, int width, int height, uint16_t* image_L, uint16_t* image_R);
    float calculateBlockAverage(cv::Mat& image, int startX, int startY, int blockWidth, int blockHeight);
    void saveGainMap(const cv::Mat& gainMap, const QString file_path);
    void loadGainMap(cv::Mat& gainMap_L, cv::Mat& gainMap_R, const QString file_path);
    void saveGainMaptoYaml(const cv::Mat& gainMap_L, const cv::Mat& gainMap_R, const QString file_path);
    void calculateBlockParameters(int width, int height, int numBlocksX, int numBlocksY, int& blockWidth, int& blockHeight,
                                  int& paddingLeft, int& paddingRight, int& paddingTop, int& paddingBottom);
    double mean(const cv::Mat& img, int x, int y, int width, int height);
    double stddev(const cv::Mat& img, int x, int y, int width, int height, double mean);
    double zncc(const cv::Mat& img1, int x1, int y1, const cv::Mat& img2, int x2, int y2, int width, int height);
    double SAD(const cv::Mat& img1, int x1, int y1, const cv::Mat& img2, int x2, int y2, int width, int height);
    double fit_parabola(double x1, double y1, double x2, double y2, double x3, double y3);
    double calculate_optimal_shift(const cv::Mat& img1, const cv::Mat& img2, int x, int y, int width, int height, int max_shift);

private slots:
    void OpenSaveFilePath();
    void OpenRawFilePath();
    void OpenRawsFilePath();
    bool CheckSaveFilePath();
private:
    QWidget *main_widget;
    QHBoxLayout *main_layout;
    QGridLayout *left_layout;
    MultiImageViewerWidget *img_viewer;
    ImageLabel *image_label;

    QPushButton *save_btn;
    QPushButton *open_btn;
    QPushButton *open_raw_btn;
    QPushButton *calibrate_GainMap_btn;
    QPushButton *calibrate_DCC_btn;
    QLineEdit *save_path;
    QLineEdit *open_path;
    QLineEdit *raw_file_path;
    QLineEdit *raw_width_edit;
    QLineEdit *raw_height_edit;
    QLineEdit *black_level_edit;
    QLineEdit *numBlocksX_edit;
    QLineEdit *numBlocksY_edit;
    QCheckBox *is_gainMap_block_cbox;
    QCheckBox *is_ZNCC_cbox;
    QPlainTextEdit *log_edit;
    QComboBox *log_level_box;
    QComboBox *bayer_combobox;

    QString directory_str;
    QString dateTime_str;
    QVector<QString> raw_files;

};

#endif // CALIBRATION_PDAF_WINDOW_HPP
