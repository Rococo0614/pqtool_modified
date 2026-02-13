#ifndef CALIBRATION_RGB_LIST_WIDGET_H
#define CALIBRATION_RGB_LIST_WIDGET_H

#include "utils.hpp"
#include "calibration_utils.hpp"

#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QTableWidget>

typedef struct {
	unsigned char ori_r;
	unsigned char ori_g;
	unsigned char ori_b;
	bool ori_valid;
	int ori_create_idx;
	unsigned char est_r;
	unsigned char est_g;
	unsigned char est_b;
	bool est_valid;
	int est_create_idx;
} RGB_PAIRS;

class RGBItem : public QObject
{
	Q_OBJECT

public:
	RGBItem() {}
	virtual ~RGBItem() {}

	RGBItem(RGB_PAIRS rgb) {
		this->ori_r = rgb.ori_r;
		this->ori_g = rgb.ori_g;
		this->ori_b = rgb.ori_b;
		this->ori_valid = rgb.ori_valid;
		this->ori_create_idx = rgb.ori_create_idx;
		this->est_r = rgb.est_r;
		this->est_g = rgb.est_g;
		this->est_b = rgb.est_b;
		this->est_valid = rgb.est_valid;
		this->est_create_idx = rgb.est_create_idx;
	}

	int getOriR() {
		return this->ori_r;
	}

	void setOriR(int ori_r) {
		this->ori_r = ori_r;
	}

	int getOriG() {
		return this->ori_g;
	}

	void setOriG(int ori_g) {
		this->ori_g = ori_g;
	}

	int getOriB() {
		return this->ori_b;
	}

	void setOriB(int ori_b) {
		this->ori_b = ori_b;
	}

	bool getOriValid() {
		return this->ori_valid;
	}

	int getOriCreateIdx() {
		return this->ori_create_idx;
	}

	int getEstR() {
		return this->est_r;
	}

	void setEstR(int est_r) {
		this->est_r = est_r;
	}

	int getEstG() {
		return this->est_g;
	}

	void setEstG(int est_g) {
		this->est_g = est_g;
	}

	int getEstB() {
		return this->est_b;
	}

	void setEstB(int est_b) {
		this->est_b = est_b;
	}

	bool getEstValid() {
		return this->est_valid;
	}

	int getEstCreateIdx() {
		return this->est_create_idx;
	}

	Qt::CheckState getSelected() {
		return this->select;
	}

	void setSelected(Qt::CheckState state) {
		if (state != this->select) {
			this->select = state;
		}
	}

private:
	unsigned char ori_r;
	unsigned char ori_g;
	unsigned char ori_b;
	bool ori_valid;
	int ori_create_idx;
	unsigned char est_r;
	unsigned char est_g;
	unsigned char est_b;
	bool est_valid;
	int est_create_idx;
	Qt::CheckState select;
};

class CalibrationRGBListWidget : public QWidget
{
	Q_OBJECT
public:
	explicit CalibrationRGBListWidget(QWidget *parent = nullptr);
	bool addTableItem(RGBItem *item);
	QList<RGBItem *> getRGBList();
	QList<RGBItem *> getSelectRGBList();
	void setOriClr(int index);
	void setEstClr(int index);
	void clearRgbTable();
	void addRgbItem();
	void importRgbData();
	void exportRgbData();

private:
	void initUI();
	void retranslateUi();

public:
	static const int MAX_RGB_ITEM_NUM = 1024;

signals:
	void sigSetInputPixmap(QPixmap *pixmap);
	void sigSetOutputPixmap(QPixmap *pixmap);

private:
	QVBoxLayout *main_layout;
	QTableWidget *rgb_table_widget;
	QList<RGBItem *> rgb_item_list;
};

#endif
