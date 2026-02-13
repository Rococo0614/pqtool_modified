#include "global_data.hpp"
#include "universal_curve.hpp"
#include "utils.hpp"

#include <QLineEdit>
#include <QFile>
#include <QTextStream>
#include <QIntValidator>

const int MAX_COLUMN_WIDTH = 100;


UniversalCurve::UniversalCurve()
{
    main_layout = new QVBoxLayout(this);
    this->setLayout(main_layout);
}

QVariant UniversalCurve::validateValue(QVariant value, int index)
{
    if ( value < this->min_list[index]) return this->min_list[index];
    if ( value > this->max_list[index]) return this->max_list[index];

    return value;
}

void UniversalCurve::setRange(QJsonValue min, QJsonValue max)
{
    if (min.isDouble() && max.isDouble()) {
        this->min_list[0] = min.toVariant();
        this->max_list[0] = max.toVariant();
    } else if(min.isArray() && max.isArray()) {
        QJsonArray min_array = min.toArray();
        QJsonArray max_array = max.toArray();
        if (min_array.count() == max_array.count()) {
            this->min_list = min_array.toVariantList();
            this->max_list = max_array.toVariantList();
            this->interp_num = min_array.count() / 2;
        } else {
             qDebug("[Error] UniversalTable::setRange. Counts are not equal.");
        }
    } else {
        qDebug("[Error] UniversalTable::setRange. Undefined data type.");
    }
}

void UniversalCurve::handleImportTable(QString file_path)
{
    // Todo: need to verify
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

void UniversalCurve::handleExportTable(QString file_path)
{
    // Todo: need to verify
    QFile output_file(file_path);
    if(!output_file.open(QIODevice::WriteOnly)) {
        qDebug("Export file :%s error", qUtf8Printable(file_path));
        return;
    }

    QTextStream out(&output_file);
    for (int i = 0; i < reg_section_key_list.size(); i++) {
        int n = item_list.count() - 1;
        for (int j = 0; j <= n; j++) {
            QVariant value = GlobalData::getInstance()->getIsoRegValue(reg_section_key_list[i].first, reg_section_key_list[i].second, j);
            out << value.toString();
            if (j < n) out << ",";
        }

         out << "\n";
    }

    output_file.close();
}

void UniversalCurve::refreshData(const SecKeyType &reg_section_key)
{
    int key_index = reg_section_key_list.indexOf(reg_section_key);
    QJsonArray value_array = GlobalData::getInstance()->getUnivMatrixRegValue(
        reg_section_key.first, reg_section_key.second).toArray();
    if (reg_section_key_list.count() != 2) {
        QString value = validateValue(value_array[item_index].toInt(), key_index).toString();
        edit_list[key_index]->setText(value);
    } else {
        for (int i = key_index; i < name_list.count(); i+=2) {
            int value_index = (i / 2) * item_list.count() + item_index;
            QString value = validateValue(value_array[value_index].toInt(), i).toString();
            edit_list[i]->setText(value);
        }
    }
    
    handleUpdateChart();
}

void UniversalCurve::setCurveType(CURVE_TYPE curve_type)
{
    this->curve_type = curve_type;
}

void UniversalCurve::init_chart()
{
    int x_max = 0, y_max = 0;

    for (int i = 0; i < max_list.count(); i+=2) {
        if (max_list[i].toInt() > x_max)
            x_max = max_list[i].toInt();

        if (max_list[i+1].toInt() > y_max)
            y_max = max_list[i+1].toInt();
    }

    iso_chart = new ISOChartView(interp_num, this);
    iso_chart->initISO(x_max, y_max);
    iso_chart->setCurveType(this->curve_type);
    connect(iso_chart, &ISOChartView::sigPointsUpdate, this, &UniversalCurve::handlePointsUpdate);
    connect(iso_chart, &ISOChartView::sigPageWriteData, this, &UniversalCurve::handlePageWriteData);
    connect(iso_chart, &ISOChartView::sigValueChanged, this, &UniversalCurve::handleValueChanged);

    main_layout->addWidget(iso_chart);
}

void UniversalCurve::init_editPanel()
{
    QGridLayout *grid = new QGridLayout;

    for (int i = 0; i < name_list.count(); i+=2) {
        QLabel *x_label = new QLabel(name_list[i]);
        QLineEdit *x_edit = new QLineEdit();
        QString x_default = validateValue(0, i).toString();
        x_edit->setValidator(new QIntValidator(min_list[i].toInt(), max_list[i].toInt(), this));
        x_edit->setText(x_default);
        x_edit->setMaximumWidth(80);
        connect(x_edit, &QLineEdit::textEdited, this, std::bind(&UniversalCurve::cellEdited, this, x_edit, i));
        connect(x_edit, &QLineEdit::returnPressed, this, &UniversalCurve::handlePageWriteData);
        edit_list.append(x_edit);
        grid->addWidget(x_label, 0, i, Qt::AlignLeft);
        grid->addWidget(x_edit, 0, i+1, Qt::AlignLeft);

        QLabel *y_label = new QLabel(name_list[i+1]);
        QLineEdit *y_edit = new QLineEdit();
        QString y_default = validateValue(0, i+1).toString();
        y_edit->setValidator(new QIntValidator(min_list[i+1].toInt(), max_list[i+1].toInt(), this));
        y_edit->setText(y_default);
        y_edit->setMaximumWidth(80);
        connect(y_edit, &QLineEdit::textEdited, this, std::bind(&UniversalCurve::cellEdited, this, y_edit, i+1));
        connect(y_edit, &QLineEdit::returnPressed, this, &UniversalCurve::handlePageWriteData);
        edit_list.append(y_edit);
        grid->addWidget(y_label, 1, i, Qt::AlignLeft);
        grid->addWidget(y_edit, 1, i+1, Qt::AlignLeft);
    }

    QLabel *item_label = new QLabel("Item:");
    QComboBox *curve_combobox = new QComboBox;
    curve_combobox->setMaximumWidth(120);
    curve_combobox->addItems(item_list);
    grid->addWidget(item_label, 2, 0, Qt::AlignLeft);
    grid->addWidget(curve_combobox, 2, 1, Qt::AlignLeft);
    connect(curve_combobox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &UniversalCurve::handleItemChanged);
    handleUpdateChart();
    main_layout->addLayout(grid);
}

void UniversalCurve::cellEdited(QLineEdit *edit, int index)
{
    int key_index = 0, value_index = 0;
    if (reg_section_key_list.count() != 2) {
        key_index = index;
        value_index = item_index;
    } else {
        key_index = index % 2;
        value_index = (index / 2) * item_list.count() + item_index;
    }

    SecKeyType sec_key = reg_section_key_list[key_index];
    QVariant value = validateValue(edit->text().toInt(), index);
    GlobalData::getInstance()->setIsoRegValueByIso(sec_key.first, sec_key.second, value_index, value);

    handleUpdateChart();
}

void UniversalCurve::handleUpdateChart()
{
    QList<QPointF> ps;

    for (int i = 0, j = 1; i < edit_list.size(); i+=2, j+=2) {
        int x_key_index = i % reg_section_key_list.count();
        int x_value_index = (i / reg_section_key_list.count()) * item_list.count() + item_index;
        int y_key_index = j % reg_section_key_list.count();
        int y_value_index = (j / reg_section_key_list.count()) * item_list.count() + item_index;
        QVariant vx = GlobalData::getInstance()->getIsoRegValue(reg_section_key_list[x_key_index].first, reg_section_key_list[x_key_index].second, x_value_index);
        QVariant vy = GlobalData::getInstance()->getIsoRegValue(reg_section_key_list[y_key_index].first, reg_section_key_list[y_key_index].second, y_value_index);
        ps.append(QPointF(validateValue(vx.toInt(), i).toInt(), validateValue(vy.toInt(), j).toInt()));
    }

    iso_chart->updateChartPoints(ps);
}

void UniversalCurve::handleItemChanged(int index)
{
    item_index = index;
    for (int i = 0; i < edit_list.size(); i++) {
        int key_index = i % reg_section_key_list.count();
        int value_index = (i / reg_section_key_list.count()) * item_list.count() + item_index;
        QVariant value = GlobalData::getInstance()->getIsoRegValue(reg_section_key_list[key_index].first, reg_section_key_list[key_index].second, value_index);
        edit_list[i]->setText(validateValue(value, i).toString());
    }

    handleUpdateChart();
}

void UniversalCurve::handlePointsUpdate(QList<QPointF> ps)
{
    ps.removeFirst();
    ps.removeLast();
    for (int i = 0, j = 1, n = 0; i < edit_list.count(); i+=2, j+=2, n++) {
        int x_key_index = i % reg_section_key_list.count();
        int x_value_index = (i / reg_section_key_list.count()) * item_list.count() + item_index;
        int y_key_index = j % reg_section_key_list.count();
        int y_value_index = (j / reg_section_key_list.count()) * item_list.count() + item_index;
        QVariant x = static_cast<int>(ps[n].x());
        QVariant y = static_cast<int>(ps[n].y());
        edit_list[i]->setText(x.toString());
        edit_list[j]->setText(y.toString());
        GlobalData::getInstance()->setIsoRegValueByIso(reg_section_key_list[x_key_index].first, reg_section_key_list[x_key_index].second, x_value_index, x);
        GlobalData::getInstance()->setIsoRegValueByIso(reg_section_key_list[y_key_index].first, reg_section_key_list[y_key_index].second, y_value_index, y);
    }
}

void UniversalCurve::handlePageWriteData()
{
    emit sigPageWriteData();
}

void UniversalCurve::handleValueChanged(QVariant value)
{
    emit sigValueChanged(value);
}

void UniversalCurve::handleUpdateTheme(bool dark_theme_enabled)
{
    iso_chart->handleUpdateTheme(dark_theme_enabled);
}
