#include "import_calibration_json_window.hpp"
#include "global_data.hpp"
#include "utils.hpp"

#define IMPORT_WINDOW_WIDTH 400
#define IMPORT_WINDOW_HEIGHT 300

ImportCalibrationJsonWindow::ImportCalibrationJsonWindow(QWidget *parent) :
    QMainWindow(parent)
{
    setWindowFlags(Qt::WindowCloseButtonHint);
    this->setFixedSize(IMPORT_WINDOW_WIDTH, IMPORT_WINDOW_HEIGHT);

    initUI();
    retranslateUi();
}

ImportCalibrationJsonWindow::~ImportCalibrationJsonWindow() {}

void ImportCalibrationJsonWindow::initUI()
{
    main_widget = new QWidget;
    main_layout = new QGridLayout;

    file_path_label = new QLabel();
    file_path_line_edit = new QLineEdit();
    browse_btn = new QPushButton();
    connect(browse_btn, &QPushButton::clicked, this, &ImportCalibrationJsonWindow::handleBrowseFile);

    blc_checkbox = new QCheckBox("BLC");
    lsc_checkbox = new QCheckBox("LSC");
    awb_checkbox = new QCheckBox("AWB");
    ccm_checkbox = new QCheckBox("CCM");
    np_checkbox = new QCheckBox("NP");
    rlsc_checkbox = new QCheckBox("RLSC");
    rgbir_checkbox = new QCheckBox("RGBIR");

    blc_checkbox->setChecked(true);
    lsc_checkbox->setChecked(true);
    awb_checkbox->setChecked(true);
    ccm_checkbox->setChecked(true);
    np_checkbox->setChecked(true);

    import_btn = new QPushButton();
    connect(import_btn, &QPushButton::clicked, this, &ImportCalibrationJsonWindow::handleImport);

    main_layout->addWidget(file_path_label, 0, 0);
    main_layout->addWidget(file_path_line_edit, 0, 1);
    main_layout->addWidget(browse_btn, 0, 2);

    main_layout->addWidget(blc_checkbox, 1, 0);
    main_layout->addWidget(lsc_checkbox, 1, 1);
    main_layout->addWidget(awb_checkbox, 1, 2);
    main_layout->addWidget(ccm_checkbox, 2, 0);
    main_layout->addWidget(np_checkbox, 2, 1);
    main_layout->addWidget(rlsc_checkbox, 2, 2);
    main_layout->addWidget(rgbir_checkbox, 3, 0);

    main_layout->addWidget(import_btn, 4, 0, 1, 3);

    setCentralWidget(main_widget);
    centralWidget()->setLayout(main_layout);
}

void ImportCalibrationJsonWindow::handleBrowseFile()
{
    QString filePath = QFileDialog::getOpenFileName(this,
        tr("Select Calibration File"), "",
        tr("JSON Files (*.json);;All Files (*)"));

    if (!filePath.isEmpty()) {
        file_path_line_edit->setText(filePath);
    }
}

void ImportCalibrationJsonWindow::handleImport()
{
    QString filePath = file_path_line_edit->text();
    if (filePath.isEmpty()) {
        QMessageBox::warning(this, tr("Import Calibration"),
            tr("Please select a calibration file!"), QMessageBox::Ok);
        return;
    }
    if (blc_checkbox->isChecked()) selectedModules << "BLC";
    if (lsc_checkbox->isChecked()) selectedModules << "LSC";
    if (awb_checkbox->isChecked()) selectedModules << "WB Attr";
    if (ccm_checkbox->isChecked()) selectedModules << "CCM";
    if (np_checkbox->isChecked()) selectedModules << "Noise Profile";
    if (rlsc_checkbox->isChecked()) selectedModules << "RLSC";
    if (rgbir_checkbox->isChecked()) selectedModules << "RGBIR";

    if (selectedModules.isEmpty()) {
        QMessageBox::warning(this, tr("Import Calibration"),
            tr("Please select at least one module!"), QMessageBox::Ok);
        return;
    }else{
        qDebug("open file:%s", qUtf8Printable(filePath));
    }
    emit sigImportCalibration(filePath, selectedModules);
    this->hide();
}

void ImportCalibrationJsonWindow::changeEvent(QEvent *event)
{
    if (event->type() == QEvent::LanguageChange) {
        retranslateUi();
    }
    QMainWindow::changeEvent(event);
}

void ImportCalibrationJsonWindow::retranslateUi()
{
    setWindowTitle(tr("Import Calibration"));
    file_path_label->setText(tr("File Path:"));
    browse_btn->setText(tr("Open file"));
    import_btn->setText(tr("Import Json"));
}


