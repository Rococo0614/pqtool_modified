#include "cmodel_ctrl_window.hpp"
#include "network_client_helper.hpp"
#include "global_data.hpp"

#include <QtCore>
#include <QLabel>
#include <QAction>
#include <QMenuBar>
#include <QFileDialog>
#include <QScrollBar>
#include <iostream>
#include <cstdlib>

#define START "CVI_CMODEL_Start"
#define STOP "CVI_CMODEL_Stop"
#define CONFIG "CVI_CMODEL_Config"
#define SET_COLOR_RED setStyleSheet("background-color: #eeeeee; color: red;")
#define SET_COLOR_GRAY setStyleSheet("background-color: #eeeeee;")

static const int MIN_WINDOW_WIDTH = 640;
static const int MIN_WINDOW_HEIGHT = 480;
static const int LOG_WIDGET_MAX_HEIGHT = 50;
static const int CMODEL_READ_WAIT_TIME_OUT = 10;

CModelCtrlWindow::CModelCtrlWindow(QWidget *parent) :
    QMainWindow(parent)
{
    setWindowTitle("CModel Ctrl");
    setWindowFlags(Qt::WindowMinimizeButtonHint|Qt::WindowMaximizeButtonHint|Qt::WindowCloseButtonHint);

    initUi();
    initMenu();
    retranslateUi();
    // init the event
    connect(NetworkClientHelper::getInstance().get(), &NetworkClientHelper::sigCtrlCmodel, this, &CModelCtrlWindow::handleRecvData);
}

void CModelCtrlWindow::initUi()
{
    main_widget = new QWidget();
    //main_widget->setMinimumSize(QSize(MIN_WINDOW_HEIGHT, MIN_WINDOW_HEIGHT));
    main_widget->setFixedSize(QSize(MIN_WINDOW_WIDTH, MIN_WINDOW_HEIGHT));
    main_layout = new QGridLayout();

    isp_pipeline_lb = new QLabel(this);
    src_lb = new QLabel(this);
    dst_lb = new QLabel(this);
    args_lb = new QLabel(this);
    progress_lb = new QLabel(this);
    log_lb = new QLabel(this);

    log_text = new QPlainTextEdit();
    log_text->setReadOnly(true);
    //log_text->setMaximumHeight(LOG_WIDGET_MAX_HEIGHT);

    const char *user_name = std::getenv("USERNAME");
    QString s_user_name(user_name);

    isp_pipeline_edit = new QLineEdit;
    isp_pipeline_edit->setText("/data/" + s_user_name + "/isp/mars3/isp-tool-daemon-cmodel/cmodel/run_with_python/mars3_pipeline");
    isp_pipeline_dir = isp_pipeline_edit->text();
    connect(isp_pipeline_edit, &QLineEdit::textChanged,
            [=] (QString text) {handle_isp_pipeline_edit_changed(text);});

    src_dir_edit = new QLineEdit;
    src_dir_edit->setText("/data/" + s_user_name + "/isp/mars3/isp-tool-daemon-cmodel/cmodel/run_with_python/mars3_pipeline/dataset");
    src_dir = src_dir_edit->text();
    connect(src_dir_edit, &QLineEdit::textChanged,
            [=] (QString text) {handle_src_dir_edit_changed(text);});

    dst_dir_edit = new QLineEdit;
    dst_dir_edit->setText("/data/" + s_user_name + "/tmp/results_Mars/");
    dst_dir = dst_dir_edit->text();
    connect(dst_dir_edit, &QLineEdit::textChanged,
            [=] (QString text) {handle_dst_dir_edit_changed(text);});

    args_edit = new QLineEdit;
    args_edit->setText("--sp 1 --pipe_version isp_v4002 --keyword isp_v4002 --end 2");
    args = args_edit->text();
    connect(args_edit, &QLineEdit::textChanged,
            [=] (QString text) {handle_args_edit_changed(text);});

    progress_bar = new QProgressBar();
    progress_bar->setRange(0, 100);
    progress_bar->setValue(0);

    main_layout->addWidget(isp_pipeline_lb, 1, 0);
    main_layout->addWidget(isp_pipeline_edit, 1, 1);
    main_layout->addWidget(src_lb, 2, 0);
    main_layout->addWidget(src_dir_edit, 2, 1);
    main_layout->addWidget(dst_lb, 3, 0);
    main_layout->addWidget(dst_dir_edit, 3, 1);
    main_layout->addWidget(args_lb, 4, 0);
    main_layout->addWidget(args_edit, 4, 1);
    main_layout->addWidget(progress_lb, 5, 0);
    main_layout->addWidget(progress_bar, 5, 1);
    main_layout->addWidget(log_lb, 6, 0);
    main_layout->addWidget(log_text, 6, 1);

    setCentralWidget(main_widget);
    centralWidget()->setLayout(main_layout);
}

void CModelCtrlWindow::initMenu()
{
    config_act = new QAction(this);
    connect(config_act, &QAction::triggered, this, &CModelCtrlWindow::handleConfig);
    menuBar()->addAction(config_act);

    start_act = new QAction(this);
    connect(start_act, &QAction::triggered, this, &CModelCtrlWindow::start);
    menuBar()->addAction(start_act);

    stop_act = new QAction(this);
    connect(stop_act, &QAction::triggered, this, &CModelCtrlWindow::stop);
    menuBar()->addAction(stop_act);

    view_raw_act = new QAction(this);
    menuBar()->addAction(view_raw_act);

    view_res_act = new QAction(this);
    menuBar()->addAction(view_res_act);

    //disable_menu_action();
    hide_menu_action();
}

void CModelCtrlWindow::retranslateUi()
{
    // lable
    isp_pipeline_lb->setText("Pipeline");
    src_lb->setText("Src");
    dst_lb->setText("Dst");
    args_lb->setText("Args");
    progress_lb->setText("progress");
    log_lb->setText("Log");
    // action
    view_raw_act->setText(tr("ViewRaw"));
    view_res_act->setText(tr("ViewRes"));
    config_act->setText("Config");
    start_act->setText(tr("Start"));
    stop_act->setText(tr("Stop"));
}

// the slots
void CModelCtrlWindow::start()
{
    QString log {"Starting ... "};
    handleProcessLog(log);
    method = START;
    composeParameters();
    readCmodelFromNetworkJsonrpc();

    // after start, reset the line edit
    line_edit_reset();
}

void CModelCtrlWindow::stop()
{
    QString log {"Stop ... "};

    handleProcessLog(log);
    method = STOP;
    jsobj_params = QJsonObject();
    readCmodelFromNetworkJsonrpc();
    progress_bar->setValue(0);
}

void CModelCtrlWindow::handleConfig()
{
    QString log {"Config the src, get the raw and the ini file!\n"};
    handleProcessLog(log);
    method = CONFIG;
    composeParameters();
    readCmodelFromNetworkJsonrpc();
}

void CModelCtrlWindow::handleRecvData(int id)
{
    QJsonObject data_obj = NetworkClientHelper::getInstance()->popCmdDataJsonrpc(id);
    if (!data_obj.isEmpty()) {
        QJsonObject result_obj = data_obj[MSG_KEY_RESULT].toObject();
        QJsonObject info_obj = data_obj[MSG_KEY_ID_INFO].toObject();
        QString cmd = info_obj[MSG_KEY_CMD].toString();
        QString module = info_obj[MSG_KEY_MODULE].toString();
        QString struct_id = info_obj[MSG_KEY_STRUCT].toString();
        QJsonObject params_obj = result_obj[MSG_KEY_PARAMS].toObject();
        QString method = params_obj["method"].toString();

        if (method == START) {
            QString log = params_obj["log"].toString();

            log.remove("\u001b[94m");
            log.remove("\u001b[92m");
            log.remove("\u001b[91m");
            log.remove("\u001b[0m");

            int progress_val = params_obj["progress"].toInt();

            progress_bar->setValue(progress_val);
            handleProcessLog(log);
        } else if (method == CONFIG) {
            QString raw = params_obj["raw_path"].toString();
            QString ini = params_obj["ini_path"].toString();

            if (!raw.isEmpty() && !ini.isEmpty()) {
                QString log = QString("raw: ") + raw + QString("\nini: ") + ini;
                QFileInfo raw_file_info(raw);
                QFileInfo ini_file_info(ini);

                handleProcessLog(log);
                //enable_menu_action();
                show_menu_action();
                setWindowTitle(raw_file_info.fileName());
                line_edit_reset();
            } else {
                QString erro_log {"Can not get the right ini and raw file! Please set the src right and then click config again!\n"};

                handleProcessLog(erro_log);
                setWindowTitle("Cmodel Ctrl");
                // disable_menu_action();
                hide_menu_action();
            }

        } else if (method == STOP) {
            progress_bar->setValue(0);
        }
    } else {
        qDebug("GlobalData::handleDataChangedJsonrpc. Error: data_obj is empty.");
    }
   // get the log
    std::cout << "You are handling the log!" << std::endl;
   // get the ip
    std::cout << "You are handling the ip!" << std::endl;
}

void CModelCtrlWindow::handleProcessLog(const QString& output)
{
    // show the process log for the cmodel. Need to read the log from it.
    log_text->appendPlainText(output);
    log_text->verticalScrollBar()->setValue(log_text->verticalScrollBar()->maximum());
}

void CModelCtrlWindow::composeParameters()
{
    // compose to the jsobj_params
    jsobj_params["isp_pipeline_dir"] = isp_pipeline_dir;
    jsobj_params["src_dir"] = src_dir;
    jsobj_params["dst_dir"] = dst_dir;
    jsobj_params["args"] = args;
}

void CModelCtrlWindow::readCmodelFromNetworkJsonrpc()
{
    NetworkClientHelper::getInstance()->clearIdInfoJsonrpc();
    QJsonObject send_obj;
    QJsonObject id_info;
    int jsonrpc_id = GlobalData::getInstance()->getJsonrpcId();

    send_obj[MSG_KEY_JSONRPC] = MSG_JSONRPC_VERSION;
    send_obj[MSG_KEY_ID] = jsonrpc_id;
    send_obj[MSG_KEY_METHOD] = method;
    send_obj[MSG_KEY_PARAMS] = jsobj_params;
    // id_info[MSG_KEY_CMD] = CMD_READ_MODULE;
    id_info[MSG_KEY_CMD] = CMD_CTRL_CMODEL;
    id_info[MSG_KEY_MODULE] = "cmodel";
    id_info[MSG_KEY_STRUCT] = "cmodel_ctrl_attr_s";

    NetworkClientHelper::getInstance()->pushIdInfoJsonrpc(jsonrpc_id, id_info);
    NetworkClientHelper::getInstance()->sendCmd(std::move(send_obj));

    bool gotResponse = false;
    QTime stopTime;
    int waitTime = CMODEL_READ_WAIT_TIME_OUT;
    stopTime = QTime::currentTime().addSecs(waitTime);

    while (QTime::currentTime() < stopTime) {
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
        if (!GlobalData::getInstance()->getWaitForResponse()) {
            gotResponse = true;
            break;
        }
    }

    if (!gotResponse) {
        NetworkClientHelper::getInstance()->removeIdInfoJsonrpc(jsonrpc_id);
        GlobalData::getInstance()->setWaitForResponse(false);
        //QString message = getTitle() + QString(" Page: Failed to read %1. Timeout: Can't get reponse from board within %2 seconds.").arg(struct_id).arg(waitTime);
        //emit sigStatusLog(message, LOG_LEVEL::LOG_ERROR);
        //break;
    }
}

void CModelCtrlWindow::handle_isp_pipeline_edit_changed(const QString& text)
{
    isp_pipeline_dir = text;
    isp_pipeline_lb->SET_COLOR_RED;
}

void CModelCtrlWindow::handle_src_dir_edit_changed(const QString& text)
{
    src_dir = text;
    src_lb->SET_COLOR_RED;
    // disable some menu's action
    //disable_menu_action();
    hide_menu_action();
}

void CModelCtrlWindow::handle_dst_dir_edit_changed(const QString& text)
{
    dst_dir = text;
    dst_lb->SET_COLOR_RED;
}

void CModelCtrlWindow::handle_args_edit_changed(const QString& text)
{
    args = text;
    args_lb->SET_COLOR_RED;
}

void CModelCtrlWindow::line_edit_reset()
{
    isp_pipeline_lb->SET_COLOR_GRAY;
    src_lb->SET_COLOR_GRAY;
    dst_lb->SET_COLOR_GRAY;
    args_lb->SET_COLOR_GRAY;
}

void CModelCtrlWindow::enable_menu_action()
{
    start_act->setDisabled(false);
    stop_act->setDisabled(false);
    view_raw_act->setDisabled(false);
    view_res_act->setDisabled(false);
}

void CModelCtrlWindow::disable_menu_action()
{
    start_act->setDisabled(true);
    stop_act->setDisabled(true);
    view_raw_act->setDisabled(true);
    view_res_act->setDisabled(true);
}

void CModelCtrlWindow::hide_menu_action()
{
    start_act->setVisible(false);
    stop_act->setVisible(false);
    view_raw_act->setVisible(false);
    view_res_act->setVisible(false);
}

void CModelCtrlWindow::show_menu_action()
{
    start_act->setVisible(true);
    stop_act->setVisible(true);
    view_raw_act->setVisible(true);
    view_res_act->setVisible(true);
}
