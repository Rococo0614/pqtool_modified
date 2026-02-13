#include "global_data.hpp"
#include "register_chart_table.hpp"

#include <QLineEdit>
#include <QtCore>
#include "utils.hpp"

RegisterChartTable::RegisterChartTable(LineChartView *chart) :
    chart(chart)
{
    setRegSectionKeyList(chart->getSectionKeyList());
    setRowCount(1);
    setColumnCount(chart->getData(0).count());
    setVerticalHeaderLabels({""});

    initLineEdits();
}

RegisterChartTable::~RegisterChartTable()
{
    for (auto reg_section_key : reg_section_key_list) {
        GlobalData::getInstance()->unSubscribeObserver(reg_section_key, this);
    }
}

void RegisterChartTable::initLineEdits()
{
    QStringList h_labels;
    QList<QPointF> points = chart->getPoints(0);

    for (int i = 0; i < points.count(); i++) {
        QString value = QString::number(points[i].y());
        h_labels.append(QString::number(points[i].x()));
        QLineEdit *lineedit = new QLineEdit;
        int colCnt = columnCount();
        lineedit->setValidator(new QIntValidator(0, 255, this));
        lineedit->setFrame(false);
        lineedit->setText(value);
        connect(lineedit, &QLineEdit::editingFinished,
                this, std::bind(&RegisterChartTable::cellEdited, this, lineedit, i));
        setCellWidget(i / colCnt, i % colCnt, lineedit);
    }
    setHorizontalHeaderLabels(h_labels);
}

void RegisterChartTable::cellEdited(QLineEdit *lineedit, int index)
{
    int x = horizontalHeaderItem(index)->text().toInt();
    double y = lineedit->text().toDouble();
    QList<QPointF> points = chart->getPoints(0);
    points[x] = QPointF(x, y);

    chart->setPoints(0, points);

    emit sigValueChanged(lineedit->text().toInt());
}

void RegisterChartTable::refreshData(const SecKeyType&)
{
    int row_cnt = rowCount();
    int col_cnt = columnCount();
    int total_count = row_cnt * col_cnt;
    QList<QPointF> points = chart->getPoints(0);

    for (int i = 0; i < total_count; i++) {
        QLineEdit *lineedit = qobject_cast<QLineEdit *>(cellWidget(i / col_cnt, i % col_cnt));
        lineedit->setText(QString::number(points[i].y()));
    }
}

void RegisterChartTable::setDoubleValidator(double min, double max)
{
    int row_cnt = rowCount();
    int col_cnt = columnCount();
    int total_count = row_cnt * col_cnt;

    for (int i = 0; i < total_count; i++) {
        QLineEdit *lineedit = qobject_cast<QLineEdit *>(cellWidget(i / col_cnt, i % col_cnt));
        QDoubleValidator *validator = new QDoubleValidator(min, max, 4);
        validator->setNotation(QDoubleValidator::StandardNotation);
        lineedit->setValidator(validator);
    }
}
