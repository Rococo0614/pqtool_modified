#include "cmodel_image_fetch_window.hpp"
#include "cmodel_video_helper.hpp"

#include <QLabel>
#include <QCloseEvent>
#include <QAction>
#include <QMenuBar>
#include <QFileDialog>
#include <QScrollBar>

const int MIN_WINDOW_WIDTH = 640;
const int MIN_WINDOW_HEIGHT = 480;
const int LOG_WIDGET_MAX_HEIGHT = 50;

CModelImageFetchWindow::CModelImageFetchWindow(QWidget *parent) :
    QMainWindow(parent)
{
    setWindowTitle(tr("CModel"));
    setWindowFlags(Qt::WindowMaximizeButtonHint|Qt::WindowCloseButtonHint);

    initUI();
    initEvent();
    initMenu();
    retranslateUi();
}

void CModelImageFetchWindow::initUI()
{
    main_widget = new QWidget();
    main_widget->setMinimumSize(QSize(MIN_WINDOW_WIDTH, MIN_WINDOW_HEIGHT));
    main_layout = new QVBoxLayout();
    image_label = new QLabel(tr("Please open exe and start..."));
    image_label->setAlignment(Qt::AlignCenter);
    main_layout->addWidget(image_label);

    img_viewer = new ImageViewerWidget();
    ImageLabel *image_label = new ImageLabel(this, ImageLabel::NORMAL);
    img_viewer->setImageLabel(image_label);
    img_viewer->setFilePathHidden();
    main_layout->addWidget(img_viewer);

    log_text = new QPlainTextEdit;
    log_text->setReadOnly(true);
    log_text->setMaximumHeight(LOG_WIDGET_MAX_HEIGHT);
    main_layout->addWidget(log_text);

    setCentralWidget(main_widget);
    centralWidget()->setLayout(main_layout);
    setExePath();
}

void CModelImageFetchWindow::initEvent()
{
    shared_ptr<CModelVideoHelper> helper = CModelVideoHelper::getInstance();
    connect(helper.get(), &CModelVideoHelper::sigDataUpdated, this, &CModelImageFetchWindow::imageRawDataUpdate);
    connect(helper->getProcess(), &QProcess::readyReadStandardOutput, this, &CModelImageFetchWindow::handleProcessLog);
}

void CModelImageFetchWindow::initMenu()
{
    choose_file_action = new QAction(this);
    connect(choose_file_action, &QAction::triggered, this, &CModelImageFetchWindow::handleChooseImageFile);
    menuBar()->addAction(choose_file_action);

    start_action = new QAction(this);
    connect(start_action, &QAction::triggered, this, &CModelImageFetchWindow::start);
    menuBar()->addAction(start_action);

    stop_action = new QAction(this);
    connect(stop_action, &QAction::triggered, this, &CModelImageFetchWindow::stop);
    menuBar()->addAction(stop_action);
}

void CModelImageFetchWindow::imageRawDataUpdate(QPixmap *image)
{
    if (!image->isNull()) {
        img_viewer->setImg(*image);
    }
}

void CModelImageFetchWindow::closeEvent(QCloseEvent *event)
{
    shared_ptr<CModelVideoHelper> helper = CModelVideoHelper::getInstance();
    helper->stop();
    disconnect(helper.get(), &CModelVideoHelper::sigDataUpdated, this, &CModelImageFetchWindow::imageRawDataUpdate);
    disconnect(helper->getProcess(), &QProcess::readyReadStandardOutput, this, &CModelImageFetchWindow::handleProcessLog);
    event->accept();
}

void CModelImageFetchWindow::changeEvent(QEvent *event)
{
    switch(event->type()) {
    case QEvent::LanguageChange:
        retranslateUi();
        break;
    default:
        break;
    }
    QWidget::changeEvent(event);
}

void CModelImageFetchWindow::retranslateUi()
{
    choose_file_action->setText(tr("&Choose File"));
    start_action->setText(tr("&Start"));
    stop_action->setText(tr("&Stop"));
}

void CModelImageFetchWindow::setExePath()
{
    QString file_path = "./cmodel/release/AlgoSim.exe";
    CModelVideoHelper::getInstance()->setExecFilePath(file_path);

    QString info_str = "Open " + file_path;
    log_text->appendPlainText(info_str);
    image_label->setText(tr("Ready to start..."));
}

void CModelImageFetchWindow::start()
{
    QString exec_path = CModelVideoHelper::getInstance()->getExecFilePath();
    if (exec_path.isEmpty()) {
        image_label->setText(tr("Exec file path is empty, please open exe first..."));
        return;
    }

    CModelVideoHelper::getInstance()->start();
    QString info_str = "Start...";
    image_label->setText(tr(info_str.toStdString().c_str()));
}

void CModelImageFetchWindow::stop()
{
    CModelVideoHelper::getInstance()->stop();
    QString ifon_str = "Stopped, ready to start ...";
    image_label->setText(tr(ifon_str.toStdString().c_str()));
}

void CModelImageFetchWindow::handleChooseImageFile()
{
    QString raw_img_path = QFileDialog::getOpenFileName(this, tr("Open Raw File"), "", "*.raw");
    CModelVideoHelper::getInstance()->setRawFilePath(raw_img_path);
}

void CModelImageFetchWindow::handleProcessLog()
{
    QProcess *process = CModelVideoHelper::getInstance()->getProcess();
    if (process == nullptr) return;

    QString output = process->readAllStandardOutput();
    log_text->appendPlainText(output);
    log_text->verticalScrollBar()->setValue(log_text->verticalScrollBar()->maximum());
}

void CModelImageFetchWindow::keyReleaseEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape) {
        this->close();
    }
}
