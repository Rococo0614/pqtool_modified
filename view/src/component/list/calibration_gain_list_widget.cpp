#include "calibration_gain_list_widget.hpp"

#include <QMenuBar>
#include <QLabel>
#include <QHeaderView>
#include <QCheckBox>
#include <QLineEdit>
#include <QPushButton>
#include <QMessageBox>


CalibrationGainListWidget::CalibrationGainListWidget(QWidget *parent) : QWidget(parent)
{
	initUI();
	retranslateUi();
}

void CalibrationGainListWidget::initUI()
{
	main_layout = new QVBoxLayout(this);
	gain_table_widget = new QTableWidget(0, 7);
	gain_table_widget->setColumnWidth(0, 40);
	gain_table_widget->setColumnWidth(1, 500);
	gain_table_widget->setColumnWidth(2, 40);
	gain_table_widget->setColumnWidth(3, 60);
	gain_table_widget->setColumnWidth(4, 100);
	gain_table_widget->setColumnWidth(5, 100);
	gain_table_widget->setColumnWidth(6, 100);
	main_layout->addWidget(gain_table_widget);
	gain_table_widget->clearContents();
	gain_table_widget->verticalHeader()->hide();
	gain_table_widget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

}

void CalibrationGainListWidget::retranslateUi()
{
	gain_table_widget->setHorizontalHeaderLabels({tr("Del"), tr("Tag"), tr("KI"), tr("Weight"),
					tr("In Temp.(K)"), tr("R/G"), tr("B/G")});
}

bool CalibrationGainListWidget::addTableItem(GainItem *item)
{
	if (item == nullptr) {
		return false;
	}

	int index = gain_item_list.size();
	if (index >= MAX_GAIN_ITEM_NUM) {
		QMessageBox message(QMessageBox::NoIcon, "Info", QString("Can import %1 raw at most").arg(MAX_GAIN_ITEM_NUM));
		message.exec();
		return false;
	}

	this->gain_item_list.append(item);
	gain_table_widget->setRowCount(gain_item_list.size());

	
	QPushButton *del_btn = new QPushButton(tr("X"));
	del_btn->setMaximumWidth(30);
	gain_table_widget->setCellWidget(index, 0, del_btn);
	connect(del_btn, &QPushButton::clicked, [=]() {
		int idx = gain_table_widget->currentRow();
		gain_table_widget->removeRow(idx);
		gain_item_list.removeAt(idx);
		emit sigDelPoint(idx);
	});

	QLineEdit *tag_edit = new QLineEdit;
	tag_edit->setFrame(false);
	tag_edit->setReadOnly(true);
	tag_edit->setText(item->getFileName());
	gain_table_widget->setCellWidget(index, 1, tag_edit);

	QCheckBox *ki_checkbox = new QCheckBox;
	if (item->getSelected()) {
		ki_checkbox->setCheckState(Qt::CheckState::Checked);
	} else {
		ki_checkbox->setCheckState(Qt::CheckState::Unchecked);
	}
	gain_table_widget->setCellWidget(index, 2, ki_checkbox);
	connect(ki_checkbox, &QCheckBox::stateChanged, [=](int state) {
		if (state == Qt::CheckState::Checked) {
			QList<GainItem *> list = getSelectGainList();
			if (list.size() >= 3) {
				ki_checkbox->setCheckState(Qt::CheckState::Unchecked);
				QMessageBox message(QMessageBox::NoIcon, "Info", "Only three of them can be selected");
				message.exec();
				return;
			}
		}
		int idx = gain_table_widget->currentRow();
		if (state == Qt::CheckState::Checked) {
			this->gain_item_list[idx]->setSelected(true);
		} else {
			this->gain_item_list[idx]->setSelected(false);
		}
	});

	QLineEdit *weight_edit = new QLineEdit;
	weight_edit->setFrame(false);
	weight_edit->setText(QString::number(item->getWeight()));
	gain_table_widget->setCellWidget(index, 3, weight_edit);
	connect(weight_edit, &QLineEdit::textChanged, [=](QString text) {
		int weight = text.toInt();
		if (weight < 1) {
			weight = 1;
		} else if (weight > 10) {
			weight = 10;
		}
		weight_edit->setText(QString::number(weight));
		int idx = gain_table_widget->currentRow();
		this->gain_item_list[idx]->setWeight(weight);
	});

	QLineEdit *temp_edit = new QLineEdit;
	temp_edit->setFrame(false);
	temp_edit->setText(QString::number(item->getTemp()));
	gain_table_widget->setCellWidget(index, 4, temp_edit);
	connect(temp_edit, &QLineEdit::textChanged, [=](QString text) {
		int temp = text.toInt();
		if (temp < 0) {
			temp = 0;
		} else if (temp > 30000) {
			temp = 30000;
		}
		temp_edit->setText(QString::number(temp));
		int idx = gain_table_widget->currentRow();
		this->gain_item_list[idx]->setTemp(temp);
	});

	QLineEdit *rg_edit = new QLineEdit(QString::number(item->getRg()));
	rg_edit->setFrame(false);
	rg_edit->setReadOnly(true);
	gain_table_widget->setCellWidget(index, 5, rg_edit);

	QLineEdit *bg_edit = new QLineEdit(QString::number(item->getBg()));
	bg_edit->setFrame(false);
	bg_edit->setReadOnly(true);
	gain_table_widget->setCellWidget(index, 6, bg_edit);

	return true;
}

QList<GainItem *> CalibrationGainListWidget::getGainList()
{
	return this->gain_item_list;
}

QList<GainItem *> CalibrationGainListWidget::getSelectGainList()
{
	QList<GainItem *> list;
	for (int i = 0; i < gain_item_list.size(); i++) {
		if (gain_item_list[i]->getSelected()) {
			list.append(gain_item_list[i]);
		}
	}

	return list;
}

void CalibrationGainListWidget::clearTable()
{
	gain_table_widget->clearContents();
	gain_item_list.clear();
}


