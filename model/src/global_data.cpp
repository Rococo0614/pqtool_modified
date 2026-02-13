#include "global_data.hpp"
#include "utils.hpp"
#include "network_client_helper.hpp"
#include "module_define.hpp"

#include <fstream>
#include <QCoreApplication>
#include <QDir>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QVariant>

class AutoWriteGuard
{
public:
    AutoWriteGuard() {
        // disable auto write first
        this->auto_write = GlobalData::getInstance()->getSettings(SETTINGS_SECTION, KEY_AUTO_WRITE);
        GlobalData::getInstance()->setSettings(SETTINGS_SECTION, KEY_AUTO_WRITE, 0);
    }

    ~AutoWriteGuard() {
        // set auto write back
        GlobalData::getInstance()->setSettings(SETTINGS_SECTION, KEY_AUTO_WRITE, this->auto_write);
    }

public:
    QVariant auto_write{0};
};

shared_ptr<GlobalData> GlobalData::m_instance = nullptr;
QString GlobalData::settings_file_path = "";
QString GlobalData::cvipq_tool_soc = "";
QString GlobalData::cvipq_tool_template = "";
QString GlobalData::template_folder = "";
quint8 GlobalData::fusionFrame = 1;
QString GlobalData::cvipq_tool_branch = "";
bool GlobalData::changetemplatestatus = false;

QMap<CHIP_ID, QString> chip_string_map = {
    {CHIP_ID::CVI1835, "CV1835"},
    {CHIP_ID::CVI1832, "CV1832"},
    {CHIP_ID::CVI1838, "CV1838"},
    {CHIP_ID::CVI1822, "CV1822"},
    {CHIP_ID::CVI1829, "CV1829"},
    {CHIP_ID::CVI1826, "CV1826"},
    {CHIP_ID::CVI1821, "CV1821"},
    {CHIP_ID::CVI1820, "CV1820"},
    {CHIP_ID::CVI1823, "CV1823"},
    {CHIP_ID::CVI1825, "CV1825"}
};

QMap<MARS_CHIP_ID, QString> mars_chip_string_map = {
    {MARS_CHIP_ID::CV1820A, "CV1820A"},
    {MARS_CHIP_ID::CV1821A, "CV1821A"},
    {MARS_CHIP_ID::CV1822A, "CV1822A"},
    {MARS_CHIP_ID::CV1823A, "CV1823A"},
    {MARS_CHIP_ID::CV1825A, "CV1825A"},
    {MARS_CHIP_ID::CV1826A, "CV1826A"},
    {MARS_CHIP_ID::CV1810C, "CV1810C"},
    {MARS_CHIP_ID::CV1811C, "CV1811C"},
    {MARS_CHIP_ID::CV1812C, "CV1812C"},
    {MARS_CHIP_ID::CV1811H, "CV1811H"},
    {MARS_CHIP_ID::CV1812H, "CV1812H"},
    {MARS_CHIP_ID::CV1813H, "CV1813H"},
    {MARS_CHIP_ID::CV1800B, "CV1800B"},
    {MARS_CHIP_ID::CV1801B, "CV1801B"},
    {MARS_CHIP_ID::CV1800C, "CV1800C"},
    {MARS_CHIP_ID::CV1801C, "CV1801C"},
    {MARS_CHIP_ID::BM1686, "BM1686"},
    {MARS_CHIP_ID::CV186AH, "CV186AH"},
    {MARS_CHIP_ID::CV184AH, "CV184AH"}
};

GlobalData::GlobalData()
{
    level = TOOL_REL_MODE;
    settings_file_path = QCoreApplication::applicationDirPath() + PATH_SEPARATOR + DEFAULT_SETTINGS_FILE_NAME;

    // bind network data updated
    shared_ptr<NetworkClientHelper> network_helper = NetworkClientHelper::getInstance();
#ifndef TCP_SOCKET_CLIENT
    QObject::connect(network_helper.get(), &NetworkClientHelper::sigReceiveCmdResp, this, &GlobalData::handleDataChanged);
    QObject::connect(network_helper.get(), &NetworkClientHelper::sigReceiveWDRMode, this, &GlobalData::handleReceiveWDRMode);
#else
    QObject::connect(network_helper.get(), &NetworkClientHelper::sigReceiveCmdRespJsonrpc, this, &GlobalData::handleDataChangedJsonrpc);
    QObject::connect(network_helper.get(), &NetworkClientHelper::sigReceiveGammaJsonrpc, this, &GlobalData::handleReceiveGammaJsonrpc);
    QObject::connect(network_helper.get(), &NetworkClientHelper::sigGetWDRMode, this, &GlobalData::handleReceiveWDRMode);
    QObject::connect(network_helper.get(), &NetworkClientHelper::sigToolJsonStatus, this, &GlobalData::handleToolJsonStatus);
    QObject::connect(network_helper.get(), &NetworkClientHelper::sigToolJsonData, this, &GlobalData::handleToolJsonData);
#endif
}

shared_ptr<GlobalData> GlobalData::getInstance()
{
    if (nullptr == GlobalData::m_instance) {
        GlobalData::m_instance = make_shared<GlobalData>();
        GlobalData::m_instance->loadSettings();
        GlobalData::m_instance->initData();
    }

    return GlobalData::m_instance;
}

void GlobalData::setRegValue(const QString &section, const QString &key, const QVariant &value, bool publish, IObserver *exclude)
{
    if (section.isEmpty() || key.isEmpty()) {
        return;
    }

    QJsonObject registers = data[REGISTERS_SECTION].toObject();
    setJsonValueHelper(registers, section, key, QJsonValue::fromVariant(value));
    data[REGISTERS_SECTION] = registers;

    SecKeyType reg_section_key = SecKeyType(section, key);
    if (publish) {
        publishObserver(reg_section_key, OP_TYPE_MANUAL, exclude);
    }
    emit sigDataModified(reg_section_key);
}

void GlobalData::setRegValueByIdx(const QString &section, const QString &key, int value_idx, const QVariant &value, bool publish)
{
    if (section.isEmpty() || key.isEmpty()) {
        return;
    }

    QJsonObject registers = data[REGISTERS_SECTION].toObject();
    setJsonValueHelper(registers, section, key, value_idx, QJsonValue::fromVariant(value));
    data[REGISTERS_SECTION] = registers;

    SecKeyType reg_section_key = SecKeyType(std::move(section), std::move(key));
    if (publish) {
        publishObserver(reg_section_key, OP_TYPE_MANUAL);
    }
    emit sigDataModified(reg_section_key);
}

void GlobalData::setRegValueByIdx(const QString &section, const QString &key, const QString &op_type, int value_idx, const QVariant &value, bool publish)
{
    if (section.isEmpty() || key.isEmpty()) {
        return;
    }

    QJsonObject registers = data[REGISTERS_SECTION].toObject();
    setJsonValueHelper(registers, section, key, value_idx, QJsonValue::fromVariant(value));
    data[REGISTERS_SECTION] = registers;

    SecKeyType reg_section_key = SecKeyType(std::move(section), std::move(key));
    if (publish) {
        publishObserver(reg_section_key, op_type);
    }
    emit sigDataModified(reg_section_key);
}

void GlobalData::setIsoRegValue(const QString &section, const QString &key, const QList<QVariant> &value_list, bool publish)
{
    if (section.isEmpty() || key.isEmpty()) {
        return;
    }

    //if (value_list.size() != iso_list.size()) return;

    QJsonObject registers = iso_matrix_data;
    setJsonValueHelper(registers, section, key, QJsonValue::fromVariant(value_list));
    iso_matrix_data = registers;

    /*for (int i = 0; i < value_list.size(); i++) {
        if (i > (iso_list.size() - 1)) break;
        //int iso = iso_list[i];
        QJsonObject registers = iso_matrix_data;//iso_data[iso];
        setJsonValueHelper(registers, section, key, i, QJsonValue::fromVariant(value_list[i]));
        iso_matrix_data = registers;
    }*/

    SecKeyType reg_section_key = SecKeyType(std::move(section), std::move(key));
    if (publish) {
        publishObserver(reg_section_key, OP_TYPE_AUTO);
    }
    emit sigDataModified(reg_section_key);
}

void GlobalData::setIsoRegValueByIso(const QString &section, const QString &key, const QString &op_type, int iso, const QVariant &value, bool publish)
{
    if (section.isEmpty() || key.isEmpty()) {
        return;
    }

    QJsonObject registers = iso_matrix_data;
    setJsonValueHelper(registers, section, key, iso, QJsonValue::fromVariant(value));
    iso_matrix_data = registers;

    SecKeyType reg_section_key = SecKeyType(std::move(section), std::move(key));
    if (publish) {
        publishObserver(reg_section_key, op_type);
    }
    emit sigDataModified(reg_section_key);
}

void GlobalData::setIsoRegValueByIso(const QString &section, const QString &key, int iso, const QVariant &value, bool publish)
{
    if (section.isEmpty() || key.isEmpty()) {
        return;
    }

    QJsonObject registers = iso_matrix_data;//iso_data[iso];
    setJsonValueHelper(registers, section, key, iso, QJsonValue::fromVariant(value));
    iso_matrix_data = registers;

    SecKeyType reg_section_key = SecKeyType(std::move(section), std::move(key));
    if (publish) {
        publishObserver(reg_section_key, OP_TYPE_AUTO);
    }
    emit sigDataModified(reg_section_key);
}

void GlobalData::initIsoRegValue(const QString &section, const QString &key, int array_size, const QVariant &value)
{
    QJsonObject registers = iso_matrix_data;
    initJsonArrayHelper(registers, section, key, array_size, QJsonValue::fromVariant(value));
    iso_matrix_data = registers;
}

void GlobalData::initJsonArrayHelper(QJsonObject &json, const QString &section, const QString &key, int array_size, QJsonValue value)
{
    QJsonObject section_object = json[section].toObject();
    if (!section_object.contains(key)) {
        QJsonObject key_object = section_object[key].toObject();
        QJsonArray value_array = QJsonArray();
        for (int i = 0; i < array_size; i++) {
            value_array.append(value);
        }

        key_object[KEY_VALUE] = value_array;
        section_object[key] = key_object;
        json[section] = section_object;
    }
}

QVariant GlobalData::getRegValue(const QString &section, const QString &key, const QString default_value)
{
    if (section.isEmpty() || key.isEmpty()) {
        return default_value;
    }

    QJsonObject registers = data[REGISTERS_SECTION].toObject();
    return getJsonValueHelper(registers, section, key, default_value).toVariant();
}

QVariant GlobalData::getRegValue(const QString &section, const QString &key, int value_index, const QString default_value)
{
    if (section.isEmpty() || key.isEmpty()) {
        return default_value;
    }

    QJsonObject registers = data[REGISTERS_SECTION].toObject();
    return getJsonValueHelper(registers, section, key, value_index, default_value).toVariant();
}

QVariant GlobalData::getIsoRegValue(const QString &section, const QString &key, int iso, const QString default_value)
{
    if (section.isEmpty() || key.isEmpty() || iso < 0) {
        return default_value;
    }

    QJsonObject registers = iso_matrix_data;//iso_data[iso];
    return getJsonValueHelper(registers, section, key, iso, default_value).toVariant();
}

QJsonObject GlobalData::getRegJson(const QString &section, const QString &key) const
{
    QJsonObject register_obj;
    if (section.isEmpty() || key.isEmpty()) {
        return register_obj;
    }

    QJsonObject registers = data[REGISTERS_SECTION].toObject();
    if (!registers.contains(section)) {
        return register_obj;
    }

    QJsonObject section_obj = registers[section].toObject();
    if (!section_obj.contains(key)) {
        return register_obj;
    }

    return section_obj[key].toObject();
}

QJsonObject GlobalData::getRegisters() const noexcept
{
    return data[REGISTERS_SECTION].toObject();
}

void GlobalData::applyRegsValue(QJsonObject apply_regs)
{
    QList<SecKeyType> changed_reg_section_key_list;
    // apply regs' value
    QJsonObject global_registers = data[REGISTERS_SECTION].toObject();
    QStringList section_list = apply_regs.keys();
    for (auto section : section_list) {
        if (section.isEmpty() || !global_registers.contains(section)) continue;

        QJsonObject global_section_obj = global_registers[section].toObject();
        QJsonObject apply_section_obj = apply_regs[section].toObject();
        QStringList key_list = apply_section_obj.keys();
        for (auto key : key_list) {
            if (!global_section_obj.contains(key)) {
                global_section_obj[key] = apply_section_obj[key];
                changed_reg_section_key_list.append(SecKeyType(section, key));
            } else {
                QJsonObject global_reg = global_section_obj[key].toObject();
                global_reg[KEY_VALUE] = apply_section_obj[key].toObject()[KEY_VALUE];
                global_section_obj[key] = global_reg;
                changed_reg_section_key_list.append(SecKeyType(section, key));
            }
        }
        global_registers[section] = global_section_obj;
    }

    if (!changed_reg_section_key_list.isEmpty()) {
        // apply back to global data
        data[REGISTERS_SECTION] = global_registers;

        // publish each changed reg
        for (auto changed_reg_section_key : changed_reg_section_key_list) {
            publishObserver(changed_reg_section_key, OP_TYPE_MANUAL);
            emit sigDataModified(changed_reg_section_key);
        }
    }
}

bool GlobalData::loadData(const QString &file_path)
{
    QJsonObject load_data;
    QString min_version = getSettings(SETTINGS_SECTION, KEY_MIN_VERSION, MIN_VERSION).toString();
    QString error_msg;
    if (utils::loadJson(file_path, load_data, error_msg) != 0) {
        qDebug() << error_msg;
        return false;
    }

    QString data_file_version = load_data[SETTINGS_SECTION].toObject()[KEY_VERSION].toString();
    if (utils::compareVersion(data_file_version, min_version) >= 0) {
        data = load_data;
        emit sigDataLoaded();
    } else {
        qDebug("Data file version check failed, version %s < %s (min version)", qUtf8Printable(data_file_version), qUtf8Printable(min_version));
    }

    return true;
}

void GlobalData::loadSettings()
{
    qDebug("load settings %s", qUtf8Printable(settings_file_path));
    QString error_msg;
    if (utils::loadJson(settings_file_path, settings, error_msg) != 0) {
        qDebug() << error_msg;
    }
}

void GlobalData::saveSettings()
{
    setSettings(SETTINGS_SECTION, KEY_MIN_VERSION, MIN_VERSION);
    utils::saveJson(settings_file_path, settings);
}

QVariant GlobalData::getSettings(const QString &section, const QString &key, const QVariant default_value)
{
    return getJsonHelper(settings, section, key, QJsonValue::fromVariant(default_value));
}

void GlobalData::setSettings(const QString &section, const QString &key, const QVariant &value)
{
    setJsonHelper(settings, section, key, QJsonValue::fromVariant(value));
}

void GlobalData::dump() const
{
    QJsonDocument doc;
    QString strJson;

    doc.setObject(settings);
    strJson = doc.toJson(QJsonDocument::Indented);
    qDebug("setting:");
    qDebug("%s", qUtf8Printable(strJson));

    doc.setObject(data);
    strJson = doc.toJson(QJsonDocument::Indented);
    qDebug("data:");
    qDebug("%s", qUtf8Printable(strJson));
}

void GlobalData::publishObserver(SecKeyType reg_section_key, QString op_type, IObserver *exclude)
{
    AutoWriteGuard auto_write_guard;
    // update observer data from global data
    for (auto observer : observer_subscribe_list[reg_section_key]) {
        if (exclude != nullptr && observer == exclude)
            continue;

        if (observer->getOpType().isEmpty()) {
            observer->refreshData(reg_section_key);
        } else if (op_type == observer->getOpType()){
            observer->refreshData(reg_section_key);
        }
    }
}

void GlobalData::publishItem(ModuleKeyType item_module_key, QVariant data, QString op_type)
{
    AutoWriteGuard auto_write_guard;
    // set item data and sync to global data
    for (auto item : item_subscribe_list[item_module_key]) {
        // update item data
        if ((item->getOpType().isEmpty()) && (op_type == OP_TYPE_MANUAL)) {
            item->setData(data, true);
        } else if (op_type == item->getOpType()){
            item->setData(data, true);
        }
    }
}

void GlobalData::subscribeObserver(SecKeyType reg_section_key, IObserver *observer)
{
    if (reg_section_key.first.isEmpty() || reg_section_key.second.isEmpty()) {
        return;
    }

    observer_subscribe_list[reg_section_key].append(observer);
}

void GlobalData::unSubscribeObserver(SecKeyType reg_section_key, IObserver *observer)
{
    if (reg_section_key.first.isEmpty() || reg_section_key.second.isEmpty()) {
        return;
    }

    observer_subscribe_list[reg_section_key].removeOne(observer);
}

void GlobalData::subscribeItem(ModuleKeyType &&item_module_key, BaseItemWidget *item)
{
    if (item_module_key.first.isEmpty() || item_module_key.second.isEmpty()) {
        return;
    }

    item_subscribe_list[item_module_key].append(item);
}

void GlobalData::unSubscribeItem(ModuleKeyType item_module_key, BaseItemWidget *item)
{
    if (item_module_key.first.isEmpty() || item_module_key.second.isEmpty()) {
        return;
    }

    item_subscribe_list[item_module_key].removeOne(item);
}

void GlobalData::setLevel(int level)
{
    this->level = level;
}

int GlobalData::getLevel()
{
    return this->level;
}

void GlobalData::setCvipqToolBranch(const QString &branch)
{
    this->cvipq_tool_branch = branch;
}

QString GlobalData::getCvipqToolBranch()
{
    return this->cvipq_tool_branch;
}

void GlobalData::setStatusTemplate(bool status){
    this->changetemplatestatus = status;
}

bool GlobalData::getStatusTemplate(){
    return this->changetemplatestatus;
}

void GlobalData::setCvipqToolSoc(const QString &soc)
{
    this->cvipq_tool_soc = soc;
}

QString GlobalData::getCvipqToolSoc()
{
    return this->cvipq_tool_soc;
}

void GlobalData::setCvipqToolTemplate(const QString &template_name)
{
    this->cvipq_tool_template = template_name;
}

QString GlobalData::getCvipqToolTemplate()
{
    return this->cvipq_tool_template;
}

void GlobalData::setPageMode(int mode)
{
    this->page_mode = mode;
}

int GlobalData::getPageMode()
{
    return this->page_mode;
}

QJsonObject GlobalData::getPageInfo(QString id)
{
    QJsonObject obj;
    if (page_info.contains(id)) {
        obj = page_info[id];
    }
    return obj;
}

QStringList GlobalData::getPageInfo()
{
    return page_info.keys();
}

void GlobalData::insertPageInfo(QString id, QJsonObject page_info_obj)
{
    page_info.insert(id, page_info_obj);
}

void GlobalData::initData()
{
    // load global registers data
    QString filepath = ":/jsons/registers.json";
    QJsonObject regs_obj;
    QString error_msg;
    if (utils::loadJson(filepath, regs_obj, error_msg) != 0) {
        qDebug() << error_msg;
    }

    data[REGISTERS_SECTION] = regs_obj;

    // load global iso data
    QString iso_dir_path = ":/jsons/iso/";
    filepath = iso_dir_path + "iso_matrix.json";
    QJsonObject iso_regs_obj;
    if (utils::loadJson(filepath, iso_regs_obj, error_msg) != 0) {
        qDebug() << error_msg;
    }

    if (iso_regs_obj.contains(REGISTERS_SECTION)) {
        iso_matrix_data = iso_regs_obj[REGISTERS_SECTION].toObject();
    }

    /*for (auto iso : iso_list) {
        filepath = iso_dir_path + NAME_ISO + QString::number(iso) + ".json";
        QJsonObject iso_regs_obj;
        utils::loadJson(filepath, iso_regs_obj);
        if (iso_regs_obj.contains(REGISTERS_SECTION)) {
            iso_data[iso] = iso_regs_obj[REGISTERS_SECTION].toObject();
        }
    }*/
}

void GlobalData::setJsonHelper(QJsonObject &json, const QString &section, const QString &key, QJsonValue value)
{
    QJsonObject sec = json[section].toObject();
    sec[key] = value;
    json[section] = sec;
}

void GlobalData::setJsonArrayHelper(QJsonObject &json, const QString &section, const QString &key, int index, QJsonValue value)
{
    QJsonObject sec = json[section].toObject();
    if (sec[key].isArray()) {
        QJsonArray value_array = sec[key].toArray();
        if (index >= value_array.size()) {
            for (int i = value_array.size(); i <= index; i++) {
                value_array.append(DEFAULT_REG_VALUE);
            }
        }

        value_array[index] = value;
        sec[key] = value_array;
    } else {
        QJsonArray value_array = QJsonArray();
        for (int i = 0; i <= index; i++) {
            value_array.append(DEFAULT_REG_VALUE);
        }
        value_array[index] = value;
        sec[key] = value_array;
    }

    json[section] = sec;
}

void GlobalData::setJsonValueHelper(QJsonObject &json, const QString &section, const QString &key, QJsonValue value)
{
    // set value to object's KEY_VALUE
    QJsonObject sec = json[section].toObject();
    setJsonHelper(sec, key, KEY_VALUE, value);
    json[section] = sec;
}

void GlobalData::setJsonValueHelper(QJsonObject &json, const QString &section, const QString &key, int value_index, QJsonValue value)
{
    // set value to object's KEY_VALUE
    QJsonObject sec = json[section].toObject();
    setJsonArrayHelper(sec, key, KEY_VALUE, value_index, value);
    json[section] = sec;
}

QJsonValue GlobalData::getJsonHelper(QJsonObject &json, const QString &section, const QString &key, QJsonValue default_value)
{
    if (!json.contains(section)) {
        json[section] = QJsonObject();
    }
    if (!json[section].toObject().contains(key)) {
        setJsonHelper(json, section, key, default_value);
    }

    return json[section].toObject()[key];
}

QJsonValue GlobalData::getJsonArrayHelper(QJsonObject &json, const QString &section, const QString &key, int index, QJsonValue default_value)
{
    if (!json.contains(section)) {
        json[section] = QJsonObject();
    }
    if (!json[section].toObject().contains(key)) {
        setJsonArrayHelper(json, section, key, index, default_value);
    }

    QJsonArray value_array = json[section].toObject()[key].toArray();

    return (index < value_array.size()) ? value_array[index] : default_value;
}

QJsonValue GlobalData::getJsonValueHelper(QJsonObject &json, const QString &section, const QString &key, QJsonValue default_value)
{
    // get object's KEY_VALUE
    QJsonObject sec = json[section].toObject();
    return getJsonHelper(sec, key, KEY_VALUE, default_value);
}

QJsonValue GlobalData::getJsonValueHelper(QJsonObject &json, const QString &section, const QString &key, int value_idx, QJsonValue default_value)
{
    // get object's KEY_VALUE
    QJsonObject sec = json[section].toObject();
    return getJsonArrayHelper(sec, key, KEY_VALUE, value_idx, default_value);
}

void GlobalData::handleDataChanged(QString cmd)
{
    if (CMD_READ_REGISTERS == cmd) {
        QString data = QString(NetworkClientHelper::getInstance()->popCmdData(cmd));
        QJsonArray data_array = utils::jsonArrayFromString(data);
        if (!data_array.isEmpty()) {
            for (auto data : data_array) {
                QJsonObject data_obj = data.toObject();
                QString section = data_obj[KEY_REG_SECTION].toString();
                QString key = data_obj[KEY_REG_KEY].toString();
                QString bitmask = data_obj[KEY_REG_BITMASK].toString();

                unsigned int value = utils::fromU32MaskValue( data_obj[KEY_VALUE].toString().toUInt(), bitmask);
                setRegValue(section, key, value);
            }
        } else {
            QJsonObject data_obj = utils::jsonObjectFromString(data);
            if (!data_obj.isEmpty()) {
                QString section = data_obj[KEY_REG_SECTION].toString();
                QString key = data_obj[KEY_REG_KEY].toString();
                QString bitmask = data_obj[KEY_REG_BITMASK].toString();

                unsigned int value = utils::fromU32MaskValue( data_obj[KEY_VALUE].toString().toUInt(), bitmask);
                setRegValue(section, key, value);
            }
        }
    } else if (CMD_READ_MODULE == cmd) {
        QString data_str = QString(NetworkClientHelper::getInstance()->popCmdData(cmd));
        QJsonObject data = utils::jsonObjectFromString(data_str);
        QString module = data[MSG_KEY_MODULE].toString();
        QJsonArray items = data[MSG_KEY_ITEMS].toArray();

        for (auto item : items) {
            QJsonObject item_obj = item.toObject();
            QString key = item_obj[KEY_REG_KEY].toString();
            QString op_type = item_obj[KEY_REG_OP_TYPE].toString();
            QVariant value = item_obj[KEY_VALUE].toVariant();

            publishItem(ModuleKeyType(module, key), value, op_type);
        }

        if (module == "Exposure Info") {
            emit sigCheckGainSepStatus();
        }
    } else {
        NetworkClientHelper::getInstance()->popCmdData(cmd);
    }
}

quint8 GlobalData::GetWDRFusionFrame()
{
    return fusionFrame;
}

quint8 GlobalData::CalculateWDRFusionFrame(WDR_MODE_E eWDRMode)
{
    quint8 u8WDRFusionFrame = 1;

    switch (eWDRMode) {
    case WDR_MODE_4To1_LINE:
    case WDR_MODE_4To1_FRAME:
    case WDR_MODE_4To1_FRAME_FULL_RATE:
        u8WDRFusionFrame = 4;
        break;
    case WDR_MODE_3To1_LINE:
    case WDR_MODE_3To1_FRAME:
    case WDR_MODE_3To1_FRAME_FULL_RATE:
        u8WDRFusionFrame = 3;
        break;
    case WDR_MODE_2To1_LINE:
    case WDR_MODE_2To1_FRAME:
    case WDR_MODE_2To1_FRAME_FULL_RATE:
        u8WDRFusionFrame = 2;
        break;
    default:
        u8WDRFusionFrame = 1;
        break;
    }

    return u8WDRFusionFrame;
}

void GlobalData::handleReceiveWDRMode(int id)
{
    QJsonObject obj = NetworkClientHelper::getInstance()->popCmdDataJsonrpc(id);
    QJsonObject result_obj = obj[MSG_KEY_RESULT].toObject();
    if (result_obj.isEmpty()) {
        qDebug("Get wdrmode failed!");
        return;
    }

    QJsonObject params_obj = result_obj[MSG_KEY_PARAMS].toObject();
    WDR_MODE_E mode = (WDR_MODE_E)params_obj["wdrmode"].toInt();
    fusionFrame = CalculateWDRFusionFrame(mode);
    emit sigGetWDRFusionFrame();
    qDebug("Get wdr mode:%d, fusionFrame:%d\n", mode, fusionFrame);

    getChipInfo();
}

void GlobalData::getChipInfo()
{
    QJsonObject id_info;
    id_info[MSG_KEY_CMD] = CMD_GET_CHIP_INFO;
    int id = GlobalData::getInstance()->getJsonrpcId();
    NetworkClientHelper::getInstance()->pushIdInfoJsonrpc(id, id_info);

    QJsonObject send_obj;
    send_obj[MSG_KEY_METHOD] = METHOD_GET_CHIP_INFO;
    send_obj[MSG_KEY_ID] = id;
    send_obj[MSG_KEY_JSONRPC] = MSG_JSONRPC_VERSION;
    NetworkClientHelper::getInstance()->sendCmd(std::move(send_obj));
}

void GlobalData::getToolJson()
{
    NetworkClientHelper::getInstance()->setCallerInfo(0, NULL);

    QJsonObject id_info;
    id_info[MSG_KEY_CMD] = CMD_GET_TOOL_JSON;
    int id = GlobalData::getInstance()->getJsonrpcId();
    NetworkClientHelper::getInstance()->pushIdInfoJsonrpc(id, id_info);

    QJsonObject param_obj;
    param_obj["content"] = TOOL_JSON_DATA;
    QJsonObject send_obj;
    send_obj[MSG_KEY_METHOD] = METHOD_PREPARE_BIN;
    send_obj[MSG_KEY_ID] = id;
    send_obj[MSG_KEY_JSONRPC] = MSG_JSONRPC_VERSION;
    send_obj[MSG_KEY_PARAMS] = param_obj;
    NetworkClientHelper::getInstance()->sendCmd(std::move(send_obj));
}

void GlobalData::handleToolJsonStatus(int id)
{
    QJsonObject obj = NetworkClientHelper::getInstance()->popCmdDataJsonrpc(id);
    QJsonObject result_obj = obj[MSG_KEY_RESULT].toObject();
    if (result_obj.isEmpty()) {
        QString error_msg = "Failed to get template from board. Please check if \"pqtool_definition.json\" exists on the board.";
        emit sigStatusLog(error_msg, LOG_LEVEL::LOG_ERROR);
        setBoardTooljsonState(JSON_NONE);
        return;
    }

    QJsonObject params_obj = result_obj[MSG_KEY_PARAMS].toObject();
    int recvSize = params_obj["Data size"].toInt();
    NetworkClientHelper::getInstance()->setCallerInfo(recvSize, CMD_GET_TOOL_JSON);
    qDebug("Set receive size:%d\n", recvSize);

    int temp_id = GlobalData::getInstance()->getJsonrpcId();
    QJsonObject param_obj;
    param_obj["content"] = TOOL_JSON_DATA;
    QJsonObject send_obj;
    send_obj[MSG_KEY_METHOD] = METHOD_GET_BIN_DATA;
    send_obj[MSG_KEY_ID] = temp_id;
    send_obj[MSG_KEY_JSONRPC] = MSG_JSONRPC_VERSION;
    send_obj[MSG_KEY_PARAMS] = param_obj;
    NetworkClientHelper::getInstance()->sendCmd(std::move(send_obj));
}

void GlobalData::handleToolJsonData(QString cmd)
{
    NetworkClientHelper::getInstance()->setCallerInfo(0, NULL);
    QByteArray data = NetworkClientHelper::getInstance()->popCmdData(cmd);

    if (data.isEmpty()) {
        QString error_msg = "Template file is empty.";
        emit sigStatusLog(error_msg, LOG_LEVEL::LOG_ERROR);
        return;
    }

    QString tempName = TEMPLATES_FOLDER + "tempTool.json";
    QFile file(tempName);
    file.open(QIODevice::WriteOnly);
    file.write(data);
    file.close();

    QJsonObject load_data;
    QString error_msg = "Template contents don't meet json standards. ";
    if (utils::loadJson(tempName, load_data, error_msg) != 0) {
        emit sigStatusLog(error_msg, LOG_LEVEL::LOG_ERROR);
        return;
    }

    QJsonObject device = load_data["DEVICE"].toObject();
    QString fullName = device["FULL_NAME"].toString();
    QString sdkVersion = device["SDK_VERSION"].toString();
    QString ispBranch = device["ISP_BRANCH"].toString();
    if ((fullName == "phobos") || (fullName == "cv180x")) {
        fullName = CV180X_SOC;
    } else if ((fullName == "mars") || (fullName == "cv181x")) {
        fullName = CV181X_SOC;
    } else if (fullName == "cv182x") {
        fullName = CV182X_SOC;
    }

    QString fileName = TEMPLATES_FOLDER + fullName + "_" + ispBranch + "_" + sdkVersion + ".json";
    QFile::remove(fileName);
    QFile::rename(tempName, fileName);

    emit sigStatusLog("Get template from board successfully.", LOG_LEVEL::LOG_INFO);
    emit sigUpdateTemplate(fileName);
    setBoardTooljsonState(JSON_EXIST);
}

void GlobalData::setRestoreFlag(bool flag)
{
    restore_flag = flag;
}

bool GlobalData::getRestoreFlag()
{
    return restore_flag;
}

void GlobalData::setReadingPagesNumber(int number)
{
    reading_pages_number = number;
}

int GlobalData::getReadingPagesNumber()
{
    return reading_pages_number;
}

int GlobalData::initPageInfoJsonrpc()
{
    QString filepath;
    page_info.clear();
    observer_subscribe_list.clear();
    item_subscribe_list.clear();
    path_to_item_map.clear();
    filepath = TEMPLATES_FOLDER + cvipq_tool_template;

    QFileInfo fileInfo(filepath);
    if (!filepath.isEmpty() && fileInfo.exists()) {
        QJsonObject tool_obj;
        QString error_msg = "File Path = " + filepath + ". ";
        if (utils::loadJson(filepath, tool_obj, error_msg) != 0) {
            emit sigStatusLog(error_msg, LOG_LEVEL::LOG_ERROR);
        } else {
            creatModuleOrderInfo(tool_obj);
            QJsonArray pages_info_array;
            if (utils::createPagesInfo(tool_obj, pages_info_array, error_msg) != 0) {
                emit sigStatusLog(error_msg, LOG_LEVEL::LOG_ERROR);
                return -1;
            }
            creatTopModuleTitleMap(tool_obj);

            creatModuleDefaultInfo(pages_info_array);
            if (utils::rearrangeLayout(tool_obj, pages_info_array, error_msg) != 0) {
                emit sigStatusLog(error_msg, LOG_LEVEL::LOG_ERROR);
                return -1;
            }

            for (int i = 0; i < pages_info_array.count(); i++) {
                QJsonObject page_info_obj = pages_info_array.at(i).toObject();
                QString page_name = page_info_obj["page_name"].toString();
                page_info.insert(page_name, page_info_obj);
            }
        }
    }

    return 0;
}

void GlobalData::creatModuleOrderInfo(QJsonObject &tool_json)
{
    module_order_info.clear();
    page_ready_list.clear();
    page_status.clear();
    QJsonObject functions_obj = tool_json["FUNCTIONS"].toObject();
    QJsonArray order_arr = functions_obj["ORDER"].toArray();
    QJsonArray fun_arr = functions_obj["FUNCTION"].toArray();

    for (int i = 0; i < order_arr.size(); i++) {
        QJsonObject obj = order_arr[i].toObject();
        QStringList keys = obj.keys();
        QJsonArray modules = obj[keys[0]].toArray();
        if (!modules.isEmpty()) {
            QStringList module_list;
            for (int j = 0; j < modules.size(); j++) {
                module_list.append(modules[j].toString());
            }
            module_order_info.append(ClassModulesType(keys[0], module_list));
        }
    }

    for (int i = 0; i < fun_arr.size(); i++) {
        QJsonObject obj = fun_arr[i].toObject();
        bool ready = obj["READY"].toBool();
        if (ready) {
            page_ready_list.append(obj["ID"].toString());
            page_status.insert(obj["ID"].toString(), false);
        }
    }
}

void GlobalData::creatTopModuleTitleMap(QJsonObject &tool_json)
{
    topTitle_moduleTitle_map.clear();

    QJsonObject functions_obj = tool_json["FUNCTIONS"].toObject();

    if (!functions_obj.contains("TOP_SWITCH")) {
        return;
    }

    QJsonArray map_info_arr = functions_obj["TOP_SWITCH"].toArray();

    for (int i = 0; i < map_info_arr.size(); i++) {
        QJsonObject obj = map_info_arr[i].toObject();
        QString top_title = obj["TOP_TITLE"].toString();
        QString module_name = obj["MODULE"].toString();
        QString module_title = obj["MODULE_TITLE"].toString();

        topTitle_moduleTitle_map.insert(top_title, QPair<QString, QString>(module_name, module_title));
    }
}

QMap<QString, QPair<QString, QString>> GlobalData::getTopModuleTitleMap()
{
    return topTitle_moduleTitle_map;
}

QJsonObject GlobalData::getPageData(QString id)
{
    QJsonObject obj;
    if (page_data.contains(id)) {
        obj = page_data[id];
    }
    return obj;
}

void GlobalData::setPageData(QString id, QJsonObject data)
{
    page_data.remove(id);
    page_data.insert(id, data);
}

QJsonObject GlobalData::getGammaData()
{
    return gammaObj;
}

void GlobalData::setGammaData(QJsonObject data)
{
    gammaObj = data;
}

bool GlobalData::getPageStatus(QString id)
{
    bool status = false;
    if (page_status.contains(id)) {
        status = page_status[id];
    }
    return status;
}

void GlobalData::setPageStatus(QString id, bool status)
{
    page_status[id] = status;
}

void GlobalData::creatModuleDefaultInfo(QJsonArray &obj)
{
    for (int i = 0; i < obj.size(); i++) {
        QJsonObject page_tmp;
        QJsonObject page_info = obj[i].toObject();
        if (!page_info["ready"].toBool()) {
            continue;
        }
        QString name = page_info["page_name"].toString();
        page_tmp["struct"] = page_info["struct"].toArray();
        if (page_info.contains("mapping")) {
            QJsonObject mapping_obj = page_info["mapping"].toObject();
            QStringList keys_list = mapping_obj.keys();
            QJsonObject structmapping_obj;
            for (int j = 0; j < keys_list.count(); j++) {
                QStringList temp_list = keys_list[j].split(".");
                QString struct_str = temp_list[0];
                QString mapping_path;
                QString mapping_struct = mapping_obj[keys_list[j]].toString();
                for (int k = 1; k < temp_list.count(); k++) {
                    if (!temp_list[k].contains("PQT_")) {
                        if (mapping_path.isEmpty())
                            mapping_path.append(temp_list[k]);
                        else
                            mapping_path.append("." + temp_list[k]);
                    }
                }

                QJsonObject mapping_obj = structmapping_obj[struct_str].toObject();
                mapping_obj.insert(mapping_path, mapping_struct);
                structmapping_obj.insert(struct_str, mapping_obj);
            }
            page_tmp["mapping"] = structmapping_obj;
        }
        QJsonArray method_arr = page_info["method"].toArray();
        page_tmp["method"] = method_arr;
        page_tmp["ro"] = false;
        for(int j = 0; j < method_arr.size(); j++) {
            QJsonObject method_obj = method_arr[j].toObject();
            if (method_obj.contains("SET") && method_obj["SET"].toString().isEmpty()) {
                page_tmp["ro"] = true;
            }
        }

        QJsonObject group_data;
        QJsonArray group_arr = page_info["groups"].toArray();
        for (int j = 0; j < group_arr.size(); j++) {
            QJsonObject group_obj = group_arr[j].toObject();
            QJsonArray item_arr = group_obj["content"].toArray();
            QJsonObject item_data_group;
            for (int k = 0; k < item_arr.size(); k++) {
                QJsonObject item_obj = item_arr[k].toObject();
                QJsonObject item_data;
                item_data["PATH"] = item_obj["key"].toString();
                if (item_obj.contains("chmod") && item_obj["chmod"] == "ReadOnly") {
                    item_data["RO"] = true;
                }

                if (item_obj.contains("value_type")) {
                    item_data["value_type"] = item_obj["value_type"];
                }

                if (item_obj["type"] == "spinner" || item_obj["type"] == "double_spinner") {
                    int min = item_obj["param"].toObject()["min"].toDouble();
                    int max = item_obj["param"].toObject()["max"].toDouble();
                    item_data["VALUE"] = qBound(min, 0, max);
                } else if (item_obj["type"] == "univ_matrix") {
                    QJsonArray count = item_obj["param"].toObject()["count"].toArray();
                    int row = count[0].toInt();
                    int col = count[1].toInt();
                    QJsonValue min = item_obj["param"].toObject()["min"];
                    QJsonValue max = item_obj["param"].toObject()["max"];
                    bool bIsCCMSumMember = utils::isMatchCCMSumTitleWithPrefix(item_obj["key"].toString());
                    if (bIsCCMSumMember) {
                        col = col - 1;
                    }
                    if (min.isArray() && max.isArray()) {
                        QJsonArray min_arr = min.toArray();
                        QJsonArray max_arr = max.toArray();
                        QJsonArray tmp_arr;
                        if (min_arr.size() == max_arr.size() && min_arr.size() == col) {
                            for (int n = 0; n < row; n++) {
                                for (int m = 0; m < col; m++) {
                                    tmp_arr.append(qBound(min[m].toInt(), 0, max[m].toInt()));
                                }
                            }
                        } else if (min_arr.size() == max_arr.size() && min_arr.size() == row) {
                            for (int n = 0; n < row; n++) {
                                for (int m = 0; m < col; m++) {
                                    tmp_arr.append(qBound(min[n].toInt(), 0, max[n].toInt()));
                                }
                            }
                        } else {
                            for (int n = 0; n < row; n++) {
                                for (int m = 0; m < col; m++) {
                                    tmp_arr.append(0);
                                }
                            }
                        }
                        item_data["VALUE"] = tmp_arr;
                    } else {
                        int size = row * col;
                        QJsonArray tmp_arr;
                        for (int n = 0; n < size; n++) {
                            tmp_arr.append(qBound(min.toInt(), 0, max.toInt()));
                        }
                        item_data["VALUE"] = tmp_arr;
                    }
                } else {
                    item_data["VALUE"] = 0;
                }
                item_data_group.insert(item_obj["key"].toString(), item_data);
                if (name == MODULE_NAME_GAMMA && item_data["PATH"].toString() == "ISP_GAMMA_ATTR_S.Table") {
                    gammaObj.insert("PQT_GAMMA.DeaultTable", item_data["VALUE"]);
                    gammaObj.insert("PQT_GAMMA.sRGBTable", item_data["VALUE"]);
                    gammaObj.insert("change", false);
                }
            }
            group_data[group_obj["section"].toString()] = item_data_group;
        }
        page_tmp["data"] = group_data;
        page_tmp["change"] = false;
        page_data[name] = page_tmp;
    }
}

QList<ClassModulesType> GlobalData::getModuleOrderInfo()
{
    return module_order_info;
}

QStringList GlobalData::getPageReadyList()
{
    return page_ready_list;
}

QStringList GlobalData::getModuleOrderInfo(QString key)
{
    QStringList module_list;
    for (int i = 0; i < module_order_info.size(); i++) {
        if (module_order_info[i].first == key) {
            module_list = module_order_info[i].second;
            break;
        }
    }

    return module_list;
}

void GlobalData::setModuleOrderInfo(QList<ClassModulesType> order)
{
    module_order_info.clear();
    module_order_info = order;
}

void GlobalData::handleDataChangedJsonrpc(int id)
{
    QJsonObject data_obj = NetworkClientHelper::getInstance()->popCmdDataJsonrpc(id);
    if (!data_obj.isEmpty()) {
        QJsonObject result_obj = data_obj[MSG_KEY_RESULT].toObject();
        QJsonObject info_obj = data_obj[MSG_KEY_ID_INFO].toObject();
        QString cmd = info_obj[MSG_KEY_CMD].toString();
        QString module = info_obj[MSG_KEY_MODULE].toString();
        QString struct_id = info_obj[MSG_KEY_STRUCT].toString();
        QJsonObject params_obj;

        if ((CMD_READ_MODULE == cmd) && (!result_obj.isEmpty())) {
            if (result_obj[MSG_KEY_PARAMS].isString()) {
                params_obj = utils::jsonObjectFromString(result_obj[MSG_KEY_PARAMS].toString());
            } else {
                params_obj = result_obj[MSG_KEY_PARAMS].toObject();
            }
            // Todo: Remap params according tool json definition.
            QJsonObject mapping_obj;
            QString struct_id_origin = struct_id;
            if (info_obj.contains(MSG_KEY_MAPPING)) {
                mapping_obj = info_obj[MSG_KEY_MAPPING].toObject();
                QStringList path_list = mapping_obj.keys();
                if ((path_list.count() == 1) && (path_list[0].left(4) == "ISP_")) {
                    // AE Route
                    struct_id = struct_id + '.' + path_list[0];
                    params_obj = utils::remapParamsFromRead(mapping_obj[path_list[0]].toString(), params_obj);
                    mapping_obj = QJsonObject();
                } else {
                    // Exposure Info
                }
            }

            if (struct_id == QString("ISP_CMOS_NOISE_CALIBRATION_S")) {
                params_obj = utils::convertNoiseProfileParamFormat(params_obj);
            }
            QJsonObject param_data;
            utils::parseParams(struct_id, params_obj, mapping_obj, param_data);
            if (page_status[module]) {
                QStringList paths = param_data.keys();
                for (int i = 0; i < paths.size(); i++) {
                    QString path = paths[i];
                    QVariant value = param_data[path].toVariant();
                    publishItemJsonrpc(paths[i], value);
                }
            } else {
                savePageData(module, struct_id_origin, param_data);
            }
            if (module == "VO Gamma") {
                emit sigReceiveGamma();
            }
        }

        if ((module == "Exposure Info") || (module == "Exposure attr") || (module == "Statistics Config")) {
            QString result = result_obj.isEmpty() ? "fail" : "success";
            emit sigModuleRWResult(module, cmd, result);
        }
    } else {
        qDebug("GlobalData::handleDataChangedJsonrpc. Error: data_obj is empty.");
    }
}

void GlobalData::loadPageData(QString id)
{
    if (id == MODULE_NAME_GAMMA) {
        if (gammaObj["change"].toBool()) {
            publishItemJsonrpc("PQT_GAMMA.DeaultTable", gammaObj["PQT_GAMMA.DeaultTable"].toVariant());
            publishItemJsonrpc("PQT_GAMMA.sRGBTable", gammaObj["PQT_GAMMA.sRGBTable"].toVariant());
        }
    }

    QJsonObject pageObj = page_data[id];
    if (!pageObj["change"].toBool())
        return;

    QJsonObject pageData = pageObj["data"].toObject();
    QStringList struct_list = pageData.keys();
    QStringList keysCur = data.keys();
    for (int i = 0; i < struct_list.size(); i++) {
        QString struct_id = struct_list[i];
        QJsonObject groupData = pageData[struct_id].toObject();
        QStringList keys = groupData.keys();
        for (int j = 0; j < keys.size(); j++) {
            QString path = keys[j];
            QJsonObject itemObj = groupData[path].toObject();
            QVariant value = itemObj["VALUE"].toVariant();
            publishItemJsonrpc(path, value);
        }
    }

    if (id == MODULE_NAME_VO_GAMMA) {
        emit sigReceiveGamma();
    }
}

void GlobalData::savePageData(QString id, QString struct_id, QJsonObject data)
{
    QJsonObject pageObj = page_data[id];
    QJsonObject pageData = pageObj["data"].toObject();
    QJsonObject groupData = pageData[struct_id].toObject();
    QStringList keysCur = data.keys();
    QStringList keysPre = groupData.keys();
    for (int i = 0; i < keysCur.size(); i++) {
        for (int j = 0; j < keysPre.size(); j++) {
            if (keysPre[j] == keysCur[i]) {
                QJsonObject itemObj = groupData[keysPre[j]].toObject();
                itemObj.insert("VALUE", data[keysCur[i]]);
                groupData.insert(keysPre[j], itemObj);
                break;
            }
        }
    }
    pageData.insert(struct_id, groupData);
    pageObj.insert("data", pageData);
    pageObj.insert("change", true);
    page_data.insert(id, pageObj);
}

void GlobalData::handleReceiveGammaJsonrpc(int id)
{
    QJsonObject data_obj = NetworkClientHelper::getInstance()->popCmdDataJsonrpc(id);
    if (!data_obj.isEmpty()) {
        QJsonObject result_obj = data_obj[MSG_KEY_RESULT].toObject();
        QJsonObject info_obj = data_obj[MSG_KEY_ID_INFO].toObject();
        QString cmd = info_obj[MSG_KEY_CMD].toString();
        QString module = info_obj[MSG_KEY_MODULE].toString();
        int curve_type = info_obj["curve_type"].toInt();
        if ((CMD_READ_GAMMA == cmd) && (!result_obj.isEmpty())) {
            QJsonObject params_obj = result_obj[MSG_KEY_PARAMS].toObject();
            QJsonValue value = params_obj["Table"].toArray();
            if (curve_type == 0) {
                if (page_status[MODULE_NAME_GAMMA]) {
                    publishItemJsonrpc("PQT_GAMMA.DeaultTable", value.toVariant());
                } else {
                    gammaObj.insert("PQT_GAMMA.DeaultTable", value);
                    gammaObj.insert("change", true);
                }
            } else if (curve_type == 1) {
                if (page_status[MODULE_NAME_GAMMA]) {
                    publishItemJsonrpc("PQT_GAMMA.sRGBTable", value.toVariant());
                } else {
                    gammaObj.insert("PQT_GAMMA.sRGBTable", value);
                    gammaObj.insert("change", true);
                }
            } else if (curve_type == 2) {
                if (page_status[MODULE_NAME_GAMMA]) {
                    publishItemJsonrpc("ISP_GAMMA_ATTR_S.Table", value.toVariant());
                } else {
                    QJsonObject gammaData = page_data[MODULE_NAME_GAMMA];
                    QJsonObject pageData = gammaData["data"].toObject();
                    QJsonObject groupData = pageData["ISP_GAMMA_ATTR_S"].toObject();
                    QJsonObject itemData = groupData["ISP_GAMMA_ATTR_S.Table"].toObject();
                    itemData.insert("VALUE", value);
                    groupData.insert("ISP_GAMMA_ATTR_S.Table", itemData);
                    pageData.insert("ISP_GAMMA_ATTR_S", groupData);
                    gammaData.insert("data", pageData);
                    gammaData.insert("change", true);
                    page_data.insert(MODULE_NAME_GAMMA, gammaData);
                }
            }
        }
    } else {
         qDebug("GlobalData::handleReceiveGammaJsonrpc. Error: data_obj is empty.");
    }
}

void GlobalData::initUinvMatrixRegValue(const QString &section, const QString &key, const QJsonValue &value)
{
    QJsonObject section_obj = iso_matrix_data[section].toObject();
    QJsonObject key_obj = section_obj[key].toObject();
    key_obj[KEY_VALUE] = value;
    section_obj[key] = key_obj;
    iso_matrix_data[section] = section_obj;
}

QJsonValue GlobalData::getUnivMatrixRegValue(const QString &section, const QString &key)
{
    QJsonValue section_obj = iso_matrix_data[section].toObject();
    QJsonObject key_obj = section_obj[key].toObject();
    QJsonValue value = key_obj[KEY_VALUE];

    return value;
}

void GlobalData::setUnivMatrixRegValue(const QString &section, const QString &key, QJsonValue value, bool publish)
{
    QJsonObject section_obj = iso_matrix_data[section].toObject();
    QJsonObject key_obj = section_obj[key].toObject();
    key_obj[KEY_VALUE] = value;
    section_obj[key] = key_obj;
    iso_matrix_data[section] = section_obj;

    SecKeyType reg_section_key = SecKeyType(std::move(section), std::move(key));
    if (publish) {
        for (int i = 0; i < observer_subscribe_list[reg_section_key].count(); i++)
            observer_subscribe_list[reg_section_key][i]->refreshData(reg_section_key);
    }

    emit sigDataModified(reg_section_key);
}

void GlobalData::subscribeItemJsonrpc(QString path, BaseItemWidget *item)
{
    if (!path.isEmpty()) {
        path_to_item_map[path].append(item);
    } else {
        qDebug("GlobalData::subscribeItemJsonrpc. Error: path is empty.");
    }
}

void GlobalData::unsubscribeItemJsonrpc(QString path, BaseItemWidget *item)
{
    if (!path.isEmpty()) {
        path_to_item_map[path].removeOne(item);
    } else {
        qDebug("GlobalData::unsubscribeItemJsonrpc. Error: path is empty");
    }
}

void GlobalData::publishItemJsonrpc(QString path, QVariant data)
{
    // set item data and sync to global data
    for (int i = 0;  i < path_to_item_map[path].count(); i++) {
        // Todo: Use setDataJsonrpc to set data.
        BaseItemWidget *item = path_to_item_map[path][i];
        if (item->getPathList().count() == 1)
            item->setData(data, true);
        else
            item->setDataJsonrpc(path, data, true);
    }
}

void GlobalData::setCurrentHighlightItem(BaseItemWidget *item)
{
    current_highlight_item = item;
}

BaseItemWidget* GlobalData::getCurrentHighlightItem()
{
    return current_highlight_item;
}

void GlobalData::setPreviousSearchString(QString str)
{
    previous_search_string = str;
}

QString& GlobalData::getPreviousSearchString()
{
    return previous_search_string;
}

int GlobalData::getSearchTimesAtSamePage()
{
    return search_same_page_times;
}

void GlobalData::setSearchTimesAtSamePage(int search_times)
{
    search_same_page_times = search_times;
}

int GlobalData::getMatchSearchStringTimes()
{
    return match_string_times;
}

void GlobalData::setMatchSearchStringTimes(int times)
{
    match_string_times = times;
}

void GlobalData::incMatchSearchStringTimes()
{
    match_string_times++;
}

void GlobalData::setSearchPageOrderIndx(int index)
{
    search_page_order_idx = index;
}

int GlobalData::getSearchPageOrderIndx()
{
    return search_page_order_idx;
}

void GlobalData::setPageOrderInfo(QStringList list)
{
    page_order = list;
}

QStringList GlobalData::getPageOrderInfo()
{
    return page_order;
}

void GlobalData::incSearchRecursiveCallTimes()
{
    search_recursive_call_times++;
}

void GlobalData::resetSearchRecursiveCallTimes()
{
    search_recursive_call_times = 0;
}

int GlobalData::getSearchRecursiveCallTimes()
{
    return search_recursive_call_times;
}

void GlobalData::resetSearchState()
{
    if (current_highlight_item != nullptr) {
        current_highlight_item->updateHighlight(false);
        current_highlight_item = nullptr;
    }
    previous_search_string.clear();
    search_same_page_times = 0;
    match_string_times = 0;
    search_page_order_idx = 0;
    search_recursive_call_times = 0;
}

void GlobalData::setBoardTooljsonState(ToolJsonState state)
{
    boardExistTooljson = state;
}

ToolJsonState GlobalData::getBoardTooljsonState()
{
    return boardExistTooljson;
}

