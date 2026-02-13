#include "foldable_register_table_window.hpp"
#include "register_table.hpp"
#include "iso_table.hpp"
#include "ccm_table.hpp"

#include <QMenuBar>
#include <QFileDialog>

const int MIN_WINDOW_WIDTH = 840;
const int MIN_WINDOW_HEIGHT = 400;
const int MAX_DESP_HEIGHT = 30;

FoldableRegisterTableWindow::FoldableRegisterTableWindow(QWidget *parent) :
    QMainWindow(parent)
{
    table_fold_state = true;
    setWindowTitle(tr("Edit Matrix"));

    initUI();
    retranslateUi();
}

FoldableRegisterTableWindow::~FoldableRegisterTableWindow() {}

void FoldableRegisterTableWindow::initUI()
{
    main_widget = new QWidget();
    main_widget->setMinimumSize(QSize(MIN_WINDOW_WIDTH, MIN_WINDOW_HEIGHT));
    main_layout = new QGridLayout();
    toggle_btn = new QPushButton();
    import_btn = new QPushButton();
    export_btn = new QPushButton();
    desp_label = new QLabel();
    desp_label->setMaximumHeight(MAX_DESP_HEIGHT);
    QHBoxLayout *control_area_layout = new QHBoxLayout();
    control_area_layout->addStretch();
    control_area_layout->addWidget(toggle_btn, 0, Qt::AlignLeft);
    control_area_layout->addStretch(1);
    control_area_layout->addWidget(import_btn, 0, Qt::AlignRight);
    control_area_layout->addStretch();
    control_area_layout->addWidget(export_btn, 0, Qt::AlignRight);
    control_area_layout->addStretch();

    main_layout->addWidget(desp_label, 0, 0);
    main_layout->addLayout(control_area_layout, 2, 0);
    setCentralWidget(main_widget);
    centralWidget()->setLayout(main_layout);

    connect(toggle_btn, &QPushButton::clicked, this, &FoldableRegisterTableWindow::handleToggleEvent);
    connect(import_btn, &QPushButton::clicked, this, &FoldableRegisterTableWindow::handleImportFile);
    connect(export_btn, &QPushButton::clicked, this, &FoldableRegisterTableWindow::handleExportFile);
}

void FoldableRegisterTableWindow::retranslateUi()
{
    updateToggleBtnLabel();
    import_btn->setText(tr("&Import"));
    export_btn->setText(tr("&Export"));
}

void FoldableRegisterTableWindow::refresh()
{
    if (this->desp_label->text().isEmpty()) {
        desp_label->hide();
        desp_label->setVisible(false);
    } else {
        desp_label->show();
        desp_label->setVisible(true);
    }
}

void FoldableRegisterTableWindow::updateToggleBtnLabel()
{
    if (table_fold_state)
    {
        toggle_btn->setText(tr("&Show Summary"));
    } else {
        toggle_btn->setText(tr("&Hide Summary"));
    }
}

void FoldableRegisterTableWindow::setWidget(QWidget *widget)
{
    if (widget == nullptr) return;

    this->main_layout->addWidget(widget, 1, 0);

    // connect events
    CcmTable *foldable_table = dynamic_cast<CcmTable *>(widget);
    if (foldable_table != nullptr)
    {
        connect(this, &FoldableRegisterTableWindow::sigToggle, foldable_table, &CcmTable::handleToggleEvent);
        connect(this, &FoldableRegisterTableWindow::sigImportFile, foldable_table, &CcmTable::handleImportTable);
        connect(this, &FoldableRegisterTableWindow::sigExportFile, foldable_table, &CcmTable::handleExportTable);
    }
}

void FoldableRegisterTableWindow::setDesp(QString desp)
{
    this->desp_label->setText(desp);
    refresh();
}

void FoldableRegisterTableWindow:: handleToggleEvent()
{
    table_fold_state = table_fold_state?false:true;
    updateToggleBtnLabel();
    emit sigToggle(table_fold_state);
}

void FoldableRegisterTableWindow::handleImportFile()
{
    QString file_path = QFileDialog::getOpenFileName(this, tr("Open File"), "", tr("Text files (*.csv *.txt)"));
    emit sigImportFile(file_path);
}

void FoldableRegisterTableWindow::handleExportFile()
{
    QString file_path = QFileDialog::getSaveFileName(this, tr("Save Param File"), "", tr("CSV (*.csv);;TXT (*.txt)"));
    emit sigExportFile(file_path);
}

void FoldableRegisterTableWindow::keyReleaseEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape) {
        this->close();
    }
}
