#include "register_tool_window.hpp"
#include <QCompleter>
#include <QRegExp>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <QWidget>
#include <QFileDialog>
#include <QMessageBox>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QStringList>
#include <QDebug>
#include <QStringListModel>
#include <QTime>
#include "network_client_helper.hpp"
#include "global_data.hpp"

#define REG_INFO_NUM 7
#define REG_WAIT_GET_TIME 2

RegToolWindow::RegToolWindow(QWidget *parent) : QMainWindow(parent), getIsHex(false), setIsHex(false) {
    setWindowTitle("Register Configuration Tool");
    setMinimumSize(800, 600);
    timer = new QTimer(this);
    setupUI();
    setupCompleters();
    initEvent();
}

void RegToolWindow::setupUI() {
    // Main layout
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    setCentralWidget(centralWidget);

    // Import button
    importBtn = new QPushButton("Import Register Json File", this);
    mainLayout->addWidget(importBtn);
    mainLayout->addSpacing(10);

    // Module and register selection group
    QGroupBox *selectGroup = new QGroupBox("Module & Register Selection", this);
    QFormLayout *selectLayout = new QFormLayout(selectGroup);

    // Module selection area - combo box with search
    QWidget *moduleWidget = new QWidget(this);
    QHBoxLayout *moduleLayout = new QHBoxLayout(moduleWidget);
    moduleLayout->setContentsMargins(0, 0, 0, 0);

    moduleCombo = new QComboBox(this);
    moduleSearchEdit = new QLineEdit(this);
    moduleSearchEdit->setPlaceholderText("Search modules (supports regex)");
    moduleSearchEdit->setStyleSheet("border:2px groove gray;border-radius:10px;padding:2px 4px");

    moduleSearchBtn = new QPushButton(this);
    moduleSearchBtn->setStyleSheet("QPushButton{image:url(:/icons/search.png); background:transparent;}");

    moduleLayout->addWidget(moduleCombo);
    moduleLayout->addSpacing(20);
    moduleLayout->addWidget(moduleSearchEdit);
    moduleLayout->addWidget(moduleSearchBtn);
    moduleLayout->addSpacing(5);

    moduleLayout->setStretch(0, 5);
    moduleLayout->setStretch(1, 0);
    moduleLayout->setStretch(2, 4);
    moduleLayout->setStretch(3, 1);

    // Register selection area - combo box with search
    QWidget *regWidget = new QWidget(this);
    QHBoxLayout *regLayout = new QHBoxLayout(regWidget);
    regLayout->setContentsMargins(0, 0, 0, 0);

    regCombo = new QComboBox(this);
    regSearchEdit = new QLineEdit(this);
    regSearchEdit->setPlaceholderText("Search registers (supports regex)");
    regSearchEdit->setStyleSheet("border:2px groove gray;border-radius:10px;padding:2px 4px");

    regSearchBtn = new QPushButton(this);
    regSearchBtn->setStyleSheet("QPushButton{image:url(:/icons/search.png); background:transparent;}");

    regLayout->addWidget(regCombo);
    regLayout->addSpacing(20);
    regLayout->addWidget(regSearchEdit);
    regLayout->addWidget(regSearchBtn);
    regLayout->addSpacing(5);

    regLayout->setStretch(0, 5);
    regLayout->setStretch(1, 0);
    regLayout->setStretch(2, 4);
    regLayout->setStretch(3, 1);

    selectLayout->addRow("Module:", moduleWidget);
    selectLayout->addRow("Register:", regWidget);
    mainLayout->addWidget(selectGroup);
    mainLayout->addSpacing(15);

    // Register information display area
    QGroupBox *infoGroup = new QGroupBox("Register Information", this);
    QVBoxLayout *infoLayout = new QVBoxLayout(infoGroup);
    regInfoText = new QPlainTextEdit(this);
    regInfoText->setReadOnly(true);
    infoLayout->addWidget(regInfoText);
    mainLayout->addWidget(infoGroup);
    mainLayout->addSpacing(10);

    // Register operation area
    QGroupBox *operateGroup = new QGroupBox("Register Operations", this);
    QGridLayout *operateLayout = new QGridLayout(operateGroup);

    getvalueEdit = new QLineEdit(this);
    getvalueEdit->setReadOnly(true);
    getBtn = new QPushButton("Get Register Value", this);
    getRadixBtn = new QPushButton("Dec -> Hex", this);

    setvalueEdit = new QLineEdit(this);
    setBtn = new QPushButton("Set Register Value", this);
    setRadixBtn = new QPushButton("Dec -> Hex", this);

    operateLayout->addWidget(getBtn, 0, 0);
    operateLayout->addWidget(getvalueEdit, 0, 1);
    operateLayout->addWidget(getRadixBtn, 0, 2);
    operateLayout->addWidget(setBtn, 1, 0);
    operateLayout->addWidget(setvalueEdit, 1, 1);
    operateLayout->addWidget(setRadixBtn, 1, 2);

    operateLayout->setColumnStretch(0, 3);
    operateLayout->setColumnStretch(1, 5);
    operateLayout->setColumnStretch(2, 2);

    mainLayout->addWidget(operateGroup);

    // Add stretch to push content up
    //mainLayout->addStretch();
    mainLayout->addSpacing(15);

    // Connect signals and slots
    connect(importBtn, &QPushButton::clicked, this, &RegToolWindow::importJson);
    connect(moduleCombo, &QComboBox::currentTextChanged, this, &RegToolWindow::onModuleChanged);
    connect(regCombo, &QComboBox::currentTextChanged, this, &RegToolWindow::onRegChanged);
    connect(getBtn, &QPushButton::clicked, this, &RegToolWindow::onGetClicked);
    connect(setBtn, &QPushButton::clicked, this, &RegToolWindow::onSetClicked);
    connect(moduleSearchEdit, &QLineEdit::textChanged, this, &RegToolWindow::onModuleSearchChanged);
    connect(regSearchEdit, &QLineEdit::textChanged, this, &RegToolWindow::onRegSearchChanged);
    connect(setvalueEdit, &QLineEdit::textChanged, this, &RegToolWindow::onValueTextChanged);
    connect(getRadixBtn, &QPushButton::clicked, this, &RegToolWindow::onGetRadixChanged);
    connect(setRadixBtn, &QPushButton::clicked, this, &RegToolWindow::onSetRadixChanged);
}

void RegToolWindow::initEvent()
{
    connect(timer, &QTimer::timeout, this, &RegToolWindow::handleTimeOut);
    connect(NetworkClientHelper::getInstance().get(), &NetworkClientHelper::sigRegGetStatus, this,
            &RegToolWindow::handleGetStatus);
    connect(NetworkClientHelper::getInstance().get(), &NetworkClientHelper::sigRegSetStatus, this,
            &RegToolWindow::handleSetStatus);
}

void RegToolWindow::setupCompleters() {
    // Setup module completer
    moduleCompleter = new QCompleter(allModuleNames, this);
    moduleCompleter->setCaseSensitivity(Qt::CaseInsensitive);
    moduleCompleter->setCompletionMode(QCompleter::PopupCompletion);
    moduleSearchEdit->setCompleter(moduleCompleter);
    //connect(moduleCompleter, &QCompleter::activated, this, &RegToolWindow::onModuleCompletionSelected);
    connect(moduleCompleter, static_cast<void (QCompleter::*)(const QString &)>(&QCompleter::activated),
        this, &RegToolWindow::onModuleCompletionSelected);

    // Setup register completer
    regCompleter = new QCompleter(currentRegNames, this);
    regCompleter->setCaseSensitivity(Qt::CaseInsensitive);
    regCompleter->setCompletionMode(QCompleter::PopupCompletion);
    regSearchEdit->setCompleter(regCompleter);
    //connect(regCompleter, &QCompleter::activated, this, &RegToolWindow::onRegCompletionSelected);
    connect(regCompleter, static_cast<void (QCompleter::*)(const QString &)>(&QCompleter::activated),
            this, &RegToolWindow::onRegCompletionSelected);
}

void RegToolWindow::importJson() {
    QString filePath = QFileDialog::getOpenFileName(this, "Select JSON File", "", "JSON Files (*.json)");
    if (filePath.isEmpty()) return;

    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Error", "Failed to open file: " + file.errorString());
        return;
    }

    QByteArray data = file.readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (doc.isNull()) {
        QMessageBox::warning(this, "Error", "JSON parsing failed!");
        return;
    }

    QJsonObject rootObj = doc.object();
    modules.clear();
    regMap.clear();
    allModuleNames.clear();

    for (auto baseIt = rootObj.begin(); baseIt != rootObj.end(); ++baseIt) {
        QString baseAddr = baseIt.key();
        QJsonObject moduleObj = baseIt.value().toObject();
        QString moduleName = moduleObj["module"].toString();

        if (moduleName.isEmpty()) {
            QMessageBox::warning(this, "Error", "Missing required field: \"module\"!");
            return;
        }

        modules[moduleName] = baseAddr;
        allModuleNames.append(moduleName);

        for (auto offsetIt = moduleObj.begin(); offsetIt != moduleObj.end(); ++offsetIt) {
            QString offset = offsetIt.key();
            if (offset == "module") continue;

            if (offsetIt.value().isArray()) {
                QJsonArray regArray = offsetIt.value().toArray();
                for (const auto &regValue : regArray) {
                    QJsonArray regInfoArray = regValue.toArray();
                    if (regInfoArray.size() < REG_INFO_NUM) {
                        qWarning() << "Invalid register information format, skipping!";
                        continue;
                    }
                    RegisterInfo regInfo;

                    regInfo.name = regInfoArray[0].toString();
                    regInfo.bitStart = regInfoArray[1].toInt();
                    regInfo.bitEnd = regInfoArray[2].toInt();
                    regInfo.defaultValue = regInfoArray[3].toInt();
                    regInfo.offset = offset;
                    regInfo.fullAddr = regInfoArray[5].toString();
                    regInfo.comment = regInfoArray[6].toString();
                    regInfo.baseAddr = baseAddr;

                    regMap[moduleName].append(regInfo);
                }
            }
        }
    }

    // Update module combo box and completer
    moduleCombo->clear();
    moduleCombo->addItems(allModuleNames);
    moduleCompleter->setModel(new QStringListModel(allModuleNames, this));

    QMessageBox::information(this, "Success", QString("Successfully loaded %1 modules!").arg(modules.size()));
}

void RegToolWindow::onModuleChanged(const QString &moduleName) {
    regCombo->clear();
    regInfoText->clear();
    getvalueEdit->clear();
    setvalueEdit->clear();
    regSearchEdit->clear();
    current_get_value = 0;

    currentRegNames.clear();

    if (regMap.contains(moduleName)) {
        for (const auto &reg : regMap[moduleName]) {
            regCombo->addItem(reg.name);
            currentRegNames.append(reg.name);
        }
    }

    // Update register completer with new list
    regCompleter->setModel(new QStringListModel(currentRegNames, this));
}

void RegToolWindow::onRegChanged(const QString &regName) {
    getvalueEdit->clear();
    setvalueEdit->clear();
    current_get_value = 0;
    QString moduleName = moduleCombo->currentText();

    if (regMap.contains(moduleName)) {
        for (const auto &reg : regMap[moduleName]) {
            if (reg.name == regName) {
                QString info;
                info += QString("(1) Register name:\t %1\n\n").arg(reg.name);
                info += QString("(2) Module base address:\t %1\n\n").arg(reg.baseAddr);
                info += QString("(3) Register offset:\t %1\n\n").arg(reg.offset);
                info += QString("(4) Register full address:\t %1\n\n").arg(reg.fullAddr);
                info += QString("(5) Bit field range:\t %1 - %2\n\n").arg(reg.bitStart).arg(reg.bitEnd);
                info += QString("(6) Default value:\t %1\n\n").arg(reg.defaultValue);
                info += QString("(7) Description:\n%1").arg(reg.comment);
                regInfoText->setPlainText(info);

                break;
            }
        }
    }
}

void RegToolWindow::onModuleSearchChanged(const QString &searchText) {
    QString currentModule = moduleCombo->currentText();
    QStringList allModules = modules.keys();
    QStringList filteredModules;

    if (searchText.isEmpty()) {
        filteredModules = allModules;
    } else {
        // Fix 1: Use QRegExp::PatternSyntax::RegExp2 for better regex support
        QRegExp regExp(searchText, Qt::CaseInsensitive, QRegExp::RegExp2);

        // Fix 2: Use indexIn() instead of exactMatch() for partial matches
        for (const QString &module : allModules) {
            if (regExp.indexIn(module) != -1) {  // -1 means no match
                filteredModules << module;
            }
        }
    }

    // Update combo box
    moduleCombo->clear();
    moduleCombo->addItems(filteredModules);

    // Update completer with filtered results
    updateModuleCompleter(filteredModules);

    // Restore previous selection if possible
    int index = moduleCombo->findText(currentModule);
    if (index >= 0) {
        moduleCombo->setCurrentIndex(index);
    }
}

void RegToolWindow::updateModuleCompleter(const QStringList &items) {
    QStringListModel *model = new QStringListModel(items, this);
    moduleCompleter->setModel(model);
}

void RegToolWindow::updateRegCompleter(const QStringList &items) {
    QStringListModel *model = new QStringListModel(items, this);
    regCompleter->setModel(model);
}

void RegToolWindow::onRegSearchChanged(const QString &searchText) {
    QString moduleName = moduleCombo->currentText();
    if (moduleName.isEmpty() || !regMap.contains(moduleName)) {
        return;
    }

    QString currentReg = regCombo->currentText();
    QStringList allRegs;
    for (const auto &reg : regMap[moduleName]) {
        allRegs << reg.name;
    }

    QStringList filteredRegs;
    if (searchText.isEmpty()) {
        filteredRegs = allRegs;
    } else {
        // Fix 1: Use QRegExp::PatternSyntax::RegExp2 for better regex support
        QRegExp regExp(searchText, Qt::CaseInsensitive, QRegExp::RegExp2);

        // Fix 2: Use indexIn() instead of exactMatch() for partial matches
        for (const QString &reg : allRegs) {
            if (regExp.indexIn(reg) != -1) {  // -1 means no match
                filteredRegs << reg;
            }
        }
    }

    // Update combo box
    regCombo->clear();
    regCombo->addItems(filteredRegs);

    // Update completer with filtered results
    updateRegCompleter(filteredRegs);

    // Restore previous selection if possible
    int index = regCombo->findText(currentReg);
    if (index >= 0) {
        regCombo->setCurrentIndex(index);
    }
}

void RegToolWindow::onModuleCompletionSelected(const QString &text) {
    int index = moduleCombo->findText(text);
    if (index >= 0) {
        moduleCombo->setCurrentIndex(index);
    }
    moduleSearchEdit->clear();
}

void RegToolWindow::onRegCompletionSelected(const QString &text) {
    int index = regCombo->findText(text);
    if (index >= 0) {
        regCombo->setCurrentIndex(index);
    }
    regSearchEdit->clear();
}

void RegToolWindow::onValueTextChanged(const QString &text) {
    if (text.isEmpty()) return;

    bool ok;
    uint32_t value;

    if (setIsHex) {
        // Handle hexadecimal input
        value = text.toUInt(&ok, 16);
        if (!ok) {
            // Try to remove non-hex characters
            QString hexText = text;
            hexText = hexText.replace(QRegExp("[^0-9a-fA-FxX]"), "");
            if (!hexText.isEmpty()) {
                setvalueEdit->setText(hexText);
            } else {
                setvalueEdit->clear();
            }
            return;
        }
        bool has0xPrefix = text.startsWith("0x", Qt::CaseInsensitive);

        if (!has0xPrefix) {
            setvalueEdit->setText("0x" + text);
        }
    } else {
        // Handle decimal input
        value = text.toUInt(&ok, 10);
        if (!ok) {
            QString numericText = text;
            numericText = numericText.replace(QRegExp("[^0-9]"), "");
            if (!numericText.isEmpty()) {
                setvalueEdit->setText(numericText);
            } else {
                setvalueEdit->clear();
            }
            return;
        }
    }

    QString moduleName = moduleCombo->currentText();
    QString regName = regCombo->currentText();

    if (moduleName.isEmpty() || regName.isEmpty()) {
        return;
    }

    int bitStart = -1, bitEnd = -1;
    if (regMap.contains(moduleName)) {
        for (const auto &reg : regMap[moduleName]) {
            if (reg.name == regName) {
                bitStart = reg.bitStart;
                bitEnd = reg.bitEnd;
                break;
            }
        }
    }

    if (bitStart < 0 || bitEnd < 0 || bitStart > bitEnd) {
        return;
    }

    uint32_t bitWidth = bitEnd - bitStart + 1;
    uint32_t maxValue = (1ULL << bitWidth) - 1;

    if (value > maxValue) {
        if (setIsHex) {
            setvalueEdit->setText("0x" + QString::number(maxValue, 16).toUpper());
        } else {
            setvalueEdit->setText(QString::number(maxValue));
        }
    }
}

void RegToolWindow::onGetClicked() {
    if (!NetworkClientHelper::getInstance()->isNormalStatus()) {
        QMessageBox::warning(this, tr("Warning"), tr("Network is not connected!"), QMessageBox::Ok);
        return;
    }

    QString moduleName = moduleCombo->currentText();
    QString regName = regCombo->currentText();
    QString fullAddress;

    if (moduleName.isEmpty() || regName.isEmpty()) {
        QMessageBox::warning(this, "Warning", "Please import register JSON file first!");
        return;
    }

    if (regMap.contains(moduleName)) {
        for (const auto &reg : regMap[moduleName]) {
            if (reg.name == regName) {
                fullAddress = reg.fullAddr;
                break;
            }
        }
    }

    if (fullAddress.isEmpty()) {
        QMessageBox::warning(this, "Warning", "Could not find register address!");
        return;
    }
     QString get_addr = fullAddress;

    NetworkClientHelper::getInstance()->setCallerInfo(0, NULL);

    QJsonObject id_info;
    id_info[MSG_KEY_CMD] = CMD_REG_GET;
    int temp_id = GlobalData::getInstance()->getJsonrpcId();
    NetworkClientHelper::getInstance()->pushIdInfoJsonrpc(temp_id, id_info);

    QJsonObject param_obj;
    param_obj["REG_GET_ADDR"] = get_addr;

    QJsonObject send_obj;
    send_obj[MSG_KEY_METHOD] = METHOD_REG_GET;
    send_obj[MSG_KEY_ID] = temp_id;
    send_obj[MSG_KEY_JSONRPC] = MSG_JSONRPC_VERSION;
    send_obj[MSG_KEY_PARAMS] = param_obj;
    NetworkClientHelper::getInstance()->sendCmd(std::move(send_obj), false);
    timer->start(3000);

    disableComponents();
}

void RegToolWindow::onSetClicked() {
    if (!NetworkClientHelper::getInstance()->isNormalStatus()) {
        QMessageBox::warning(this, tr("Warning"), tr("Network is not connected!"), QMessageBox::Ok);
        return;
    }

    QString moduleName = moduleCombo->currentText();
    QString regName = regCombo->currentText();
    QString valueStr = setvalueEdit->text();

    if (moduleName.isEmpty() || regName.isEmpty()) {
        QMessageBox::warning(this, "Warning", "Please import register JSON file first!");
        return;
    }

    if (valueStr.isEmpty()) {
        QMessageBox::warning(this, "Warning", "Please enter a value to set!");
        return;
    }

    bool ok;
    int value;

    if (setIsHex) {
        value = valueStr.toInt(&ok, 16);
    } else {
        value = valueStr.toInt(&ok, 10);
    }

    if (!ok) {
        QMessageBox::warning(this, "Warning", "Please enter a valid integer!");
        return;
    }

    QString fullAddress;

    if (regMap.contains(moduleName)) {
        for (const auto &reg : regMap[moduleName]) {
            if (reg.name == regName) {
                fullAddress = reg.fullAddr;
                break;
            }
        }
    }

    if (fullAddress.isEmpty()) {
        QMessageBox::warning(this, "Warning", "Could not find register address!");
        return;
    }

    // first get the 32bit value
    QJsonObject id_info;
    id_info[MSG_KEY_CMD] = CMD_REG_GET;
    int temp_id = GlobalData::getInstance()->getJsonrpcId();
    NetworkClientHelper::getInstance()->pushIdInfoJsonrpc(temp_id, id_info);

    QJsonObject param_obj;
    QString get_addr = fullAddress;

    param_obj["REG_GET_ADDR"] = get_addr;

    QJsonObject send_obj;
    send_obj[MSG_KEY_METHOD] = METHOD_REG_GET;
    send_obj[MSG_KEY_ID] = temp_id;
    send_obj[MSG_KEY_JSONRPC] = MSG_JSONRPC_VERSION;
    send_obj[MSG_KEY_PARAMS] = param_obj;

    // before send the cmd, set the flag
    get_ok_flag = 0;

    NetworkClientHelper::getInstance()->sendCmd(std::move(send_obj));

    bool gotResponse = false;
    QTime stopTime = QTime::currentTime().addSecs(REG_WAIT_GET_TIME);

    while (QTime::currentTime() < stopTime) {
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
        if (!GlobalData::getInstance()->getWaitForResponse()) {
        gotResponse = true;
        break;
        }
    }

    if (!gotResponse) {
        NetworkClientHelper::getInstance()->removeIdInfoJsonrpc(temp_id);
        GlobalData::getInstance()->setWaitForResponse(false);
        QMessageBox::warning(this, "Warning", "Fail to get the register value. Timeout: Can't get reponse from board within %2 seconds.");
        return;
    }

    // check the flag
    stopTime = QTime::currentTime().addSecs(REG_WAIT_GET_TIME);

    while (QTime::currentTime() < stopTime) {
        if (get_ok_flag == 1) {
            break;
        }
    }

    if (get_ok_flag == 0) {
        QMessageBox::warning(this, "Warning", "Set fail! Because can not get the register value from board!");
        return;
    }

    // second change the value
    int bitStart = -1, bitEnd = -1;

    if (regMap.contains(moduleName)) {
        for (const auto &reg : regMap[moduleName]) {
            if (reg.name == regName) {
                bitStart = reg.bitStart;
                bitEnd = reg.bitEnd;
                break;
            }
        }
    }

    if (bitStart < 0 || bitEnd < 0 || bitStart > bitEnd) {
        QMessageBox::warning(this, tr("Warning"),
             tr("Invalid bit range (start: %1, end: %2)").arg(bitStart).arg(bitEnd), QMessageBox::Ok);
        return;
    }
    // reg_32_bit_value: pravite member
    uint32_t bitWidth = bitEnd - bitStart + 1;
    uint32_t clearMask = ~(((1 << bitWidth) - 1) << bitStart);
    uint32_t targetValue = (reg_32_bit_value & clearMask) | (static_cast<uint32_t>(value) << bitStart);

    // constrcut set cmd
    QString set_addr = fullAddress;
    QString set_value = "0x" + QString::number(targetValue, 16);

    id_info[MSG_KEY_CMD] = CMD_REG_SET;
    temp_id = GlobalData::getInstance()->getJsonrpcId();
    NetworkClientHelper::getInstance()->pushIdInfoJsonrpc(temp_id, id_info);

    param_obj["REG_SET_ADDR"] = set_addr;
    param_obj["REG_SET_VAL"] = set_value;

    send_obj[MSG_KEY_METHOD] = METHOD_REG_SET;
    send_obj[MSG_KEY_ID] = temp_id;
    send_obj[MSG_KEY_JSONRPC] = MSG_JSONRPC_VERSION;
    send_obj[MSG_KEY_PARAMS] = param_obj;
    NetworkClientHelper::getInstance()->sendCmd(std::move(send_obj), false);
    timer->start(3000);
}

void RegToolWindow::handleGetStatus(int id)
{
    timer->stop();
    enableComponents();

    QJsonObject obj = NetworkClientHelper::getInstance()->popCmdDataJsonrpc(id);
    QJsonObject result_obj = obj[MSG_KEY_RESULT].toObject();
    if (result_obj.isEmpty()) {
        QMessageBox::warning(this, tr("warning"), tr("Reg read failed!"), QMessageBox::Ok);
        return;
    }

    QJsonObject params_obj = result_obj[MSG_KEY_PARAMS].toObject();
    QString hexStr = params_obj["Reg value"].toString();

    if (hexStr.isEmpty()) {
        QMessageBox::warning(this, tr("Warning"), tr("No data received!"), QMessageBox::Ok);
        return;
    }

    bool ok;
    uint32_t regValue = hexStr.toUInt(&ok, 16);
    if (!ok) {
        reg_32_bit_value = 0;
        current_get_value = 0;
        QMessageBox::warning(this, tr("Warning"), tr("Invalid hex data: %1").arg(hexStr), QMessageBox::Ok);
        return;
    } else {
        reg_32_bit_value = regValue;
    }

    int bitStart = -1, bitEnd = -1;
    QString regName = regCombo->currentText();
    QString moduleName = moduleCombo->currentText();

    if (regMap.contains(moduleName)) {
        for (const auto &reg : regMap[moduleName]) {
            if (reg.name == regName) {
                bitStart = reg.bitStart;
                bitEnd = reg.bitEnd;
                break;
            }
        }
    }

    if (bitStart < 0 || bitEnd < 0 || bitStart > bitEnd) {
        QMessageBox::warning(this, tr("Warning"),
             tr("Invalid bit range (start: %1, end: %2)").arg(bitStart).arg(bitEnd), QMessageBox::Ok);
        return;
    }

    uint32_t bitWidth = bitEnd - bitStart + 1;
    uint32_t mask = ((1 << bitWidth) - 1) << bitStart;
    current_get_value = (regValue & mask) >> bitStart;

    updateGetValueDisplay();
    get_ok_flag = 1;
}

void RegToolWindow::handleSetStatus(int id)
{
    timer->stop();
    enableComponents();

    QJsonObject obj = NetworkClientHelper::getInstance()->popCmdDataJsonrpc(id);
    QJsonObject result_obj = obj[MSG_KEY_RESULT].toObject();
    if (result_obj.isEmpty()) {
        QMessageBox::warning(this, tr("warning"), tr("Reg Set failed!"), QMessageBox::Ok);
        return;
    }
}

void RegToolWindow::handleTimeOut()
{
    timer->stop();
    enableComponents();
    QMessageBox::warning(this, "warning", "Timeout, can not get response from board!", QMessageBox::Ok);
}

void RegToolWindow::disableComponents()
{
    moduleCombo->setEnabled(false);
    regCombo->setEnabled(false);
    regInfoText->setEnabled(false);
    getvalueEdit->setEnabled(false);
    setvalueEdit->setEnabled(false);
    getBtn->setEnabled(false);
    setBtn->setEnabled(false);
    importBtn->setEnabled(false);
    moduleSearchEdit->setEnabled(false);
    regSearchEdit->setEnabled(false);
    getRadixBtn->setEnabled(false);
    setRadixBtn->setEnabled(false);
}

void RegToolWindow::enableComponents()
{
    moduleCombo->setEnabled(true);
    regCombo->setEnabled(true);
    regInfoText->setEnabled(true);
    getvalueEdit->setEnabled(true);
    setvalueEdit->setEnabled(true);
    getBtn->setEnabled(true);
    setBtn->setEnabled(true);
    importBtn->setEnabled(true);
    moduleSearchEdit->setEnabled(true);
    regSearchEdit->setEnabled(true);
    getRadixBtn->setEnabled(true);
    setRadixBtn->setEnabled(true);
}

void RegToolWindow::onGetRadixChanged()
{
    getIsHex = !getIsHex;
    getRadixBtn->setText(getIsHex ? "Hex -> Dec" : "Dec -> Hex");
    updateGetValueDisplay();
}

void RegToolWindow::onSetRadixChanged()
{
    QString currentText = setvalueEdit->text();

    setIsHex = !setIsHex;
    if (!currentText.isEmpty()) {
        bool ok;
        uint32_t value;

        if (!setIsHex) {
            // Convert from hex to decimal
            value = currentText.toUInt(&ok, 16);
        } else {
            // Convert from decimal to hex
            value = currentText.toUInt(&ok, 10);
        }

        if (ok) {
            setvalueEdit->setText(setIsHex ? "0x" + QString::number(value, 16).toUpper() : QString::number(value));
        }
    }
    setRadixBtn->setText(setIsHex ? "Hex -> Dec" : "Dec -> Hex");
}

void RegToolWindow::updateGetValueDisplay()
{
    if (getIsHex) {
        getvalueEdit->setText("0x" + QString::number(current_get_value, 16).toUpper());
    } else {
        getvalueEdit->setText(QString::number(current_get_value));
    }
}
