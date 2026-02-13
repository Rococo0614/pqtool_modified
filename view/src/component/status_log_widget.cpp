#include "status_log_widget.hpp"

#include <QLabel>
#include <QDate>
#include <QScrollBar>
#include <QApplication>
#include <QTextStream>
#include <QProcessEnvironment>

const int MAX_WIDGET_HEIGHT = 120;
const QColor BLACK_COLOR = QColor(0, 0, 0);
const QColor WHITE_COLOR = QColor(224, 225, 226);
const QColor YELLOW_COLOR = QColor(192, 192, 0);
const QColor RED_COLOR = QColor(255, 0, 0);

StatusLogWidget::StatusLogWidget(QWidget *parent) : QWidget(parent)
{
    info_color = BLACK_COLOR;
    initUI();
}

void StatusLogWidget::initUI()
{
    main_layout = new QVBoxLayout(this);
    log_text = new QPlainTextEdit;
    log_text->setReadOnly(true);
    clear_btn = new QPushButton(tr("Clear"));
    connect(clear_btn, &QPushButton::clicked, [=]{
        log_text->clear();
    });
    clear_btn->setMaximumWidth(60);
    scroll_check = new QCheckBox(tr("Scroll to bottom once new logs emited"));
    record_log_check = new QCheckBox(tr("Show json file record log"));
    scroll_check->setMaximumWidth(320);
    log_level_lab = new QLabel("Log Level:");
    log_level_box = new QComboBox;
    QStringList level_str_list = {"LOG_ERROR", "LOG_WARNING", "LOG_INFO", "LOG_DEBUG"};
    log_level_box->addItems(level_str_list);
    log_level_box->setCurrentIndex(3);
    QHBoxLayout *h = new QHBoxLayout;
    h->addWidget(clear_btn);
    h->addSpacing(15);
    h->addWidget(log_level_lab);
    h->addSpacing(10);
    h->addWidget(log_level_box);
    h->addStretch(1);
    h->addWidget(scroll_check);
    h->addSpacing(10);
    h->addWidget(record_log_check);
    h->addSpacing(20);
    main_layout->addLayout(h);
    main_layout->addWidget(log_text);

    appenLog(tr("Welcome to cvitek isp tool."));
    this->setMaximumHeight(MAX_WIDGET_HEIGHT);

    QProcessEnvironment envTest = QProcessEnvironment::systemEnvironment();
    if(envTest.contains("isp_tool_autotest" ) && envTest.value("isp_tool_autotest") == "true"){
        Logger::instance().setAutoTestFlag(true);
    }else{
        Logger::instance().setAutoTestFlag(false);
    }
}

void StatusLogWidget::appenLog(const QString &log, LOG_LEVEL log_level)
{
    if (log_level > this->log_level) return;

    if (log_level > (LOG_LEVEL)log_level_box->currentIndex()) {
        return;
    }

    QTextCharFormat tf = log_text->currentCharFormat();
    QString log_prefix = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    switch (log_level) {
        case LOG_LEVEL::LOG_INFO:
            log_prefix += " [INFO] ";
            tf.setForeground(QBrush(info_color));
            break;
        case LOG_LEVEL::LOG_WARNING:
            log_prefix += " [WARNING] ";
            tf.setForeground(QBrush(YELLOW_COLOR));
            break;
        case LOG_LEVEL::LOG_ERROR:
            log_prefix += " [ERROR] ";
            tf.setForeground(QBrush(RED_COLOR));
            break;
        case LOG_LEVEL::LOG_DEBUG:
            log_prefix += " [DEBUG] ";
            tf.setForeground(QBrush(info_color));
            break;
        default:
            log_prefix += " ";
            break;
    };
    log_text->setCurrentCharFormat(tf);
    log_text->appendPlainText(log_prefix + log);

    Logger& logger = Logger::instance();
    if(logger.getAutoTestFlag()){
        logger.write(log_prefix + log);
    }

    if (scroll_check && scroll_check->checkState() == Qt::CheckState::Checked) {
        QScrollBar *scrollbar = log_text->verticalScrollBar();
        if (scrollbar) scrollbar->setSliderPosition(scrollbar->maximum());
    }
}

void StatusLogWidget::handleAppenLog(const QString &log, LOG_LEVEL log_level)
{
    appenLog(log, log_level);
    this->repaint();
}

void StatusLogWidget::setLogLevel(LOG_LEVEL log_level)
{
    this->log_level = log_level;
}

void StatusLogWidget::handleUpdateTheme(bool dark_theme_enabled)
{
    if (dark_theme_enabled) {
        info_color = WHITE_COLOR;
    } else {
        info_color = BLACK_COLOR;
    }
}

void StatusLogWidget::clearLog()
{
    log_text->clear();
}

bool StatusLogWidget::getRcordLogCheck()
{
    if (record_log_check && record_log_check->checkState() == Qt::CheckState::Checked) {
        return true;
    }
    return false;
}
