#include "gamma_curve_item_widget.hpp"
#include "global_data.hpp"
#include "utils.hpp"

#include <QtCore>
#include <QMainWindow>
#include <QTableWidget>

GammaCurveItemWidget::GammaCurveItemWidget(QString module, QString title, GAMMA_CURVE_TYPE type, QWidget *parent) :
    BaseItemWidget(module, title, parent)
{
    this->pageWidget = parent;
    this->type = type;
    initValueWidget();
    retranslateUi();
}

void GammaCurveItemWidget::initValueWidget()
{
    register_window = new RegisterTableWindow(CONTENT_CURVE, module);
    switch(type) {
        case MAP_CURVE_TYPE_T:
            gamma_curve = new MapCurve(module, title, pageWidget);
            break;
        case GAMMA_CURVE_TYPE_T:
            gamma_curve = new GammaCurve(module, title, pageWidget);
            break;
        case GAMMA_CURVE_TYPE_LV:
            gamma_curve = new GammaCurveLV(module, title, pageWidget);
            break;
        case GAMMA_CURVE_TYPE_RGB:
            gamma_curve = new GammaCurveRGB(module, title, pageWidget);
            break;
    }
    register_window->setWidget(gamma_curve);
    register_window->setImportButton(false);
    register_window->setExportButton(false);
    register_window->setMinimumSize(1280, 720);
    register_window->hide();

    connect(gamma_curve, &GammaCurve::sigValueChanged,
            this, &GammaCurveItemWidget::handleUiChanged);

    connect(gamma_curve, &GammaCurve::sigWritePageData,
                this, &GammaCurveItemWidget::handleWritePageData);

    push_button = new QPushButton;

    value_layout->addWidget(push_button);
    connect(push_button, &QPushButton::clicked,
            this, &GammaCurveItemWidget::handleShowWindow);
}

void GammaCurveItemWidget::handleShowWindow()
{
    if (register_window->isMinimized()) {
        register_window->showNormal();
    }
    register_window->show();
    register_window->activateWindow();

    gamma_curve->onOpenWindow();
}

void GammaCurveItemWidget::refreshData(const SecKeyType&)
{
}

void GammaCurveItemWidget::setParam(QVariant param)
{
    gamma_curve->setParam(param);
}

void GammaCurveItemWidget::handleWritePageData()
{
    emit sigWritePageData();
}

void GammaCurveItemWidget::setRegSectionKeyList(QList<SecKeyType> &&reg_section_key_list)
{
    BaseItemWidget::setRegSectionKeyList(std::move(reg_section_key_list));

    if (!reg_section_key_list.isEmpty()) {
        register_window->setDesp(reg_section_key_list.first().first + "/" + title_label->text());
    }

    gamma_curve->initUI();
}

void GammaCurveItemWidget::setOpType(QString op_type)
{
    BaseItemWidget::setOpType(op_type);
}

void GammaCurveItemWidget::setData(QVariant data, bool sync_to_global)
{
    if (sync_to_global == false) {
        gamma_curve->blockSignals(true);
    }

    gamma_curve->setData(data);

    if (sync_to_global == false) {
        gamma_curve->blockSignals(false);
    }
}

QVariant GammaCurveItemWidget::getData() const
{
    return gamma_curve->getData();
}

void GammaCurveItemWidget::retranslateUi()
{
    if (chmod == MOD_RO)
        push_button->setText(tr("View this Curve"));
    else
        push_button->setText(tr("Edit this Curve"));
}

void GammaCurveItemWidget::setChmod(int mod)
{
    BaseItemWidget::setChmod(mod);
    retranslateUi();
}

GammaCurveItemWidget::~GammaCurveItemWidget()
{
    delete register_window;
}
