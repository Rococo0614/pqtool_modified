#ifndef REGISTER_ITEM_FACTORY_H
#define REGISTER_ITEM_FACTORY_H

#include "base_item_widget.hpp"
#include "binary_item_widget.hpp"
#include "combobox_item_widget.hpp"
#include "double_spinner_item_widget.hpp"
#include "ccmatrix_item_widget.hpp"
#include "matrix_item_widget.hpp"
#include "iso_matrix_item_widget.hpp"
#include "spinner_item_widget.hpp"
#include "header_widget.hpp"
#include "op_type_combobox_widget.hpp"
#include "hexspinner_item_widget.hpp"
#include "uint_spinner_item_widget.hpp"
#include "iso_curve_item_widget.hpp"
#include "curve_item_widget.hpp"
#include "curve_userdefine_item_widget.hpp"
#include "universal_matrix_item_widget.hpp"
#include "universal_curve_item_widget.hpp"
#include "gamma_curve_item_widget.hpp"
#include "universal_matrix_curve_item_widget.hpp"

#include <QString>

enum REGISTER_TYPE {
    BINARY,
    COMBOBOX,
    SPINNER,
    DOUBLE_SPINNER,
    MATRIX,
    ISO_MATRIX,
    HEADER,
    OP_TYPE_COMBOBOX,
    CC_MATRIX,
    HEX_SPINNER,
    UINT_SPINNER,
    ISO_CURVE,
    MANUAL_CURVE,
    USERDEFINE_CURVE,
    UNIV_MATRIX,
    UNIV_CURVE,
    GAMMA_CURVE,
    GAMMA_CURVE_LV,
    GAMMA_CURVE_RGB,
    SIMPLE_TABLE_CURVE,
    METHOD_TABLE_CURVE,
    MAP_CURVE
};

class RegisterItemFactory
{
public:
    static BaseItemWidget *createRegisterItemByJson(QString module, QJsonObject json, int titleLen = 150, QWidget *widget = nullptr);

private:
    static BaseItemWidget *createRegisterItem(REGISTER_TYPE type, QString module, QString title, QWidget *widget = nullptr);
    static REGISTER_TYPE getTypeByStr(QString str, QVariant param);

};

#endif // REGISTER_ITEM_FACTORY_H
