#include "calibration_window.hpp"
#include "raw_image_format_dialog.hpp"
#include "blc_calibration_page_widget.hpp"
#include "dpc_calibration_page_widget.hpp"
#include "ccm_calibration_page_widget.hpp"
#include "noise_profile_calibration_page_widget.hpp"
#include "awb_calibration_page_widget.hpp"
#include "lsc_calibration_page_widget.hpp"
#include "lblc_calibration_page_widget.hpp"
#include "bnr_calibration_page_widget.hpp"
#include "rgbir_calibration_page_widget.hpp"
#include "rgbirtorggb_calibration_page_widget.hpp"
#include "clut_calibration_page_widget.hpp"
#include "quick_calibration_page_widget.hpp"
#include "raw_scene_dialog.hpp"
#include "utils.hpp"
#include "main_window.hpp"

#include <QMenuBar>
#include <QFileDialog>
#include <QMessageBox>

CalibrationWindow::CalibrationWindow(QWidget *parent) :
    QMainWindow(parent)
{
    setWindowTitle(tr("Calibration"));

    initUI();
    initEvent();
    initMenu();
    retranslateUi();
}

CalibrationWindow::~CalibrationWindow() {}

void CalibrationWindow::initUI()
{
    main_widget = new QWidget();
    main_layout = new QGridLayout();

    image_list_widget = new CalibrationImageListWidget;
    main_layout->addWidget(image_list_widget, 0, 0);

    // raw config area
    QVBoxLayout *centent_layout = new QVBoxLayout;
    QHBoxLayout *raw_config_layout = new QHBoxLayout;
    QLabel *raw_config_label = new QLabel("Current raw config:");
    raw_config_line_edit = new QLineEdit;
    raw_config_line_edit->setDisabled(true);
    raw_config_layout->addWidget(raw_config_label);
    raw_config_layout->addWidget(raw_config_line_edit);
    centent_layout->addLayout(raw_config_layout);
    // module tab
    calibration_module_tab_widget = new ModuleTabWidget;
    calibration_module_tab_widget->setTabClosable(false);
    centent_layout->addWidget(calibration_module_tab_widget);
    main_layout->addLayout(centent_layout, 0, 1, 2, 1);

    // Add module tab
    // blc
    BLCCalibrationPageWidget *blc_cali_page = new BLCCalibrationPageWidget;
    calibration_module_tab_widget->addTab(blc_cali_page, blc_cali_page->getTitle());
    connect(blc_cali_page, &BaseCalibrationPageWidget::sigImportRaw, [=]() {
        this->handleImportRaw(blc_cali_page);
    });
    connect(blc_cali_page, &BaseCalibrationPageWidget::sigChangeCaliStatus, this, &CalibrationWindow::handleUpdateStatus);
    connect(blc_cali_page, &BasePageWidget::sigStatusLog, MainWindow::getInstance()->getLogWidget(), &StatusLogWidget::handleAppenLog);
    // dpc
    /*DPCCalibrationPageWidget *dpc_cali_page = new DPCCalibrationPageWidget;
    calibration_module_tab_widget->addTab(dpc_cali_page, dpc_cali_page->getTitle());
    connect(dpc_cali_page, &BaseCalibrationPageWidget::sigImportRaw, [=]() {
        this->handleImportRaw(dpc_cali_page);
    });
    connect(dpc_cali_page, &BaseCalibrationPageWidget::sigChangeCaliStatus, this, &CalibrationWindow::handleUpdateStatus);*/
    // lsc
    LSCCalibrationPageWidget *lsc_cali_page = new LSCCalibrationPageWidget;
    connect(lsc_cali_page, &BasePageWidget::sigStatusLog, MainWindow::getInstance()->getLogWidget(), &StatusLogWidget::handleAppenLog);
    calibration_module_tab_widget->addTab(lsc_cali_page, lsc_cali_page->getTitle());
    connect(lsc_cali_page, &BaseCalibrationPageWidget::sigImportRaw, [=]() {
        this->handleImportRaw(lsc_cali_page);
    });
    connect(lsc_cali_page, &BaseCalibrationPageWidget::sigChangeCaliStatus, this, &CalibrationWindow::handleUpdateStatus);
    // bnr
    /*BNRCalibrationPageWidget *bnr_cali_page = new BNRCalibrationPageWidget;
    calibration_module_tab_widget->addTab(bnr_cali_page, bnr_cali_page->getTitle());
    connect(bnr_cali_page, &BaseCalibrationPageWidget::sigImportRaw, [=]() {
        this->handleImportRaw(bnr_cali_page);
    });
    connect(bnr_cali_page, &BaseCalibrationPageWidget::sigChangeCaliStatus, this, &CalibrationWindow::handleUpdateStatus);*/
    //awb
    AWBCalibrationPageWidget *awb_cali_page = new AWBCalibrationPageWidget;
    calibration_module_tab_widget->addTab(awb_cali_page, awb_cali_page->getTitle());
    connect(awb_cali_page, &BaseCalibrationPageWidget::sigImportRaw, [=]() {
        this->handleImportRaw(awb_cali_page);
    });
    connect(awb_cali_page, &BaseCalibrationPageWidget::sigChangeCaliStatus, this, &CalibrationWindow::handleUpdateStatus);
    // ccm
    CCMCalibrationPageWidget *ccm_cali_page = new CCMCalibrationPageWidget;
    calibration_module_tab_widget->addTab(ccm_cali_page, ccm_cali_page->getTitle());
    connect(ccm_cali_page, &BaseCalibrationPageWidget::sigImportRaw, [=]() {
        this->handleImportRaw(ccm_cali_page);
    });
    connect(ccm_cali_page, &BaseCalibrationPageWidget::sigChangeCaliStatus, this, &CalibrationWindow::handleUpdateStatus);
    // noise profile
    NoiseProfileCalibrationPageWidget *noise_profile_cali_page = new NoiseProfileCalibrationPageWidget;
    calibration_module_tab_widget->addTab(noise_profile_cali_page, noise_profile_cali_page->getTitle());
    connect(noise_profile_cali_page, &BaseCalibrationPageWidget::sigImportRaw, [=]() {
        this->handleImportRaw(noise_profile_cali_page);
    });
    connect(noise_profile_cali_page, &BaseCalibrationPageWidget::sigChangeCaliStatus, this, &CalibrationWindow::handleUpdateStatus);
    // clut
    CLUTCalibrationPageWidget *clut_cali_page = new CLUTCalibrationPageWidget;
    calibration_module_tab_widget->addTab(clut_cali_page, clut_cali_page->getTitle());
    connect(clut_cali_page, &BaseCalibrationPageWidget::sigImportRaw, [=]() {
        this->handleImportRaw(clut_cali_page);
    });
    connect(clut_cali_page, &BaseCalibrationPageWidget::sigChangeCaliStatus, this, &CalibrationWindow::handleUpdateStatus);
    //quick calibration
    QuickCalibrationPageWidget *quick_cali_page = new QuickCalibrationPageWidget;
    calibration_module_tab_widget->insertTab(0, quick_cali_page, quick_cali_page->getTitle());

    connect(quick_cali_page, &QuickCalibrationPageWidget::sigStartBlcCalibration, blc_cali_page, &BLCCalibrationPageWidget::HandleCalibrationRequst);
    connect(blc_cali_page, &BaseCalibrationPageWidget::sigCaliStatus, quick_cali_page, &QuickCalibrationPageWidget::handleCalibrationStatus);
    connect(quick_cali_page, &QuickCalibrationPageWidget::sigStartLscCalibration, lsc_cali_page, &LSCCalibrationPageWidget::HandleCalibrationRequst);
    connect(lsc_cali_page, &BaseCalibrationPageWidget::sigCaliStatus, quick_cali_page, &QuickCalibrationPageWidget::handleCalibrationStatus);
    connect(quick_cali_page, &QuickCalibrationPageWidget::sigStartAwbCalibration, awb_cali_page, &AWBCalibrationPageWidget::HandleCalibrationRequst);
    connect(awb_cali_page, &BaseCalibrationPageWidget::sigCaliStatus, quick_cali_page, &QuickCalibrationPageWidget::handleCalibrationStatus);
    connect(quick_cali_page, &QuickCalibrationPageWidget::sigStartCcmCalibration, ccm_cali_page, &CCMCalibrationPageWidget::HandleCalibrationRequst);
    connect(ccm_cali_page, &BaseCalibrationPageWidget::sigCaliStatus, quick_cali_page, &QuickCalibrationPageWidget::handleCalibrationStatus);
    connect(quick_cali_page, &QuickCalibrationPageWidget::sigStartNoiseCalibration, noise_profile_cali_page, &NoiseProfileCalibrationPageWidget::HandleCalibrationRequst);
    connect(noise_profile_cali_page, &BaseCalibrationPageWidget::sigCaliStatus, quick_cali_page, &QuickCalibrationPageWidget::handleCalibrationStatus);
    // lsc -> ccm
    connect(lsc_cali_page, &LSCCalibrationPageWidget::sigCv184xMlscChromaLumaFlag, ccm_cali_page, &CCMCalibrationPageWidget::handleCv184xMlscMode);

    QString tool_soc = GlobalData::getInstance()->getCvipqToolSoc();
    if (tool_soc == CV186X_SOC) {
        // lblc
        LBLCCalibrationPageWidget *lblc_cali_page = new LBLCCalibrationPageWidget;
        calibration_module_tab_widget->addTab(lblc_cali_page, lblc_cali_page->getTitle());
        connect(lblc_cali_page, &BaseCalibrationPageWidget::sigImportRaw, [=]() {
            this->handleImportRaw(lblc_cali_page);
        });
        connect(lblc_cali_page, &BaseCalibrationPageWidget::sigChangeCaliStatus, this, &CalibrationWindow::handleUpdateStatus);
        connect(quick_cali_page, &QuickCalibrationPageWidget::sigStartLblcCalibration, lblc_cali_page, &LBLCCalibrationPageWidget::HandleCalibrationRequst);
        connect(lblc_cali_page, &BaseCalibrationPageWidget::sigCaliStatus, quick_cali_page, &QuickCalibrationPageWidget::handleCalibrationStatus);

        // rgbir
        RGBIRCalibrationPageWidget *rgbir_cali_page = new RGBIRCalibrationPageWidget;
        calibration_module_tab_widget->addTab(rgbir_cali_page, rgbir_cali_page->getTitle());
        connect(rgbir_cali_page, &BaseCalibrationPageWidget::sigImportRaw, [=]() {
            this->handleImportRaw(rgbir_cali_page);
        });
        connect(rgbir_cali_page, &BaseCalibrationPageWidget::sigChangeCaliStatus, this, &CalibrationWindow::handleUpdateStatus);
        //rgbirtorggb
        RGBIRToRGGBCalibrationPageWidget *rgbir2rggb_cali_page = new RGBIRToRGGBCalibrationPageWidget;
        calibration_module_tab_widget->addTab(rgbir2rggb_cali_page, rgbir2rggb_cali_page->getTitle());
        connect(rgbir2rggb_cali_page, &BaseCalibrationPageWidget::sigImportRaw, [=]() {
            this->handleImportRaw(rgbir2rggb_cali_page);
        });
        connect(rgbir2rggb_cali_page, &BaseCalibrationPageWidget::sigChangeCaliStatus, this, &CalibrationWindow::handleUpdateStatus);
    }


    calibration_module_tab_widget->setCurrentWidget(quick_cali_page);
    status_label = new QLabel();
    handleUpdateStatus(CALIBRATION_STATUS::READY);
    status_label->setAlignment(Qt::AlignCenter);
    main_layout->addWidget(status_label, 1, 0);

    main_layout->setColumnStretch(0, 0);
    main_layout->setColumnStretch(1, 8);
    main_layout->setRowStretch(0, 9);
    setCentralWidget(main_widget);
    centralWidget()->setLayout(main_layout);

    // init default raw config
   raw_config_item.setConfig("1920", "1080", "12", utils::BAYER_ID_BG, utils::RAW_PACK_TYPE::RAW_UNPACKED, utils::IR_NO);
    handleUpdateRawConfig();
}

void CalibrationWindow::initEvent()
{
    connect(image_list_widget, &CalibrationImageListWidget::sigRawImageItemChecked, this, &CalibrationWindow::handleUpdateInfo);
}

void CalibrationWindow::initMenu()
{
    open_file_action = new QAction(this);
    connect(open_file_action, &QAction::triggered, this, &CalibrationWindow::handleOpenRawFile);
    menuBar()->addAction(open_file_action);

    load_param_action = new QAction(this);
    connect(load_param_action, &QAction::triggered, this, &CalibrationWindow::handleLoadParam);
    //menuBar()->addAction(load_param_action);

    save_param_action = new QAction(this);
    connect(save_param_action, &QAction::triggered, this, &CalibrationWindow::handleSaveParam);
    //menuBar()->addAction(save_param_action);
}

void CalibrationWindow::retranslateUi()
{
    status_label->setText(tr(status_label->text().toStdString().c_str()));
    open_file_action->setText(tr("&Open raw files"));
    load_param_action->setText(tr("&Load param"));
    save_param_action->setText(tr("&Save param"));
}

void CalibrationWindow::handleOpenRawFile()
{
    QFileDialog file_dialog(this);
    file_dialog.setNameFilter(tr("Images (*.raw *.yuv)"));
    file_dialog.setViewMode(QFileDialog::Detail);
    file_dialog.setFileMode(QFileDialog::ExistingFiles);
    if (QDialog::Accepted == file_dialog.exec() ) {
        QStringList file_list = file_dialog.selectedFiles();
        for (auto file_path : file_list) {
             RawImageFormatDialog *formt_dialog = new RawImageFormatDialog;
             formt_dialog->setData(raw_config_item.getWidth(), raw_config_item.getHeight(),
                raw_config_item.getBits(), raw_config_item.getBayer(), raw_config_item.getPacked(),
                raw_config_item.getIrPosition(), raw_config_item.getScene(), raw_config_item.getFrameMode());
             formt_dialog->setFilepath(file_path);
             if (QDialog::Accepted == formt_dialog->exec()) {
                 std::tuple<QString, QString> size = formt_dialog->getSize();
                 raw_config_item.setConfig(std::get<0>(size), std::get<1>(size), formt_dialog->getBits(),
                    formt_dialog->getBayer(), formt_dialog->getPacked(), formt_dialog->getIrPosition(),
                    formt_dialog->getScene(), formt_dialog->getFrameMode());
                 RawImageItem *raw_img_item = new RawImageItem(file_path, formt_dialog->getScene());
                 raw_img_item->setImgSize(raw_config_item.getWidth().toInt(), raw_config_item.getHeight().toInt());
                 raw_img_item->setBits(raw_config_item.getBits());
                 raw_img_item->setBayer(raw_config_item.getBayer());
                 raw_img_item->setPacked(raw_config_item.getPacked());
                 raw_img_item->setIrPosition(raw_config_item.getIrPosition());
                 raw_img_item->setFrameMode(raw_config_item.getFrameMode());
                 RECT_S roi_size;
                 int width =  raw_config_item.getWidth().toInt();
                 int height =  raw_config_item.getHeight().toInt();
                 bool wdr_mode = raw_config_item.getFrameMode() == FRAME_MODE::WDR ? true : false;
                 if (wdr_mode) width /= 2;
                 roi_size.w = (int)((width * (float)0.125) / 2) * 2;
                 roi_size.h = (int)((height * (float)0.1667) / 2) * 2;
                 roi_size.x = (width - roi_size.w) / 2;
                 roi_size.y = (height - roi_size.h) / 2;
                 raw_img_item->setRect(roi_size);
                 image_list_widget->addTableItem(raw_img_item);
             }
        }
    }
}

void CalibrationWindow::handleLoadParam()
{
    QString param_file_path = QFileDialog::getOpenFileName(this, tr("Open File"), "", "*.json");
}

void CalibrationWindow::handleSaveParam()
{
    QString param_file_path = QFileDialog::getSaveFileName(this, tr("Save Param File"), "", "*.json");
}

void CalibrationWindow::handleUpdateRawConfig()
{
    QString config_str = "Size: " + raw_config_item.getWidth() + " x " + raw_config_item.getHeight() +
            ", Pixel depth: " + raw_config_item.getBits().toString() + " bits, Components: " + utils::getBayerName(raw_config_item.getBayer().toInt()) +
            ", Packeted:" + utils::getPackTypeName(raw_config_item.getPacked()) + ", isWDR:" + QString::number(static_cast<int>(raw_config_item.getFrameMode()));
    raw_config_line_edit->setText(config_str);
}

void CalibrationWindow::handleImportRaw(BaseCalibrationPageWidget *cali_page)
{
    auto raw_img_item_list = image_list_widget->getRawImageList();
    RawImageItem *raw_img_item = nullptr;
    bool check = false;
    for (auto raw_item : raw_img_item_list) {
        // check scene and is checked
        if (raw_item->getCheckState() == Qt::CheckState::Checked) {
            if (raw_img_item != nullptr) {
                check = false;
                break;
            }

            check = true;
            raw_img_item = raw_item;
        }
    }

    if ((raw_img_item != nullptr) && (true == check)) {
        if (raw_img_item->getScene() != cali_page->getAcceptScene()) {
            if (cali_page->getTitle()!= "RGBIR To RGGB"){
                QMessageBox message(QMessageBox::NoIcon, "Error",
                                QString("The Raw Data %1 is not marked as %2 scene")
                                    .arg(raw_img_item->getFileName())
                                    .arg(getSceneName(cali_page->getAcceptScene()))
                                    );
                message.exec();
                return;
            }
        }

        CALIBRATION_MODULE_ID id = static_cast<CALIBRATION_MODULE_ID>(cali_page->property("cali_id").toInt());
        bool is_yuv_image = false;
        unsigned int cnt = 0;
        QString file_name = raw_img_item->getFileName();

        if (file_name.indexOf(".yuv", 0, Qt::CaseInsensitive) != -1) {
            cali_page->SetCalibrationBtnState(false);
            is_yuv_image = true;
        } else {
            cali_page->SetCalibrationBtnState(true);
        }

        if (is_yuv_image) {
            if (id != CALIBRATION_MODULE_ID::CCM) {
                QMessageBox message(QMessageBox::NoIcon, "Error", QString("The current selected image is yuv format!")
                                    .arg(raw_img_item->getWidth()).arg(raw_img_item->getHeight()));
                message.exec();
                return;
            } else {
                cnt = 1;
            }
        } else {
            QFile file(raw_img_item->getFilePath());
            file.open(QIODevice::ReadOnly);
            cnt = static_cast<unsigned int>(file.readAll().size()) / static_cast<unsigned int>((raw_img_item->getWidth() * raw_img_item->getHeight() * sizeof(short)));
            file.close();
            if (cnt == 0) {
                QMessageBox message(QMessageBox::NoIcon, "Error", QString("The raw size is smaller than %1x%2. Please check the raw file and config.")
                                    .arg(raw_img_item->getWidth()).arg(raw_img_item->getHeight()));
                message.exec();
                return;
            }
        }

        raw_img_item ->setFramesCnt(cnt);

        cali_page->clearAllRawImgItem();
        cali_page->addRawImgItem(raw_img_item);
    } else {
        QMessageBox message(QMessageBox::NoIcon, "Error", "Please select exactly one raw");
        message.exec();
    }
}

void CalibrationWindow::handleImportMultiRaw(BaseCalibrationPageWidget *cali_page)
{
    auto raw_img_item_list = image_list_widget->getRawImageList();

    cali_page->clearAllRawImgItem();
    for (auto raw_item : raw_img_item_list) {
        // check scene and is checked
        if (raw_item->getCheckState() == Qt::CheckState::Checked) {
            if (raw_item == nullptr) {
                continue;
            }
                        if (raw_item->getScene() != cali_page->getAcceptScene()) {
                if (cali_page->getTitle() != "RGBIR" && cali_page->getTitle()!= "RGBIR To RGGB"){
                    QMessageBox message(QMessageBox::NoIcon, "Error",
                                    QString("The Raw Data %1 is not marked as %2 scene")
                                        .arg(raw_item->getFileName())
                                        .arg(getSceneName(cali_page->getAcceptScene()))
                                        );
                    message.exec();
                    continue;
                 }else{
                    continue;
                 }

            }
            cali_page->addRawImgItem(raw_item);
        }
    }
}

void CalibrationWindow::handleUpdateStatus(CALIBRATION_STATUS status)
{
    QString status_str = "Progress: ";
    switch(status) {
        case CALIBRATION_STATUS::READY:
            status_label->setText(status_str + "Ready");
            break;
        case CALIBRATION_STATUS::CALIBRATION:
            status_label->setText(status_str + "Calibrating");
            break;
        case CALIBRATION_STATUS::CAL_ERROR:
            status_label->setText(status_str + "Error");
            break;
        default :
            status_label->setText(status_str + "Ready");
            break;
    }
    status_label->repaint();
}

void CalibrationWindow::handleUpdateInfo(int index)
{
    QList<RawImageItem *> raw_image_list = image_list_widget->getRawImageList();
    raw_config_item.setConfig(QString::number(raw_image_list[index]->getWidth()), QString::number(raw_image_list[index]->getHeight()),
        raw_image_list[index]->getBits(), raw_image_list[index]->getBayer(), raw_image_list[index]->getPacked(),
        raw_image_list[index]->getIrPosition(), raw_image_list[index]->getScene(), raw_image_list[index]->getFrameMode());
    handleUpdateRawConfig();
}

void CalibrationWindow::keyReleaseEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape) {
        this->close();
    }
}

void CalibrationWindow::closeEvent(QCloseEvent *event)
{
    if (QMessageBox::question(this, tr("Quit"), tr("Are you sure to quit calibration tool?"),
                              QMessageBox::Yes, QMessageBox::No) == QMessageBox::Yes)
    {
        event->accept();
        emit sigCloseWindow();
    }
    else {
        event->ignore();
    }
}
