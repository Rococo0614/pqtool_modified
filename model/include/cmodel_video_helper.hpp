#ifndef CMODEL_VIDEO_HELPER_HPP
#define CMODEL_VIDEO_HELPER_HPP

#include <memory>
#include <QObject>
#include <QFileSystemWatcher>
#include <QProcess>

using namespace std;

class CModelVideoHelper : public QObject
{
    Q_OBJECT

public:
    explicit CModelVideoHelper();
    ~CModelVideoHelper();
    static shared_ptr<CModelVideoHelper> getInstance();
    void start();
    void stop();
    void setExecFilePath(QString path);
    QString getExecFilePath();
    void setRawFilePath(QString path);
    QProcess *getProcess();

signals:
    void sigDataUpdated(QPixmap *image);

private slots:
    void handleFileChanged();
    void handleExecCmdFinished();

private:
    static shared_ptr<CModelVideoHelper> m_instance;
    QFileSystemWatcher *file_watcher;
    QString raw_file_path;
    QString result_image_path;
    QString exe_file_path;
    QString cmodel_cmd;
    QProcess *process;
    bool start_flag;
};

#endif // CMODEL_VIDEO_HELPER_HPP
