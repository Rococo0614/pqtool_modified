#ifndef IMPORT_CALIBRATION_JSON_WINDOW_HPP
#define IMPORT_CALIBRATION_JSON_WINDOW_HPP

#include <QMainWindow>
#include <QGridLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QCheckBox>
#include <QWidget>
#include <QLabel>
#include <QEvent>
#include <QHBoxLayout>
#include <QTabWidget>
#include <QTranslator>
#include <QMessageBox>
#include <QApplication>
#include <QDesktopWidget>
#include <QLabel>
#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include <QFileDialog>
#include <QPlainTextEdit>
#include <QGuiApplication>
#include <functional>

class ImportCalibrationJsonWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ImportCalibrationJsonWindow(QWidget *parent = nullptr);
    ~ImportCalibrationJsonWindow();

protected:
    void changeEvent(QEvent *event) override;

private:
    void initUI();
    void retranslateUi();

private:
    QWidget *main_widget;
    QGridLayout *main_layout;

    QLabel *file_path_label;
    QLineEdit *file_path_line_edit;
    QPushButton *browse_btn;

    QCheckBox *blc_checkbox;
    QCheckBox *lsc_checkbox;
    QCheckBox *awb_checkbox;
    QCheckBox *ccm_checkbox;
    QCheckBox *np_checkbox;
    QCheckBox *rlsc_checkbox;
    QCheckBox *rgbir_checkbox;

    QPushButton *import_btn;
    QStringList selectedModules;



private slots:
    void handleBrowseFile();
    void handleImport();

signals:
    void sigImportCalibration(const QString &filePath, const QStringList &selectedModules);
};

#endif // IMPORT_CALIBRATION_JSON_WINDOW_HPP
