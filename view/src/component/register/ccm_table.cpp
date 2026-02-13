#include "global_data.hpp"
#include "ccm_table.hpp"
#include "utils.hpp"
#include "value_validator.hpp"

#include <QJsonArray>
#include <QLineEdit>
#include <QFile>
#include <QTextStream>

constexpr int MAX_COLUMN_WIDTH = 100;

CcmTable::~CcmTable()
{
    for (auto reg_section_key : this->reg_section_key_list) {
        GlobalData::getInstance()->unSubscribeObserver(reg_section_key, this);
    }
}

void CcmTable::initLineEdits()
{
    if (this->reg_value_type == TYPE_ARRAY) {
        initCCMLineEdits();
    }
}

QVariant CcmTable::getData()
{
    QVariantList data;
    int row_cnt = rowCount();
    int col_cnt = columnCount() - 1;//ignore sum column
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

void CcmTable::setData(QVariant data)
{
    QVariantList data_array = data.toList();
    if (data_array.isEmpty()) return;

    if (!data_array.first().toList().isEmpty()) {
        // is 2d array
        for (int row = 0; (row < data_array.size()) && (row < rowCount()); row++) {
            QVariantList cow_data_list = data_array[row].toList();
            for (int col = 0; (col < cow_data_list.size()) && (col < columnCount()); col++) {
                QLineEdit *lineedit = qobject_cast<QLineEdit *>(cellWidget(row, col));
                if (lineedit) {
                    lineedit->setText(validateValue(cow_data_list[col]).toString());
                }
            }
        }
    } else {
        // is 1d array data and 2D table display
        int col_cnt = columnCount() - 1;
        for (int i=0; i<data_array.size(); i++)
        {
            QLineEdit *lineedit = qobject_cast<QLineEdit *>(cellWidget(i / col_cnt, i % col_cnt));
            if (lineedit) {
                lineedit->setText(validateValue(data_array[i]).toString());
            }
        }
    }
}

void CcmTable::setRange(int min, int max)
{
    this->value_min = min;
    this->value_max = max;
}

void CcmTable::setFoldableColumn(int col)
{
    foldable_column = col;
}

void CcmTable::initArrayValueLineEdits()
{
}

void CcmTable::initCCMLineEdits()
{
    if (rowCount() <= 0) {
        setRowCount(ccm_vertical_headers.size());
        setVerticalHeaderLabels(ccm_vertical_headers);
    }

    if (columnCount() <= 0) {
        setColumnCount(ccm_horizon_headers.size());
        setHorizontalHeaderLabels(ccm_horizon_headers);
    }

    for (int i = 0; i < columnCount(); i++) {
        setColumnWidth(i, MAX_COLUMN_WIDTH);
    }

    bool readonly = (getChmod() == MOD_RO);
    int colCnt = columnCount();
    QList<int> sum_list;
    QVariantList value_array = GlobalData::getInstance()->getRegValue(reg_section_key_list.first().first, reg_section_key_list.first().second).toList();
    for (int i = 0; i < rowCount(); i++) {
        sum_list.append(0);
        for (int j = 0; j < colCnt - 1; j++) {
            int idx = i * (colCnt - 1) + j;
            QVariant v = value_array[idx];
            QLineEdit *lineedit = new QLineEdit;
            lineedit->setValidator(new ValueIntValidator(this->value_min, this->value_max, this));
            lineedit->setFrame(false);
            lineedit->setText(validateValue(v.toInt()).toString());
            lineedit->setReadOnly(readonly);
            sum_list[i] = sum_list[i] + validateValue(v.toInt()).toInt();
            connect(lineedit, &QLineEdit::textChanged,
                    this, std::bind(&CcmTable::cellEdited, this, lineedit, idx));
            setCellWidget(i, j, lineedit);
        }
    }

    for (int i=0; i<rowCount(); i++)
    {
        QLineEdit *lineedit = new QLineEdit;
        lineedit->setValidator(new QIntValidator(this->value_min, this->value_max, this));
        lineedit->setFrame(false);
        lineedit->setReadOnly(true);
        lineedit->setText(QString::number(sum_list[i]));
        setCellWidget(i, colCnt - 1, lineedit);
    }

    setColumnHidden(foldable_column, true);
}

void CcmTable::cellEdited(QLineEdit *lineedit, int index)
{
    int value = lineedit->text().toInt();
    int col_cnt = columnCount() -1;

    if (col_cnt < 0 || rowCount() < 0 || index > col_cnt*rowCount() - 1) return;

    if (this->reg_value_type == TYPE_ARRAY) {
        GlobalData::getInstance()->setRegValueByIdx(reg_section_key_list.first().first, reg_section_key_list.first().second, getOpType(), index, value);
    }

    int row = index / col_cnt;
    int sum = 0;
    for (int i=0; i<col_cnt; i++)
    {
        QLineEdit *cell = qobject_cast<QLineEdit *> (cellWidget(row, i));
        if (cell)
        {
            sum += cell->text().toInt();
        }
    }
    QLineEdit *cell_sum = qobject_cast<QLineEdit *> (cellWidget(row, col_cnt));
    cell_sum->setText(QVariant::fromValue(sum).toString());

    emit sigValueChanged(value);
    // if auto write, trigger write data
    if (GlobalData::getInstance()->getSettings(SETTINGS_SECTION, KEY_AUTO_WRITE).toInt()) {
        emit sigPageWriteData();
    }
}

void CcmTable::handleToggleEvent(bool table_fold_state)
{
    //qDebug("FoldableRegisterTable: table_fold_state is %d\n", table_fold_state);
    this->setColumnHidden(foldable_column, table_fold_state);
}

void CcmTable::handleImportTable(QString file_path)
{
    QFile inut_file(file_path);
    if(!inut_file.open(QIODevice::ReadOnly)) {
        qDebug("Import file :%s error", qUtf8Printable(file_path));
        return;
    }

    QString data = "";
    int idx = 0;
    int col_cnt = columnCount() - 1;
    QTextStream in(&inut_file);
    if (this->reg_value_type == TYPE_ARRAY)
    {
        while (!in.atEnd())
        {
            data = in.readLine();
            QList<QVariant> data_list = utils::stringToList<QVariant>(data);
            if (idx > rowCount() - 1) break;

            for (int j=0; j<col_cnt && j<data_list.size(); j++)
            {
                GlobalData::getInstance()->setRegValueByIdx(reg_section_key_list.first().first, reg_section_key_list.first().second,
                                                            this->getOpType(), idx*col_cnt+j, data_list[j]);
            }
            idx++;
        }
    }

    inut_file.close();
}

void CcmTable::handleExportTable(QString file_path)
{
    QFile output_file(file_path);
    if(!output_file.open(QIODevice::WriteOnly)) {
        qDebug("Export file :%s error", qUtf8Printable(file_path));
        return;
    }

    QTextStream out(&output_file);
    int col_cnt = columnCount() - 1;
    if (this->reg_value_type == TYPE_ARRAY)
    {
        QVariantList value_list = GlobalData::getInstance()->getRegValue(reg_section_key_list.first().first, reg_section_key_list.first().second).toList();
        for (int i = 0; i < rowCount(); i++)
        {
            for (int j = 0; j < col_cnt; j++)
            {
                int idx = i*col_cnt + j;
                if (idx > value_list.size()) break;
                out << value_list[idx].toInt();

                if (j < col_cnt - 1) out << ",";
            }
            out << "\n";
        }
    }

    output_file.close();
}

void CcmTable::refreshData(const SecKeyType &reg_section_key)
{
    int row_cnt = rowCount();
    int col_cnt = columnCount() - 1;
    int cell_num = row_cnt * col_cnt;

    if ((row_cnt <= 0) || (col_cnt <= 0)) return;

    if (this->reg_value_type == TYPE_ARRAY) {
        QVariantList value_array = GlobalData::getInstance()->getRegValue(reg_section_key.first, reg_section_key.second).toList();
        for (int i = 0; i < value_array.size() && i < cell_num; i++) {
            QLineEdit *lineedit = qobject_cast<QLineEdit *>(cellWidget(i / col_cnt, i % col_cnt));
            if (lineedit) {
                lineedit->setText(value_array[i].toString());
            }
        }
    }
}

QVariant CcmTable::validateValue(QVariant value)
{
    if (value < this->value_min) return this->value_min;
    if (value > this->value_max) return this->value_max;

    return value;
}
