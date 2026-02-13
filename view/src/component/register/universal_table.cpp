#include "global_data.hpp"
#include "universal_table.hpp"
#include "utils.hpp"
#include "cv_line_edit.hpp"

#include <QLineEdit>
#include <QFile>
#include <QTextStream>

const int LINEEDIT_MAX_LENGTH = 12;

void UniversalTable::initLineEdits()
{
    bool readonly = (chmod == MOD_RO);
    int size = 0;
    int row = rowCount();
    int column = columnCount();

    if (reg_section_key_list[0].first == "CCM") {
        is_ccm_sum_member = utils::isMatchCCMSumTitleWithPrefix(reg_section_key_list[0].second);
        if (this->is_ccm_sum_member) {
            column -= 1;
        }
    }

    size = row * column / reg_section_key_list.count();
    QVector<int> data(size);
    QJsonArray data_array;
    std::copy(data.begin(), data.end(), std::back_inserter(data_array));
    for (int i = 0; i < reg_section_key_list.count(); i++) {
        GlobalData::getInstance()->initUinvMatrixRegValue(reg_section_key_list[i].first, reg_section_key_list[i].second, data_array);
    }

    bool is_invalid;

    for (int i = 0, index = 0; i < row; i++) {
        for (int j = 0; j < column; j++, index++) {
            QString value_str = validateValue(0, index, is_invalid).toString();
            QLineEdit *lineedit = new QLineEdit;
            lineedit->setFrame(false);
            lineedit->setText(value_str);
            lineedit->setReadOnly(readonly);
            lineedit->setMaxLength(LINEEDIT_MAX_LENGTH);
            if (this->is_ccm_sum_member) {
                connect(lineedit, &QLineEdit::textChanged,
                    this, std::bind(&UniversalTable::cellEdited, this, lineedit, index));
            } else {
                connect(lineedit, &QLineEdit::editingFinished,
                    this, std::bind(&UniversalTable::cellEdited, this, lineedit, index));
            }
            setCellWidget(i, j, lineedit);
        }
    }

    if (this->is_ccm_sum_member) {
        for (int i = 0; i < row; i++) {
            QLineEdit *lineedit = new QLineEdit;
            lineedit->setFrame(false);
            lineedit->setReadOnly(true);
            lineedit->setText(QString::number(0));
            lineedit->setMaxLength(LINEEDIT_MAX_LENGTH);
            setCellWidget(i, column, lineedit);
        }
        setColumnHidden(column, false);
    }
}

void UniversalTable::hideSumColumn(bool colum_state)
{
    if (this->is_ccm_sum_member) {
        this->setColumnHidden(columnCount() - 1, colum_state);
    }
}

void UniversalTable::cellEdited(QLineEdit *lineedit, int index)
{
    int key_index = 0, value_index = index;
    double value = lineedit->text().toDouble();

    if (this->is_ccm_sum_member) {
        int col_cnt = columnCount() -1;
        int row = index / col_cnt;
        int sum = 0;
        for (int i = 0; i < col_cnt; i++) {
            QLineEdit *cell = qobject_cast<QLineEdit *> (cellWidget(row, i));
            if (cell) {
                sum += cell->text().toInt();
            }
        }
        QLineEdit *cell_sum = qobject_cast<QLineEdit *> (cellWidget(row, col_cnt));
        cell_sum->setText(QVariant::fromValue(sum).toString());
    }

    bool is_invalid;
    QVariant reg_value = validateValue(value, index, is_invalid);
    if (reg_section_key_list.count() > 1) {
        if (reg_section_key_list.count() != 2) {
            key_index = index / columnCount();
            value_index = index % columnCount();
        } else {
            key_index = index / columnCount() % reg_section_key_list.count();
            value_index = (index / columnCount() / reg_section_key_list.count()) * columnCount() + index % columnCount();
        }
    }

    QVariant iso_matrix_value = GlobalData::getInstance()->getIsoRegValue(reg_section_key_list[key_index].first,
        reg_section_key_list[key_index].second, value_index);

    if (reg_value == iso_matrix_value) {
        lineedit->setText(reg_value.toString());
        return;
    }

    if (value_type == QMetaType::Int) {
        GlobalData::getInstance()->setIsoRegValueByIso(reg_section_key_list[key_index].first,
        reg_section_key_list[key_index].second, value_index, qRound(reg_value.toDouble()));
    } else {
        GlobalData::getInstance()->setIsoRegValueByIso(reg_section_key_list[key_index].first,
        reg_section_key_list[key_index].second, value_index, reg_value);
    }

    emit sigValueChanged();
    // if auto write, trigger write data
    if (GlobalData::getInstance()->getSettings(SETTINGS_SECTION, KEY_AUTO_WRITE).toInt()) {
        emit sigPageWriteData();
    }
}

QVariant UniversalTable::validateValue(QVariant value, int index, bool &is_invalid)
{
    int row = rowCount();
    int column = columnCount();

    is_invalid = true;

    if (min_list.count() == 1) {
        if (value < this->min_list[0]) return this->min_list[0];
        if (value > this->max_list[0]) return this->max_list[0];
    } else if (min_list.count() == column) {
        int column_index = index % column;
        if (value < this->min_list[column_index]) return this->min_list[column_index];
        if (value > this->max_list[column_index]) return this->max_list[column_index];
    } else if (min_list.count() == row) {
        int row_index = index / column;
        if (value < this->min_list[row_index]) return this->min_list[row_index];
        if (value > this->max_list[row_index]) return this->max_list[row_index];
    }

    is_invalid = false;

    return value;
}

void UniversalTable::setRange(QJsonValue min, QJsonValue max)
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
        } else {
             qDebug("[Error] UniversalTable::setRange --> Counts are not equal.");
        }
    } else {
        qDebug("[Error] UniversalTable::setRange --> Undefined data type.");
    }
}

void UniversalTable::setValueType(QString type)
{
    if (type == "int") {
        this->value_type = QMetaType::Int;
    } else if (type == "double") {
        this->value_type = QMetaType::Double;
    } else {
        qDebug("[Error] UniversalTable::setValueType --> Undefined value type.");
    }
}

void UniversalTable::handleImportTable(QString file_path)
{
    QFile input_file(file_path);
    if(!input_file.open(QIODevice::ReadOnly)) {
        qDebug("Import file :%s error", qUtf8Printable(file_path));
        return;
    }

    QVariantList data_array;
    QTextStream in(&input_file);
    int col = columnCount();
    if (this->is_ccm_sum_member) {
        col -= 1;
    }
    while (!in.atEnd())
    {
        QString data = in.readLine();
        QList<QVariant> value_list = utils::stringToList<QVariant>(data);
        for (int i = 0; i < value_list.count(); i++)
            data_array.append(value_list[i].toDouble());
    }

    input_file.close();

    int table_size = rowCount() * col;
    int divided_size = table_size / reg_section_key_list.count();
    if (data_array.count() != table_size) {
        qDebug("[Error] UniversalTable::setData --> table size mismatch.");
        return;
    }

    int row = rowCount();
    int block = reg_section_key_list.count();
    int block_row = row / block;

    for (int i = 0; i < block; i++) {
        QVariantList temp_array;
        for (int j = 0; j < block_row; j++) {
            for(int k = 0; k < col; k++) {
                int x = (j * block + i) * col + k;
                temp_array.append(data_array[x]);
            }
        }
        GlobalData::getInstance()->setUnivMatrixRegValue(reg_section_key_list[i].first,
            reg_section_key_list[i].second, QJsonValue::fromVariant(temp_array));
    }
}

void UniversalTable::handleExportTable(QString file_path)
{
    QFile output_file(file_path);
    if(!output_file.open(QIODevice::WriteOnly)) {
        qDebug("Export file :%s error", qUtf8Printable(file_path));
        return;
    }

    QJsonArray value_array;

    for (int i = 0; i < reg_section_key_list.count(); i++) {
        QJsonArray temp_array = GlobalData::getInstance()->getUnivMatrixRegValue(
            reg_section_key_list[i].first, reg_section_key_list[i].second).toArray();
        for (int j = 0; j < temp_array.count(); j++)
            value_array.append(temp_array[j]);
    }

    int row = rowCount();
    int col = columnCount();
    int block = reg_section_key_list.count();
    int blockSize = value_array.count() / block;
    QTextStream out(&output_file);

    if (this->is_ccm_sum_member) {
        col -= 1;
    }
    for (int i = 0; i < row; i++) {
        for(int j = 0; j < col; j++) {
            int x = (i % block) * blockSize + i / block * col + j;
            QVariant value = value_array[x].toVariant();
            if (j == (col-1)) {
                out << value.toString() << "\n";
            } else {
                out << value.toString() << ",";
            }
        }
    }

    output_file.close();
}

void UniversalTable::refreshData(const SecKeyType &reg_section_key)
{
    bool isValueChanged = false;
    int size = 0, key_index = -1;
    int row = rowCount();
    int column = columnCount();

    if (this->is_ccm_sum_member) {
        column -= 1;
    }
    if (reg_section_key_list.count() == 1) {
        size = row * column;
        key_index = 0;
    } else if (reg_section_key_list.count() > 1) {
        if (reg_section_key_list.count() != 2)
            size = column;
        else
            size = row * column / 2;

        for (int i = 0; i < reg_section_key_list.count(); i++) {
            if (reg_section_key_list[i] == reg_section_key) {
                key_index = i;
                break;
            }
        }
    }

    QJsonArray value_array = GlobalData::getInstance()->getUnivMatrixRegValue(
        reg_section_key.first, reg_section_key.second).toArray();

    if (value_array.count() != size) {
        qDebug("[Error] UniversalTable::refreshData --> matrix size mismatch. section = %s, key = %s",
            qUtf8Printable(reg_section_key.first),  qUtf8Printable(reg_section_key.second));
        return;
    } else if (key_index == -1) {
        qDebug("[Error] UniversalTable::refreshData --> key_index is invalid.");
        return;
    }

    bool is_invalid = false;
    bool update_global_data = false;

    if (reg_section_key_list.count() == 1) {
        for (int i = 0, index = 0; i < row; i++) {
            for (int j = 0; j < column; j++, index++) {
                QVariant value = value_array[index].toDouble();
                QString value_str = validateValue(value, index, is_invalid).toString();
                if (is_invalid) {
                    qDebug("section: %s, key: %s, row: %d, column: %d, value: %f is invalid",
                            qUtf8Printable(reg_section_key.first), qUtf8Printable(reg_section_key.second),
                            i+1, j+1, value.toDouble());
                    update_global_data = true;
                }
                QLineEdit *lineedit = qobject_cast<QLineEdit *>(cellWidget(i, j));
                if (lineedit != nullptr) {
                    QString lineedit_str = lineedit->text();
                    if (lineedit_str != value_str) {
                        isValueChanged = true;
                        lineedit->setText(value_str);
                    }
                }
            }
        }
    } else if (reg_section_key_list.count() > 1) {
        if (reg_section_key_list.count() != 2) {
            for (int j = 0, index = key_index*column; j < column; j++, index++) {
                QVariant value = value_array[j].toDouble();
                QString value_str = validateValue(value, index, is_invalid).toString();
                if (is_invalid) {
                    qDebug("section: %s, key: %s, column: %d, value: %f is invalid",
                            qUtf8Printable(reg_section_key.first), qUtf8Printable(reg_section_key.second),
                            j+1, value.toDouble());
                    update_global_data = true;
                }
                QLineEdit *lineedit = qobject_cast<QLineEdit *>(cellWidget(key_index, j));
                if (lineedit != nullptr) {
                    QString lineedit_str = lineedit->text();
                    if (lineedit_str != value_str) {
                        isValueChanged = true;
                        lineedit->setText(value_str);
                    }
                }
            }
        } else {
            for (int i = key_index; i < row; i+=2) {
                int index = i*column;
                int value_index = (i/2)*column;
                for (int j = 0; j < column; j++, index++) {
                    QVariant value = value_array[value_index+j].toDouble();
                    QString value_str = validateValue(value, index, is_invalid).toString();
                    if (is_invalid) {
                        qDebug("section: %s, key: %s, row: %d, column: %d, value: %f is invalid",
                                qUtf8Printable(reg_section_key.first), qUtf8Printable(reg_section_key.second),
                                i+1,j+1, value.toDouble());
                        update_global_data = true;
                    }
                    QLineEdit *lineedit = qobject_cast<QLineEdit *>(cellWidget(i, j));
                    if (lineedit != nullptr) {
                        QString lineedit_str = lineedit->text();
                        if (lineedit_str != value_str) {
                            isValueChanged = true;
                            lineedit->setText(value_str);
                        }
                    }
                }
            }
        }
    }

    if (update_global_data) {
        // update the value_array
        if (reg_section_key_list.count() == 1) {
            for (int i = 0, index = 0; i < row; i++) {
                for (int j = 0; j < column; j++, index++) {
                    QLineEdit *lineedit = qobject_cast<QLineEdit *>(cellWidget(i, j));
                    if (lineedit != nullptr) {
                        QString lineedit_str = lineedit->text();
                        value_array[index] = lineedit_str.toDouble();
                    }
                }
            }
        } else if (reg_section_key_list.count() > 1) {
            if (reg_section_key_list.count() != 2) {
                for (int j = 0, index = key_index*column; j < column; j++, index++) {
                    QVariant value = value_array[j].toDouble();
                    QLineEdit *lineedit = qobject_cast<QLineEdit *>(cellWidget(key_index, j));
                    if (lineedit != nullptr) {
                        QString lineedit_str = lineedit->text();
                        value_array[index] = lineedit_str.toDouble();
                    }
                }
            } else {
                for (int i = key_index; i < row; i+=2) {
                    int index = i*column;
                    int value_index = (i/2)*column;
                    for (int j = 0; j < column; j++, index++) {
                        QVariant value = value_array[value_index+j].toDouble();
                        QLineEdit *lineedit = qobject_cast<QLineEdit *>(cellWidget(i, j));
                        if (lineedit != nullptr) {
                            QString lineedit_str = lineedit->text();
                            value_array[index] = lineedit_str.toDouble();
                        }
                    }
                }
            }
        }
        // set universal matrix reg value, without publishing!
        if (reg_section_key_list.count() == 1) {
            GlobalData::getInstance()->setUnivMatrixRegValue(reg_section_key_list[0].first,
                reg_section_key_list[0].second, QJsonValue::fromVariant(value_array), false);
        } else {
            int table_size = rowCount() * columnCount();
            int divided_size = table_size / reg_section_key_list.count();
            for (int i = 0; i < reg_section_key_list.count(); i++) {
                QVariantList temp_array;
                for (int j = 0, k = i*divided_size; j < divided_size; j++)
                    temp_array.append(value_array[k+j].toDouble());

                GlobalData::getInstance()->setUnivMatrixRegValue(reg_section_key_list[i].first,
                    reg_section_key_list[i].second, QJsonValue::fromVariant(temp_array), false);
            }
        }
    }

    if (isValueChanged) {
        emit sigValueChanged();
    }
}

QVariant UniversalTable::getData(QString path)
{
    // Todo: Use getDataJsonrpc to get json value with path
    QVariantList data;
    if (reg_section_key_list.isEmpty()) {
        qDebug("[Error] UniversalTable::getData --> reg_section_key_list is empty.");
        return data;
    }

    int key_index = -1;
    if (path.isEmpty()) {
        key_index = 0;
    } else {
        for (int i = 0; i < reg_section_key_list.count(); i++) {
            if (reg_section_key_list[i].second == path) {
                key_index = i;
                break;
            }
        }
    }

    if (key_index == -1) {
        qDebug("[Error] UniversalTable::getData --> key_index is invalid.");
        return data;
    }

    int size = rowCount() * columnCount() / reg_section_key_list.count();
    QJsonArray value_array;
    if (path.isEmpty()) {
         for (int i = 0; i < reg_section_key_list.count(); i++) {
            QJsonArray temp_array = GlobalData::getInstance()->getUnivMatrixRegValue(
                reg_section_key_list[i].first, reg_section_key_list[i].second).toArray();
            for (int j = 0; j < temp_array.count(); j++)
                value_array.append(temp_array[j]);
         }
    } else {
        value_array = GlobalData::getInstance()->getUnivMatrixRegValue(
            reg_section_key_list[key_index].first, reg_section_key_list[key_index].second).toArray();
    }

    if ((value_array.count() != size) && (!path.isEmpty())) {
        qDebug("[Error] UniversalTable::getData --> data size mismatch.");
        return data;
    }

    for (int i = 0; i < value_array.count(); i++)
        data.append(value_array[i].toVariant());

    return data;
}

void UniversalTable::setData(QString path, QVariant data)
{
    if (reg_section_key_list.isEmpty()) {
        qDebug("[Error] UniversalTable::setData --> reg_section_key_list is empty.");
        return;
    }

    int table_size = rowCount() * columnCount();
    int divided_size = table_size / reg_section_key_list.count();
    QVariantList data_array = data.toList();

    if (this->is_ccm_sum_member) {
        table_size = rowCount() * (columnCount() - 1);
    }
    if (path.isEmpty()) {
        if (data_array.count() != table_size) {
            qDebug("[Error] UniversalTable::setData --> table size mismatch.");
            return;
        }

        if (reg_section_key_list.count() == 1) {
            GlobalData::getInstance()->setUnivMatrixRegValue(reg_section_key_list[0].first,
                reg_section_key_list[0].second, QJsonValue::fromVariant(data_array));
        } else {
            for (int i = 0; i < reg_section_key_list.count(); i++) {
                QVariantList temp_array;
                for (int j = 0, k = i*divided_size; j < divided_size; j++)
                    temp_array.append(data_array[k+j]);

                GlobalData::getInstance()->setUnivMatrixRegValue(reg_section_key_list[i].first,
                    reg_section_key_list[i].second, QJsonValue::fromVariant(temp_array));
            }
        }
    } else {
        if (data_array.count() != divided_size) {
            qDebug("[Error] UniversalTable::setData --> divided size mismatch.");
            return;
        }

        int key_index = -1;
        for (int i = 0; i < reg_section_key_list.count(); i++) {
            if (reg_section_key_list[i].second == path) {
                key_index = i;
                break;
            }
        }

        if (key_index != -1) {
            GlobalData::getInstance()->setUnivMatrixRegValue(reg_section_key_list[key_index].first,
                reg_section_key_list[key_index].second, QJsonValue::fromVariant(data_array));
        } else {
            qDebug("[Error] UniversalTable::setData --> key_index is invalid.");
            return;
        }
    }
}
