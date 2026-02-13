#include "global_data.hpp"
#include "module_define.hpp"
#include "register_page_widget.hpp"
#include "register_lineedit.hpp"
#include "register_checkbox.hpp"
#include "ini_helper.hpp"
#include "utils.hpp"

#include <QLabel>
#include <QTableWidget>
#include <QHeaderView>
#include <QPushButton>
#include <QVariant>
#include <QVariantMap>
#include <QStringList>
#include <QLineEdit>
#include <QCheckBox>

RegisterPageWidget::RegisterPageWidget(QWidget *parent) : BasePageWidget(MODULE_NAME_REG_LIST, parent)
{
    initData();
    initUI();
    retranslateUi();
}

void RegisterPageWidget::initCenterArea()
{
    QWidget *register_center_widget = new QWidget;
    QWidget *register_desp_widget = new QWidget;
    QVBoxLayout *register_main_layout = new QVBoxLayout;
    QVBoxLayout *register_desp_layout = new QVBoxLayout;
    desp_label = new QLabel;
    desp_label->setAlignment(Qt::AlignTop);
    checkbox_edit = new RegisterCheckBox(this);
    register_main_layout->setAlignment(Qt::AlignTop);
    register_center_widget->setLayout(register_main_layout);
    register_desp_widget->setLayout(register_desp_layout);
    register_desp_layout->addWidget(desp_label);
    register_main_layout->addWidget(register_desp_widget, 8);
    register_main_layout->addWidget(checkbox_edit, 1);

    main_layout->addWidget(register_center_widget);
}

void RegisterPageWidget::retranslateUi()
{
    register_table_widget->setHorizontalHeaderLabels({tr("Address"), tr("Value")});

    BasePageWidget::retranslateUi();
}

void RegisterPageWidget::initLeftArea()
{
    register_table_widget = new QTableWidget(0, 2);
    register_table_widget->verticalHeader()->hide();
    register_table_widget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    register_table_widget->horizontalHeader()->setSectionsClickable(false);

    connect(register_table_widget, &QTableWidget::itemSelectionChanged,
            this, &RegisterPageWidget::itemSelectionChanged);

    register_table_widget->clearContents();
    main_layout->addWidget(register_table_widget);

    int ori_row = register_table_widget->currentRow();
    int ori_col = register_table_widget->currentColumn();

    int row = 0;

    // get all registers and create table item
    QJsonObject registers = GlobalData::getInstance()->getRegisters();
    for (auto reg_section : registers.keys()) {
        QJsonObject regs_in_sec = registers[reg_section].toObject();
        for (auto reg_key : regs_in_sec.keys()) {
            QJsonObject reg_object = regs_in_sec[reg_key].toObject();
            if (reg_object[KEY_REG_ADDRESS].isNull() || reg_object[KEY_REG_ADDRESS].toString().isEmpty()) {
                continue;
            }

            register_table_widget->insertRow(row);
            // address
            QTableWidgetItem *item = new QTableWidgetItem(utils::HEX_PREFIX + reg_object[KEY_REG_ADDRESS].toString());
            item->setFlags(item->flags() & ~Qt::ItemIsEditable & ~Qt::ItemIsUserCheckable);
            register_table_widget->setItem(row, 0, item);

            // value
            RegisterLineEdit *line_edit = new RegisterLineEdit;
            line_edit->addRegSectionKey(reg_section, reg_key);
            line_edit->setBitmask(reg_object[KEY_REG_BITMASK].toString());
            line_edit->setDesp(reg_object[KEY_REG_DESP].toString());
            connect(line_edit, &RegisterLineEdit::sigFocusIn,
                    [=] () {
                        register_table_widget->setCurrentItem(item);
                    });
            register_table_widget->setCellWidget(row, 1, line_edit);

            row++;
        }
    }

    register_table_widget->setRowCount(row);
    register_table_widget->sortItems(0);
    register_table_widget->setCurrentCell(ori_row, ori_col);
}

void RegisterPageWidget::itemSelectionChanged()
{
    int row = register_table_widget->currentRow();
    RegisterLineEdit *item = dynamic_cast<RegisterLineEdit *>(register_table_widget->cellWidget(row, 1));
    if (item == nullptr){
        return;
    }

    QString address = register_table_widget->item(row, 0)->text();
    QString bitmask = item->getBitmask();
    QList<SecKeyType> reg_section_key_list = item->getSectionKeyList();

    // set reg desp
    QString reg_detail = "";
    reg_detail += "Register:\n";
    for (auto reg_section_key : reg_section_key_list) {
        reg_detail += reg_section_key.first + "." + reg_section_key.second + "\n\n";
    }
    reg_detail += "Desp:\n" + item->getDesp() + "\n\n";
    desp_label->setText(reg_detail);

    checkbox_edit->setAddressText(address);
    checkbox_edit->setBitmask(utils::HEX_PREFIX + bitmask);
    checkbox_edit->setRegSectionKeyList(item->getSectionKeyList());
}
