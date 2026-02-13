#ifndef SIMPLE_PIPE_DIALOG_H
#define SIMPLE_PIPE_DIALOG_H

#include "utils.hpp"
#include <QDialog>
#include <QComboBox>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QKeyEvent>
#include <QGroupBox>

class SimplePipeDialog : public QDialog
{
	Q_OBJECT

public:
	explicit SimplePipeDialog(QWidget *parent = nullptr);
	void initUI();

private:
	void retranslateUi();

private slots:
	void handleApply();
	void handleOk();

public:
void getSimplePipeAttr(utils::BLC_ATTR_S *blc_attr, utils::WBG_ATTR_S *wbg_attr,
		utils::SIMPLE_PIPE_ENABLE_ATTR_S *simple_pipe_enable);
void setSimplePipeAttr(utils::BLC_ATTR_S *blc_attr, utils::WBG_ATTR_S *wbg_attr,
		utils::SIMPLE_PIPE_ENABLE_ATTR_S *simple_pipe_enable);

private:
void handlePipeCheckboxChanged();

signals:
	void updateSimplePipeAttr();

private:
	QVBoxLayout *main_layout;

	QGroupBox *pipe_group;
	QGroupBox *blc_group;
	QGroupBox *wbg_group;
	QGroupBox *rgbgamma_group;

	QLabel *pipe_enable_label;
	QCheckBox *pipe_checkbox;

	QLabel *blc_enable_label;
	QCheckBox *blc_checkbox;
	QLabel *blc_offset_r_label;
	QLineEdit *blc_offset_r_line_edit;
	QLabel *blc_offset_g_label;
	QLineEdit *blc_offset_g_line_edit;
	QLabel *blc_offset_b_label;
	QLineEdit *blc_offset_b_line_edit;

	QLabel *wbg_enable_label;
	QCheckBox *wbg_checkbox;
	QLabel *wbg_gain_r_label;
	QLineEdit *wbg_gain_r_line_edit;
	QLabel *wbg_gain_g_label;
	QLineEdit *wbg_gain_g_line_edit;
	QLabel *wbg_gain_b_label;
	QLineEdit *wbg_gain_b_line_edit;

	QLabel *rgbgamma_enable_label;
	QCheckBox *rgbgamma_checkbox;

	QPushButton *ok_btn;
	QPushButton *apply_btn;
};

#endif // SIMPLE_PIPE_DIALOG_H
