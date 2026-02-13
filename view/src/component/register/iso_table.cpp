#include "global_data.hpp"
#include "iso_table.hpp"
#include "utils.hpp"
#include "cv_line_edit.hpp"

#include <QLineEdit>
#include <QFile>
#include <QTextStream>

const int MAX_COLUMN_WIDTH = 100;
const int LINEEDIT_MAX_LENGTH = 10;
const int DOUBLEVALIDATOR_DECIMALS = 8;
const double DOUBLEVALIDATLR_BOTTOM = 0;
const double DOUBLEVALIDATLR_TOP = 9999999999;

void IsoTable::initLineEdits()
{
    setColumnCount(num_list.size());
    setRowCount(this->reg_section_key_list.size());

    QStringList horizontal_headers;
    for (int i = 0; i < columnCount(); i++) {
        setColumnWidth(i, MAX_COLUMN_WIDTH);
        horizontal_headers.append(header_name + QString::number(num_list[i]));
    }

    setHorizontalHeaderLabels(horizontal_headers);

    for (int i = 0; i < reg_section_key_list.size(); i++) {
        GlobalData::getInstance()->initIsoRegValue(reg_section_key_list[i].first, reg_section_key_list[i].second, columnCount(), 0);
    }

    bool readonly = (chmod == MOD_RO);
    int colCnt = columnCount();
    int min = value_min_list[0].toInt();
    int max = value_max_list[0].toInt();
    bool bMultiRange = value_min_list.size() > 1 && value_max_list.size() > 1;
    for (int i = 0; i < rowCount(); i++) {
        if (i > reg_section_key_list.size() - 1) break;
        auto reg_section_key = reg_section_key_list[i];
        for (int j = 0; j < colCnt; j++) {
            if (j > num_list.size() - 1) break;
            int iso = num_list[j];
            int idx = i * columnCount() + j;
            QString value = validateValue(getValue(reg_section_key, iso), i).toString();
            QLineEdit *lineedit = new QLineEdit;
            QLineEdit *lineedit_double = new QLineEdit;

            if (bMultiRange && i < value_min_list.size()) {
                if (value_type == VALUE_TYPE::DOUBLE)
                    lineedit_double->setValidator(new QDoubleValidator(DOUBLEVALIDATLR_BOTTOM, DOUBLEVALIDATLR_TOP, DOUBLEVALIDATOR_DECIMALS, this));
            }
            else {
                if (value_type == VALUE_TYPE::DOUBLE)
                    lineedit_double->setValidator(new QDoubleValidator(DOUBLEVALIDATLR_BOTTOM, DOUBLEVALIDATLR_TOP, DOUBLEVALIDATOR_DECIMALS, this));
            }

            if (value_type == VALUE_TYPE::INT) {
                lineedit->setFrame(false);
                lineedit->setText(value);
                lineedit->setReadOnly(readonly);
                lineedit->setMaxLength(LINEEDIT_MAX_LENGTH);
                connect(lineedit, &QLineEdit::editingFinished,
                    this, std::bind(&IsoTable::cellEdited, this, lineedit, idx));
                setCellWidget(idx / colCnt, idx % colCnt, lineedit);
            } else if (value_type == VALUE_TYPE::DOUBLE) {
                lineedit_double->setFrame(false);
                lineedit_double->setText(value);
                //lineedit_double->setReadOnly(readonly);
                lineedit_double->setMaxLength(LINEEDIT_MAX_LENGTH);
                /*connect(lineedit, &QLineEdit::editingFinished,
                    this, std::bind(&IsoTable::cellEdited, this, lineedit, idx));*/
                setCellWidget(idx / colCnt, idx % colCnt, lineedit_double);
            }
        }
    }
}

void IsoTable::cellEdited(QLineEdit *lineedit, int index)
{
    int value = lineedit->text().toInt();

    int iso_idx = index % columnCount();
    if (iso_idx > num_list.size() - 1) return;
    int iso = num_list[iso_idx];
    int reg_idx = index / columnCount();
    if (reg_idx > reg_section_key_list.size() - 1) return;

    QVariant reg_value = validateValue(value);
    /*if (this->reg_value_type == TYPE_ARRAY) {
        if (!this->value_mapping_vec.isEmpty()) {
            reg_value = getRegValueListByValueMapping(value);
        }
    }*/

    QVariant iso_matrix_value = GlobalData::getInstance()->getIsoRegValue(reg_section_key_list[reg_idx].first, reg_section_key_list[reg_idx].second, iso_idx);
    if(reg_value == iso_matrix_value) {
        lineedit->setText(reg_value.toString());
        return;
    }

    GlobalData::getInstance()->setIsoRegValueByIso(reg_section_key_list[reg_idx].first, reg_section_key_list[reg_idx].second, iso_idx, reg_value);
    emit sigValueChanged(value);
    // if auto write, trigger write data
    if (GlobalData::getInstance()->getSettings(SETTINGS_SECTION, KEY_AUTO_WRITE).toInt()) {
        emit sigPageWriteData();
    }
}

QVariant IsoTable::getValue(const SecKeyType &reg_section_key, int iso_num)
{
    QVariant value;
    QVariantList value_list;
    // check data type is array or single value
    bool is_reg_value_type_array = (this->reg_value_type == TYPE_ARRAY) ? true : false;
    int iso = num_list.indexOf(iso_num);
    if (is_reg_value_type_array) {
        value_list = GlobalData::getInstance()->getIsoRegValue(reg_section_key.first, reg_section_key.second, iso).toList();
        value = value_list;
    } else {
        value = GlobalData::getInstance()->getIsoRegValue(reg_section_key.first, reg_section_key.second, iso);
    }

    return value;
}

QVariant IsoTable::getValueByValueMapping(QVariantList &&values) const
{
    return QVariant(this->value_mapping_vec.indexOf(values));
}

QVariant IsoTable::getRegValueByValueMapping(QVariant value, SecKeyType &&reg_section_key) const
{
    // value is value table's idx
    if (value >= this->value_mapping_vec.size()) return value;

    QVariantList value_list = this->value_mapping_vec.at(value.toInt());
    int value_idx = this->reg_section_key_list.indexOf(reg_section_key);
    if (value_idx >= value_list.size()) return value;

    return value_list[value_idx];
}

QVariantList IsoTable::getRegValueListByValueMapping(QVariant value) const
{
    QVariantList value_list;
    // value is value table's idx
    if (value >= this->value_mapping_vec.size()) return value_list;
    value_list = this->value_mapping_vec.at(value.toInt());

    return value_list;
}

QVariant IsoTable::validateValue(QVariant value)
{
    if (value < this->value_min_list[0]) return this->value_min_list[0];
    if (value > this->value_max_list[0]) return this->value_max_list[0];

    return value;
}

QVariant IsoTable::validateValue(QVariant value, int row)
{
    if (row >= value_min_list.size() || row >= value_max_list.size() || row < 0)
        return validateValue(value);

    if ( value < this->value_min_list[row]) return this->value_min_list[row];
    if ( value > this->value_max_list[row]) return this->value_max_list[row];

    return value;
}

void IsoTable::setValueMappingVec(QVector<QVariantList> &&value_mapping_vec)
{
    this->value_mapping_vec = value_mapping_vec;
}

void IsoTable::setRange(int min, int max)
{
    this->value_min_list[0] = min;
    this->value_max_list[0] = max;
}

void IsoTable::setRangeList(const QVariantList &min, const QVariantList &max)
{
    value_min_list = min;
    value_max_list = max;
}

void IsoTable::setHeaderType(QString type)
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

void IsoTable::setValueType(QString type)
{
    if (type == "int") {
        this->value_type = VALUE_TYPE::INT;
    } else if (type == "double") {
        this->value_type = VALUE_TYPE::DOUBLE;
    }
}

void IsoTable::handleImportTable(QString file_path)
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
       QList<int64_t> data_list = utils::stringToList<int64_t>(data);
       if (reg_idx > reg_section_key_list.size() - 1) break;
       for (int i=0; i<data_list.size() && i <num_list.size(); i++) {
           GlobalData::getInstance()->setIsoRegValueByIso(reg_section_key_list[reg_idx].first, reg_section_key_list[reg_idx].second, i, data_list[i]);
       }

       reg_idx++;
    }
    inut_file.close();
}

void IsoTable::handleExportTable(QString file_path)
{
    QFile output_file(file_path);
    if(!output_file.open(QIODevice::WriteOnly)) {
        qDebug("Export file :%s error", qUtf8Printable(file_path));
        return;
    }

    QTextStream out(&output_file);
    for (int i = 0; i < reg_section_key_list.size(); i++) {
        int j = 0;
        for (auto iso : num_list) {
            QVariant value = getValue(reg_section_key_list[i], iso);
            out << value.toString();
            if (j < num_list.size() - 1) {
              out << ",";
            }
            j++;
        }
        out << "\n";
    }
}

void IsoTable::refreshData(const SecKeyType &reg_section_key)
{
    bool isValueChanged = false;
    int row_cnt = rowCount();
    int col_cnt = columnCount();
    if ((row_cnt <= 0) || (col_cnt <= 0)) return;

    // load json for value
    QString value;
    int idx = reg_section_key_list.indexOf(reg_section_key);
    for (int col = 0; col < col_cnt; col++) {
        if (col > (num_list.size() - 1)) break;
        int iso = num_list[col];

        value = validateValue(getValue(reg_section_key, iso), idx % row_cnt).toString();
        QLineEdit *lineedit = qobject_cast<QLineEdit *>(cellWidget(idx % row_cnt, col));
        QString lineedit_value = lineedit->text();
        if (lineedit_value != value) {
            isValueChanged = true;
            lineedit->setText(value);
        }
    }

    if (isValueChanged) {
        emit sigValueChanged(value);
    }
}

QVariant IsoTable::getData()
{
    QVariantList data;
    int row_cnt = rowCount();
    int col_cnt = columnCount();
    if (row_cnt == 1) {
        for (int col = 0; col < col_cnt; col++) {
            QLineEdit *lineedit = qobject_cast<QLineEdit *>(cellWidget(0, col));
            if (lineedit) {
                if (value_type == VALUE_TYPE::INT)
                    data.append(lineedit->text().toInt());
                else if (value_type == VALUE_TYPE::DOUBLE)
                    data.append(lineedit->text().toDouble());
            }
        }
    } else {
        for (int row = 0; row < row_cnt; row++) {
            QVariantList row_data;
            for (int col = 0; col < col_cnt; col++) {
                QLineEdit *lineedit = qobject_cast<QLineEdit *>(cellWidget(row, col));
                if (lineedit) {
                    if (value_type == VALUE_TYPE::INT)
                        row_data.append(lineedit->text().toInt());
                    else if (value_type == VALUE_TYPE::DOUBLE)
                        row_data.append(lineedit->text().toDouble());
                }
            }
            data.push_back(row_data);
        }
    }

    return data;
}

void IsoTable::setData(QVariant data)
{
    QVariantList data_array = data.toList();
    if (data_array.isEmpty()) return;

    if (!data_array.first().toList().isEmpty()) {
        // is 2d array
        for (int row = 0; (row < data_array.size()) && (row < rowCount()); row++) {
            QVariantList cow_data_list = data_array[row].toList();
            if (cow_data_list.size() == columnCount()) {
                GlobalData::getInstance()->setIsoRegValue(reg_section_key_list[row].first, reg_section_key_list[row].second, cow_data_list);
            } else if (cow_data_list.size() < columnCount()){
                // Fix LV-5 JSON Issue
                for (int col = columnCount() - 1, col_data = cow_data_list.size() - 1; col >= 0; col--) {
                    QLineEdit *lineedit = qobject_cast<QLineEdit *>(cellWidget(row, col));
                    if (lineedit) {
                        if (col_data > 0) {
                            lineedit->setText(validateValue(cow_data_list[col_data], row).toString());
                            col_data--;
                        } else {
                            lineedit->setText(validateValue(cow_data_list[0], row).toString());
                        }
                    }
                }
            }
        }
    } else {
        // is 1d array
        if (data_array.size() == columnCount()) {
            GlobalData::getInstance()->setIsoRegValue(reg_section_key_list[0].first, reg_section_key_list[0].second, data_array);
        } else if (data_array.size() < columnCount()) {
            // Fix LV-5 JSON Issue
            for (int col = columnCount() - 1, col_data = data_array.size() - 1; col >= 0; col--) {
                QLineEdit *lineedit = qobject_cast<QLineEdit *>(cellWidget(0, col));
                if (lineedit) {
                    if (col_data > 0) {
                        lineedit->setText(validateValue(data_array[col_data]).toString());
                        col_data--;
                    } else {
                        lineedit->setText(validateValue(data_array[0]).toString());
                    }
                }
            }
        }
    }
}
