#include "global_data.hpp"
#include "iso_curve.hpp"
#include "utils.hpp"

#include <QLineEdit>
#include <QFile>
#include <QTextStream>
#include <QIntValidator>

const int MAX_COLUMN_WIDTH = 100;


IsoCurve::IsoCurve()
{
    main_layout = new QVBoxLayout(this);
    this->setLayout(main_layout);
}

QVariant IsoCurve::getValue(const SecKeyType &reg_section_key, int iso)
{
    QVariant value = 0;
    // check data type is array or single value
    //bool is_reg_value_type_array = (this->reg_value_type == TYPE_ARRAY) ? true : false;

    // handle value array or single value
    value = GlobalData::getInstance()->getIsoRegValue(reg_section_key.first, reg_section_key.second, iso);


    return value;
}

QVariant IsoCurve::setValue(const SecKeyType &reg_section_key, int iso, QVariant value)
{
    //bool is_reg_value_type_array = (this->reg_value_type == TYPE_ARRAY) ? true : false;

    // handle value array or single value
    GlobalData::getInstance()->setIsoRegValueByIso(reg_section_key.first, reg_section_key.second, this->op_type, iso, value);
}

QVariant IsoCurve::validateValue(QVariant value)
{
    if (value < this->value_min_list[0]) return this->value_min_list[0];
    if (value > this->value_max_list[0]) return this->value_max_list[0];

    return value;
}

QVariant IsoCurve::validateValue(QVariant value, int row)
{
    if (row >= value_min_list.size() || row >= value_max_list.size() || row < 0)
        return validateValue(value);

    if ( value < this->value_min_list[row]) return this->value_min_list[row];
    if ( value > this->value_max_list[row]) return this->value_max_list[row];

    return value;
}

void IsoCurve::setRange(int min, int max)
{
    this->value_min_list[0] = min;
    this->value_max_list[0] = max;
}

void IsoCurve::setRangeList(const QVariantList &min, const QVariantList &max)
{
    value_min_list = min;
    value_max_list = max;
}

void IsoCurve::setEditLabels(QVariantList list)
{
    if (list.isEmpty()) return;

    foreach (QVariant a, list) {
        QStringList str_list = a.toStringList();
        if (!str_list.isEmpty()) {
            this->str_list.append(str_list);
        }
    }
}

void IsoCurve::handleImportTable(QString file_path)
{
    QFile inut_file(file_path);
    if(!inut_file.open(QIODevice::ReadOnly)) {
        qDebug("Import file :%s error", qUtf8Printable(file_path));
        return;
    }

    QString data = "";
    int reg_idx = 0;
    QTextStream in(&inut_file);
    while (!in.atEnd())
    {
       data = in.readLine();
       QList<QVariant> data_list = utils::stringToList<QVariant>(data);
       if (reg_idx > reg_section_key_list.size() - 1) break;

       GlobalData::getInstance()->setIsoRegValue(reg_section_key_list[reg_idx].first, reg_section_key_list[reg_idx].second, data_list);
       reg_idx++;
    }
    inut_file.close();
}

void IsoCurve::handleExportTable(QString file_path)
{
    QFile output_file(file_path);
    if(!output_file.open(QIODevice::WriteOnly)) {
        qDebug("Export file :%s error", qUtf8Printable(file_path));
        return;
    }

    QTextStream out(&output_file);
    for (int i = 0; i < reg_section_key_list.size(); i++) {
        for (auto iso : num_list) {
            QVariant value = getValue(reg_section_key_list[i], iso);
            out << value.toString();
            if (i < num_list.size() - 1) {
              out << ",";
            }
        }
        out << "\n";
    }
}

void IsoCurve::refreshData(const SecKeyType &reg_section_key)
{
    int idx = reg_section_key_list.indexOf(reg_section_key);
    int iso = cur_iso;
    if (iso >=0 && iso < reg_section_key_list.size()) {
        QString value = validateValue(getValue(reg_section_key, iso), idx).toString();
        if (idx >=0 && idx < edit_list.size())
            edit_list[idx]->setText(value);
    }
}

QVariant IsoCurve::getData()
{
    QVariantList data;
    return data;
}

void IsoCurve::setData(QVariant data)
{
    QVariantList value_list = data.toList();
    if (value_list.isEmpty() || interp_num*2 != value_list.size())
        return;
}

void IsoCurve::setInterpNum(int interp_num)
{
    this->interp_num = interp_num;
}

void IsoCurve::setHeaderType(QString type)
{
    if (type == "ISO") {
        this->horizontal_header_type = HORIZONTAL_HEADER_TYPE::ISO;
        header_name = NAME_ISO;
        num_list.clear();
        for(int i=0; i<iso_list.size(); i++) {
            num_list.append(iso_list[i]);
        }
    } else if (type == "LV") {
        this->horizontal_header_type = HORIZONTAL_HEADER_TYPE::LV;
        header_name = NAME_LV;
        num_list.clear();
        for(int i=0; i<lv_list.size(); i++) {
            num_list.append(lv_list[i]);
        }
    }
}

void IsoCurve::setCurveType(CURVE_TYPE curve_type)
{
    this->curve_type = curve_type;
}

void IsoCurve::init_chart()
{
    iso_chart = new ISOChartView(interp_num, this);
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

    iso_chart->initISO(x_max, y_max);
    iso_chart->setCurveType(this->curve_type);
    connect(iso_chart, &ISOChartView::sigPointsUpdate, this, &IsoCurve::handlePointsUpdate);

    connect(iso_chart, &ISOChartView::sigPageWriteData, this, &IsoCurve::handlePageWriteData);
    connect(iso_chart, &ISOChartView::sigValueChanged, this, &IsoCurve::handleValueChanged);

    main_layout->addWidget(iso_chart);
}

void IsoCurve::init_editPanel()
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
            QString value = validateValue(getValue(reg_section_key, cur_iso), key_idx).toString();
            if (isMultiRange && idx < value_min_list.size()) {
                edit->setValidator(new QIntValidator(value_min_list[idx].toInt(), value_max_list[idx].toInt(), this));
            }else {
                edit->setValidator(new QIntValidator(min, max, this));
            }
            idx++;
            edit->setText(value);
            edit->setMaximumWidth(150);
            connect(edit, &QLineEdit::textChanged, this, std::bind(&IsoCurve::cellEdited, this, edit, key_idx));
            edit_list.append(edit);
            QLabel *label = new QLabel(str);
            grid->addWidget(label, row, col++, Qt::AlignLeft);
            grid->addWidget(edit, row, col++, Qt::AlignLeft);
        }
        row++;
    }

    QLabel *iso_label;
    if (horizontal_header_type == HORIZONTAL_HEADER_TYPE::ISO)
        iso_label = new QLabel(tr("Current ISO"));
    else if (horizontal_header_type == HORIZONTAL_HEADER_TYPE::LV)
        iso_label = new QLabel(tr("Current LV"));

    iso_box = new QComboBox;
    iso_box->setMaximumWidth(150);
    for(int i=0; i<num_list.size(); i++) {
        iso_box->addItem(header_name + QString::number(num_list[i]));
    }
    grid->addWidget(iso_label, row, 0, Qt::AlignLeft);
    grid->addWidget(iso_box, row, 1, Qt::AlignLeft);
    connect(iso_box, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &IsoCurve::handleIsoChanged);
    handleUpdateChart();
    main_layout->addLayout(grid);
}

void IsoCurve::cellEdited(QLineEdit *edit, int idx)
{
    //update global data
    SecKeyType sec_key = reg_section_key_list[idx];
    //QVariant value(edit->text().toInt());
    setValue(sec_key, this->cur_iso, validateValue(edit->text().toInt(), idx).toInt());

    //update chart
    handleUpdateChart();
}

void IsoCurve::handleUpdateChart()
{
    QList<QPointF> ps;
    int size = reg_section_key_list.size();
    for (int i=0, j=interp_num; i<interp_num && j<size; i++, j++) {
        QVariant vx = getValue(reg_section_key_list[i], cur_iso);
        QVariant vy = getValue(reg_section_key_list[j], cur_iso);
        ps.append(QPointF(validateValue(vx.toInt(), i).toInt(), validateValue(vy.toInt(), j).toInt()));
    }

    iso_chart->updateChartPoints(ps);
}

void IsoCurve::handleIsoChanged(int iso)
{
    cur_iso = iso;
    for (int i=0; i<edit_list.size(); i++) {
        SecKeyType key = reg_section_key_list[i];
        QVariant v = getValue(key, iso);
        edit_list[i]->setText(validateValue(v, i).toString());
    }

    //handleUpdateChart();
}

void IsoCurve::handlePointsUpdate(QList<QPointF> ps)
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

void IsoCurve::handlePageWriteData()
{
    emit sigPageWriteData();
}

void IsoCurve::handleValueChanged(QVariant value)
{
    emit sigValueChanged(value);
}

void IsoCurve::handleUpdateTheme(bool dark_theme_enabled)
{
    iso_chart->handleUpdateTheme(dark_theme_enabled);
}
