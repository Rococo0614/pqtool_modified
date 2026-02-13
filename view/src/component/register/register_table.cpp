#include "global_data.hpp"
#include "register_table.hpp"
#include "utils.hpp"
#include "int64validator.hpp"
#include "cv_line_edit.hpp"


#include <QJsonArray>
#include <QLineEdit>
#include <QFile>
#include <QTextStream>

constexpr int MAX_COLUMN_WIDTH = 100;

RegisterTable::~RegisterTable()
{
    for (auto reg_section_key : this->reg_section_key_list) {
        GlobalData::getInstance()->unSubscribeObserver(reg_section_key, this);
    }
}

void RegisterTable::initLineEdits()
{
    if (this->reg_value_type == TYPE_ARRAY) {
        initArrayValueLineEdits();
    } else {
        initSingleValueLineEdits();
    }
}

QVariant RegisterTable::getData()
{
    QVariantList data;
    int row_cnt = rowCount();
    int col_cnt = columnCount();
    if (row_cnt == 1) {
        for (int col = 0; col < col_cnt; col++) {
            QLineEdit *lineedit = qobject_cast<QLineEdit *>(cellWidget(0, col));
            if (lineedit) {
                data.append(lineedit->text().toInt());
            }
        }
    } else {
        for (int row = 0; row < row_cnt; row++) {
            QVariantList row_data;
            for (int col = 0; col < col_cnt; col++) {
                QLineEdit *lineedit = qobject_cast<QLineEdit *>(cellWidget(row, col));
                if (lineedit) {
                    row_data.append(lineedit->text().toInt());
                }
            }
            data.push_back(row_data);
        }
    }

    return data;
}

void RegisterTable::setData(QVariant data)
{
    QVariantList data_array = data.toList();
    if (data_array.isEmpty()) return;

    if (!data_array.first().toList().isEmpty()) {
        // is 2d array
        QVariant value;
        for (int row = 0; (row < data_array.size()) && (row < rowCount()); row++) {
            QVariantList cow_data_list = data_array[row].toList();
            int reg_idx = row % rowCount();
            if (reg_idx > reg_section_key_list.size() - 1) break;
            for (int col = 0; (col < cow_data_list.size()) && (col < columnCount()); col++) {
                value = validateValue(cow_data_list[col], col);
                QLineEdit *lineedit = qobject_cast<QLineEdit *>(cellWidget(row, col));
                if (lineedit) {
                    lineedit->setText(value.toString());
                }
                GlobalData::getInstance()->setRegValueByIdx(reg_section_key_list[reg_idx].first, reg_section_key_list[reg_idx].second, col % columnCount(), value);
            }
        }
    } else {
        // is 1d array

        // Fix LV-5 JSON Issue
        if (data_array.size() < columnCount()) {
            QVariant lv0 = data_array[0];
            while (data_array.size() < columnCount())
                data_array.push_front(lv0);
        }

        for (int col = 0; (col < data_array.size()) && (col < columnCount()); col++) {
            QLineEdit *lineedit = qobject_cast<QLineEdit *>(cellWidget(0, col));
            if (lineedit) {
                data_array[col] = validateValue(data_array[col], col);
                lineedit->setText(data_array[col].toString());
            }

        }

        if (this->reg_value_type == TYPE_ARRAY) {
            GlobalData::getInstance()->setRegValue(reg_section_key_list[0].first, reg_section_key_list[0].second, data_array);
        } else {
            for (int col = 0; (col < data_array.size()) && (col < columnCount()); col++) {
                int reg_idx = col % columnCount();
                QVariant value = validateValue(data_array[col], col);
                GlobalData::getInstance()->setRegValue(reg_section_key_list[reg_idx].first, reg_section_key_list[reg_idx].second, value);
            }
        }
    }

    emit sigValueChanged(QVariant());
}

void RegisterTable::setRange(long long min, long long max)
{
    value_min_list[0] = min;
    value_max_list[0] = max;
}

void RegisterTable::setRangeList(const QVariantList &min, const QVariantList &max)
{
    value_min_list = min;
    value_max_list = max;
}

void RegisterTable::initArrayValueLineEdits()
{
    if (reg_section_key_list.isEmpty()) {
        return;
    }

    if (rowCount() <= 0) {
        setRowCount(reg_section_key_list.size());
    }
    if (columnCount() <= 0) {
        QVariant value_array = GlobalData::getInstance()->getRegValue(reg_section_key_list.first().first, reg_section_key_list.first().second);
        setColumnCount(value_array.toList().size());
    }

    for (int i = 0; i < columnCount(); i++) {
        setColumnWidth(i, MAX_COLUMN_WIDTH);
    }

    bool readonly = (chmod == MOD_RO);
    int colCnt = columnCount();
    int64_t min = value_min_list[0].toLongLong();
    int64_t max = value_max_list[0].toLongLong();
    bool bMultiRange = value_min_list.size() > 1 && value_max_list.size() > 1;
    for (int i = 0; i < rowCount(); i++) {
        QVariantList value_array = GlobalData::getInstance()->getRegValue(reg_section_key_list[i].first, reg_section_key_list[i].second).toList();
        for (int j = 0; j < colCnt; j++) {
            if (j >= value_array.size()) break;
            int idx = i * columnCount() + j;
            CvLineEdit *lineedit = new CvLineEdit;

            if (bMultiRange && j < value_min_list.size())
            {
                lineedit->setValidator(new ValueInt64Validator(value_min_list[j].toLongLong(), value_max_list[j].toLongLong(), this));
            }else {
                lineedit->setValidator(new ValueInt64Validator(min, max, this));
            }

            lineedit->setText(validateValue(value_array[j], j).toString());
            lineedit->setFrame(false);
            lineedit->setReadOnly(readonly);
            //connect(lineedit, &QLineEdit::editingFinished,
                   // this, std::bind(&RegisterTable::cellEdited, this, lineedit, idx));
            connect(lineedit, &CvLineEdit::sigLostFocus,
                    this, std::bind(&RegisterTable::cellEdited, this, lineedit, idx));

            setCellWidget(idx / colCnt, idx % colCnt, lineedit);
        }
    }
}

void RegisterTable::initSingleValueLineEdits()
{
    if (rowCount() <= 0) {
        setRowCount(1);
    }

    if (columnCount() < reg_section_key_list.size()) {
        setColumnCount(reg_section_key_list.size());
    }

    for (int i = 0; i < columnCount(); i++) {
        setColumnWidth(i, MAX_COLUMN_WIDTH);
    }

    bool readonly = (chmod == MOD_RO);
    int colCnt = columnCount();
    bool bMultiRange = this->value_min_list.size() > 1;
    for (int i = 0; i < rowCount(); i++) {
        for (int j = 0; j < colCnt; j++) {
            if (j > reg_section_key_list.size() - 1) break;
            auto reg_section_key = reg_section_key_list[j];
            int idx = i * columnCount() + j;
            QVariant v = GlobalData::getInstance()->getRegValue(reg_section_key.first, reg_section_key.second);
            QString value = validateValue(v, j).toString();
            CvLineEdit *lineedit = new CvLineEdit;
            if (bMultiRange && j < value_min_list.size()) {
                lineedit->setValidator(new QIntValidator(value_min_list[j].toInt(), value_max_list[j].toInt(), this));
            } else {
                lineedit->setValidator(new QIntValidator(value_min_list[0].toInt(), value_max_list[0].toInt(), this));
            }
            lineedit->setFrame(false);
            lineedit->setText(value);
            lineedit->setReadOnly(readonly);
            //connect(lineedit, &QLineEdit::editingFinished,
                    //this, std::bind(&RegisterTable::cellEdited, this, lineedit, idx));
            connect(lineedit, &CvLineEdit::sigLostFocus,
                    this, std::bind(&RegisterTable::cellEdited, this, lineedit, idx));

            setCellWidget(idx / colCnt, idx % colCnt, lineedit);
        }
    }
}

void RegisterTable::cellEdited(QLineEdit *lineedit, int index)
{
    int col = index % columnCount();
    QVariant value = validateValue(lineedit->text().toLongLong(), col);

    if (this->reg_value_type == TYPE_ARRAY) {
        int value_idx = index % columnCount();
        int reg_idx = index / columnCount();
        if (reg_idx > reg_section_key_list.size() - 1) return;
        GlobalData::getInstance()->setRegValueByIdx(reg_section_key_list[reg_idx].first, reg_section_key_list[reg_idx].second, value_idx, value);
    } else {
        int reg_idx = index % columnCount();
        if (reg_idx > reg_section_key_list.size() - 1) return;
        GlobalData::getInstance()->setRegValue(reg_section_key_list[reg_idx].first, reg_section_key_list[reg_idx].second, value);
    }

    emit sigValueChanged(value);
    // if auto write, trigger write data
    if (GlobalData::getInstance()->getSettings(SETTINGS_SECTION, KEY_AUTO_WRITE).toInt()) {
        emit sigPageWriteData();
    }
}

void RegisterTable::handleImportTable(QString file_path)
{
    ///TODO: VALUE_TYPE_ARRAY
    QFile inut_file(file_path);
    if(!inut_file.open(QIODevice::ReadOnly)) {
        qDebug("Import file :%s error", qUtf8Printable(file_path));
        return;
    }

    QString data = "";
    QTextStream in(&inut_file);
    while (!in.atEnd())
    {
       data += in.readLine();
    }
    inut_file.close();

    QVariantList data_list = utils::stringToVariantList(data);

    if (this->reg_value_type == TYPE_ARRAY) {
        for (int i = 0; i < reg_section_key_list.size(); i++) {
            QVariant reg_value = GlobalData::getInstance()->getRegValue(reg_section_key_list[i].first, reg_section_key_list[i].second);
            int reg_value_size = reg_value.toList().size();
            for (int j = 0; j < reg_value_size; j++) {
                data_list[j] = validateValue(data_list[j], j);
            }

            GlobalData::getInstance()->setRegValue(reg_section_key_list[i].first, reg_section_key_list[i].second, data_list);
            if (data_list.size() > reg_value_size) {
                for (int j = 0; j < reg_value_size; j++)
                    data_list.removeFirst();
            }
        }
    } else {
        if (data_list.size() != reg_section_key_list.size()) {
            qDebug("Data size mismatch: %d != %d", data_list.size(), reg_section_key_list.size());
            return;
        }
        for (int i = 0; i < reg_section_key_list.size(); i++) {
            GlobalData::getInstance()->setRegValue(reg_section_key_list[i].first, reg_section_key_list[i].second, validateValue(data_list[i]));
        }
    }
}

void RegisterTable::handleExportTable(QString file_path)
{
    QFile output_file(file_path);
    if(!output_file.open(QIODevice::WriteOnly)) {
        qDebug("Export file :%s error", qUtf8Printable(file_path));
        return;
    }

    QTextStream out(&output_file);
    for (int i = 0; i < reg_section_key_list.size(); i++) {
        QVariant value = GlobalData::getInstance()->getRegValue(reg_section_key_list[i].first, reg_section_key_list[i].second);
        if (this->reg_value_type == TYPE_ARRAY) {
            QVariantList value_list = value.toList();
            for (int j = 0; j < value_list.size(); j++) {
                out << value_list[j].toString();
                if (j < value_list.size() - 1) {
                  out << ",";
                }
            }
        } else {
            out << value.toString();
        }
        if (i < reg_section_key_list.size() - 1) {
          out << ",";
        }
    }
}

void RegisterTable::refreshData(const SecKeyType &reg_section_key)
{
    int row_cnt = rowCount();
    int col_cnt = columnCount();

    if ((row_cnt <= 0) || (col_cnt <= 0)) return;

    if (this->reg_value_type == TYPE_ARRAY) {
        int idx = reg_section_key_list.indexOf(reg_section_key);
        QVariantList value_array = GlobalData::getInstance()->getRegValue(reg_section_key.first, reg_section_key.second).toList();
        for (int col = 0; col < col_cnt && col < value_array.size(); col++) {
            QLineEdit *lineedit = qobject_cast<QLineEdit *>(cellWidget(idx % row_cnt, col));
            if (lineedit) {
                lineedit->setText(value_array[col].toString());
            }
        }
    } else {
        int idx = reg_section_key_list.indexOf(reg_section_key);
        QString value = GlobalData::getInstance()->getRegValue(reg_section_key.first, reg_section_key.second).toString();
        for (int row = 0; row < row_cnt; row++) {
            QLineEdit *lineedit = qobject_cast<QLineEdit *>(cellWidget(row, idx % col_cnt));
            if (lineedit) {
                lineedit->setText(value);
            }
        }
    }
}

QVariant RegisterTable::validateValue(QVariant value)
{
    if (value < this->value_min_list[0]) return this->value_min_list[0];
    if (value > this->value_max_list[0]) return this->value_max_list[0];

    return value;
}

QVariant RegisterTable::validateValue(QVariant value, int col_idx)
{
    //multi range mode
    if (value_min_list.size() > 1 && value_max_list.size() > 1)
    {
        if (value < this->value_min_list[col_idx]) return this->value_min_list[col_idx];
        if (value > this->value_max_list[col_idx]) return this->value_max_list[col_idx];
    }
    else {
        if (value < this->value_min_list[0]) return this->value_min_list[0];
        if (value > this->value_max_list[0]) return this->value_max_list[0];
    }

    return value;
}
