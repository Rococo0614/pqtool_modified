#include "cmodel_video_helper.hpp"
#include "utils.hpp"

#include <QPixmap>
#include <QFileInfo>
#include <QDir>
#include <QMessageBox>
#include <QtCore>

shared_ptr<CModelVideoHelper> CModelVideoHelper::m_instance = nullptr;

CModelVideoHelper::CModelVideoHelper()
{
    exe_file_path = "";
    process = new QProcess(this);
    start_flag = false;
    raw_file_path = QString("..") + PATH_SEPARATOR +
            "Data" + PATH_SEPARATOR + "1920X1080_Hisi1226w1lux1l3dnr_-color=3_-bits=12_-frame=1_BLC_14_0_14.raw";

    connect(process, SIGNAL(finished(int)), this, SLOT(handleExecCmdFinished()));
}

CModelVideoHelper::~CModelVideoHelper()
{
    stop();
    disconnect(file_watcher, &QFileSystemWatcher::fileChanged, this, &CModelVideoHelper::handleFileChanged);
    disconnect(process, SIGNAL(finished(int)), this, SLOT(handleExecCmdFinished()));
    process->close();
}

shared_ptr<CModelVideoHelper> CModelVideoHelper::getInstance()
{
    if (nullptr == CModelVideoHelper::m_instance) {
        CModelVideoHelper::m_instance = make_shared<CModelVideoHelper>();
    }

    return CModelVideoHelper::m_instance;
}

void CModelVideoHelper::start()
{
    QFileInfo file_info(exe_file_path);
    QString root_dir_path = file_info.dir().path();
    QString config_file_path = "config.xml";
    cmodel_cmd = exe_file_path + " " + config_file_path + " " + raw_file_path;

    process->setWorkingDirectory(root_dir_path);

    result_image_path = root_dir_path + PATH_SEPARATOR + ".." + PATH_SEPARATOR +
            "Results" + PATH_SEPARATOR + "1920X1080_Hisi1226w1lux1l3dnr_-color=3_-bits=12_-frame=0_BLC_14_0_14.tiff";
    file_watcher = new QFileSystemWatcher(this);
    file_watcher->addPath(result_image_path);
    connect(file_watcher, &QFileSystemWatcher::fileChanged, this, &CModelVideoHelper::handleFileChanged);

    process->start(cmodel_cmd);
    start_flag = true;
}

void CModelVideoHelper::stop()
{
    process->kill();
    start_flag = false;
}

void CModelVideoHelper::setExecFilePath(QString path)
{
    exe_file_path = path;
}

QString CModelVideoHelper::getExecFilePath()
{
    return exe_file_path;
}

void CModelVideoHelper::setRawFilePath(QString path)
{
    if (path.indexOf(" ") > 0) {
        QMessageBox msgBox;
        msgBox.setText(tr("File path can't include space."));
        msgBox.exec();
        return;
    }
    raw_file_path = path;
}

void CModelVideoHelper::handleFileChanged()
{
    QPixmap image;
    image.load(result_image_path);
    emit sigDataUpdated(&image);
}

void CModelVideoHelper::handleExecCmdFinished()
{
    if (file_watcher->files().length() == 0) {
        QPixmap image;
        image.load(result_image_path);
        emit sigDataUpdated(&image);
        file_watcher->addPath(result_image_path);
    }

    if (start_flag) {
        process->start(cmodel_cmd);
    }
}

QProcess *CModelVideoHelper::getProcess()
{
    return process;
}
