#include "global_data.hpp"
#include "manual_curve.hpp"
#include "utils.hpp"

#include <QLineEdit>
#include <QFile>
#include <QTextStream>
#include <QIntValidator>

ManualCurve::ManualCurve()
{
    main_layout = new QVBoxLayout(this);
    this->setLayout(main_layout);
}

QVariant ManualCurve::getValue(const SecKeyType &reg_section_key)
{
    QVariant value = 0;
    value = GlobalData::getInstance()->getRegValue(reg_section_key.first, reg_section_key.second);
    return value;
}

void ManualCurve::setValue(const SecKeyType &reg_section_key, QVariant value)
{
    GlobalData::getInstance()->setRegValue(reg_section_key.first, reg_section_key.second, value);
}

QVariant ManualCurve::validateValue(QVariant value)
{
    if (value < this->value_min_list[0]) return this->value_min_list[0];
    if (value > this->value_max_list[0]) return this->value_max_list[0];

    return value;
}

QVariant ManualCurve::validateValue(QVariant value, int row)
{
    if (row >= value_min_list.size() || row >= value_max_list.size() || row < 0)
        return validateValue(value);

    if ( value < this->value_min_list[row]) return this->value_min_list[row];
    if ( value > this->value_max_list[row]) return this->value_max_list[row];

    return value;
}

void ManualCurve::setRange(int min, int max)
{
    this->value_min_list[0] = min;
    this->value_max_list[0] = max;
}

void ManualCurve::setRangeList(const QVariantList &min, const QVariantList &max)
{
    value_min_list = min;
    value_max_list = max;
}

void ManualCurve::setEditLabels(QVariantList list)
{
    if (list.isEmpty()) return;

    foreach (QVariant a, list) {
        QStringList str_list = a.toStringList();
        if (!str_list.isEmpty()) {
            this->str_list.append(str_list);
        }
    }
}

void ManualCurve::handleImportTable(QString file_path)
{
    QFile inut_file(file_path);
    if(!inut_file.open(QIODevice::ReadOnly)) {
        qDebug("Import file :%s error", qUtf8Printable(file_path));
        return;
    }

    QString data = "";
    int row_idx = 0;
    QTextStream in(&inut_file);
    while (!in.atEnd())
    {
       data = in.readLine();
       QList<QVariant> data_list = utils::stringToList<QVariant>(data);
       if (row_idx > 1) break;

       for (int reg_idx=row_idx*interp_num, i = 0; i < data_list.size() && reg_idx < reg_section_key_list.size();reg_idx++, i++) {
           GlobalData::getInstance()->setRegValue(reg_section_key_list[reg_idx].first, reg_section_key_list[reg_idx].second, data_list[i].toInt());
       }
       row_idx++;
    }
    inut_file.close();
}

void ManualCurve::handleExportTable(QString file_path)
{
    QFile output_file(file_path);
    if(!output_file.open(QIODevice::WriteOnly)) {
        qDebug("Export file :%s error", qUtf8Printable(file_path));
        return;
    }

    QTextStream out(&output_file);
    for (int i = 0; i < reg_section_key_list.size(); i++) {
            QVariant value = getValue(reg_section_key_list[i]);
            out << value.toString();
            if (i == interp_num - 1) {
                out << "\n";
            }else if (i < reg_section_key_list.size() - 1) {
              out << ",";
            }
    }
}

void ManualCurve::refreshData(const SecKeyType &reg_section_key)
{
    int idx = reg_section_key_list.indexOf(reg_section_key);

    QString value = validateValue(getValue(reg_section_key), idx).toString();
    if (idx >=0 && idx < edit_list.size())
        edit_list[idx]->setText(value);

}

QVariant ManualCurve::getData()
{
    QVariantList data;

    for (int i = 0; i < reg_section_key_list.size(); i++) {
        data.append(validateValue(edit_list[i]->text().toInt(), i).toInt());
    }

    return data;
}

void ManualCurve::setData(QVariant data)
{
    QVariantList value_list = data.toList();
    if (value_list.isEmpty() || interp_num*2 != value_list.size())
        return;

    for (int i = 0; i < reg_section_key_list.size(); i++) {
        edit_list[i]->setText(validateValue(value_list[i].toInt(), i).toString());
    }
}

void ManualCurve::setInterpNum(int interp_num)
{
    this->interp_num = interp_num;
}

void ManualCurve::init_chart()
{
    chart = new ISOChartView(interp_num, this);
    int x_max = 0;
    bool multi_range = value_max_list.size() > 1;
    for(int i=0; i < interp_num; i++) {
        int v = value_max_list[0].toInt();
        if (multi_range && i < value_max_list.size())
            v = value_max_list[i].toInt();
        x_max = (v > x_max? v:x_max);
    }
    int y_max = 0;
    for(int i=interp_num; i < reg_section_key_list.size(); i++) {
        int v = value_max_list[0].toInt();
        if (multi_range && i < value_max_list.size())
            v = value_max_list[i].toInt();
        y_max = (v > y_max? v: y_max);
    }

    chart->initManual(x_max, y_max);
    connect(chart, &ISOChartView::sigPointsUpdate, this, &ManualCurve::handlePointsUpdate);
    connect(chart, &ISOChartView::sigValueChanged, this, &ManualCurve::handleValueChanged);
    connect(chart, &ISOChartView::sigPageWriteData, this, &ManualCurve::handlePageWriteData);

    main_layout->addWidget(chart);
}

void ManualCurve::init_editPanel()
{
    QGridLayout *grid = new QGridLayout;
    bool isMultiRange = value_min_list.size() > 1;
    int min = value_min_list[0].toInt();
    int max = value_max_list[0].toInt();
    int row = 0;
    foreach(QStringList e, str_list) {
        int col = 0;
        int idx = 0;
        foreach(QString str, e) {
            int key_idx = e.size() * row + idx;
            auto reg_section_key = reg_section_key_list[key_idx];
            QLineEdit *edit = new QLineEdit();
            QString value = validateValue(getValue(reg_section_key), key_idx).toString();
            if (isMultiRange && idx < value_min_list.size()) {
                edit->setValidator(new QIntValidator(value_min_list[idx].toInt(), value_max_list[idx].toInt(), this));
            }else {
                edit->setValidator(new QIntValidator(min, max, this));
            }
            idx++;
            edit->setText(value);
            edit->setMaximumWidth(150);
            connect(edit, &QLineEdit::textChanged, this, std::bind(&ManualCurve::cellEdited, this, edit, key_idx));
            edit_list.append(edit);
            QLabel *label = new QLabel(str);
            grid->addWidget(label, row, col++, Qt::AlignLeft);
            grid->addWidget(edit, row, col++, Qt::AlignLeft);
        }
        row++;
    }

    handleUpdateChart();
    main_layout->addLayout(grid);
}

void ManualCurve::cellEdited(QLineEdit *edit, int idx)
{
    //update global data
    SecKeyType sec_key = reg_section_key_list[idx];
    QVariant value(edit->text().toInt());
    setValue(sec_key, value);

    emit sigValueChanged(value);
    // if auto write, trigger write data
    if (GlobalData::getInstance()->getSettings(SETTINGS_SECTION, KEY_AUTO_WRITE).toInt()) {
        emit sigPageWriteData();
    }
    //update chart
    handleUpdateChart();
}

void ManualCurve::handleUpdateChart()
{
    QList<QPointF> ps;
    int size = reg_section_key_list.size();
    for (int i=0, j=interp_num; i<interp_num && j<size; i++, j++) {
        QVariant vx = getValue(reg_section_key_list[i]);
        QVariant vy = getValue(reg_section_key_list[j]);
        ps.append(QPointF(vx.toInt(), vy.toInt()));
    }

    chart->updateChartPoints(ps);
}

void ManualCurve::handlePointsUpdate(QList<QPointF> ps)
{
    ps.removeFirst();
    ps.removeLast();
    for (int i=0, j=interp_num; i < interp_num && j < reg_section_key_list.size(); i++, j++) {
        QVariant x = static_cast<int>(ps[i].x());
        QVariant y = static_cast<int>(ps[i].y());
        edit_list[i]->setText(x.toString());
        edit_list[j]->setText(y.toString());
    }
}

void ManualCurve::handleValueChanged(QVariant value)
{
    emit sigValueChanged(value);
}

void ManualCurve::handlePageWriteData()
{
    emit sigPageWriteData();
}

void ManualCurve::handleUpdateTheme(bool dark_theme_enabled)
{
    chart->handleUpdateTheme(dark_theme_enabled);
}
