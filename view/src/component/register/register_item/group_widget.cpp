#include "group_widget.hpp"
#include "config_set_widget.hpp"
#include "global_data.hpp"
#include "utils.hpp"
#include "base_page_widget.hpp"

#include <QtCore>
#include <QHBoxLayout>
#include <QStyleOption>
#include <QPainter>
#include <QMessageBox>

const QString TEMP_DIR_NAME = "temp";

GroupItemWidget::GroupItemWidget(QString title, QWidget *parent) :
    QWidget(parent)
{
    // set size retain when hidden
    QSizePolicy sp_retain = this->sizePolicy();
    sp_retain.setRetainSizeWhenHidden(true);
    this->setSizePolicy(sp_retain);

    QVBoxLayout *wraper_layout = new QVBoxLayout(this);
    wraper_layout->setAlignment(Qt::AlignTop);
    wraper_layout->setSpacing(0);
    wraper_layout->setMargin(0);

    group_widget = new QWidget;
    group_layout = new QVBoxLayout(group_widget);

    group_layout->setSpacing(1);
    group_layout->setContentsMargins(1, 1, 1, 1);
    group_layout->setAlignment(Qt::AlignTop);

    group_widget->setObjectName("LeftArea");
    group_widget->setStyleSheet("#LeftArea{ background-color: #c0c0c0; }");

    QHBoxLayout *title_layout = new QHBoxLayout;
    title_label = new QLabel(title);
    title_label->setMargin(5);
    config_set_btn = new QPushButton;
    config_set_btn->setFlat(true);
    config_set_btn->setIcon(QIcon(":/icons/list.png"));
    config_set_btn->setMaximumWidth(48);
    title_layout->addWidget(title_label);
    title_layout->addWidget(config_set_btn, Qt::AlignRight);
    group_layout->addLayout(title_layout);

    // Todo: check config_set_widget
    ConfigSetWidget *config_set_widget = new ConfigSetWidget;
    group_layout->addWidget(config_set_widget);
    config_set_widget->setVisible(false);
    connect(config_set_btn, &QPushButton::clicked, config_set_widget, &ConfigSetWidget::toggleVisible);
    connect(config_set_widget, &ConfigSetWidget::sigSaveConfigSet, this, &GroupItemWidget::handleSaveConfigSet);
    connect(config_set_widget, &ConfigSetWidget::sigLoadConfigSet, this, &GroupItemWidget::handleLoadConfigSet);

    wraper_layout->addWidget(group_widget);
}

void GroupItemWidget::paintEvent(QPaintEvent *)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

QString GroupItemWidget::getTempDirPath()
{
    return QDir::currentPath() + PATH_SEPARATOR + TEMP_DIR_NAME;
}

void GroupItemWidget::setConfigSetHidden()
{
    this->config_set_btn->hide();
}

QString GroupItemWidget::getConfigFilePath(int idx)
{
    return getTempDirPath() + PATH_SEPARATOR +
            "config_set" + QString::number(idx) + "_" + this->reg_section + "_" + title_label->text() + ".json";
}

void GroupItemWidget::handleSaveConfigSet(int idx)
{
    QJsonArray items;
    for (auto base_item : base_item_list) {
        QJsonObject json_data = base_item->getJsonData();
        if (!json_data.isEmpty()) {
            items.append(json_data);
        }
    }

    QJsonObject data_obj;
    data_obj[this->reg_section] = items;

    // save temp config file
    QString temp_dir_str = getTempDirPath();
    const QFileInfo temp_dir(temp_dir_str);
    if ((!temp_dir.exists()) || (!temp_dir.isDir()) || (!temp_dir.isWritable())) {
        QDir().mkdir(temp_dir_str);
    }
    utils::saveJson(getConfigFilePath(idx), data_obj);
}

void GroupItemWidget::handleLoadConfigSet(int idx)
{
    QString config_name = getConfigFilePath(idx);
    QJsonObject data_obj;
    QString error_msg;
    if (utils::loadJson(config_name, data_obj, error_msg) != 0) {
        QMessageBox::critical(this, "Error", error_msg, QMessageBox::Ok);
        return;
    }

    QJsonArray array = data_obj[this->reg_section].toArray();
    foreach (BaseItemWidget *base_item_widget, base_item_list) {
        QString key = base_item_widget->getKey();
        for(int i=0; i<array.size(); i++) {
            QJsonObject obj = array.at(i).toObject();
            if (obj.value("key") == key && obj.value("optype") == base_item_widget->getOpType())
            {
                QVariant v = obj.value("value").toVariant();
                base_item_widget->setData(v, true);
                break;
            }
        }
    }
}

void GroupItemWidget::addWidget(QWidget *widget)
{
    group_layout->addWidget(widget);

    BaseItemWidget *base_item = dynamic_cast<BaseItemWidget *>(widget);
    if (base_item != nullptr) {
        base_item_list.append(base_item);
    }
}

void GroupItemWidget::addPageWidget(QWidget *widget, QWidget *page)
{
    addWidget(widget);

    BaseItemWidget *base_item = dynamic_cast<BaseItemWidget *>(widget);
    BasePageWidget *page_widget = dynamic_cast<BasePageWidget *>(page);
    if ((base_item != nullptr) && (page_widget != nullptr)) {
        // bind write page data signal to slot
        connect(base_item, &BaseItemWidget::sigWritePageData, page_widget, &BasePageWidget::writeData);
    }
}

void GroupItemWidget::addLayout(QLayout *layout)
{
    group_layout->addLayout(layout);
}

void GroupItemWidget::setRegSection(QString reg_setion)
{
    this->reg_section = reg_setion;
}

void GroupItemWidget::updateVisible()
{
    if (GlobalData::getInstance()->getLevel() < this->level) {
        if (!this->isHidden()) {
            this->hide();
        }
    } else {
        if (this->isHidden()) {
            this->show();
        }
    }
}

void GroupItemWidget::setLevel(int level)
{
    this->level = level;
}

int GroupItemWidget::getLevel()
{
    return this->level;
}
