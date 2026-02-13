#ifndef STATUS_LOG_WIDGET_H
#define STATUS_LOG_WIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QCheckBox>
#include <QTimer>
#include <QFile>
#include <QStringList>
#include <QTextStream>
#include <QString>
#include <QApplication>
#include <QLabel>
#include <QComboBox>

enum class LOG_LEVEL
{
  LOG_ERROR,
  LOG_WARNING,
  LOG_INFO,
  LOG_DEBUG,
};

class StatusLogWidget : public QWidget
{
    Q_OBJECT
public:
    explicit StatusLogWidget(QWidget *parent = nullptr);
    void initUI();
    void appenLog(const QString &log, LOG_LEVEL log_level = LOG_LEVEL::LOG_INFO);
    void setLogLevel(LOG_LEVEL log_level);
    void clearLog();
    bool getRcordLogCheck();

public slots:
    void handleAppenLog(const QString &log, LOG_LEVEL log_level = LOG_LEVEL::LOG_INFO);
    void handleUpdateTheme(bool dark_theme_enabled);

private:
    QVBoxLayout *main_layout;
    QPushButton *clear_btn;
    QCheckBox   *scroll_check;
    QCheckBox   *record_log_check;
    QPlainTextEdit* log_text;
    QColor info_color;
    LOG_LEVEL log_level{LOG_LEVEL::LOG_INFO};
    QLabel *log_level_lab;
    QComboBox *log_level_box;
};


class Logger : public QObject
{
    Q_OBJECT
public:
    //Frist instance to clear old log
    static Logger& instance()
    {
        static Logger _instance(4096);
        return  _instance;
    }

    void write(const QString &message)
    {
        m_buffer.append(message);
        if(m_buffer.size() >= m_maxBuffSize){
            flush();
        }
    }

public slots:
    void flush()
    {
        if(m_buffer.size() == 0){
            return;
        }
        if(logFile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)){
            QTextStream out(&logFile);
            for(auto it : m_buffer){
                out << it << "\n";
            }
            logFile.close();
        }
        m_buffer.clear();
    }

private:
    Logger( int maxBuffSize, QObject *parent = nullptr)
        : QObject(parent), m_maxBuffSize(maxBuffSize)
    {
        logFile.setFileName(QApplication::applicationDirPath() + "/log.txt");
        clearLog();
        m_timer = new QTimer(this);
        connect(m_timer, &QTimer::timeout, this, &Logger::flush);
        m_timer->start(500);
    }
    void clearLog()
    {
        if(logFile.exists()){
            logFile.open(QIODevice::WriteOnly | QIODevice::Truncate);
            logFile.close();
        }
    }

public:
    void setAutoTestFlag(bool value)
    {
        m_AutoTestFlag = value;
    }
    bool getAutoTestFlag()
    {
        return m_AutoTestFlag;
    }
    ~Logger()
    {
        flush();
        m_timer->stop();
        delete m_timer;
    }
private:
    QStringList m_buffer;
    int m_maxBuffSize ;
    QTimer *m_timer;
    QFile logFile;
    bool m_AutoTestFlag = false;
};

#endif // STATUS_LOG_WIDGET_H
