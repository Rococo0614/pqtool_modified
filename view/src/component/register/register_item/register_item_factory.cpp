#include "register_item_factory.hpp"
#include "global_data.hpp"

#include <QJsonObject>
#include <QJsonArray>

BaseItemWidget *RegisterItemFactory::createRegisterItem(REGISTER_TYPE type, QString module, QString title, QWidget *widget)
{
    BaseItemWidget *item = nullptr;
    switch(type) {
        case BINARY:
            item = new BinaryItemWidget(module, title);
            break;
        case COMBOBOX:
            item = new ComboBoxItemWidget(module, title);
            break;
        case SPINNER:
            item = new SpinnerItemWidget(module, title);
            break;
        case DOUBLE_SPINNER:
            item = new DoubleSpinnerItemWidget(module, title);
            break;
        case MATRIX:
            item = new MatrixItemWidget(module, title);
            break;
        case ISO_MATRIX:
            item = new IsoMatrixItemWidget(module, title);
            break;
        case CC_MATRIX:
            item = new CCMatrixItemWidget(module, title);
            break;
        case HEADER:
            item = new HeaderWidget(module, title);
            break;
        case OP_TYPE_COMBOBOX:
            item = new OpTypeComboBoxWidget(module, title);
            break;
        case HEX_SPINNER:
            item = new HexSpinnerItemWidget(module, title);
            break;
        case UINT_SPINNER:
            item = new UIntSpinnerItemWidget(module, title);
            break;
        case ISO_CURVE:
            item = new IsoCurveItemWidget(module, title);
            break;
        case MANUAL_CURVE:
            item = new CurveItemWidget(module, title);
            break;
        case USERDEFINE_CURVE:
            item = new CurveUserDefineItemWidget(module, title);
            break;
        case UNIV_MATRIX:
            item = new UniversalMatrixItemWidget(module, title);
            break;
        case UNIV_CURVE:
            item = new UniversalCurveItemWidget(module, title);
            break;
        case GAMMA_CURVE:
            item = new GammaCurveItemWidget(module, title, GAMMA_CURVE_TYPE_T, widget);
            break;
        case GAMMA_CURVE_LV:
            item = new GammaCurveItemWidget(module, title, GAMMA_CURVE_TYPE_LV, widget);
            break;
        case GAMMA_CURVE_RGB:
            item = new GammaCurveItemWidget(module, title, GAMMA_CURVE_TYPE_RGB, widget);
            break;
        case MAP_CURVE:
            item = new GammaCurveItemWidget(module, title, MAP_CURVE_TYPE_T, widget);
            break;
        case SIMPLE_TABLE_CURVE:
            item = new UniversalMatrixCurveItemWidget(module, title, SIMPLE_TYPE, widget);
            break;
        case METHOD_TABLE_CURVE:
            item = new UniversalMatrixCurveItemWidget(module, title,METHOD_TYPE,  widget);
            break;
    }

    return item;
}

BaseItemWidget *RegisterItemFactory::createRegisterItemByJson(QString module, QJsonObject json, int titleLen, QWidget *widget)
{
    BaseItemWidget *item;

    QString title = json["title"].toString();
    QVariant param = QVariant(json["param"]);

    REGISTER_TYPE type = getTypeByStr(json["type"].toString(), param);
    item = RegisterItemFactory::createRegisterItem(type, module, title, widget);
    item->setLabelWidth(titleLen);

    if (type == SPINNER) {
        QMap<QString, QVariant> m = param.toMap();
        m[KEY_REG_SECTION] = json[KEY_REG_SECTION].toString();
        item->setParam(m);
    } else {
        item->setParam(param);
    }

    if (json.contains(KEY_REG_INT_BASE))
    {
        int base = json[KEY_REG_INT_BASE].toInt();
        //TODO set int base here
        item->setDisplayIntBase(base);
    }

    //get read/write mode
    if (json.contains(KEY_REG_CHMOD))
    {
        QString reg_chmod = json[KEY_REG_CHMOD].toString();
        if (!reg_chmod.isEmpty())
        {
            if (reg_chmod.compare("ReadOnly") == 0)
                item->setChmod(MOD_RO);
            else if (reg_chmod.compare("ReadWrite") == 0) {
                item->setChmod(MOD_RW);
            }
        }
    }

    // get section
    if (!json.contains(KEY_REG_SECTION)) {
        return item;
    }

    QString reg_section = json[KEY_REG_SECTION].toString();

    // special for config combobox
    if (type == OP_TYPE_COMBOBOX) {
        OpTypeComboBoxWidget *configCombobox = dynamic_cast<OpTypeComboBoxWidget *>(item);
        if (configCombobox != nullptr) {
            configCombobox->setConfigSection(reg_section);
        }
    }

    // set group, for item get key
    if (json.contains(KEY_REG_GROUP)) {
        QString reg_group = json[KEY_REG_GROUP].toString();
        if (!reg_group.isEmpty()) {
            item->setGroup(reg_group);
        }
    }

    // set level
    if (json.contains(KEY_REG_LEVEL)) {
        int level = json[KEY_REG_LEVEL].toInt();
        item->setLevel(level);
    }

    // set op type
    if (json.contains(KEY_REG_OP_TYPE)) {
        QString reg_optype = json[KEY_REG_OP_TYPE].toString();
        if (!reg_optype.isEmpty()) {
            item->setOpType(reg_optype);
        }
    }

    // set desp
    if (json.contains(KEY_REG_DESP)) {
        QString reg_desp = json[KEY_REG_DESP].toString();
        if (!reg_desp.isEmpty()) {
            item->setDesp(reg_desp);
        }
    }

    if (json.contains(KEY_TITLE_LABEL_WIDTH)) {
        int w = json[KEY_TITLE_LABEL_WIDTH].toInt();
        if (w > titleLen) {
            item->setLabelWidth(w);
        }
    }

    // set reg value type
    if (json.contains(KEY_REG_VALUE_TYPE)) {
        QString reg_value_type = json[KEY_REG_VALUE_TYPE].toString();
        if (!reg_value_type.isEmpty()) {
            item->setRegValueType(reg_value_type);
        }
    }

    if (json.contains(KEY_VALUE_TYPE)) {
        QString value_type = json[KEY_VALUE_TYPE].toString();
        if (!value_type.isEmpty()) {
            item->setValueType(value_type);
        }
    }

    // set keys
    if (json.contains(KEY_REG_KEY)) {
        QList<QVariant> reg_keys;
        if (json[KEY_REG_KEY].isArray()) {
            reg_keys = json[KEY_REG_KEY].toArray().toVariantList();
        } else if (json[KEY_REG_KEY].isString()) {
            reg_keys.append(json[KEY_REG_KEY].toString());
        }

        QList<SecKeyType> sec_key_list;
        for (auto reg_key : reg_keys) {
            if (!reg_key.toString().isEmpty()) {
                sec_key_list.append(SecKeyType(reg_section, reg_key.toString()));
                GlobalData::getInstance()->subscribeItemJsonrpc(reg_key.toString(), item);
                item->addPath(reg_key.toString());
            }
        }
        // this will refresh item data, need put the last
        if (!sec_key_list.isEmpty()) {
            item->setRegSectionKeyList(std::move(sec_key_list));
        }
    }

    // set structure
    if (json.contains(KEY_STRUCTURE)) {
        QString structure = json[KEY_STRUCTURE].toString();
        if (!structure.isEmpty()) {
            item->setStructure(structure);
        }
    }

    return item;
}

REGISTER_TYPE RegisterItemFactory::getTypeByStr(QString str, QVariant param)
{
    REGISTER_TYPE type = BINARY;

    if (str == "binary") {
        type = BINARY;
    } else if (str == "combobox") {
        type = COMBOBOX;
    } else if (str == "spinner") {
        type = SPINNER;
    } else if (str == "double_spinner") {
        type = DOUBLE_SPINNER;
    } else if (str == "matrix") {
        type = MATRIX;
    } else if (str == "iso_matrix") {
        type = ISO_MATRIX;
    } else if (str == "header") {
        type = HEADER;
    } else if (str == "op_type_combobox") {
        type = OP_TYPE_COMBOBOX;
    } else if (str == "cc_matrix") {
        type = CC_MATRIX;
    } else if (str == "hexspinner")
    {
        type = HEX_SPINNER;
    } else if (str == "uint_spinner")
    {
        type = UINT_SPINNER;
    } else if (str == "iso_curve") {
        type = ISO_CURVE;
    } else if (str == "manual_curve") {
        type = MANUAL_CURVE;
    } else if (str == "userdefine_curve") {
        type = USERDEFINE_CURVE;
    } else if (str == "univ_matrix") {
        int col_cout = 1;
        int col_cout_max = GlobalData::getInstance()->getSettings(SETTINGS_SECTION, KEY_MAX_LUT_SHOW_COUNT, 51).toInt();

        if (param.toJsonObject().contains("count")) {
            QJsonValue count_val = param.toJsonObject()["count"];
            if (count_val.isDouble()) {
                col_cout = count_val.toInt();
            } else if (count_val.isArray()){
                col_cout = count_val.toArray()[1].toInt();
            }
        }
        if (col_cout > col_cout_max) {
            type = UNIV_MATRIX;
        } else {
            type = SIMPLE_TABLE_CURVE;
        }
    } else if (str == "univ_curve") {
        type = UNIV_CURVE;
    } else if (str == "gamma_curve") {
        type = GAMMA_CURVE;
    } else if (str == "gamma_curve_lv") {
        type = GAMMA_CURVE_LV;
    } else if (str == "gamma_curve_rgb") {
        type = GAMMA_CURVE_RGB;
    } else if (str == "method_table_curve") {
        type = METHOD_TABLE_CURVE;
    } else if (str == "map_curve") {
        type = MAP_CURVE;
    }

    return type;
}
