#include "calibration_rgb_list_widget.hpp"

#include <QMenuBar>
#include <QLabel>
#include <QHeaderView>
#include <QCheckBox>
#include <QLineEdit>
#include <QPushButton>
#include <QFileDialog>
#include <QTextStream>


CalibrationRGBListWidget::CalibrationRGBListWidget(QWidget *parent) : QWidget(parent)
{
	initUI();
	retranslateUi();
}

void CalibrationRGBListWidget::initUI()
{
	main_layout = new QVBoxLayout(this);
	QHBoxLayout *rgb_layout = new QHBoxLayout;
	QPushButton *clear_btn = new QPushButton(tr("Clear RGB"));
	clear_btn->setMaximumSize(100, 35);
	connect(clear_btn, &QPushButton::clicked, [=]() {
		clearRgbTable();
	});
	QPushButton *add_btn = new QPushButton(tr("Add RGB"));
	add_btn->setMaximumSize(100, 35);
	connect(add_btn, &QPushButton::clicked, [=]() {
		addRgbItem();
	});
	QPushButton *import_btn = new QPushButton(tr("Import RGB Data"));
	import_btn->setMaximumSize(200, 35);
	connect(import_btn, &QPushButton::clicked, [=]() {
		importRgbData();
	});
	QPushButton *export_btn = new QPushButton(tr("Export RGB Data"));
	export_btn->setMaximumSize(200, 35);
	connect(export_btn, &QPushButton::clicked, [=]() {
		exportRgbData();
	});
	rgb_layout->addWidget(clear_btn, 0, Qt::AlignLeft);
	rgb_layout->addWidget(add_btn, 0, Qt::AlignLeft);
	rgb_layout->addWidget(import_btn, 0, Qt::AlignLeft);
	rgb_layout->addWidget(export_btn, 0, Qt::AlignLeft);
	rgb_layout->addStretch();
	main_layout->addLayout(rgb_layout);
	rgb_table_widget = new QTableWidget(0, 10);
	rgb_table_widget->setColumnWidth(0, 50);
	rgb_table_widget->setColumnWidth(1, 100);
	rgb_table_widget->setColumnWidth(2, 100);
	rgb_table_widget->setColumnWidth(3, 100);
	rgb_table_widget->setColumnWidth(4, 100);
	rgb_table_widget->setColumnWidth(5, 100);
	rgb_table_widget->setColumnWidth(6, 100);
	rgb_table_widget->setColumnWidth(7, 100);
	rgb_table_widget->setColumnWidth(8, 100);
	rgb_table_widget->setColumnWidth(9, 50);
	rgb_table_widget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	main_layout->addWidget(rgb_table_widget);
	rgb_table_widget->clearContents();
}

void CalibrationRGBListWidget::retranslateUi()
{
	rgb_table_widget->setHorizontalHeaderLabels({tr("Sel"), tr("OriClr"), tr("EstClr"), tr("OriR"),
					tr("OriG"), tr("OriB"), tr("EstR"), tr("EstG"), tr("EstB"), tr("Del")});
}

void CalibrationRGBListWidget::addRgbItem()
{
	RGB_PAIRS rgb;
	rgb.ori_r = 128;
	rgb.ori_g = 128;
	rgb.ori_b = 128;
	rgb.ori_valid = true;
	rgb.ori_create_idx = 0;
	rgb.est_r = 0;
	rgb.est_g = 0;
	rgb.est_b = 0;
	rgb.est_valid = true;
	rgb.est_create_idx = 0;
	RGBItem * item = new RGBItem(rgb);
	addTableItem(item);
	int row = rgb_item_list.size() - 1;
	QLineEdit *del_edit = (QLineEdit *)rgb_table_widget->cellWidget(row,9);
	del_edit->setEnabled(true);
}

bool CalibrationRGBListWidget::addTableItem(RGBItem *item)
{
	if (item == nullptr) {
		return false;
	}

	int index = rgb_item_list.size();
	if (index >= MAX_RGB_ITEM_NUM) {
		clearRgbTable();
		index = 0;
	}

	this->rgb_item_list.append(item);
	rgb_table_widget->setRowCount(rgb_item_list.size());

	QCheckBox *sel_checkbox = new QCheckBox;
    // sel_checkbox->setCheckState(item->getSelected());
	sel_checkbox->setCheckState(Qt::CheckState::Checked);
	sel_checkbox->setEnabled(false);
	rgb_table_widget->setCellWidget(index, 0, sel_checkbox);
	connect(sel_checkbox, &QCheckBox::stateChanged, [=](int state) {
		this->rgb_item_list[index]->setSelected(static_cast<Qt::CheckState>(state));
	});

	QLineEdit *oriclr_edit = new QLineEdit;
	oriclr_edit->setFrame(false);
	oriclr_edit->setReadOnly(true);
	//oriclr_edit->setValidator(new QIntValidator(0, 255));
	oriclr_edit->setStyleSheet(QString("background-color:rgb(%1, %2, %3)").arg(item->getOriR()).arg(item->getOriG()).arg(item->getOriB()));
	rgb_table_widget->setCellWidget(index, 1, oriclr_edit);

	QLineEdit *estclr_edit = new QLineEdit;
	estclr_edit->setFrame(false);
	estclr_edit->setReadOnly(true);
	//estclr_edit->setValidator(new QIntValidator(0, 255));
	estclr_edit->setStyleSheet(QString("background-color:rgb(%1, %2, %3)").arg(item->getEstR()).arg(item->getEstG()).arg(item->getEstB()));
	rgb_table_widget->setCellWidget(index, 2, estclr_edit);

	QLineEdit *ori_r_edit = new QLineEdit(QString::number(item->getOriR()));
	ori_r_edit->setFrame(false);
	//ori_r_edit->setValidator(new QIntValidator(0, 255));
	rgb_table_widget->setCellWidget(index, 3, ori_r_edit);
	connect(ori_r_edit, &QLineEdit::textChanged, [=](QString text) {
		int ori_r = text.toInt();
		if (ori_r < 0) {
			ori_r = 0;
		} else if (ori_r > 255) {
			ori_r = 255;
		}
		ori_r_edit->setText(QString::number(ori_r));
		int idx = rgb_table_widget->currentRow();
		this->rgb_item_list[idx]->setOriR(ori_r);
		setOriClr(idx);
	});

	QLineEdit *ori_g_edit = new QLineEdit(QString::number(item->getOriG()));
	ori_g_edit->setFrame(false);
	//ori_g_edit->setValidator(new QIntValidator(0, 255));
	rgb_table_widget->setCellWidget(index, 4, ori_g_edit);
	connect(ori_g_edit, &QLineEdit::textChanged, [=](QString text) {
		int ori_g = text.toInt();
		if (ori_g < 0) {
			ori_g = 0;
		} else if (ori_g > 255) {
			ori_g = 255;
		}
		ori_g_edit->setText(QString::number(ori_g));
		int idx = rgb_table_widget->currentRow();
		this->rgb_item_list[idx]->setOriG(ori_g);
		setOriClr(idx);
	});

	QLineEdit *ori_b_edit = new QLineEdit(QString::number(item->getOriB()));
	ori_b_edit->setFrame(false);
	//ori_b_edit->setValidator(new QIntValidator(0, 255));
	rgb_table_widget->setCellWidget(index, 5, ori_b_edit);
	connect(ori_b_edit, &QLineEdit::textChanged, [=](QString text) {
		int ori_b = text.toInt();
		if (ori_b < 0) {
			ori_b = 0;
		} else if (ori_b > 255) {
			ori_b = 255;
		}
		ori_b_edit->setText(QString::number(ori_b));
		int idx = rgb_table_widget->currentRow();
		this->rgb_item_list[idx]->setOriB(ori_b);
		setOriClr(idx);
	});

	QLineEdit *est_r_edit = new QLineEdit(QString::number(item->getEstR()));
	est_r_edit->setFrame(false);
	//est_r_edit->setValidator(new QIntValidator(0, 255));
	rgb_table_widget->setCellWidget(index, 6, est_r_edit);
	connect(est_r_edit, &QLineEdit::textChanged, [=](QString text) {
		int est_r = text.toInt();
		if (est_r < 0) {
			est_r = 0;
		} else if (est_r > 255) {
			est_r = 255;
		}
		est_r_edit->setText(QString::number(est_r));
		int idx = rgb_table_widget->currentRow();
		this->rgb_item_list[idx]->setEstR(est_r);
		setEstClr(idx);
	});

	QLineEdit *est_g_edit = new QLineEdit(QString::number(item->getEstG()));
	est_g_edit->setFrame(false);
	//est_g_edit->setValidator(new QIntValidator(0, 255));
	rgb_table_widget->setCellWidget(index, 7, est_g_edit);
	connect(est_g_edit, &QLineEdit::textChanged, [=](QString text) {
		int est_g = text.toInt();
		if (est_g < 0) {
			est_g = 0;
		} else if (est_g > 255) {
			est_g = 255;
		}
		est_g_edit->setText(QString::number(est_g));
		int idx = rgb_table_widget->currentRow();
		this->rgb_item_list[idx]->setEstG(est_g);
		setEstClr(idx);
	});

	QLineEdit *est_b_edit = new QLineEdit(QString::number(item->getEstB()));
	est_b_edit->setFrame(false);
	//est_b_edit->setValidator(new QIntValidator(0, 255));
	rgb_table_widget->setCellWidget(index, 8, est_b_edit);
	connect(est_b_edit, &QLineEdit::textChanged, [=](QString text) {
		int est_b = text.toInt();
		if (est_b < 0) {
			est_b = 0;
		} else if (est_b > 255) {
			est_b = 255;
		}
		est_b_edit->setText(QString::number(est_b));
		int idx = rgb_table_widget->currentRow();
		this->rgb_item_list[idx]->setEstB(est_b);
		setEstClr(idx);
	});

	QPushButton *del_btn = new QPushButton(tr("X"));
	del_btn->setMaximumWidth(30);
	del_btn->setEnabled(false);
	rgb_table_widget->setCellWidget(index, 9, del_btn);
	connect(del_btn, &QPushButton::clicked, [=]() {
		int idx = rgb_table_widget->currentRow();
		rgb_table_widget->removeRow(idx);
		rgb_item_list.removeAt(idx);
	});

	return true;
}

QList<RGBItem *> CalibrationRGBListWidget::getRGBList()
{
	return this->rgb_item_list;
}

QList<RGBItem *> CalibrationRGBListWidget::getSelectRGBList()
{
	QList<RGBItem *> list;
	for (int i = 0; i < rgb_item_list.size(); i++) {
		if (rgb_item_list[i]->getSelected() == Qt::CheckState::Checked) {
			list.append(rgb_item_list[i]);
		}
	}

	return list;
}

void CalibrationRGBListWidget::setOriClr(int index)
{
	unsigned char r, g, b;
	QLineEdit *cell_edit = qobject_cast<QLineEdit *>(rgb_table_widget->cellWidget(index, 1));
	r = rgb_item_list[index]->getOriR();
	g = rgb_item_list[index]->getOriG();
	b = rgb_item_list[index]->getOriB();
	cell_edit->setStyleSheet(QString("background-color:rgb(%1, %2, %3)").arg(r).arg(g).arg(b));

}

void CalibrationRGBListWidget::setEstClr(int index)
{
	unsigned char r, g, b;
	QLineEdit *cell_edit = qobject_cast<QLineEdit *>(rgb_table_widget->cellWidget(index, 2));
	r = rgb_item_list[index]->getEstR();
	g = rgb_item_list[index]->getEstG();
	b = rgb_item_list[index]->getEstB();
	cell_edit->setStyleSheet(QString("background-color:rgb(%1, %2, %3)").arg(r).arg(g).arg(b));
}

void CalibrationRGBListWidget::clearRgbTable()
{
	int size = rgb_item_list.size();
	for (int i = 0; i < size; i++) {
		rgb_table_widget->removeRow(0);
		rgb_item_list.removeAt(0);
	}
}

void CalibrationRGBListWidget::importRgbData()
{
	QString file_path = QFileDialog::getOpenFileName(this, tr("Open File"), "", tr("Text files (*.csv *.txt)"));
    QFile input_file(file_path);
    if(!input_file.open(QIODevice::ReadOnly)) {
        qDebug("Import file :%s error", qUtf8Printable(file_path));
        return;
    }

	// clearRgbTable();
    QTextStream in(&input_file);
	in.readLine();
    while (!in.atEnd())
    {
        QString data = in.readLine();
    	QList<QVariant> data_list = utils::stringToList<QVariant>(data);
		if (data_list.count() == 7) {
            RGB_PAIRS rgb;
			rgb.ori_r = data_list[1].toInt();
			rgb.ori_g = data_list[2].toInt();
			rgb.ori_b = data_list[3].toInt();
			rgb.ori_valid = true;
			rgb.ori_create_idx = 0;
			rgb.est_r = data_list[4].toInt();
			rgb.est_g = data_list[5].toInt();
			rgb.est_b = data_list[6].toInt();
			rgb.est_valid = true;
			rgb.est_create_idx = 0;
			RGBItem * item = new RGBItem(rgb);
			if (data_list[0].toBool())
                item->setSelected(Qt::CheckState::Checked);
			else
                item->setSelected(Qt::CheckState::Unchecked);

			addTableItem(item);
			int row = rgb_item_list.size() - 1;
			QLineEdit *del_edit = (QLineEdit *)rgb_table_widget->cellWidget(row,9);
			del_edit->setEnabled(true);
		}
    }

    input_file.close();
}

void CalibrationRGBListWidget::exportRgbData()
{
	QString file_path = QFileDialog::getSaveFileName(this, tr("Save Param File"), "", tr("CSV (*.csv);;TXT (*.txt)"));
	QFile output_file(file_path);
    if(!output_file.open(QIODevice::WriteOnly)) {
        qDebug("Export file :%s error", qUtf8Printable(file_path));
        return;
    }

    QTextStream out(&output_file);
    out << "Sel,OriR,OriG,OriB,EstR,EstG,EstB\n";
    for (int i = 0; i < rgb_item_list.count(); i++) {
        out << QString("%1,%2,%3,%4,%5,%6,%7\n")
			.arg((rgb_item_list[i]->getSelected()!=0)?"1":"0")
			.arg(rgb_item_list[i]->getOriR())
			.arg(rgb_item_list[i]->getOriG())
			.arg(rgb_item_list[i]->getOriB())
			.arg(rgb_item_list[i]->getEstR())
			.arg(rgb_item_list[i]->getEstG())
			.arg(rgb_item_list[i]->getEstB());
    }
}


