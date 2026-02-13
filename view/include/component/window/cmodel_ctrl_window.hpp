#ifndef CMODEL_CTRL_WINDOW_HPP
#define CMODEL_CTRL_WINDOW_HPP

#include <QMainWindow>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QScrollArea>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QProgressBar>
#include <QJsonObject>

class CModelCtrlWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit CModelCtrlWindow(QWidget *parent = nullptr);
    void initUi();
    void initMenu();

private:
    void retranslateUi();
    void handle_isp_pipeline_edit_changed(const QString& text);
    void handle_src_dir_edit_changed(const QString& text);
    void handle_dst_dir_edit_changed(const QString& text);
    void handle_args_edit_changed(const QString& text);
    void readCmodelFromNetworkJsonrpc();
    void writeCmodelToNetworkJsonrpc();
    void composeParameters();
    void line_edit_reset();
    void enable_menu_action();
    void disable_menu_action();
    void hide_menu_action();
    void show_menu_action();

private slots:
    void start();
    void stop();
    void handleProcessLog(const QString& output);
    void handleConfig();
    void handleRecvData(int id);

private:
    QWidget *main_widget;
    QGridLayout *main_layout;
    // action
    QAction *view_raw_act;
    QAction *view_res_act;
    QAction *config_act;
    QAction *start_act;
    QAction *stop_act;
    // label
    QLabel *isp_pipeline_lb;
    QLabel *src_lb;
    QLabel *dst_lb;
    QLabel *args_lb;
    QLabel *progress_lb;
    QLabel *log_lb;
    // text edit
    QLineEdit *isp_pipeline_edit;
    QLineEdit *src_dir_edit;
    QLineEdit *dst_dir_edit;
    QLineEdit *args_edit;
    QPlainTextEdit *log_text;
    // progress bar
    QProgressBar *progress_bar;

private:
    QJsonObject jsobj_params;
    QString method;
    // the dir path
    QString isp_pipeline_dir;
    QString src_dir;
    QString dst_dir;
    QString args;

    // the log
    QString log_str;

    // the response data
    QByteArray received_data;
};

#endif // CMODEL_CTRL_WINDOW_HPP
