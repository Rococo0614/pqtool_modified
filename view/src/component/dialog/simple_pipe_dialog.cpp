#include "simple_pipe_dialog.hpp"
#include "utils.hpp"
#include <QFileDialog>

const QString DEFAULT_WIDTH = "960";
const QString DEFAULT_HEIGHT = "720";
const int ITEM_WIDTH = 100;

using utils::RAW_BITS;
using utils::RAW_PACK_TYPE;

SimplePipeDialog::SimplePipeDialog(QWidget *parent) :
	QDialog(parent)
{
	setWindowTitle(tr("Simple Pipe Dialog"));
	setWindowFlags(Qt::WindowCloseButtonHint);
	this->setMinimumWidth(600);

	initUI();
	retranslateUi();
}

void SimplePipeDialog::initUI()
{
	main_layout = new QVBoxLayout(this);

	// PIPE
	pipe_enable_label = new QLabel;
	pipe_enable_label->setFixedWidth(ITEM_WIDTH);
	pipe_checkbox = new QCheckBox;
	pipe_checkbox->setChecked(false);

	pipe_group = new QGroupBox("Simple Pipe");
	QGridLayout *pipe_layout = new QGridLayout;
	pipe_layout->addWidget(pipe_enable_label, 0, 0, Qt::AlignLeft);
	pipe_layout->addWidget(pipe_checkbox, 0, 1, Qt::AlignLeft);
	pipe_group->setLayout(pipe_layout);
	connect(pipe_checkbox, &QCheckBox::stateChanged, this, &SimplePipeDialog::handlePipeCheckboxChanged);
	// BLC
	blc_enable_label = new QLabel;
	blc_enable_label->setFixedWidth(ITEM_WIDTH);
	blc_checkbox = new QCheckBox;
	blc_checkbox->setChecked(true);

	blc_offset_r_label = new QLabel;
	blc_offset_r_label->setFixedWidth(ITEM_WIDTH);
	blc_offset_r_line_edit = new QLineEdit("128");
	blc_offset_r_line_edit->setFixedWidth(ITEM_WIDTH);

	blc_offset_g_label = new QLabel;
	blc_offset_g_label->setFixedWidth(ITEM_WIDTH);
	blc_offset_g_line_edit = new QLineEdit("128");
	blc_offset_g_line_edit->setFixedWidth(ITEM_WIDTH);

	blc_offset_b_label = new QLabel;
	blc_offset_b_label->setFixedWidth(ITEM_WIDTH);
	blc_offset_b_line_edit = new QLineEdit("128");
	blc_offset_b_line_edit->setFixedWidth(ITEM_WIDTH);

	blc_group = new QGroupBox("Blc");
	QGridLayout *blc_layout = new QGridLayout;
	blc_layout->addWidget(blc_enable_label, 0, 0, Qt::AlignLeft);
	blc_layout->addWidget(blc_checkbox, 0, 1, Qt::AlignLeft);
	blc_layout->addWidget(blc_offset_r_label, 1, 0, Qt::AlignLeft);
	blc_layout->addWidget(blc_offset_r_line_edit, 1, 1, Qt::AlignLeft);
	blc_layout->addWidget(blc_offset_g_label, 1, 2, Qt::AlignLeft);
	blc_layout->addWidget(blc_offset_g_line_edit, 1, 3, Qt::AlignLeft);
	blc_layout->addWidget(blc_offset_b_label, 1, 4, Qt::AlignLeft);
	blc_layout->addWidget(blc_offset_b_line_edit, 1, 5, Qt::AlignLeft);
	blc_group->setLayout(blc_layout);

	// WBG
	wbg_enable_label = new QLabel;
	wbg_enable_label->setFixedWidth(ITEM_WIDTH);
	wbg_checkbox = new QCheckBox;
	wbg_checkbox->setChecked(true);

	wbg_gain_r_label = new QLabel;
	wbg_gain_r_label->setFixedWidth(ITEM_WIDTH);
	wbg_gain_r_line_edit = new QLineEdit("1024");
	wbg_gain_r_line_edit->setFixedWidth(ITEM_WIDTH);

	wbg_gain_g_label = new QLabel;
	wbg_gain_g_label->setFixedWidth(ITEM_WIDTH);
	wbg_gain_g_line_edit = new QLineEdit("1024");
	wbg_gain_g_line_edit->setFixedWidth(ITEM_WIDTH);

	wbg_gain_b_label = new QLabel;
	wbg_gain_b_label->setFixedWidth(ITEM_WIDTH);
	wbg_gain_b_line_edit = new QLineEdit("1024");
	wbg_gain_b_line_edit->setFixedWidth(ITEM_WIDTH);

	wbg_group = new QGroupBox("Wbg");
	QGridLayout *wbg_layout = new QGridLayout;
	wbg_layout->addWidget(wbg_enable_label, 0, 0, Qt::AlignLeft);
	wbg_layout->addWidget(wbg_checkbox, 0, 1, Qt::AlignLeft);
	wbg_layout->addWidget(wbg_gain_r_label, 1, 0, Qt::AlignLeft);
	wbg_layout->addWidget(wbg_gain_r_line_edit, 1, 1, Qt::AlignLeft);
	wbg_layout->addWidget(wbg_gain_g_label, 1, 2, Qt::AlignLeft);
	wbg_layout->addWidget(wbg_gain_g_line_edit, 1, 3, Qt::AlignLeft);
	wbg_layout->addWidget(wbg_gain_b_label, 1, 4, Qt::AlignLeft);
	wbg_layout->addWidget(wbg_gain_b_line_edit, 1, 5, Qt::AlignLeft);
	wbg_group->setLayout(wbg_layout);

	// RGBGAMMA
	rgbgamma_enable_label = new QLabel;
	rgbgamma_enable_label->setFixedWidth(ITEM_WIDTH);
	rgbgamma_checkbox = new QCheckBox;
	rgbgamma_checkbox->setChecked(true);

	rgbgamma_group = new QGroupBox("RgbGamma");
	QGridLayout *rgbgamma_layout = new QGridLayout;
	rgbgamma_layout->addWidget(rgbgamma_enable_label, 0, 0, Qt::AlignLeft);
	rgbgamma_layout->addWidget(rgbgamma_checkbox, 0, 1, Qt::AlignLeft);
	rgbgamma_group->setLayout(rgbgamma_layout);

	QHBoxLayout *select_layout = new QHBoxLayout();
	apply_btn = new QPushButton;
	apply_btn->setFixedWidth(ITEM_WIDTH);
	ok_btn = new QPushButton;
	ok_btn->setFixedWidth(ITEM_WIDTH);
	select_layout->addStretch();
	select_layout->addWidget(apply_btn);
	select_layout->addWidget(ok_btn);

	main_layout->addWidget(pipe_group);
	main_layout->addWidget(blc_group);
	main_layout->addWidget(wbg_group);
	main_layout->addWidget(rgbgamma_group);
	main_layout->addLayout(select_layout);

	QFont titleFont("Arial Black", 10);
	titleFont.setBold(true);

	pipe_group->setFont(titleFont);
	blc_group->setFont(titleFont);
	wbg_group->setFont(titleFont);
	rgbgamma_group->setFont(titleFont);

	blc_group->setEnabled(false);
	wbg_group->setEnabled(false);
	rgbgamma_group->setEnabled(false);

	connect(apply_btn, &QPushButton::clicked, this, &SimplePipeDialog::handleApply);
	connect(ok_btn, &QPushButton::clicked, this, &SimplePipeDialog::handleOk);
}

void SimplePipeDialog::retranslateUi()
{
	pipe_enable_label->setText(tr("Enable:"));
	pipe_checkbox->setText("");

	blc_enable_label->setText(tr("Enable:"));
	blc_checkbox->setText("");
	blc_offset_r_label->setText(tr("blc_offset_r:"));
	blc_offset_g_label->setText(tr("blc_offset_g:"));
	blc_offset_b_label->setText(tr("blc_offset_b:"));

	wbg_enable_label->setText(tr("Enable:"));
	wbg_checkbox->setText("");
	wbg_gain_r_label->setText(tr("wbg_gain_r:"));
	wbg_gain_g_label->setText(tr("wbg_gain_g:"));
	wbg_gain_b_label->setText(tr("wbg_gain_b:"));

	rgbgamma_enable_label->setText(tr("Enable:"));
	rgbgamma_checkbox->setText("");

	apply_btn->setText(tr("Apply"));
	ok_btn->setText(tr("OK"));
}

void SimplePipeDialog::handleOk()
{
	handleApply();
	this->accept();
}

void SimplePipeDialog::handleApply()
{
	emit updateSimplePipeAttr();
}

void  SimplePipeDialog::handlePipeCheckboxChanged()
{
	if (pipe_checkbox->isChecked()) {
		blc_group->setEnabled(true);
		wbg_group->setEnabled(true);
		rgbgamma_group->setEnabled(true);
	} else {
		blc_group->setEnabled(false);
		wbg_group->setEnabled(false);
		rgbgamma_group->setEnabled(false);
	}
}

void SimplePipeDialog::setSimplePipeAttr(utils::BLC_ATTR_S *blc_attr, utils::WBG_ATTR_S *wbg_attr,
		utils::SIMPLE_PIPE_ENABLE_ATTR_S *simple_pipe_enable)
{
	bool ok;

	// blc
	blc_attr->offset_r = blc_offset_r_line_edit->text().toUInt(&ok);
	if (!ok) {
		blc_attr->offset_r = 240;
	}
	blc_attr->offset_g = blc_offset_g_line_edit->text().toUInt(&ok);
	if (!ok) {
		blc_attr->offset_g = 240;
	}
	blc_attr->offset_b = blc_offset_b_line_edit->text().toUInt(&ok);
	if (!ok) {
		blc_attr->offset_b = 240;
	}

	// wbg
	wbg_attr->wb_gain_r = wbg_gain_r_line_edit->text().toUInt(&ok);
	if (!ok) {
		wbg_attr->wb_gain_r = 1500;
	}
	wbg_attr->wb_gain_g = wbg_gain_g_line_edit->text().toUInt(&ok);
	if (!ok) {
		wbg_attr->wb_gain_g = 1024;
	}
	wbg_attr->wb_gain_b = wbg_gain_b_line_edit->text().toUInt(&ok);
	if (!ok) {
		wbg_attr->wb_gain_b = 2000;
	}

	// enable
	simple_pipe_enable->pipe_enable = pipe_checkbox->isChecked();
	simple_pipe_enable->blc_enable = blc_checkbox->isChecked();
	simple_pipe_enable->wbg_enable = wbg_checkbox->isChecked();
	simple_pipe_enable->rgbgamma_enable = rgbgamma_checkbox->isChecked();
}

void SimplePipeDialog::getSimplePipeAttr(utils::BLC_ATTR_S *blc_attr, utils::WBG_ATTR_S *wbg_attr,
		utils::SIMPLE_PIPE_ENABLE_ATTR_S *simple_pipe_enable)
{
	// blc
	blc_offset_r_line_edit->setText(QString::number(blc_attr->offset_r));
	blc_offset_g_line_edit->setText(QString::number(blc_attr->offset_g));
	blc_offset_b_line_edit->setText(QString::number(blc_attr->offset_b));
	// wbg
	wbg_gain_r_line_edit->setText(QString::number(wbg_attr->wb_gain_r));
	wbg_gain_g_line_edit->setText(QString::number(wbg_attr->wb_gain_g));
	wbg_gain_b_line_edit->setText(QString::number(wbg_attr->wb_gain_b));
	// enable
	Qt::CheckState check_state;
	check_state = simple_pipe_enable->pipe_enable ? Qt::CheckState::Checked : Qt::CheckState::Unchecked;
	pipe_checkbox->setCheckState(check_state);
	check_state = simple_pipe_enable->blc_enable ? Qt::CheckState::Checked : Qt::CheckState::Unchecked;
	blc_checkbox->setCheckState(check_state);
	check_state = simple_pipe_enable->wbg_enable ? Qt::CheckState::Checked : Qt::CheckState::Unchecked;
	wbg_checkbox->setCheckState(check_state);
	check_state = simple_pipe_enable->rgbgamma_enable ? Qt::CheckState::Checked : Qt::CheckState::Unchecked;
	rgbgamma_checkbox->setCheckState(check_state);
}
