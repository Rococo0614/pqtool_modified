#include "register_table_window.hpp"
#include "register_table.hpp"
#include "universal_table.hpp"
#include "universal_curve.hpp"
#include "manual_curve.hpp"
#include "curve_userdefine.hpp"
#include "main_window.hpp"
#include "curve_set_dialog.hpp"
#include <QMenuBar>
#include <QFileDialog>

const int MIN_WINDOW_WIDTH = 900;
const int MIN_WINDOW_HEIGHT = 400;
const int MAX_DESP_HEIGHT = 30;

RegisterTableWindow::RegisterTableWindow(QWidget *parent) :
    QMainWindow(parent)
{
    initUI();
    retranslateUi();
}

RegisterTableWindow::RegisterTableWindow(QString module, QWidget *parent) :
    module_name(module)
{
    initUI();
    retranslateUi();
}

RegisterTableWindow::RegisterTableWindow(int type, QWidget *parent) :
    content_type(type)
{
    initUI();
    retranslateUi();
}

RegisterTableWindow::RegisterTableWindow(int type, QString module, QWidget *parent) :
    content_type(type), module_name(module)
{
    initUI();
    retranslateUi();
}

RegisterTableWindow::~RegisterTableWindow() {
    if (curve_set_dialog) {
        delete curve_set_dialog;
        curve_set_dialog = nullptr;
    }
}

void RegisterTableWindow::initUI()
{
    main_widget = new QWidget();
    main_widget->setMinimumSize(QSize(MIN_WINDOW_WIDTH, MIN_WINDOW_HEIGHT));
    main_layout = new QGridLayout();
    import_btn = new QPushButton();
    export_btn = new QPushButton();
    switch_btn = new QPushButton();
    ccm_sum_btn = new QPushButton();
    read_btn = new QPushButton();
    write_btn = new QPushButton();
    chart_btn = new QPushButton();
    chart_setting_btn = new QPushButton();
    desp_label = new QLabel();
    desp_label->setMaximumHeight(MAX_DESP_HEIGHT);
    QHBoxLayout *control_area_layout = new QHBoxLayout();
    control_area_layout->addWidget(ccm_sum_btn, 0, Qt::AlignRight);
    control_area_layout->addStretch();
    control_area_layout->addWidget(chart_btn, 0, Qt::AlignLeft);
    control_area_layout->addWidget(chart_setting_btn, 0, Qt::AlignLeft);
    control_area_layout->addStretch(1);
    control_area_layout->addWidget(read_btn, 0, Qt::AlignRight);
    control_area_layout->addWidget(write_btn, 0, Qt::AlignRight);
    control_area_layout->addWidget(switch_btn, 0, Qt::AlignRight);
    control_area_layout->addWidget(import_btn, 0, Qt::AlignRight);
    control_area_layout->addWidget(export_btn, 0, Qt::AlignRight);

    main_layout->addWidget(desp_label, 0, 0);
    main_layout->addLayout(control_area_layout, 2, 0);
    setCentralWidget(main_widget);
    centralWidget()->setLayout(main_layout);

    switch_btn->hide();
    ccm_sum_btn->hide();
    if (QString::compare(module_name, QObject::tr("")) == 0) {
        read_btn->hide();
        write_btn->hide();
    }
    switch_btn->setToolTip(tr("Switch between curve and matrix."));

    connect(ccm_sum_btn, &QPushButton::clicked, this, &RegisterTableWindow::handleCCMHideEvent);
    connect(chart_setting_btn, &QPushButton::clicked, this, &RegisterTableWindow::openCurveSettingDialog);
    connect(chart_btn, &QPushButton::clicked, this, &RegisterTableWindow::switchChart);
    connect(read_btn, &QPushButton::clicked, this, &RegisterTableWindow::readCurrentPageData);
    connect(write_btn, &QPushButton::clicked, this, &RegisterTableWindow::writeCurrentPageData);
    connect(switch_btn, &QPushButton::clicked, this, &RegisterTableWindow::handleSwitchContent);
    connect(import_btn, &QPushButton::clicked, this, &RegisterTableWindow::handleImportFile);
    connect(export_btn, &QPushButton::clicked, this, &RegisterTableWindow::handleExportFile);
}

void RegisterTableWindow::retranslateUi()
{
    if (ccm_sum_btn) {
        ccm_sum_btn->setText(tr("&Hide Sum"));
    }

    if (chart_setting_btn)
    {
        chart_setting_btn->setText(tr("&CurveSet"));
    }

    if (chart_btn)
    {
        chart_btn->setText(tr("&Chart"));
    }

    if (read_btn)
    {
        read_btn->setText(tr("&Read"));
    }

    if (write_btn)
    {
        write_btn->setText(tr("&Write"));
    }

    if (import_btn)
    {
        import_btn->setText(tr("&Import"));
    }

    if (export_btn)
    {
        export_btn->setText(tr("&Export"));
    }

    if (switch_btn) {
        switch_btn->setText(tr("&Switch"));
    }

    if (CONTENT_CURVE == content_type) {
        setWindowTitle(tr("Edit Curve"));
    }
    else if (CONTENT_HIST == content_type)
    {
        setWindowTitle(tr("Display Histogram"));
    }
    else
    {
        setWindowTitle(tr("Edit Matrix"));
    }
}

void RegisterTableWindow::refresh()
{
    if (this->desp_label->text().isEmpty()) {
        desp_label->hide();
        desp_label->setVisible(false);
    } else {
        desp_label->show();
        desp_label->setVisible(true);
    }
}

void RegisterTableWindow::addIsoWidget(QWidget *widget)
{
    this->main_layout->addWidget(widget, 1, 0);

    UniversalTable *iso_table = dynamic_cast<UniversalTable *>(widget);
    if (iso_table != nullptr) {
        this->iso_table = iso_table;
    }

    UniversalCurve *iso_curve = dynamic_cast<UniversalCurve *>(widget);
    if (iso_curve) {
        this->curve_widget = iso_curve;
    }

    GammaCurve *gamma_widget = dynamic_cast<GammaCurve *>(widget);
    if (gamma_widget != nullptr) {
        this->curve_widget = gamma_widget;
    }
}

void RegisterTableWindow::setWidget(QWidget *widget)
{
    if (widget == nullptr) return;

    this->main_layout->addWidget(widget, 1, 0);

    UniversalTable *iso_table = dynamic_cast<UniversalTable *>(widget);
    if (iso_table != nullptr) {
        this->iso_table = iso_table;
    }

    UniversalCurve *curve_widget = dynamic_cast<UniversalCurve *>(widget);
    if (curve_widget != nullptr) {
        this->curve_widget = curve_widget;
    }

    GammaCurve *gamma_widget = dynamic_cast<GammaCurve *>(widget);
    if (gamma_widget != nullptr) {
        this->curve_widget = gamma_widget;
    }

    connectEvent(widget);
}

void RegisterTableWindow::connectEvent(QWidget *widget)
{
    // connect events
    RegisterTable *register_table = dynamic_cast<RegisterTable *>(widget);
    if (register_table != nullptr) {
        connect(this, &RegisterTableWindow::sigImportFile, register_table, &RegisterTable::handleImportTable);
        connect(this, &RegisterTableWindow::sigExportFile, register_table, &RegisterTable::handleExportTable);
    }

    UniversalTable *iso_table = dynamic_cast<UniversalTable *>(widget);
    if (iso_table != nullptr) {
        connect(this, &RegisterTableWindow::sigImportFile, iso_table, &UniversalTable::handleImportTable);
        connect(this, &RegisterTableWindow::sigExportFile, iso_table, &UniversalTable::handleExportTable);
    }

    UniversalCurve *curve_widget = dynamic_cast<UniversalCurve *>(widget);
    if (curve_widget != nullptr) {
        connect(this, &RegisterTableWindow::sigImportFile, curve_widget, &UniversalCurve::handleImportTable);
        connect(this, &RegisterTableWindow::sigExportFile, curve_widget, &UniversalCurve::handleExportTable);
    }

    ManualCurve *manual_curve = dynamic_cast<ManualCurve *>(widget);
    if (manual_curve != nullptr) {
        connect(this, &RegisterTableWindow::sigImportFile, manual_curve, &ManualCurve::handleImportTable);
        connect(this, &RegisterTableWindow::sigExportFile, manual_curve, &ManualCurve::handleExportTable);
    }

    CurveUserDefine *userdefine = dynamic_cast<CurveUserDefine *>(widget);
    if (userdefine != nullptr) {
        connect(this, &RegisterTableWindow::sigImportFile, userdefine, &CurveUserDefine::handleImportTable);
        connect(this, &RegisterTableWindow::sigExportFile, userdefine, &CurveUserDefine::handleExportTable);
    }
}

void RegisterTableWindow::disconnectEvent(QWidget *widget)
{
    // connect events
    RegisterTable *register_table = dynamic_cast<RegisterTable *>(widget);
    if (register_table != nullptr) {
        disconnect(this, &RegisterTableWindow::sigImportFile, register_table, &RegisterTable::handleImportTable);
        disconnect(this, &RegisterTableWindow::sigExportFile, register_table, &RegisterTable::handleExportTable);
    }

    UniversalTable *iso_table = dynamic_cast<UniversalTable *>(widget);
    if (iso_table != nullptr) {
        disconnect(this, &RegisterTableWindow::sigImportFile, iso_table, &UniversalTable::handleImportTable);
        disconnect(this, &RegisterTableWindow::sigExportFile, iso_table, &UniversalTable::handleExportTable);
    }

    UniversalCurve *curve_widget = dynamic_cast<UniversalCurve *>(widget);
    if (curve_widget != nullptr) {
        disconnect(this, &RegisterTableWindow::sigImportFile, curve_widget, &UniversalCurve::handleImportTable);
        disconnect(this, &RegisterTableWindow::sigExportFile, curve_widget, &UniversalCurve::handleExportTable);
    }

    ManualCurve *manual_curve = dynamic_cast<ManualCurve *>(widget);
    if (manual_curve != nullptr) {
        disconnect(this, &RegisterTableWindow::sigImportFile, manual_curve, &ManualCurve::handleImportTable);
        disconnect(this, &RegisterTableWindow::sigExportFile, manual_curve, &ManualCurve::handleExportTable);
    }

    CurveUserDefine *userdefine = dynamic_cast<CurveUserDefine *>(widget);
    if (userdefine != nullptr) {
        disconnect(this, &RegisterTableWindow::sigImportFile, userdefine, &CurveUserDefine::handleImportTable);
        disconnect(this, &RegisterTableWindow::sigExportFile, userdefine, &CurveUserDefine::handleExportTable);
    }
}

void RegisterTableWindow::setDesp(QString desp)
{
    this->desp_label->setText(desp);
    refresh();
}

void RegisterTableWindow::handleImportFile()
{
    QString file_path = QFileDialog::getOpenFileName(this, tr("Open File"), "", tr("Text files (*.csv *.txt)"));
    emit sigImportFile(file_path);
}

void RegisterTableWindow::handleExportFile()
{
    QString file_path = QFileDialog::getSaveFileName(this, tr("Save Param File"), "", tr("CSV (*.csv);;TXT (*.txt)"));
    emit sigExportFile(file_path);
}

void RegisterTableWindow::setReadOnly(bool m)
{
    if (m)
    {
        if (content_type == CONTENT_CURVE) {
            setWindowTitle(tr("View Curve"));
        }
        if (CONTENT_HIST == content_type)
        {
            setWindowTitle(tr("Display Histogram"));
        }
        else {
            setWindowTitle(tr("View Matrix"));
        }

        if (write_btn)
        {
            disconnect(write_btn, &QPushButton::clicked, this, &RegisterTableWindow::writeCurrentPageData);
            write_btn->hide();
        }

        if (import_btn)
        {
            disconnect(import_btn, &QPushButton::clicked, this, &RegisterTableWindow::handleImportFile);
            import_btn->hide();
        }
    }
}

void RegisterTableWindow::setContentType(int t)
{
    content_type = t;
}

void RegisterTableWindow::switchContent(int t)
{
    if (!switch_enabled)
        return;

    if (t == CONTENT_CURVE) {
        main_layout->removeWidget(iso_table);
        iso_table->hide();
        curve_widget->show();
        main_layout->addWidget(curve_widget, 1, 0);
    } else if (t == CONTENT_MATRIX) {
        main_layout->removeWidget(curve_widget);
        curve_widget->hide();
        iso_table->show();
        main_layout->addWidget(iso_table, 1, 0);
    }

    content_type = t;
}

void RegisterTableWindow::handleCCMHideEvent()
{
    sum_column_hide = sum_column_hide? false : true;
    if (sum_column_hide) {
        ccm_sum_btn->setText(tr("&Show Sum"));
    } else {
        ccm_sum_btn->setText(tr("&Hide Sum"));
    }
    emit sigHideSumColumn(sum_column_hide);
}

void RegisterTableWindow::showCCMSumButton(bool state)
{
    if (state) {
        this->ccm_sum_btn->show();
    } else {
        this->ccm_sum_btn->hide();
    }
}

void RegisterTableWindow::setSwitchButton(bool enabled)
{
    this->switch_btn->show();
    this->switch_enabled = enabled;
}


void RegisterTableWindow::setImportButton(bool enabled)
{
    if (enabled) {
        if (this->import_btn->isHidden())
            this->import_btn->show();
    } else {
        if (!this->import_btn->isHidden())
            this->import_btn->hide();
    }
}

void RegisterTableWindow::setExportButton(bool enabled)
{
    if (enabled) {
        if (this->export_btn->isHidden())
            this->export_btn->show();
    } else {
        if (!this->export_btn->isHidden())
            this->export_btn->hide();
    }
}

void RegisterTableWindow::readCurrentPageData()
{
    MainWindow::getInstance()->readPageData(module_name);
}

void RegisterTableWindow::writeCurrentPageData()
{
    MainWindow::getInstance()->writePageData(module_name);
}

void RegisterTableWindow::switchChart()
{
    emit sigSwithChart();
}

void RegisterTableWindow::handleSwitchContent()
{
    if (content_type == CONTENT_CURVE) {
        switchContent(CONTENT_MATRIX);
    } else {
        switchContent(CONTENT_CURVE);
    }
}

void RegisterTableWindow::keyReleaseEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape) {
        this->close();
    }
}

void RegisterTableWindow::openCurveSettingDialog()
{
    QPoint mousePos = QCursor::pos();

    if (!curve_set_dialog) {
        curve_set_dialog = new CurveSetDialog(this->min_x, this->max_x, this->min_y, this->max_y);
        curve_set_dialog->move(mousePos + QPoint(40, 10));
        curve_set_dialog->show();
        // curve_set_dialog & table_curve setup signal & slot
        connect(curve_set_dialog, &CurveSetDialog::sigLineParamsChanged,
                this->table_curve, &SimpleTableCurve::slotUpdateLineCurve);
        connect(curve_set_dialog, &CurveSetDialog::sigGaussianParamsChanged,
                this->table_curve, &SimpleTableCurve::slotUpdateGaussianCurve);
        connect(curve_set_dialog, &CurveSetDialog::sigGammaParamsChanged,
                this->table_curve, &SimpleTableCurve::slotUpdateGammaCurve);
        return;
    }

    if (curve_set_dialog->isHidden()) {
        curve_set_dialog->move(mousePos + QPoint(40, 10));
        curve_set_dialog->show();
    } else {
        curve_set_dialog->hide();
    }
}

void RegisterTableWindow::setRange(double min_x, double max_x, double min_y, double max_y)
{
    this->min_x = min_x;
    this->max_x = max_x;
    this->min_y = min_y;
    this->max_y = max_y;
}
