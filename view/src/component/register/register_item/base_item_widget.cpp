#include "base_item_widget.hpp"

#include <QtCore>
#include <QHBoxLayout>
#include <QStyleOption>
#include <QPainter>
#include "global_data.hpp"

const QString OP_TYPE_TITLE_SEPARATOR = ".";
const QString KEY_SEPARATOR = ".";

BaseItemWidget::BaseItemWidget(QString title, QWidget *parent) :
    QWidget(parent),
    IObserver()
{
    layout = new QHBoxLayout(this);
    icon_widget = new QWidget;
    icon_button = new QPushButton;
    icon_layout = new QHBoxLayout(icon_widget);
    title_label = new QLabel(title);
    value_widget = new QWidget();
    value_layout = new QHBoxLayout(value_widget);

    icon_widget->setStyleSheet("background-color: #ffffff;");
    icon_widget->setFixedWidth(60);
    icon_button->setFlat(true);
    icon_button->setIcon(QIcon(":/icons/rw.png"));
    icon_button->setStyleSheet("background-color: #ffffff;");
    icon_button->setDisabled(true);

    icon_layout->addWidget(icon_button);
    layout->setSpacing(1);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(icon_widget);
    layout->addWidget(title_label);
    layout->addWidget(value_widget);

    title_label->setStyleSheet("background-color: #eeeeee;");
    title_label->setMargin(5);
    title_label->setFixedWidth(155);
    value_widget->setStyleSheet("background-color: #ffffff;");
    value_changed = false;
    this->title = title;
    // get data type
    value_type = TYPE_INT;
    // register value type
    reg_value_type = TYPE_INT;

    QObject::connect(GlobalData::getInstance().get(), &GlobalData::sigDataLoaded,
                     this, &BaseItemWidget::dataLoaded);
}

BaseItemWidget::BaseItemWidget(QString module, QString title, QWidget *parent) :
    BaseItemWidget(title, parent)
{
    this->module = module;
    GlobalData::getInstance()->subscribeItem(ModuleKeyType(this->module, this->title), this);
}

QString BaseItemWidget::getKey() const
{
    return (this->group.isEmpty()) ? this->title : this->group + KEY_SEPARATOR + this->title;
}

QString BaseItemWidget::getTitle() const
{
    return this->title;
}

void BaseItemWidget::syncValueChanged(QVariant value)
{
    // sync value to global data
    QList<SecKeyType> section_key_list = getSectionKeyList();
    for (auto section_key : section_key_list) {
        QVariant reg_value = value;
        if (this->reg_value_type == TYPE_ARRAY) {
            reg_value = getRegValueListByValueMapping(value);
        } else {
            if (!this->value_mapping_vec.isEmpty()) {
                // handle value mapping to each reg
                reg_value = getRegValueByValueMapping(value, SecKeyType(section_key.first, section_key.second)).toInt();
            }
        }

        if (!reg_value.isNull()) {
            GlobalData::getInstance()->setRegValue(section_key.first, section_key.second, reg_value, false);
        }
    }

    emit sigValueChanged(value);
    // if auto write, trigger write data
    if (GlobalData::getInstance()->getSettings(SETTINGS_SECTION, KEY_AUTO_WRITE).toInt()) {
        emit sigWritePageData();
    }
}

void BaseItemWidget::paintEvent(QPaintEvent *)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void BaseItemWidget::changeEvent(QEvent *event)
{
    switch(event->type()) {
    case QEvent::LanguageChange:
        retranslateUi();
        break;
    default:
        break;
    }
    QWidget::changeEvent(event);
}

void BaseItemWidget::retranslateUi()
{
}

void BaseItemWidget::setValueMappingVec(QVector<QVariantList> &&value_mapping_vec)
{
    this->value_mapping_vec = std::move(value_mapping_vec);
}

void BaseItemWidget::setDesp(QString desp)
{
    this->desp = desp;
}

QString BaseItemWidget::getDesp() const
{
    return this->desp;
}

void BaseItemWidget::setGroup(QString group)
{
    this->group = group;
}

QString BaseItemWidget::getGroup() const
{
    return this->group;
}

void BaseItemWidget::setOpType(QString op_type)
{
    IObserver::setOpType(op_type);
    // update title
    title_label->setText(op_type + OP_TYPE_TITLE_SEPARATOR + this->title);
    this->op_type = op_type;
}

QString BaseItemWidget::getOpType() const
{
    return this->op_type;
}

void BaseItemWidget::uiReset()
{
    if (value_changed) {
        if (dark_theme) {
            title_label->setStyleSheet("background-color: #455364;");
        } else {
            title_label->setStyleSheet("background-color: #eeeeee;");
        }

        value_changed = false;
    }
}

void BaseItemWidget::setLevel(int level)
{
    this->level = level;
}

int BaseItemWidget::getLevel()
{
    return this->level;
}

void BaseItemWidget::updateVisible()
{
    if (GlobalData::getInstance()->getLevel() < this->level) {
        if (!this->isHidden()) {
            this->hide();
        }
    } else {
        if (this->isHidden()) {
            this->show();
        }
    }
}

void BaseItemWidget::updateHighlight(bool bHiglight)
{
   if (bHiglight) {
        icon_widget->setStyleSheet("background-color: #a5cdff;");
        title_label->setStyleSheet("background-color: #a5cdff;");
    } else {
        icon_widget->setStyleSheet("background-color: #ffffff;");
        title_label->setStyleSheet("background-color: #eeeeee;");
    }
}

QString BaseItemWidget::getValueType() const
{
    return this->value_type;
}

void BaseItemWidget::setValueType(QString value_type)
{
    this->value_type = value_type;
}

QString BaseItemWidget::getModule() const
{
    return this->module;
}

void BaseItemWidget::setModule(QString module)
{
    this->module = module;
}

void BaseItemWidget::setParam(QVariant)
{
}

QVariant BaseItemWidget::getData() const
{
    return QVariant("");
}

QJsonObject BaseItemWidget::getJsonData() const
{
    qDebug("BaseItemWidget::getJsonData");
    QJsonObject data_obj;
    if (this->reg_section_key_list.isEmpty()) {
        qDebug("[Error] BaseItemWidget::getJsonData. reg_section_key_list is empty");
        return data_obj;
    }

    // data_obj[KEY_VALUE_TYPE] = this->value_type;
    data_obj[KEY_REG_KEY] = this->getKey();
    data_obj[KEY_REG_OP_TYPE] = this->getOpType();
    QJsonValue value;
    if (path_list.count() == 1)
        value = getData().toJsonValue();
    else
        value = getDataJsonrpc("").toJsonValue();

    if (value.isArray()) {
        data_obj[KEY_VALUE] = value;
    } else if(value.isDouble()) {
        data_obj[KEY_VALUE] = value.toDouble();
    } else {
        data_obj[KEY_VALUE] = value.toInt();
    }

    if (!this->structure.isEmpty()) {
        data_obj[KEY_STRUCTURE] = this->structure;
    }

    return data_obj;
}

QJsonArray BaseItemWidget::getSaveData() const
{
    //qDebug("BaseItemWidget::getSaveData");
    QJsonArray save_data;

    if (path_list.isEmpty()) {
        qDebug("[Error] BaseItemWidget::getSaveData --> %s page's \"%s\" path is empty.",
            qUtf8Printable(module), qUtf8Printable(title));
        return save_data;
    }

    int n = path_list.count();
    for (int i = 0; i < n; i++) {
        QJsonObject data_obj;
        QJsonValue value;
        data_obj["PATH"] = path_list[i];
        if (n == 1)
            value = getData().toJsonValue();
        else
            value = getDataJsonrpc(path_list[i]).toJsonValue();

        data_obj["VALUE"] = value;
        save_data.append(data_obj);
    }

    return save_data;
}

void BaseItemWidget::dataLoaded()
{
    for (auto reg_section_key : reg_section_key_list) {
        refreshData(reg_section_key);
    }
    uiReset();
}

void BaseItemWidget::handleUiChanged()
{
    value_changed = true;
    if (dark_theme) {
        title_label->setStyleSheet("background-color: #455364; color: red;");
    } else {
        title_label->setStyleSheet("background-color: #eeeeee; color: red;");
    }
}

void BaseItemWidget::setData(QVariant, bool)
{
}

void BaseItemWidget::refreshData(const SecKeyType&)
{
    QVariant value;
    QVariantList value_list;
    // check data type is array or single value
    bool is_reg_value_type_array = (this->reg_value_type == TYPE_ARRAY) ? true : false;

    for (auto reg_section_key : reg_section_key_list) {
        if (reg_section_key.first.isEmpty() || reg_section_key.second.isEmpty()) {
            continue;
        }

        // handle value array or single value
        if (is_reg_value_type_array) {
            value_list = GlobalData::getInstance()->getRegValue(reg_section_key.first, reg_section_key.second).toList();
            value = value_list;
        } else {
            value = GlobalData::getInstance()->getRegValue(reg_section_key.first, reg_section_key.second);
        }
        qDebug("base item widget refreshData [%s] [%s] [%s]", qUtf8Printable(reg_section_key.first),
               qUtf8Printable(reg_section_key.second), qUtf8Printable(value.toString()));
    }

    setData(value);
    // update tooltip
    if (!this->desp.isEmpty()) {
        this->title_label->setToolTip(this->desp);
    }
}

QVariant BaseItemWidget::getValueByValueMapping(QVariantList &&values) const
{
    return QVariant(this->value_mapping_vec.indexOf(values));
}

QVariant BaseItemWidget::getRegValueByValueMapping(QVariant value, SecKeyType &&reg_section_key) const
{
    // value is value table's idx
    if (value >= this->value_mapping_vec.size()) return value;

    QVariantList value_list = this->value_mapping_vec.at(value.toInt());
    int value_idx = this->reg_section_key_list.indexOf(reg_section_key);
    if (value_idx >= value_list.size()) return value;

    return value_list[value_idx];
}

QVariantList BaseItemWidget::getRegValueListByValueMapping(QVariant value) const
{
    QVariantList value_list;
    // value is value table's idx
    if (value >= this->value_mapping_vec.size()) return value_list;
    value_list = this->value_mapping_vec.at(value.toInt());

    return value_list;
}

void BaseItemWidget::setChmod(int mod)
{
    IObserver::setChmod(mod);

    if (mod == MOD_RO)
    {
        icon_button->setIcon(QIcon(":/icons/ro.png"));
    }
}

void BaseItemWidget::initSubWidgets()
{

}

void BaseItemWidget::setLabelWidth(int w, bool is_extended)
{
    if (title_label) {
        if (is_extended) {
            w = (w > 0) ? w : 155;
        } else {
            w = w > 155? w : 155;
            w = w > 380? 380 : w;
        }
        title_label->setFixedWidth(w);
    }
}


void BaseItemWidget::setLTKey(SecKeyType &key)
{
    this->lt_key = key;
}

void BaseItemWidget::setMTKey(SecKeyType &key)
{
    this->mt_key = key;
}

void BaseItemWidget::setStructure(QString structure)
{
    this->structure = structure;
}

void BaseItemWidget::handleUpdateTheme(bool dark_theme_enabled)
{
    dark_theme = dark_theme_enabled;
    if (dark_theme_enabled) {
        icon_widget->setStyleSheet("background-color: #19232D;");
        icon_button->setStyleSheet("background-color: #19232D;");
        title_label->setStyleSheet("background-color: #455364;");
        value_widget->setStyleSheet("background-color: #19232D;");
    } else {
        icon_widget->setStyleSheet("background-color: #ffffff;");
        icon_button->setStyleSheet("background-color: #ffffff;");
        title_label->setStyleSheet("background-color: #eeeeee;");
        value_widget->setStyleSheet("background-color: #ffffff;");
    }
}

QJsonValue BaseItemWidget::getJsonValue(QString path) const
{
    // Todo: Use getDataJsonrpc to get json value
    QJsonValue value;

    if (this->path_list.contains(path)) {
        QVariant data;
        if (this->path_list.count() == 1) {
            data = getData();
        } else {
            data = getDataJsonrpc(path);
        }

        if (value_type == TYPE_DOUBLE){
            if (data.type() == QVariant::List) {
                QVariantList tmpList = data.toList();
                QJsonArray tmpArr;
                for (int i = 0; i < tmpList.size(); i++) {
                    QString strValue = QString::number(tmpList[i].toDouble(), 'f', 10);
                    tmpArr.append(QJsonValue(strValue));
                }
                value = QJsonValue(tmpArr);
            } else {
                QString strValue = QString::number(data.toDouble(), 'f', 2);
                value = QJsonValue(strValue);
            }
        } else {
            value = data.toJsonValue();
        }
    } else {
        qDebug("[Error] BaseItemWidget::getJsonValue. Can't find path in path_list.");
    }

    return value;
}

QVariant BaseItemWidget::getDataJsonrpc(QString path) const
{
    QVariant var;

    return var;
}

void BaseItemWidget::setDataJsonrpc(QString path, QVariant data, bool sync_to_global)
{

}
