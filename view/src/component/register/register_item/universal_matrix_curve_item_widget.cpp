#include "universal_matrix_curve_item_widget.hpp"
#include "global_data.hpp"
#include "utils.hpp"

#include <QtCore>
#include <QMainWindow>
#include <QTableWidget>

UniversalMatrixCurveItemWidget::UniversalMatrixCurveItemWidget(QString module, QString title, TABLE_CURVE_TYPE type, QWidget *parent) :
    UniversalMatrixItemWidget(module, title, 0, parent)
{
    this->pageWidget = parent;
    this->module_name = module;
    this->title = title;
    this->type = type;
    initValueWidget();
    retranslateUi();
}

void UniversalMatrixCurveItemWidget::initValueWidget()
{
    register_window = new RegisterTableWindow(CONTENT_CURVE, module);
    univ_table = new UniversalTable();
    //univ_table->setMinimumWidth(1280);

    switch(type) {
        case SIMPLE_TYPE:
            table_curve = new SimpleTableCurve(module, title, pageWidget);
            break;
        case METHOD_TYPE:
            table_curve = new MethodTableCurve(module, title, pageWidget);
            break;
    }

    register_window->setWidget(table_curve);
    // table_curve for curve set dialog to setup sig&slot
    register_window->table_curve = table_curve;
    //register_window->setMinimumSize(1280, 720);
    register_window->hide();
    if ((this->module_name == "CCM") && (utils::isMatchCCMSumTitle(this->title))) {
        register_window->showCCMSumButton(true);
    }

    connect(univ_table, &UniversalTable::sigValueChanged, this, &UniversalMatrixCurveItemWidget::handleUiChanged);
    connect(univ_table, &UniversalTable::sigValueChanged, this, &UniversalMatrixCurveItemWidget::handleTableValueChanged);
    connect(univ_table, &UniversalTable::sigPageWriteData, this, &UniversalMatrixCurveItemWidget::handleWritePageData);
    connect(register_window, &RegisterTableWindow::sigHideSumColumn, univ_table, &UniversalTable::hideSumColumn);
    connect(register_window, &RegisterTableWindow::sigImportFile, univ_table, &UniversalTable::handleImportTable);
    connect(register_window, &RegisterTableWindow::sigExportFile, univ_table, &UniversalTable::handleExportTable);
    connect(register_window, &RegisterTableWindow::sigSwithChart, table_curve, &SimpleTableCurve::slotSwitchChart);

    push_button = new QPushButton;

    value_layout->addWidget(push_button);
    connect(push_button, &QPushButton::clicked,
            this, &UniversalMatrixCurveItemWidget::handleShowWindow);
}

void UniversalMatrixCurveItemWidget::setParam(QVariant param)
{
    if (param.toJsonObject().contains("widget_obj")) {
        QJsonObject item_json = param.toJsonObject()["widget_obj"].toObject();
        UniversalMatrixItemWidget::setParam(QVariant(item_json["param"]));
    } else {
        UniversalMatrixItemWidget::setParam(param);
    }
    table_curve->setParam(param);
    table_curve->setUnivTable(univ_table);
    // update axis range
    double min_x, max_x, min_y, max_y;

    table_curve->getAxis(&min_x, &max_x, &min_y, &max_y);

    register_window->setRange(min_x, max_x, min_y, max_y);
}

void UniversalMatrixCurveItemWidget::setRegSectionKeyList(QList<SecKeyType> &&reg_section_key_list)
{
    UniversalMatrixItemWidget::setRegSectionKeyList(std::move(reg_section_key_list));
    table_curve->initUI();
    if (!table_curve->isMultRow()) {
        if (auto_type == NAME_ISO) {
            table_curve->setAxisXLabelList({"ISO100", "ISO800", "ISO6400", "ISO51200",
                                                                "ISO409600", "ISO3276800"});
        } else if (auto_type == NAME_LV) {
            table_curve->setAxisXLabelList({"LV-5", "LV0", "LV5", "LV10", "LV15"});
        }
    }
}

void UniversalMatrixCurveItemWidget::setData(QVariant data, bool sync_to_global)
{
    UniversalMatrixItemWidget::setData(data, sync_to_global);
    table_curve->refreshChart();
}

void UniversalMatrixCurveItemWidget::retranslateUi()
{
    if (chmod == MOD_RO)
        push_button->setText(tr("View This Data"));
    else
        push_button->setText(tr("Edit This Data"));
}

UniversalMatrixCurveItemWidget::~UniversalMatrixCurveItemWidget()
{
    if (register_window) {
        delete register_window;
        register_window = nullptr;
    }
}
