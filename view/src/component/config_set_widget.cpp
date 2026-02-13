#include "config_set_widget.hpp"
#include <QPushButton>
#include <QStyleOption>
#include <QPainter>

const int COLUMN_MAX_CONFIG_NUM = 4;
const int MAX_CONFIG_SET_NUM = 4;

ConfigSetWidget::ConfigSetWidget(QWidget *parent) : QWidget(parent)
{
    initUI();
}

void ConfigSetWidget::initUI()
{
    main_layout = new QGridLayout(this);
    main_layout->setSpacing(0);
    main_layout->setContentsMargins(0, 0, 0, 0);

    int config_idx = 0;
    for (int row = 0; row < (MAX_CONFIG_SET_NUM/COLUMN_MAX_CONFIG_NUM) * 2; row += 2) {
        for (int i = config_idx; (i < (config_idx + COLUMN_MAX_CONFIG_NUM)) && (i < MAX_CONFIG_SET_NUM); i++) {
            // to set
            QString title = "To Set" + QString::number(i + 1);
            QPushButton *config_set_save_btn = new QPushButton(title);
            main_layout->addWidget(config_set_save_btn, row, i - ((row / 2) * COLUMN_MAX_CONFIG_NUM));
            connect(config_set_save_btn, &QPushButton::clicked,
                    [=] () {
                        emit sigSaveConfigSet(i);
                    });

            // load set
            title = "Load Set" + QString::number(i + 1);
            QPushButton *config_set_load_btn = new QPushButton(title);
            config_set_load_btn->setEnabled(false);
            main_layout->addWidget(config_set_load_btn, row + 1, i - ((row / 2) * COLUMN_MAX_CONFIG_NUM));
            connect(config_set_load_btn, &QPushButton::clicked,
                    [=] () {
                        emit sigLoadConfigSet(i);
                    });
            connect(this, &ConfigSetWidget::sigSaveConfigSet,
                    [=] (int idx) {
                        if (idx == i) {
                            config_set_load_btn->setEnabled(true);
                        }
                    });
        }
        config_idx += COLUMN_MAX_CONFIG_NUM;
    }
}

void ConfigSetWidget::toggleVisible()
{
    this->isVisible() ? this->setVisible(false) : this->setVisible(true);
}
