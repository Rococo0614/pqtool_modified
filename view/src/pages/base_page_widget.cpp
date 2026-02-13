#include "main_window.hpp"
#include "base_page_widget.hpp"
#include "network_client_helper.hpp"
#include "global_data.hpp"
#include "ini_helper.hpp"
#include "config_set_widget.hpp"

#include <QLabel>
#include <QPushButton>
#include <QtCore>
#include <QScrollArea>
#include <QQuickWidget>
#include <QQuickItem>
#include <QCheckBox>
#include <QMessageBox>
#include <QShortcut>
#include <QApplication>

QMap<QString, QList<class BinaryItemWidget *>> BinaryItemWidget::map_module_binaryItems;

BasePageWidget::BasePageWidget(QString id, QWidget *parent) : QWidget(parent)
{
    setProperty("module_id", QVariant(id));
}

QString BasePageWidget::getTitle()
{
    return this->title;
}

void BasePageWidget::setTitle(const QString &title)
{
    this->title = title;
}

void BasePageWidget::clearDirty()
{
    foreach (BaseItemWidget *base_item_widget, base_item_list) {
        // Reset base item ui
        base_item_widget->uiReset();
    }
}

void BasePageWidget::initUI()
{
    main_layout = new QHBoxLayout;

    initLeftArea();
    initCenterArea();
    initRightArea();

    // UI stretch
    main_layout->setStretch(0, 3);
    main_layout->setStretch(1, 6);
    main_layout->setStretch(2, 1);

    setLayout(main_layout);

    retranslateUi();

    initManuAutoWidgets();

    handleShortCut();
}

void BasePageWidget::initData()
{
}

void BasePageWidget::initLeftArea()
{
}

void BasePageWidget::initCenterArea()
{
    // interaction
    QWidget *interact_widget = new QWidget;
    QVBoxLayout *integration_layout = new QVBoxLayout;
    interact_widget->setLayout(integration_layout);
    QLabel *resister_desp = new QLabel(tr("This is center session."));
    integration_layout->addWidget(resister_desp);
    main_layout->addWidget(interact_widget);
}

void BasePageWidget::showEvent(QShowEvent *event)
{
    QWidget::showEvent(event);

    updateUI();
}

void BasePageWidget::changeEvent(QEvent *event)
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

void BasePageWidget::initGroupLeftArea()
{
    QWidget *scroll_widget = new QWidget();
    QGridLayout *grid_layout = new QGridLayout(scroll_widget);
    grid_layout->setSizeConstraint(QLayout::SetFixedSize);
    grid_layout->setSpacing(8);
    grid_layout->setMargin(8);

    QList<GroupItemWidget *> *gs = initGroupWidgets();
    int column = 0;

    for (auto group : *gs) {
        grid_layout->addWidget(group, 0, column);
        column++;
    }

    scroll_area = new QScrollArea();
    scroll_area->setWidget(scroll_widget);

    main_layout->addWidget(scroll_area);
}

int BasePageWidget::getGroupTitleMaxLen(const QJsonArray items_json)
{
    QFont currentFont = QApplication::font();
    QFontMetrics fm(currentFont);
    QString iMaxTitle;
    int iMaxTitleLen = 0;

    for (int j = 0; j < items_json.count(); j++) {
        QJsonObject item_json = items_json[j].toObject();
        QString title = item_json["title"].toString();

        if (item_json.contains(KEY_REG_OP_TYPE)) {
            QString title_optype = item_json[KEY_REG_OP_TYPE].toString();
            title = title_optype + "." + title;
        }
        if (title.size() > iMaxTitleLen) {
            iMaxTitle = title;
            iMaxTitleLen = title.size();
        }
    }

    QRect rec = fm.boundingRect(iMaxTitle);

    return rec.width() * 1.1;
}

QList<GroupItemWidget *> *BasePageWidget::initGroupWidgets()
{
    QList<GroupItemWidget *> *list = new QList<GroupItemWidget *>;
    QString id = property("module_id").toString();
    QJsonObject obj = GlobalData::getInstance()->getPageInfo(id);

    if (obj.contains("ready")) {
        this->ready_flag = obj["ready"].toBool();
    }

    if (obj.contains("struct")) {
        QJsonArray struct_array = obj["struct"].toArray();
        for (int i = 0; i < struct_array.count(); i++)
            struct_list.append(struct_array[i].toString());
    }

    if (obj.contains("method")) {
        QJsonArray method_array = obj["method"].toArray();
        for (int i = 0; i < method_array.count(); i++) {
            QJsonObject method_obj = method_array[i].toObject();
            QString sturct_str = method_obj["STRUCT"].toString();
            if (method_obj["SET"].toString().isEmpty())
                this->ro_page = true;

            if (!sturct_str.isEmpty()) {
                struct_get_api_map.insert(sturct_str, method_obj["GET"].toString());
                struct_set_api_map.insert(sturct_str, method_obj["SET"].toString());
            }
        }
    }

    if (obj.contains("mapping")) {
        QJsonObject mapping_obj = obj["mapping"].toObject();
        QStringList keys_list = mapping_obj.keys();
        for (int i = 0; i < keys_list.count(); i++) {
            QStringList temp_list = keys_list[i].split(".");
            QString struct_str = temp_list[0];
            QString mapping_path;
            QString mapping_struct = mapping_obj[keys_list[i]].toString();
            for (int j = 1; j < temp_list.count(); j++) {
                if (!temp_list[j].contains("PQT_")) {
                    if (mapping_path.isEmpty())
                        mapping_path.append(temp_list[j]);
                    else
                        mapping_path.append("." + temp_list[j]);
                }
            }

            QJsonObject mapping_obj = struct_mapping_map[struct_str];
            mapping_obj.insert(mapping_path, mapping_struct);
            struct_mapping_map[struct_str] = mapping_obj;
        }
    }

    QJsonArray groups_json = obj["groups"].toArray();
    for (int i = 0; i < groups_json.count(); i++) {
        QJsonObject group_obj = groups_json[i].toObject();
        QString group_name = group_obj["group_name"].toString();
        QJsonArray items_json = group_obj["content"].toArray();
        QString reg_section = group_obj[KEY_REG_SECTION].toString();
        bool bTitleWidthCfg = false;
        int same_level_cnt = 0;
        int last_item_level = 0;
        int min_level = 0;

        if (group_obj.contains(KEY_TITLE_LABEL_WIDTH)) {
            bTitleWidthCfg = true;
        }

        GroupItemWidget *group_widget = new GroupItemWidget(group_name);
        QSizePolicy sp_retain = group_widget->sizePolicy();
        sp_retain.setRetainSizeWhenHidden(false);
        group_widget->setSizePolicy(sp_retain);

        group_widget->setRegSection(reg_section);
        if (group_obj.contains(KEY_REG_LEVEL)) {
            group_widget->setLevel(group_obj[KEY_REG_LEVEL].toInt());
        }

        int iTitleLen = getGroupTitleMaxLen(items_json);
        // handle reg items
        for (int j = 0; j < items_json.count(); j++) {
            QJsonObject item_json = items_json[j].toObject();
            if ((!item_json.contains(KEY_REG_SECTION)) && (!reg_section.isEmpty())) {
                item_json[KEY_REG_SECTION] = reg_section;
            }

            if (bTitleWidthCfg) {
                item_json[KEY_TITLE_LABEL_WIDTH] = group_obj[KEY_TITLE_LABEL_WIDTH].toInt();
            }

            BaseItemWidget *widget = RegisterItemFactory::createRegisterItemByJson(id, item_json, iTitleLen, this);
            group_widget->addPageWidget(widget, this);
            base_item_list.append(widget);

            /*find min level to set current group level.*/
            if (j == 0) {
                min_level = widget->getLevel();
                last_item_level = widget->getLevel();
            } else {
                if (widget->getLevel() < min_level) {
                    min_level = widget->getLevel();
                }
            }

            if (widget->getLevel() == last_item_level) {
                same_level_cnt++;
            }
            last_item_level = widget->getLevel();
        }

        if (same_level_cnt == items_json.count()) {
            /*set group when all items is same level.*/
            group_widget->setLevel(last_item_level);
        } else {
            group_widget->setLevel(min_level);
        }

        if (items_json.count() != 0) {
            list->append(group_widget);
        }
    }

    return list;
}

void BasePageWidget::updateUI()
{
    // update visible
    foreach (BaseItemWidget *base_item_widget, base_item_list) {
        base_item_widget->updateVisible();
    }

    QList<GroupItemWidget *> group_widget_list = this->findChildren<GroupItemWidget *>();
    foreach (auto group_widget, group_widget_list) {
        group_widget->updateVisible();
    }

    if (optype_widget) {
        handleOpTypeChanged(optype_widget->getData());
    }
}

void BasePageWidget::handleShortCut()
{
    new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_R), this, SLOT(readData()));
    new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_W), this, SLOT(writeData()));
    new QShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_R), this, SLOT(pageReadAll()));
    new QShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_W), this, SLOT(pageWriteAll()));
}

void BasePageWidget::initRightArea()
{
    QString id = property("module_id").toString();

    // buttons
    QWidget *control_buttons_widget = new QWidget;
    QVBoxLayout *control_buttons_layout = new QVBoxLayout;
    control_buttons_widget->setLayout(control_buttons_layout);

    env_value_checkbox = new QCheckBox("Ev show");
    iso_label = new QLabel(ISO_DEFAULT_VALUE);
    iso_label->setMinimumWidth(300);
    lv_label = new QLabel(LV_DEFAULT_VALUE);
    lv_label->setMinimumWidth(300);

    control_buttons_layout->addStretch(1);
    if (id == MODULE_NAME_LDC) {
        ldc_bin_send_checkbox = new QCheckBox("BinWay");
        ldc_bin_send_checkbox->setChecked(true);
        control_buttons_layout->addWidget(ldc_bin_send_checkbox);

        connect(NetworkClientHelper::getInstance().get(), &NetworkClientHelper::sigReceiveLdcSize, this, &BasePageWidget::receiveLdcSize);
        connect(NetworkClientHelper::getInstance().get(), &NetworkClientHelper::sigSendLDCBinData, this, &BasePageWidget::sendLdcBinData);
    } else {
        control_buttons_layout->addWidget(lv_label);
        control_buttons_layout->addWidget(iso_label);
        control_buttons_layout->addWidget(env_value_checkbox);
    }

    QLabel *auto_write_label = new QLabel("Auto write");
    auto_write_label->setMinimumWidth(100);
    auto_write_label->setMaximumHeight(15);
    control_buttons_layout->addStretch(2);
    control_buttons_layout->addWidget(auto_write_label);
    auto_write_checkbox = new QCheckBox("Enable");
    control_buttons_layout->addWidget(auto_write_checkbox);
    int auto_write = GlobalData::getInstance()->getSettings(SETTINGS_SECTION, KEY_AUTO_WRITE, 0).toInt();
    if ((auto_write > 0) && !ro_page) {
        auto_write_checkbox->setChecked(true);
    }
    int ev_show = GlobalData::getInstance()->getSettings(SETTINGS_SECTION, KEY_EV_SHOW, 0).toBool();
    if (ev_show) {
        env_value_checkbox->setChecked(true);
    } else {
        iso_label->setText("");
        lv_label->setText("");
    }

    connect(auto_write_checkbox, &QCheckBox::clicked,
            this, &BasePageWidget::handleAutoWriteChanged);
    connect(env_value_checkbox, &QCheckBox::toggled,
            this, &BasePageWidget::handleEvShowChanged);

    /// TODO: qml widget disappear for release version
//    QQuickWidget *switch_qml_widget = new QQuickWidget;
//    switch_qml_widget->setSource(QUrl("qrc:/qml/switch.qml"));
//    switch_qml_widget->setMaximumHeight(60);
//    control_buttons_layout->addWidget(switch_qml_widget);
//    QQuickItem *switch_item = switch_qml_widget->rootObject()->findChild<QQuickItem *>("autoWriteSwitch");
//    if (switch_item) {
//        connect(switch_item, SIGNAL(valueChanged(int)), this, SLOT(handleAutoWriteChanged(int)));
//        int auto_write = GlobalData::getInstance()->getSettings(SETTINGS_SECTION, KEY_AUTO_WRITE, 0).toInt();
//        switch_item->setProperty("checked", auto_write);
//    }

    QStringList texts;
    for (int i : lv_list) {
        texts.append(NAME_LV + QString::number(i));
    }
    for (int i = 0; i < iso_list.count(); i++) {
        texts[i] = NAME_ISO + QString::number(iso_list[i]) + QString("/") + texts[i];
    }
    texts.append("Manual");

    QLabel *apply_iso_label = new QLabel("Copy From");
    apply_iso_label->setMinimumWidth(100);
    apply_iso_label->setMaximumHeight(15);
    control_buttons_layout->addStretch(2);
    control_buttons_layout->addWidget(apply_iso_label);
    copy_from_cbox = new QComboBox();
    copy_from_cbox->setMinimumWidth(150);
    copy_from_cbox->addItems(texts);
    control_buttons_layout->addWidget(copy_from_cbox);

    copy_apply_btn = new QPushButton();
    control_buttons_layout->addStretch(1);
    control_buttons_layout->addWidget(copy_apply_btn);
    connect(copy_apply_btn, &QPushButton::clicked, this, &BasePageWidget::handleCopyApply);

    read_all_btn = new QPushButton();
    control_buttons_layout->addStretch(3);
    control_buttons_layout->addWidget(read_all_btn);
    connect(read_all_btn, &QPushButton::clicked, this, &BasePageWidget::pageReadAll);

    write_all_btn = new QPushButton();
    control_buttons_layout->addStretch(3);
    control_buttons_layout->addWidget(write_all_btn);
    connect(write_all_btn, &QPushButton::clicked, this, &BasePageWidget::pageWriteAll);

    read_btn = new QPushButton();
    control_buttons_layout->addStretch(3);
    control_buttons_layout->addWidget(read_btn);
    connect(read_btn, &QPushButton::clicked, this, &BasePageWidget::readData);

    write_btn = new QPushButton();
    control_buttons_layout->addStretch(3);
    control_buttons_layout->addWidget(write_btn);
    control_buttons_layout->addStretch(3);
    connect(write_btn, &QPushButton::clicked, this, &BasePageWidget::writeData);
    connect(write_btn, &QPushButton::clicked, this, &BasePageWidget::doRecord);

    control_buttons_widget->setMinimumWidth(100);
    main_layout->addWidget(control_buttons_widget);

    if (ro_page) {
        auto_write_checkbox->setDisabled(true);
        write_all_btn->setDisabled(true);
        write_btn->setDisabled(true);
        copy_apply_btn->setDisabled(true);
        copy_from_cbox->setDisabled(true);
    }
    updatePageAutoWrite(auto_write);
}

void BasePageWidget::initManuAutoWidgets()
{
    auto_item_list.clear();
    manu_item_list.clear();
    auto_iso_item_list.clear();
    auto_lv_item_list.clear();
    auto_manu_item_map.clear();
    for (BaseItemWidget *item : base_item_list) {
        if (item->getTitle() == OP_TYPE && item->getOpType().isEmpty()) {
            if (optype_widget == nullptr)
                optype_widget = (ComboBoxItemWidget *)item;
            if (((ComboBoxItemWidget *)optype_widget)->getComboboxCount() > 2)
                optype_widget = nullptr;
        } else if (item->getOpType() == OP_TYPE_AUTO) {
            auto_item_list.append(item);
        } else if (item->getOpType() == OP_TYPE_MANUAL) {
            manu_item_list.append(item);
        }
    }

    if (auto_item_list.count() <= 0 || manu_item_list.count() <= 0) {
        optype_widget = nullptr;
        return;
    }

    if (optype_widget) {
        QVariantList op_items;
        QStringList texts;

        for (int i : lv_list) {
            texts.append(NAME_LV + QString::number(i));
        }
        for (int i = 0; i < iso_list.count(); i++) {
            texts[i] = NAME_ISO + QString::number(iso_list[i]) + QString("/") + texts[i];
        }
        op_items.append(QVariantMap({{"label", "OP_TYPE_ALL"}, {"value", 2}}));
        for (int i = 0; i < texts.count(); i++) {
            op_items.append(QVariantMap({{"label", texts[i]}, {"value", 3 + i}}));
        }
        optype_widget->setParam(op_items);
        connect(optype_widget, &ComboBoxItemWidget::sigValueChanged, this, &BasePageWidget::handleOpTypeChanged);
    }

    for (BaseItemWidget *item_m : manu_item_list) {
        for (BaseItemWidget *item_a : auto_item_list) {
            if (!dynamic_cast<UniversalMatrixItemWidget *>(item_a)) {
                continue;
            }
            if (item_m->getTitle() == item_a->getTitle()) {
                UniversalMatrixItemWidget *item_u = (UniversalMatrixItemWidget *)item_a;
                if (item_u->getAutoType() == NAME_ISO) {
                    auto_iso_item_list.append(item_m);
                } else if (item_u->getAutoType() == NAME_LV) {
                    auto_lv_item_list.append(item_m);
                }
                auto_manu_item_map.insert(item_a, item_m);
                break;
            }
        }
    }
}

void BasePageWidget::handleOpTypeChanged(QVariant value)
{
    int choice = value.toInt();

    if (last_choice > 2 && choice <= 2) {
        autoManuDisconnect();
    }

    if (choice == 0) {  // Auto
        for (BaseItemWidget *item : auto_item_list) {
            item->setVisible(true);
        }
        for (BaseItemWidget *item : manu_item_list) {
            item->setOpType(OP_TYPE_MANUAL);
            item->setVisible(false);
        }
    } else if (choice == 1) {  // Manual
        if (manu_data_covered) {
            recoverManuData();
            manu_data_covered = false;
        }
        for (BaseItemWidget *item : auto_item_list) {
            item->setVisible(false);
        }
        for (BaseItemWidget *item : manu_item_list) {
            item->setOpType(OP_TYPE_MANUAL);
            item->setVisible(true);
        }
    } else if (choice == 2) {  // show all
        for (BaseItemWidget *item : auto_item_list) {
            item->setVisible(true);
        }
        for (BaseItemWidget *item : manu_item_list) {
            item->setOpType(OP_TYPE_MANUAL);
            item->setVisible(true);
        }
    } else {  // Auto ISO/LV
        if (!manu_data_covered) {
            manu_item_data.clear();
            for (BaseItemWidget *item : manu_item_list) {
                manu_item_data.append(item->getData());
            }
            manu_data_covered = true;
        }

        int index = choice - 3;

        for (BaseItemWidget *item : auto_item_list) {
            item->setVisible(false);
            if (!dynamic_cast<UniversalMatrixItemWidget *>(item)) {
                continue;
            }
            if (!checkIndex((UniversalMatrixItemWidget *)item, index)) {
                emit sigStatusLog(QString("%1: Index out of range!").arg(item->getTitle()), LOG_LEVEL::LOG_WARNING);
                continue;
            }
            ((UniversalMatrixItemWidget *)item)->setTableIndex(index);
        }

        for (BaseItemWidget *item : manu_item_list) {
            item->setVisible(true);
        }

        if (index < iso_list.count()) {
            QString text = NAME_ISO + QString::number(iso_list[index]);
            for (BaseItemWidget *item : auto_iso_item_list) {
                item->setOpType(text);
            }
        }
        if (index < lv_list.count()) {
            QString text = NAME_LV + QString::number(lv_list[index]);
            for (BaseItemWidget *item : auto_lv_item_list) {
                item->setOpType(text);
            }
        }

        moveAuto2Manu(index);
        if (last_choice <= 2)
            autoManuConnect();
    }
    last_choice = choice;
}

void BasePageWidget::moveAuto2Manu(int index)
{

    for (auto it = auto_manu_item_map.begin(); it != auto_manu_item_map.end(); it++) {
        UniversalMatrixItemWidget *item_a = (UniversalMatrixItemWidget *)it.key();
        BaseItemWidget *item_m = it.value();
        if (!checkIndex(item_a, index)) {
            emit sigStatusLog(QString("%1: Index out of range!").arg(item_a->getTitle()), LOG_LEVEL::LOG_WARNING);
            continue;
        }
        QVariant data = item_a->getDataByIndex(index);
        item_m->setData(data);
    }
}

void BasePageWidget::autoManuConnect()
{
    for (auto it = auto_manu_item_map.begin(); it != auto_manu_item_map.end(); it++) {
        UniversalMatrixItemWidget *item_a = (UniversalMatrixItemWidget *)it.key();
        BaseItemWidget *item_m = it.value();
        connect(item_m, &BaseItemWidget::sigValueChanged, item_a, &UniversalMatrixItemWidget::handleLoadManuData);
    }
}

void BasePageWidget::autoManuDisconnect()
{
    for (auto it = auto_manu_item_map.begin(); it != auto_manu_item_map.end(); it++) {
        UniversalMatrixItemWidget *item_a = (UniversalMatrixItemWidget *)it.key();
        BaseItemWidget *item_m = it.value();
        disconnect(item_m, &BaseItemWidget::sigValueChanged, item_a, &UniversalMatrixItemWidget::handleLoadManuData);
    }
}

void BasePageWidget::handleCopyApply()
{
    int choice;
    int index = copy_from_cbox->currentIndex();

    if (!optype_widget) return;

    optype_widget->getValue(&choice);
    if (choice == 1) {  // Manual: copy auto iso/lv to manu
        if (index >= copy_from_cbox->count() - 1) {
            return;
        } else {
            moveAuto2Manu(index);
        }
        return;
    }
    choice = choice -3;
    if (choice < 0) return;
    if (index == copy_from_cbox->count() - 1) {  // copy manu to auto iso/lv
        if (manu_data_covered) {
            recoverManuData();
            manu_data_covered = false;
        }
        for (auto it = auto_manu_item_map.begin(); it != auto_manu_item_map.end(); it++) {
            UniversalMatrixItemWidget *item_a = (UniversalMatrixItemWidget *)it.key();
            BaseItemWidget *item_m = it.value();
            if (!checkIndex(item_a, index) || !checkIndex(item_a, choice)) {
                emit sigStatusLog(QString("%1: Index out of range!").arg(item_a->getTitle()), LOG_LEVEL::LOG_WARNING);
                continue;
            }
            item_a->handleLoadManuData(item_m->getData());
        }
    } else if (index < copy_from_cbox->count() - 1) {  // copy from iso/lv
        for (BaseItemWidget *item : auto_item_list) {
            if (!dynamic_cast<UniversalMatrixItemWidget *>(item)) {
                continue;
            }
            UniversalMatrixItemWidget * item_a = (UniversalMatrixItemWidget *)item;
            QVariant data = item_a->getDataByIndex(index);
            if (!checkIndex(item_a, index) || !checkIndex(item_a, choice)) {
                emit sigStatusLog(QString("%1: Index out of range!").arg(item_a->getTitle()), LOG_LEVEL::LOG_WARNING);
                continue;
            }
            item_a->handleLoadManuData(data);
        }
    }
}

void BasePageWidget::recoverManuData()
{
    for (int i = 0; i < manu_item_list.count(); i++) {
        manu_item_list[i]->setData(manu_item_data[i]);
    }
}

bool BasePageWidget::checkIndex(UniversalMatrixItemWidget *item, int index)
{
    QString auto_type = item->getAutoType();
    if (auto_type == NAME_ISO) {
        if (index >= iso_list.count()) {
            return false;
        }
    } else if (auto_type == NAME_LV) {
        if (index >= lv_list.count()) {
            return false;
        }
    } else {
        return false;
    }
    return true;
}

void BasePageWidget::readData()
{
    setStatusBusy(true);
    this->page_state = true;

    if (base_item_list.isEmpty()) {
        base_item_list = this->findChildren<BaseItemWidget *>();
    }

    QString connect_type = GlobalData::getInstance()->getSettings(DEVICE_SECTION, KEY_CONNECT_TYPE).toString();
    if (CONNECT_TYPE_NETWORK == connect_type || CONNECT_TYPE_UART == connect_type) {
        GlobalData::getInstance()->setRestoreFlag(false);
        if (NetworkClientHelper::getInstance()->isNormalStatus()) {
            readFromNetwork();
        } else {
            QMessageBox::warning(this, tr("Warning"), connect_type + tr(" is not connected!"), QMessageBox::Ok);
        }
    } else {
        readFromIni();
    }

    this->page_state = false;
    setStatusBusy(false);
    if (last_choice > 2) {
        autoManuDisconnect();
        last_choice = 0;
    }
    manu_data_covered = false;
}

void BasePageWidget::writeData()
{
    if (!GlobalData::getInstance()->getSettings(SETTINGS_SECTION, KEY_AUTO_WRITE).toInt()) {
       setStatusBusy(true);
    } else {
        if (!MainWindow::getInstance()->getAllPageState()) {
            return;
        }
    }
    this->page_state = true;

    if (manu_data_covered) {
        recoverManuData();
        manu_data_covered = false;
    }

    if (base_item_list.isEmpty()) {
        base_item_list = this->findChildren<BaseItemWidget *>();
    }

    QString connect_type = GlobalData::getInstance()->getSettings(DEVICE_SECTION, KEY_CONNECT_TYPE).toString();
    if (CONNECT_TYPE_NETWORK == connect_type || CONNECT_TYPE_UART == connect_type) {
        if (NetworkClientHelper::getInstance()->isNormalStatus()) {
#ifndef TCP_SOCKET_CLIENT
            writeToNetwork();
#else
            writeToNetworkJsonrpc();
#endif
        }else {
            QMessageBox::warning(this, tr("Warning"), connect_type + tr(" is not connected!"), QMessageBox::Ok);
        }
    } else {
        writeToIni();
    }

    if (!GlobalData::getInstance()->getSettings(SETTINGS_SECTION, KEY_AUTO_WRITE).toInt()) {
       setStatusBusy(false);
    }
    this->page_state = false;
}

void BasePageWidget::doRecord()
{
    MainWindow::getInstance()->doRecordAction();
}

bool BasePageWidget::getPageBusyState()
{
    return this->page_state;
}

void BasePageWidget::updatePageAutoWrite(bool checked_state)
{
    read_all_btn->setEnabled(!checked_state);
    read_btn->setEnabled(!checked_state);
    if (ro_page) {
        read_btn->setEnabled(true);
        checked_state = false;
    } else {
        write_all_btn->setEnabled(!checked_state);
        write_btn->setEnabled(!checked_state);
    }

    auto_write_checkbox->setChecked(checked_state);
}

void BasePageWidget::handleAutoWriteChanged(bool checked_state)
{
    MainWindow::getInstance()->updateAllPageAutoWriteState(checked_state);
}

void BasePageWidget::updateEvShowState(bool checked_state, QString iso_value, QString lv_value)
{
    if (checked_state) {
        env_value_checkbox->setChecked(true);
        iso_label->setText(iso_value);
        lv_label->setText(lv_value);
    } else {
        env_value_checkbox->setChecked(false);
        iso_label->setText("");
        lv_label->setText("");
    }
}
void BasePageWidget::handleEvShowChanged(bool checked_state)
{
    emit sigEnvirValue(checked_state);
}

void BasePageWidget::setStatusBusy(bool busy)
{
    this->setEnabled(!busy);
    this->repaint();
}

void BasePageWidget::retranslateUi()
{
    if (this->title.isEmpty()) {
        QString title = property("module_id").toString();
        setTitle(title);
    } else {
        setTitle(this->title);
    }

    copy_apply_btn->setText(tr("Apply"));
    read_all_btn->setText(tr("Read all"));
    write_all_btn->setText(tr("Write all"));
    read_btn->setText(tr("Read"));
    write_btn->setText(tr("Write"));
}

void BasePageWidget::disableAllPageButtons()
{
    MainWindow::getInstance()->disableAllPageButtons();
}

void BasePageWidget::enableAllPageButtons()
{
    MainWindow::getInstance()->enableAllPageButtons("", "", BUTTON_MODE::MODE_NORMAL);
}

void BasePageWidget::disableButtons()
{
    read_all_btn->setEnabled(false);
    read_btn->setEnabled(false);
    if (!this->ro_page) {
        write_all_btn->setEnabled(false);
        write_btn->setEnabled(false);
        copy_apply_btn->setEnabled(false);
        copy_from_cbox->setEnabled(false);
        int auto_write = GlobalData::getInstance()->getSettings(SETTINGS_SECTION, KEY_AUTO_WRITE, 0).toInt();
        if (!auto_write) {
            auto_write_checkbox->setEnabled(false);
        }
    }
}

void BasePageWidget::enableButtons()
{
    int auto_write = GlobalData::getInstance()->getSettings(SETTINGS_SECTION, KEY_AUTO_WRITE, 0).toInt();
    if (auto_write) {
        return;
    }
    read_all_btn->setEnabled(true);
    read_btn->setEnabled(true);
    if (!this->ro_page) {
        write_all_btn->setEnabled(true);
        write_btn->setEnabled(true);
        copy_apply_btn->setEnabled(true);
        copy_from_cbox->setEnabled(true);
        auto_write_checkbox->setEnabled(true);
    }
}

void BasePageWidget::writeToIni()
{
    foreach (BaseItemWidget *base_item_widget, base_item_list) {
        // reset base item ui
        base_item_widget->uiReset();
        QList<SecKeyType> reg_section_key_list = base_item_widget->getSectionKeyList();
        for (auto reg_section_key : reg_section_key_list) {
            if (reg_section_key.first.isEmpty() || reg_section_key.second.isEmpty()) {
                continue;
            }

            // value should get from reg, rather than base ui item
            QString value;
            if (base_item_widget->getRegValueType() == TYPE_ARRAY) {
                value = utils::listToString(GlobalData::getInstance()->getRegValue(reg_section_key.first, reg_section_key.second).toList());
            } else {
                value = GlobalData::getInstance()->getRegValue(reg_section_key.first, reg_section_key.second).toString();
            }
            if (value.isEmpty()) {
                continue;
            }
            qDebug () << "write to ini" << reg_section_key.first << reg_section_key.second << value;
            IniHelper::getInstance()->writeReg(reg_section_key.first, reg_section_key.second, value);
        }
    }
}

void BasePageWidget::writeToNetwork()
{
    if (!NetworkClientHelper::getInstance()->isNormalStatus()) {
        emit sigStatusLog(getTitle() + " Page: Failed to write. Network error.", LOG_LEVEL::LOG_ERROR);
        return;
    }
    if (this->ro_page) {
        emit sigStatusLog(getTitle() + " page is read-only.", LOG_LEVEL::LOG_INFO);
        return;
    }

    QJsonObject send_obj;
    QString id = property("module_id").toString();
    if (id == MODULE_NAME_REG_LIST) {
        send_obj = GetRegObjectToNetwork();
    } else {
        send_obj = GetModuleObjectToNetwork();
    }

    if (send_obj.contains(MSG_KEY_CMD)) {
        NetworkClientHelper::getInstance()->sendCmd(std::move(send_obj));
    }
}

QJsonArray BasePageWidget::GetModuleObjectToLocal()
{
    QJsonArray items;

    foreach (BaseItemWidget *base_item_widget, base_item_list) {
        QJsonObject json_data = base_item_widget->getJsonData();
        if (!json_data.isEmpty()) {
            items.append(json_data);
        }
    }

    return items;
}

QJsonObject BasePageWidget::GetRegObjectToNetwork()
{
    QList<QString> data_list;
    QJsonObject send_obj;
    QJsonArray data_array;

    foreach (BaseItemWidget *base_item_widget, base_item_list) {
        // reset base item ui
        base_item_widget->uiReset();

        QList<SecKeyType> section_key_list = base_item_widget->getSectionKeyList();
        for (auto section_key : section_key_list) {
            /// TODO: skip empty address reg
            QJsonObject reg = GlobalData::getInstance()->getRegJson(section_key.first, section_key.second);
            // value should get from reg, rather than base ui item
            unsigned int value = reg[KEY_VALUE].toVariant().toUInt();
            if (reg.contains(KEY_REG_BITMASK)) {
                QString bitmask = reg[KEY_REG_BITMASK].toString();
                value = utils::toU32MaskValue(value, bitmask);
            }

            reg[KEY_REG_SECTION] = section_key.first;
            reg[KEY_REG_KEY] = section_key.second;
            reg[KEY_VALUE] = static_cast<qint64>(value);
            reg.remove(KEY_REG_VALUE_TYPE);
            data_array.append(reg);
        }
    }

    if (!data_array.isEmpty()) {
        send_obj[MSG_KEY_CMD] = CMD_WRITE_REGISTERS;
        send_obj[MSG_KEY_DATA] = data_array;
    }

    return send_obj;
}

void BasePageWidget::receiveLdcSize(int id)
{
    QJsonObject data_obj = NetworkClientHelper::getInstance()->popCmdDataJsonrpc(id);

    if (!data_obj.isEmpty()) {
        QJsonObject result_obj = data_obj[MSG_KEY_RESULT].toObject();
        QJsonObject params_obj = result_obj[MSG_KEY_PARAMS].toObject();

        if (!params_obj.isEmpty()) {
            QJsonObject stViAttr_obj = params_obj["stViAttr"].toObject();
            QJsonObject stVpssAttr_obj = params_obj["stVpssAttr"].toObject();

            vi_size.u32Width = stViAttr_obj["u32Width"].toInt();
            vi_size.u32Height = stViAttr_obj["u32Height"].toInt();
            vpss_size.u32Width = stVpssAttr_obj["u32Width"].toInt();
            vpss_size.u32Height = stVpssAttr_obj["u32Height"].toInt();
        }
    }
}

QJsonObject BasePageWidget::GetModuleObjectToNetwork()
{
    QJsonObject send_obj;
    QJsonObject data_obj;
    QJsonArray items;
    QString optype = OP_TYPE_MANUAL;
    // send object with manual op type when item's op type is empty by default
    QSet<QString> empty_op_type_set {OP_TYPE_MANUAL};

    QString id = property("module_id").toString();
    data_obj[MSG_KEY_MODULE] = id;

    // get current page op type
    OpTypeComboBoxWidget *op_type_widget = this->findChild<OpTypeComboBoxWidget *>();
    if (op_type_widget != nullptr) {
        QVariant op_type = op_type_widget->getData();
        optype = OpTypeComboBoxWidget::convertOpTypeName(static_cast<OP_TYPE_IDX>(op_type.toInt()));
        // if has op type widget, means need send object with all op type when item's op type is empty
        empty_op_type_set.insert(OP_TYPE_AUTO);
    }

    foreach (BaseItemWidget *base_item_widget, base_item_list) {
        // reset base item ui
        base_item_widget->uiReset();

        QString reg_optype = base_item_widget->getOpType();
        // if op type is empty, send obj with all need op type
        if (reg_optype.isEmpty()) {
            for (auto empty_op_type : empty_op_type_set) {
                QJsonObject json_data = base_item_widget->getJsonData();
                if (!json_data.isEmpty()) {
                    json_data[KEY_REG_OP_TYPE] = empty_op_type;
                    items.append(json_data);
                }
            }
        } else {
            QJsonObject json_data = base_item_widget->getJsonData();
            if (!json_data.isEmpty()) {
                items.append(json_data);
            }
        }
    }

    if (!items.isEmpty()) {
        if (id != MODULE_NAME_NP_PRO) {
                    send_obj[MSG_KEY_CMD] = CMD_WRITE_MODULE;
                    data_obj[MSG_KEY_OP_TYPE] = optype;
                    data_obj[MSG_KEY_ITEMS] = items;
                    send_obj[MSG_KEY_DATA] = data_obj;
                } else {
                    QJsonArray new_items;

                    for (int i = 0; i < 8; i++) {
                        QJsonObject temp = items[i].toObject();
                        temp[KEY_VALUE_TYPE] = TYPE_ARRAY_FLOAT;
                        new_items.append(temp);
                    }

                    send_obj[MSG_KEY_CMD] = CMD_WRITE_MODULE;
                    data_obj[MSG_KEY_OP_TYPE] = optype;
                    data_obj[MSG_KEY_ITEMS] = new_items;
                    send_obj[MSG_KEY_DATA] = data_obj;
                }
    }

    return send_obj;
}

void BasePageWidget::readFromIni()
{
    foreach (BaseItemWidget *base_item_widget, base_item_list) {
        // reset base item ui
        base_item_widget->uiReset();
        QList<SecKeyType> section_key_list = base_item_widget->getSectionKeyList();
        for (auto section_key : section_key_list) {
            if (section_key.first.isEmpty() || section_key.second.isEmpty()) {
                continue;
            }

            QString value_str = IniHelper::getInstance()->readReg(section_key.first, section_key.second, "");
            qDebug() << "read from ini " << section_key.first << " " << section_key.second << " value:" << value_str << " current value:" << base_item_widget->getData();

            if (value_str != GlobalData::getInstance()->getRegValue(section_key.first, section_key.second).toString()) {
                // if data from ini is different, update global data
                setValueToGlobalData(section_key.first, section_key.second, value_str);
            }
        }
    }
}

void BasePageWidget::readFromNetwork()
{
    if (!NetworkClientHelper::getInstance()->isNormalStatus()) {
        emit sigStatusLog(getTitle() + " Page: Failed to read. Network error.", LOG_LEVEL::LOG_ERROR);
        return;
    }

    QString id = property("module_id").toString();
    if (id == MODULE_NAME_REG_LIST) {
        readRegFromNetwork();
    } else {
#ifndef TCP_SOCKET_CLIENT
        readModuleFromNetwork();
#else
        readModuleFromNetworkJsonrpc();
#endif
    }
}

void BasePageWidget::readRegFromNetwork()
{
    QJsonObject send_obj;
    QJsonArray data_array;

    foreach (BaseItemWidget *base_item_widget, base_item_list) {
        // reset base item ui
        base_item_widget->uiReset();
        QList<SecKeyType> section_key_list = base_item_widget->getSectionKeyList();
        for (auto section_key : section_key_list) {
            if (section_key.first.isEmpty() || section_key.second.isEmpty()) {
                continue;
            }

            ///TODO: skip empty address reg
            QJsonObject reg = GlobalData::getInstance()->getRegJson(section_key.first, section_key.second);
            reg[KEY_REG_SECTION] = section_key.first;
            reg[KEY_REG_KEY] = section_key.second;
            reg.remove(KEY_VALUE);
            data_array.append(reg);
        }
    }

    if (!data_array.isEmpty()) {
        send_obj[MSG_KEY_CMD] = CMD_READ_REGISTERS;
        send_obj[MSG_KEY_DATA] = data_array;
        NetworkClientHelper::getInstance()->sendCmd(std::move(send_obj));
    }
}

void BasePageWidget::readModuleFromNetwork()
{
    QJsonObject send_obj;
    QJsonObject data_obj;

    data_obj[MSG_KEY_MODULE] = property("module_id").toString();

    foreach (BaseItemWidget *base_item_widget, base_item_list) {
        // reset base item ui
        base_item_widget->uiReset();
    }

    send_obj[MSG_KEY_CMD] = CMD_READ_MODULE;
    send_obj[MSG_KEY_DATA] = data_obj;
    NetworkClientHelper::getInstance()->sendCmd(std::move(send_obj));
}

void BasePageWidget::setValueToGlobalData(const QString &section, const QString &key, QString value_str)
{
    QList<QVariant> value_list = utils::stringToList<QVariant>(value_str);
    if (value_list.isEmpty()) {
        return;
    }

    // handle single value or value array
    if (1 == value_list.size()) {
        GlobalData::getInstance()->setRegValue(section, key, value_list.first().toInt());
    } else {
        QJsonArray value_array;
        for (int i = 0; i < value_list.size(); i++) {
            // avoid empty value append
            if (!value_list[i].toString().isEmpty()) {
                value_array.append(value_list[i].toDouble());
            }
        }
        GlobalData::getInstance()->setRegValue(section, key, value_array);
    }
}

bool BasePageWidget::getPageOnlyReadProperty()
{
    return this->ro_page;
}

bool BasePageWidget::getReadyFlag()
{
    return ready_flag;
}

void BasePageWidget::setReadyFlag(bool flag)
{
    ready_flag = flag;
}

void BasePageWidget::pageWriteAll()
{
    if (!NetworkClientHelper::getInstance()->isNormalStatus()) {
        QMessageBox::warning(this, tr("Warning"), tr("Network is not connected!"), QMessageBox::Ok);
        return;
    }

    disableAllPageButtons();
    emit sigWriteAll();
}

void BasePageWidget::pageReadAll()
{
    if (!NetworkClientHelper::getInstance()->isNormalStatus()) {
        QMessageBox::warning(this, tr("Warning"), tr("Network is not connected!"), QMessageBox::Ok);
        return;
    }
    disableAllPageButtons();
    emit sigReadAll();
}

void BasePageWidget::setBaseItemList(BaseItemWidget *widget)
{
    base_item_list.append(widget);
}

void BasePageWidget::insertBaseItemList(int idx, BaseItemWidget *widget)
{
    base_item_list.insert(idx, widget);
}

void BasePageWidget::paegUpdateTheme(bool dark_theme_enabled)
{
    foreach (BaseItemWidget *base_item_widget, base_item_list) {
        base_item_widget->handleUpdateTheme(dark_theme_enabled);
    }
}

void BasePageWidget::readModuleFromNetworkJsonrpc()
{
    QString id = property("module_id").toString();
    foreach (BaseItemWidget *base_item_widget, base_item_list) {
        // reset base item ui
        base_item_widget->uiReset();
    }

    NetworkClientHelper::getInstance()->clearIdInfoJsonrpc();
    NetworkClientHelper::getInstance()->setCallerInfo(0, NULL);
    for (auto struct_id : struct_list) {
        QJsonObject send_obj;
        QJsonObject id_info;
        int jsonrpc_id = GlobalData::getInstance()->getJsonrpcId();
        send_obj[MSG_KEY_JSONRPC] = MSG_JSONRPC_VERSION;
        send_obj[MSG_KEY_METHOD] = struct_get_api_map[struct_id];
        send_obj[MSG_KEY_ID] = jsonrpc_id;
        id_info[MSG_KEY_CMD] = CMD_READ_MODULE;
        id_info[MSG_KEY_MODULE] = id;
        id_info[MSG_KEY_STRUCT] = struct_id;
        if (!struct_mapping_map.isEmpty() && struct_mapping_map.contains(struct_id)) {
            id_info[MSG_KEY_MAPPING] = struct_mapping_map[struct_id];
        }

        NetworkClientHelper::getInstance()->pushIdInfoJsonrpc(jsonrpc_id, id_info);
        NetworkClientHelper::getInstance()->sendCmd(std::move(send_obj));

        bool gotResponse = false;
        QTime stopTime;
        int waitTime;
        if (id == MODULE_NAME_LDC) {
            waitTime = READ_WRITE_TIMEOUT * 60 / 2;
        } else {
            waitTime = READ_WRITE_TIMEOUT;
        }
        if (CONNECT_TYPE_UART == GlobalData::getInstance()->getSettings(DEVICE_SECTION, KEY_CONNECT_TYPE).toString())
            waitTime *= 5;
        stopTime = QTime::currentTime().addSecs(waitTime);
        while (QTime::currentTime() < stopTime) {
            QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
            if (!GlobalData::getInstance()->getWaitForResponse()) {
                gotResponse = true;
                break;
            }
        }

        if (!gotResponse) {
            NetworkClientHelper::getInstance()->removeIdInfoJsonrpc(jsonrpc_id);
            GlobalData::getInstance()->setWaitForResponse(false);
            enableAllPageButtons();
            QString message = getTitle() + QString(" Page: Failed to read %1. Timeout: Can't get reponse from board within %2 seconds.").arg(struct_id).arg(waitTime);
            emit sigStatusLog(message, LOG_LEVEL::LOG_ERROR);
            break;
        }
    }
}

void BasePageWidget::getViVpssLdcChnSize()
{
    QJsonObject id_info;
    id_info[MSG_KEY_CMD] = CMD_GET_LDC_CHN_SIZE;
    int id = GlobalData::getInstance()->getJsonrpcId();
    NetworkClientHelper::getInstance()->pushIdInfoJsonrpc(id, id_info);

    QJsonObject send_obj;
    send_obj[MSG_KEY_METHOD] = METHOD_GET_VI_VPSS_LDC_CHN_SIZE;
    send_obj[MSG_KEY_ID] = id;
    send_obj[MSG_KEY_JSONRPC] = MSG_JSONRPC_VERSION;
    NetworkClientHelper::getInstance()->sendCmd(std::move(send_obj));
}

void BasePageWidget::convertBinToCFiles(QByteArray &data, QString path)
{
    if (data.isEmpty() || path.isEmpty()) {
        return;
    }

    QFileInfo info(path);
    QString fileName = info.fileName();
    int lastDotPos = fileName.lastIndexOf('.');
    if (lastDotPos != -1) {
        fileName = fileName.left(lastDotPos);
    }
    QString fileNameLower = fileName.toLower();
    QString c_file_path = info.absolutePath() +  QString("/%1_param.c").arg(fileNameLower);
    QString h_file_path = info.absolutePath() + QString("/%1_param.h").arg(fileNameLower);

    QString c_file_str = QString("#include \"%1_param.h\"\n").arg(fileNameLower);
    c_file_str += QString("\nunsigned char cvi_%1_bin_param[] = {\n").arg(fileNameLower);
    for (int i = 0; i < data.size(); i++) {
        c_file_str += "0x" + QString::number((unsigned char)data[i], 16).toUpper();
        if (i  == data.size() - 1) {
            c_file_str += "\n";
            break;
        }
        c_file_str += ", ";
        if ((i != 0) && ((i + 1) % 50 == 0)) {
            c_file_str += "\n";
        }
    }
    c_file_str += "};\n";

    QFile c_file(c_file_path);
    c_file.open(QIODevice::WriteOnly);
    c_file.write(c_file_str.toStdString().c_str());
    c_file.close();

    QString h_file_str = QString("#ifndef _%1_PARAM_H_\n").arg(fileName.toUpper());
    h_file_str += QString("#define _%1_PARAM_H_\n").arg(fileName.toUpper());
    h_file_str += QString("\n#define LDC_BIN_DATA_SIZE %1\n").arg(data.size());
    h_file_str += QString("\nextern unsigned char cvi_%1_bin_param[];\n").arg(fileNameLower);
    h_file_str += "\n#endif\n";

    QFile h_file(h_file_path);
    h_file.open(QIODevice::WriteOnly);
    h_file.write(h_file_str.toStdString().c_str());
    h_file.close();
}

int BasePageWidget::sendLDCAttrToBoard(QJsonObject module_obj)
{
    int ret = 0;
    for (auto struct_id : struct_list) {
        QJsonObject send_obj = module_obj[struct_id].toObject();
        QJsonObject data = send_obj["params"].toObject();

        data["bEnable"] = 0;
        send_obj.insert("params", data);
        NetworkClientHelper::getInstance()->sendCmd(std::move(send_obj));

        bool gotResponse = false;
        QTime stopTime;
        stopTime = QTime::currentTime().addSecs(READ_WRITE_TIMEOUT);
        while (QTime::currentTime() < stopTime) {
            QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
            if (!GlobalData::getInstance()->getWaitForResponse()) {
                gotResponse = true;
                ret = 0;
                break;
            }
        }

        if (!gotResponse) {
            GlobalData::getInstance()->setWaitForResponse(false);
            enableAllPageButtons();
            QString message = getTitle() + QString(" Page: Failed to write %1. Timeout: Can't get reponse from board within %2 seconds.").arg(struct_id).arg(READ_WRITE_TIMEOUT);
            emit sigStatusLog(message, LOG_LEVEL::LOG_ERROR);
            ret = -1;
            break;
        }
    }

    return ret;
}

MODULE_LDC_ATTR_S BasePageWidget::getModuleLDCAttr(QJsonObject module_obj)
{
    MODULE_LDC_ATTR_S ldc_attr;

    for (auto struct_id : struct_list) {
        QJsonObject send_obj = module_obj[struct_id].toObject();
        QJsonObject data = send_obj["params"].toObject();
        QJsonObject stAttrData = data["stAttr"].toObject();

        if (send_obj["method"] == "CVI_VI_SetChnLDCAttr") {
            ldc_attr.vi_enable = data["bEnable"].toInt();
            ldc_attr.vi_ldc_attr.bAspect = stAttrData["bAspect"].toInt();
            ldc_attr.vi_ldc_attr.s32CenterXOffset = stAttrData["s32CenterXOffset"].toInt();
            ldc_attr.vi_ldc_attr.s32CenterYOffset = stAttrData["s32CenterYOffset"].toInt();
            ldc_attr.vi_ldc_attr.s32DistortionRatio = stAttrData["s32DistortionRatio"].toInt();
            ldc_attr.vi_ldc_attr.s32XRatio = stAttrData["s32XRatio"].toInt();
            ldc_attr.vi_ldc_attr.s32XYRatio = stAttrData["s32XYRatio"].toInt();
            ldc_attr.vi_ldc_attr.s32YRatio = stAttrData["s32YRatio"].toInt();
        } else if (send_obj["method"] == "CVI_VPSS_SetChnLDCAttr") {
            ldc_attr.vpss_enable = data["bEnable"].toInt();
            ldc_attr.vpss_ldc_attr.bAspect = stAttrData["bAspect"].toInt();
            ldc_attr.vpss_ldc_attr.s32CenterXOffset = stAttrData["s32CenterXOffset"].toInt();
            ldc_attr.vpss_ldc_attr.s32CenterYOffset = stAttrData["s32CenterYOffset"].toInt();
            ldc_attr.vpss_ldc_attr.s32DistortionRatio = stAttrData["s32DistortionRatio"].toInt();
            ldc_attr.vpss_ldc_attr.s32XRatio = stAttrData["s32XRatio"].toInt();
            ldc_attr.vpss_ldc_attr.s32XYRatio = stAttrData["s32XYRatio"].toInt();
            ldc_attr.vpss_ldc_attr.s32YRatio = stAttrData["s32YRatio"].toInt();
        }
    }

    return ldc_attr;
}

int BasePageWidget::getLDCMeshTable(MODULE_LDC_ATTR_S ldc_attr)
{
    if (!ldc_attr.vi_enable && !ldc_attr.vpss_enable) {
        return 0;
    }

    #define ERROR_CODE_RESPONSE         (-1)
    #define ERROR_CODE_MALLOC_ERROR     (-2)
    #define ERROR_CODE_VI_MESH_CALCULATE   (-3)
    #define ERROR_CODE_VPSS_MESH_CALCULATE   (-4)

    unsigned int mesh_1st_size = 0, mesh_2nd_size = 0;
    int mesh_size = 0;
    bool gotResponse = false;
    void *vaddr = NULL;
    QTime stopTime;
    QByteArray vi_byteArray, vpss_byteArray;
    QString progress_info = getTitle() + QString(" page is calculating, please wait(about 10s)......!");
    if (ldc_attr.vi_enable && ldc_attr.vpss_enable) {
        progress_info = getTitle() + QString(" page is calculating, please wait(about 20s)......!");
    }

    getViVpssLdcChnSize();
    GlobalData::getInstance()->setWaitForResponse(true);
    stopTime = QTime::currentTime().addSecs(READ_TIMEOUT);
    while (QTime::currentTime() < stopTime) {
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
        if (!GlobalData::getInstance()->getWaitForResponse()) {
            gotResponse = true;
            break;
        }
    }

    if (!gotResponse) {
        GlobalData::getInstance()->setWaitForResponse(false);
        QString message = getTitle() + QString(" Failed to get param when press write button. Timeout: Can't get reponse from board within %1 seconds.").arg(READ_TIMEOUT);
        emit sigStatusLog(message, LOG_LEVEL::LOG_WARNING);
        qDebug() << "[LDC Mesh] don't get reponse from board!" << endl;
        return ERROR_CODE_RESPONSE;
    } else {
        emit sigStatusLog(progress_info, LOG_LEVEL::LOG_INFO);
        QCoreApplication::processEvents(QEventLoop::AllEvents);
    }

    if (ldc_attr.vi_enable) {
        vi_size.u32Width = ALIGN(vi_size.u32Width, DEFAULT_ALIGN);
        vi_size.u32Height = ALIGN(vi_size.u32Height, DEFAULT_ALIGN);
        mesh_gen_get_size(vi_size, vi_size, &mesh_1st_size, &mesh_2nd_size);
        mesh_size = mesh_1st_size + mesh_2nd_size;
        vaddr = (void *)calloc(1, mesh_size);
        if (vaddr == NULL) {
            qDebug () << "[LDC Mesh](vi) calloc error!" << endl;
            return ERROR_CODE_MALLOC_ERROR;
        }
        if (mesh_gen_ldc(vi_size, vi_size, &(ldc_attr.vi_ldc_attr), NULL, vaddr, ROTATION_0) != 0) {
            qDebug() << "[LDC Mesh](vi) calculate error!" << endl;
            free(vaddr);
            return ERROR_CODE_VI_MESH_CALCULATE;
        }
        vi_byteArray = QByteArray((char *)vaddr, mesh_size);
        free(vaddr);
    }

    if (ldc_attr.vpss_enable) {
        vpss_size.u32Width = ALIGN(vpss_size.u32Width, DEFAULT_ALIGN);
        vpss_size.u32Height = ALIGN(vpss_size.u32Height, DEFAULT_ALIGN);
        mesh_gen_get_size(vpss_size, vpss_size, &mesh_1st_size, &mesh_2nd_size);
        mesh_size = mesh_1st_size + mesh_2nd_size;
        vaddr = (void *)calloc(1, mesh_size);
        if (vaddr == NULL) {
            qDebug () << "[LDC Mesh](vpss) calloc error!" << endl;
            return ERROR_CODE_MALLOC_ERROR;
        }
        if (mesh_gen_ldc(vpss_size, vpss_size, &(ldc_attr.vpss_ldc_attr), NULL, vaddr, ROTATION_0) != 0) {
            qDebug () << "[LDC Mesh](vpss) calculate error!" << endl;
            free(vaddr);
            return ERROR_CODE_VPSS_MESH_CALCULATE;
        }
        vpss_byteArray = QByteArray((char *)vaddr, mesh_size);
        free(vaddr);
    }

    qDebug () << "222 [LDC Mesh](vpss) calculate error!" << endl;

    //generate ldc bin file to pc
    QString tempName_vi = LDC_FOLDER + "vi_ldc.bin";
    QString tempName_vpss = LDC_FOLDER + "vpss_ldc.bin";
    QDir dir;

    if(dir.exists(LDC_FOLDER)) {
        QDir Dir(LDC_FOLDER);
        Dir.removeRecursively();
    }
    dir.mkdir(LDC_FOLDER);

    if (ldc_attr.vi_enable) {
        QFile file(tempName_vi);
        file.open(QIODevice::WriteOnly);
        file.write(vi_byteArray);
        file.close();
        convertBinToCFiles(vi_byteArray, tempName_vi);
        sendLdcBinSize(LDC_VI);
    }
    if (ldc_attr.vpss_enable) {
        QFile file_vpss(tempName_vpss);
        file_vpss.open(QIODevice::WriteOnly);
        file_vpss.write(vpss_byteArray);
        file_vpss.close();

        convertBinToCFiles(vpss_byteArray, tempName_vpss);
        if (!ldc_attr.vi_enable) {
            sendLdcBinSize(LDC_VPSS);
        }
    }

    return 0;
}

void BasePageWidget::sendLdcBinSize(LDC_MODULE_E module)
{
    if (!NetworkClientHelper::getInstance()->isNormalStatus()) {
        QMessageBox::warning(this, tr("Warning"), tr("Network is not connected!"), QMessageBox::Ok);
        return;
    }

    QString cmd_type;
    int bin_data_type;
    QString file_path;
    QByteArray data;

    if (module == LDC_VI) {
        cmd_type = CMD_LDC_VI_BIN_SIZE;
        bin_data_type = VI_LDC_BIN_DATA;
        file_path = LDC_FOLDER + "vi_ldc.bin";
    } else if (module == LDC_VPSS) {
        cmd_type = CMD_LDC_VPSS_BIN_SIZE;
        bin_data_type = VPSS_LDC_BIN_DATA;
        file_path = LDC_FOLDER + "vpss_ldc.bin";
    }

    QFile bin_file(file_path);
    if (bin_file.exists()) {
        bin_file.open(QIODevice::ReadOnly);
        data = bin_file.readAll();
        bin_file.close();
    } else {
        qDebug() << file_path << " file isn't existent." << endl;
        return;
    }

    QJsonObject id_info;
    id_info[MSG_KEY_CMD] = cmd_type;
    int id = GlobalData::getInstance()->getJsonrpcId();
    NetworkClientHelper::getInstance()->pushIdInfoJsonrpc(id, id_info);

    QJsonObject param_obj;
    param_obj["content"] = bin_data_type;
    param_obj["size"] = data.size();
    QJsonObject send_obj;
    send_obj[MSG_KEY_METHOD] = METHOD_SEND_BIN_INFO;
    send_obj[MSG_KEY_ID] = id;
    send_obj[MSG_KEY_JSONRPC] = MSG_JSONRPC_VERSION;
    send_obj[MSG_KEY_PARAMS] = param_obj;
    NetworkClientHelper::getInstance()->sendCmd(std::move(send_obj));
}

void BasePageWidget::sendLdcBinData(int id)
{
    qDebug("CaptureWindow::handleReceiveLdcBinData\n");
    NetworkClientHelper::getInstance()->setCallerInfo(0, NULL);

    QJsonObject obj = NetworkClientHelper::getInstance()->popCmdDataJsonrpc(id);
    QJsonObject result_obj = obj[MSG_KEY_RESULT].toObject();
    QJsonObject info_obj = obj[MSG_KEY_ID_INFO].toObject();
    QString cmd = info_obj[MSG_KEY_CMD].toString();
    QString file_path;

    if (result_obj.isEmpty()) {
        QJsonObject error_obj = obj[MSG_KEY_ERROR].toObject();
        QVariant code = error_obj[MSG_KEY_CODE].toVariant();
        QString message = error_obj[MSG_KEY_MESSAGE].toString();
        QString log;

        if (cmd == CMD_LDC_VI_BIN_SIZE) {
            log = "Send vi size failed: ";
        } else if (cmd == CMD_LDC_VPSS_BIN_SIZE) {
            log = "Send vpss size failed: ";
        } else if (cmd == CMD_LDC_VI_BIN_DATA) {
            log = "Send vi bin failed: ";
        } else if (cmd == CMD_LDC_VPSS_BIN_DATA) {
            log = "Send vpss bin failed: ";
        }

        log = log + message + " ,errorcode = " + code.toString();
        emit sigStatusLog(log, LOG_LEVEL::LOG_ERROR);
        return;
    }

    if (cmd == CMD_LDC_VI_BIN_SIZE) {
        file_path = LDC_FOLDER + "vi_ldc.bin";
        QFile bin_file(file_path);
        bin_file.open(QIODevice::ReadOnly);
        QByteArray data = bin_file.readAll();
        bin_file.close();

        NetworkClientHelper::getInstance()->setCallerInfo(0, CMD_LDC_VI_BIN_DATA);
        NetworkClientHelper::getInstance()->sendData(data);
        QString message = QString("VI Ldc bin is sending data to board, please wait......!");
        emit sigStatusLog(message, LOG_LEVEL::LOG_INFO);
    } else if (cmd == CMD_LDC_VPSS_BIN_SIZE) {
        file_path = LDC_FOLDER + "vpss_ldc.bin";
        QFile bin_file(file_path);
        bin_file.open(QIODevice::ReadOnly);
        QByteArray data = bin_file.readAll();
        bin_file.close();

        NetworkClientHelper::getInstance()->setCallerInfo(0, CMD_LDC_VPSS_BIN_DATA);
        NetworkClientHelper::getInstance()->sendData(data);
        QString message = QString("VPSS Ldc bin is sending data to board, please wait......!");
        emit sigStatusLog(message, LOG_LEVEL::LOG_INFO);
    } else if (cmd == CMD_LDC_VI_BIN_DATA) {
        QString message = QString("Send VI LDC bin successfully!");
        emit sigStatusLog(message, LOG_LEVEL::LOG_INFO);
        qDebug() << "start vpss bin size send" << endl;
        sendLdcBinSize(LDC_VPSS);
    } else if (cmd == CMD_LDC_VPSS_BIN_DATA) {
        QString message = QString("Send VPSS LDC bin to flash successfully!");
        emit sigStatusLog(message, LOG_LEVEL::LOG_INFO);
    }
}

void BasePageWidget::writeToNetworkJsonrpc()
{
    if (!NetworkClientHelper::getInstance()->isNormalStatus()) {
        emit sigStatusLog(getTitle() + " Page: Failed to write. Network error.", LOG_LEVEL::LOG_ERROR);
        return;
    }

    if (this->ro_page) {
        emit sigStatusLog(getTitle() + " page is read-only.", LOG_LEVEL::LOG_INFO);
        return;
    }

    foreach (BaseItemWidget *base_item_widget, base_item_list) {
        // reset base item ui
        base_item_widget->uiReset();
    }

    QJsonObject module_obj = GetModuleObjectToNetworkJsonrpc();
    QString id = property("module_id").toString();

    if ((id == MODULE_NAME_LDC) && ldc_bin_send_checkbox->isChecked()) {
        MODULE_LDC_ATTR_S ldc_attr;
        int ret;

        ret = sendLDCAttrToBoard(module_obj);
        if (ret != 0) {
            QString message = getTitle() + QString(" page calculate exit (%1)!").arg(QString::number(ret));
            emit sigStatusLog(message, LOG_LEVEL::LOG_WARNING);
            return;
        }
        ldc_attr = getModuleLDCAttr(module_obj);
        ret = getLDCMeshTable(ldc_attr);
        if (ret != 0) {
            QString message = getTitle() + QString(" page calculate abnormally(error code:%1)!").arg(ret);
            emit sigStatusLog(message, LOG_LEVEL::LOG_WARNING);
        }
        return;
    }

    for (auto struct_id : struct_list) {
        QJsonObject send_obj = module_obj[struct_id].toObject();

        if ((id == MODULE_NAME_CLUT && struct_id == QString("ISP_CLUT_ATTR_S"))
            || (id == MODULE_NAME_LSC && struct_id == QString("ISP_MESH_SHADING_GAIN_LUT_ATTR_S"))) {
            send_obj["params"] = utils::stringFromObject(send_obj["params"].toObject());
        }
        if ((id == MODULE_NAME_NP_PRO) && (struct_id == QString("ISP_CMOS_NOISE_CALIBRATION_S"))) {
            send_obj["params"] = utils::revertNoiseProfileParamFormat(send_obj["params"].toObject());
        }

        NetworkClientHelper::getInstance()->sendCmd(std::move(send_obj));

        bool gotResponse = false;
        QTime stopTime;
        int waitTime;
        if ((id == MODULE_NAME_LDC) && (ldc_bin_send_checkbox->isChecked() == false)) {
            waitTime = READ_WRITE_TIMEOUT * 60 / 2;
        } else {
            waitTime = READ_WRITE_TIMEOUT;
        }
        if (CONNECT_TYPE_UART == GlobalData::getInstance()->getSettings(DEVICE_SECTION, KEY_CONNECT_TYPE).toString())
            waitTime *= 10;
        stopTime = QTime::currentTime().addSecs(waitTime);
        while (QTime::currentTime() < stopTime) {
            QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
            if (!GlobalData::getInstance()->getWaitForResponse()) {
                gotResponse = true;
                break;
            }
        }

        if (!gotResponse) {
            GlobalData::getInstance()->setWaitForResponse(false);
            enableAllPageButtons();
            QString message = getTitle() + QString(" Page: Failed to write %1. Timeout: Can't get reponse from board within %2 seconds.").arg(struct_id).arg(waitTime);
            emit sigStatusLog(message, LOG_LEVEL::LOG_ERROR);
            break;
        }
    }
}

QJsonObject BasePageWidget::GetModuleObjectToNetworkJsonrpc()
{
    QJsonObject module_obj;

    NetworkClientHelper::getInstance()->clearIdInfoJsonrpc();
    for (auto struct_id : struct_list) {
        QJsonObject send_obj;
        QJsonObject id_info;
        int jsonrpc_id = GlobalData::getInstance()->getJsonrpcId();
        send_obj[MSG_KEY_JSONRPC] = MSG_JSONRPC_VERSION;
        send_obj[MSG_KEY_METHOD] = struct_set_api_map[struct_id];
        send_obj[MSG_KEY_ID] = jsonrpc_id;
        module_obj[struct_id] = send_obj;
        id_info[MSG_KEY_CMD] = CMD_WRITE_MODULE;
        id_info[MSG_KEY_MODULE] = property("module_id").toString();
        id_info[MSG_KEY_STRUCT] = struct_id;

        NetworkClientHelper::getInstance()->pushIdInfoJsonrpc(jsonrpc_id, id_info);
    }

    foreach (BaseItemWidget *base_item_widget, base_item_list) {
        QStringList path_list = base_item_widget->getPathList();
        for (int i = 0; i < path_list.count(); i++) {
            QJsonValue value = base_item_widget->getJsonValue(path_list[i]);
            QStringList path_split = path_list[i].split(QLatin1Char('.'));
            QString struct_id = path_split[0];
            if (!module_obj.contains(struct_id)) {
                qDebug("[Error] BasePageWidget::GetModuleObjectToNetworkJsonrpc. module_obj doesn't contain %s.", qUtf8Printable(struct_id));
                continue;
            }

            if (base_item_widget->getChmod() == MOD_RO) {
                qDebug("[Info] BasePageWidget::GetModuleObjectToNetworkJsonrpc. %s is readonly.", qUtf8Printable(path_list[i]));
                continue;
            }

            QJsonObject mapping_obj;
            if (!struct_mapping_map.isEmpty() && struct_mapping_map.contains(struct_id)) {
                mapping_obj = struct_mapping_map[struct_id];
            }

            QJsonObject send_obj = module_obj[struct_id].toObject();
            QJsonObject params_obj = send_obj[MSG_KEY_PARAMS].toObject();
            if (path_split.count() < 2) {
                qDebug("[Error] BasePageWidget::GetModuleObjectToNetworkJsonrpc. Invalid path: %s.", qUtf8Printable(path_list[i]));
                continue;
            } else {
                path_split.removeFirst();
                utils::createParamJsonrpc(path_split, params_obj, value, mapping_obj);
            }

            send_obj[MSG_KEY_PARAMS] = params_obj;
            module_obj[struct_id] = send_obj;
        }
    }

    return module_obj;
}

void BasePageWidget::initItemWidgets()
{
    QString id = property("module_id").toString();
    QJsonObject obj = GlobalData::getInstance()->getPageInfo(id);

    if (obj.contains("ready")) {
        this->ready_flag = obj["ready"].toBool();
    }

    if (obj.contains("ro_page")) {
        this->ro_page = obj["ro_page"].toBool();
    }

    if (obj.contains("struct")) {
        QJsonArray struct_array = obj["struct"].toArray();
        for (int i = 0; i < struct_array.count(); i++)
            struct_list.append(struct_array[i].toString());
    }

    if (obj.contains("method")) {
        QJsonArray method_array = obj["method"].toArray();
        for (int i = 0; i < method_array.count(); i++) {
            QJsonObject method_obj = method_array[i].toObject();
            QString sturct_str = method_obj["STRUCT"].toString();
            if (method_obj["SET"].toString().isEmpty())
                this->ro_page = true;

            if (!sturct_str.isEmpty()) {
                struct_get_api_map.insert(sturct_str, method_obj["GET"].toString());
                struct_set_api_map.insert(sturct_str, method_obj["SET"].toString());
            }
        }
    }

    QJsonArray groups_json = obj["groups"].toArray();
    for (int i = 0; i < groups_json.count(); i++) {
        QJsonObject group_obj = groups_json[i].toObject();
        QString group_name = group_obj["group_name"].toString();
        QJsonArray items_json = group_obj["content"].toArray();
        QString reg_section = group_obj[KEY_REG_SECTION].toString();
        bool bTitleWidthCfg = false;
        if (group_obj.contains(KEY_TITLE_LABEL_WIDTH)) {
            bTitleWidthCfg = true;
        }

        int iTitleLen = getGroupTitleMaxLen(items_json);
        for (int j = 0; j < items_json.count(); j++) {
            QJsonObject item_json = items_json[j].toObject();
            if ((!item_json.contains(KEY_REG_SECTION)) && (!reg_section.isEmpty())) {
                item_json[KEY_REG_SECTION] = reg_section;
            }

            if (bTitleWidthCfg) {
                item_json[KEY_TITLE_LABEL_WIDTH] = group_obj[KEY_TITLE_LABEL_WIDTH].toInt();
            }

            BaseItemWidget *widget = RegisterItemFactory::createRegisterItemByJson(id, item_json, iTitleLen);
            base_item_list.append(widget);
        }
    }
}

QJsonArray BasePageWidget::GetPageSaveData()
{
    QJsonArray page_save_data;

    foreach (BaseItemWidget *base_item_widget, base_item_list) {
        QJsonArray save_data = base_item_widget->getSaveData();
        if (!save_data.isEmpty()) {
            for (int i = 0; i < save_data.count(); i++) {
                // TODO HGJ: read only curve item should skip!
                //    QJsonObject obj = save_data[i].toObject();
                //    if (obj.contains("PATH") && obj.contains("VALUE")) {
                //        page_save_data.append(save_data[i]);
                //    } else {
                //        qDebug() << "PATH: VALUE not complete, " << save_data;
                //    }
                page_save_data.append(save_data[i]);
            }
        }
    }

    return page_save_data;
}

void BasePageWidget::readDataFromOldJsonFormat(QJsonArray array)
{
    int total = 0, unprocessed = 0, processed = 0, mismatch = 0;

    foreach (BaseItemWidget *base_item_widget, base_item_list) {
        int found = 0;
        QString key = base_item_widget->getKey();
        QStringList path_list = base_item_widget->getPathList();
        if (path_list.isEmpty()) {
            continue;
        }

        for(int i = 0; i < array.size(); i++) {
            QJsonObject obj = array.at(i).toObject();
            QJsonValue value = obj.value("value");
            if (obj.value("key") == key && obj.value("optype") == base_item_widget->getOpType())
            {
                found++;
                RemapValueA(base_item_widget, value, title, key, obj.value("key").toString(),
                    obj.value("optype").toString(), path_list, found, processed, unprocessed);
            } else if (obj.value("optype") == base_item_widget->getOpType()) {
                RemapValueB(base_item_widget, value, title, key, obj.value("key").toString(),
                    obj.value("optype").toString(), path_list, found, processed, unprocessed);
            } else if (obj.value("key") == key && obj.value("optype") != base_item_widget->getOpType()) {
                RemapValueC(base_item_widget, value, title, key, obj.value("key").toString(),
                    base_item_widget->getOpType(), obj.value("optype").toString(), path_list, found, processed, unprocessed);
            } else {
                RemapValueD(base_item_widget, value, title, key, obj.value("key").toString(),
                    base_item_widget->getOpType(), obj.value("optype").toString(), path_list, found, processed, unprocessed);
            }
        }

        total++;
        if (found == 0) {
            mismatch++;
            emit sigStatusLog(title + " Page --> Can't find " + key + " in parameter json file.", LOG_LEVEL::LOG_WARNING);
        } else if (found > 1) {
            // emit sigStatusLog(title + " Page --> Find multi-" + key + " in parameter json file.", LOG_LEVEL::LOG_WARNING);
        }
    }

    // QString info = title + QString(" Page: total = %1, unprocessed = %2, processed = %3,  mismatch = %4").arg(total).arg(unprocessed).arg(processed).arg(mismatch);
    // emit sigStatusLog(info, LOG_LEVEL::LOG_INFO);
}

void BasePageWidget::readDataFromNewJsonFormat(QJsonArray array)
{
    foreach (BaseItemWidget *base_item_widget, base_item_list) {
        QStringList path_list = base_item_widget->getPathList();
        for (int i = 0; i < path_list.count(); i++) {
            int found = 0;
            for (int j = 0; j < array.count(); j++) {
                QJsonObject temp_obj = array[j].toObject();
                QString path = temp_obj["PATH"].toString();
                QJsonValue value = temp_obj["VALUE"];
                if (path_list[i] == path) {
                    found++;
                    GlobalData::getInstance()->publishItemJsonrpc(path, value.toVariant());
                    break;
                }
            }

            if (found == 0) {
                emit sigStatusLog(title + " Page --> Can't find " + path_list[i] + " in json file.", LOG_LEVEL::LOG_WARNING);
            }
        }
    }
}

inline void remapModeA(BaseItemWidget *item, QJsonValue &value)
{
    QJsonArray value_array;
    QJsonArray array_2d = value.toArray();
    int row = array_2d.count();
    for (int i = 0; i < row; i++) {
        int column = array_2d[i].toArray().count();
        for (int j = 0; j < column; j++)
            value_array.append(array_2d[i].toArray()[j]);
    }

    item->setDataJsonrpc("", QJsonValue(value_array), true);
}

inline void remapModeB(BaseItemWidget *item, QJsonValue &value)
{
    QJsonArray value_array;
    QJsonArray array_2d = value.toArray();
    int row = array_2d.count();
    for (int i = 0; i < row; i++) {
        int column = array_2d[i].toArray().count();
        for (int j = 0; j < column; j++)
            value_array.append(array_2d[i].toArray()[j]);
    }

    item->setData(QJsonValue(value_array), true);
}

inline void remapModeC(BaseItemWidget *item, QJsonValue &value)
{
    // Todo: need to modify the keys order of matrix
    QJsonArray value_array;
    QJsonArray array_2d = value.toArray();
    int row = array_2d.count();
    for (int i = 0; i < row; i+=2) {
        int column = array_2d[i].toArray().count();
        for (int j = 0; j < column; j++)
            value_array.append(array_2d[i].toArray()[j]);
    }

    for (int i = 1; i < row; i+=2) {
        int column = array_2d[i].toArray().count();
        for (int j = 0; j < column; j++)
            value_array.append(array_2d[i].toArray()[j]);
    }

    item->setDataJsonrpc("", QJsonValue(value_array), true);
}

void BasePageWidget::RemapValueA(BaseItemWidget *item, QJsonValue &value, QString title, QString new_key, QString old_key, QString op_type, QStringList path_list, int &found, int &processed, int &unprocessed)
{
    if (title == "3DNR") {
        if ((new_key == "L2mCurve0" && old_key == "L2mCurve0" && op_type == "Auto") ||
            (new_key == "L2mCurve1" && old_key == "L2mCurve1" && op_type == "Auto") ||
            (new_key == "PrtctCurve0" && old_key == "PrtctCurve0" && op_type == "Auto") ||
            (new_key == "PrtctCurve1" && old_key == "PrtctCurve1" && op_type == "Auto") ||
            (new_key == "LowMtPrtCurveY" && old_key == "LowMtPrtCurveY" && op_type == "Auto") ||
            (new_key == "LowMtPrtCurveU" && old_key == "LowMtPrtCurveU" && op_type == "Auto") ||
            (new_key == "LowMtPrtCurveV" && old_key == "LowMtPrtCurveV" && op_type == "Auto") ||
            (new_key == "AdaptNrLumaStr" && old_key == "AdaptNrLumaStr" && op_type == "Auto") ||
            (new_key == "AdaptNrChromaStr" && old_key == "AdaptNrChromaStr" && op_type == "Auto"))
        {
            remapModeA(item, value);
            processed++;
        } else {
            item->setData(value.toVariant(), true);
            unprocessed++;
        }
    } else if (title == "BNR") {
        if (new_key == "LumaStr" && old_key == "LumaStr" && op_type == "Auto") {
            remapModeB(item, value);
            processed++;
        } else {
            item->setData(value.toVariant(), true);
            unprocessed++;
        }
    } else if (title == "CNR") {
        if (new_key == "MotionCnrStrLut" && old_key == "MotionCnrStrLut" && op_type == "Auto") {
            remapModeB(item, value);
            processed++;
        } else {
            item->setData(value.toVariant(), true);
            unprocessed++;
        }
    } else if (title == "DPC") {
        if (new_key == "Enable" && old_key == "Enable" && op_type == "") {
            if (path_list[0] == "ISP_DP_DYNAMIC_ATTR_S.Enable") {
                item->setData(value.toVariant(), true);
                processed++;
            } else if (path_list[0] == "ISP_DP_STATIC_ATTR_S.Enable") {
                found--;
            }
        } else {
            item->setData(value.toVariant());
            unprocessed++;
        }
    } else if (title == "DRC") {
        if (new_key == "CurveUserDefine" && old_key == "CurveUserDefine" && op_type == "") {
            found--;
        } else if ((new_key == "ContrastDarkCurve" && old_key == "ContrastDarkCurve" && op_type == "Auto") ||
            (new_key == "ContrastBrightCurve" && old_key == "ContrastBrightCurve" && op_type == "Auto"))
        {
            remapModeA(item, value);
            processed++;
        } else if ((new_key == "DeltaAdpGain" && old_key == "DeltaAdpGain" && op_type == "Auto") ||
            (new_key == "LumaAdpGain" && old_key == "LumaAdpGain" && op_type == "Auto") ||
            (new_key == "ColorCorrectionIn" && old_key == "ColorCorrectionIn" && op_type == "Auto") ||
            (new_key == "ColorCorrectionOut" && old_key == "ColorCorrectionOut" && op_type == "Auto"))
        {
            remapModeB(item, value);
            processed++;
        } else {
            item->setData(value.toVariant(), true);
            unprocessed++;
        }
    } else if (title == "Dehaze") {
        if ((new_key == "DehazeLumaLut" && old_key == "DehazeLumaLut" && op_type == "Auto") ||
            (new_key == "DehazeSkinLut" && old_key == "DehazeSkinLut" && op_type == "Auto"))
        {
            remapModeB(item, value);
            processed++;
        } else {
            item->setData(value.toVariant(), true);
            unprocessed++;
        }
    } else if (title == "FSWDR") {
        if ((new_key == "WDRCombineCurve" && old_key == "WDRCombineCurve" && op_type == "Auto") ||
            (new_key == "WDRMotionCombineCurve" && old_key == "WDRMotionCombineCurve" && op_type == "Auto"))
        {
            remapModeC(item, value);
            processed++;
        } else if ((new_key == "WDRMtIn" && old_key == "WDRMtIn" && op_type == "Auto") ||
            (new_key == "WDRMtOut" && old_key == "WDRMtOut" && op_type == "Auto"))
        {
            remapModeB(item, value);
            processed++;
        } else {
            item->setData(value.toVariant(), true);
            unprocessed++;
        }
    } else if (title == "WB Attr") {
        if (new_key == "AlgType" && old_key == "AlgType" && op_type == "") {
            if (path_list[0] == "ISP_WB_ATTR_S.enAlgType") {
                item->setData(value.toVariant(), true);
                processed++;
            } else if (path_list[0] == "ISP_WB_ATTR_S.stAuto.enAlgType") {
                found--;
            }
        } else if (new_key == "ZoneWt" && old_key == "ZoneWt" && op_type == "") {
            remapModeB(item, value);
            processed++;
        } else {
            item->setData(value.toVariant(), true);
            unprocessed++;
        }
    } else if (title == "WDR Exposure Attr") {
        if (new_key == "ExpRatio" && old_key == "ExpRatio" && op_type == "") {
            QJsonArray value_array = item->getData().toJsonValue().toArray();
            value_array[0] = value;
            value_array[1] = 64;
            value_array[2] = 64;
            item->setData(QJsonValue(value_array), true);
            processed++;
        } else {
            item->setData(value.toVariant(), true);
            unprocessed++;
        }
    } else if (title == "Y Sharpen"  || title == "PreSharpen") {
        if ((new_key == "LumaAdpGain" && old_key == "LumaAdpGain" && op_type == "Auto") ||
            (new_key == "DeltaShtCtrl" && old_key == "DeltaShtCtrl" && op_type == "Auto") ||
            (new_key == "LumaAdpGain" && old_key == "LumaAdpGain" && op_type == "Manual") ||
            (new_key == "DeltaShtCtrl" && old_key == "DeltaShtCtrl" && op_type == "Manual")||
            (new_key == "DeltaAdpGain" && old_key == "DeltaAdpGain" && op_type == ""))
        {
            remapModeB(item, value);
            processed++;
        } else if (new_key == "MotionShtGainCurve" && old_key == "MotionShtGainCurve" && op_type == "Auto") {
            remapModeA(item, value);
            processed++;
        } else {
            item->setData(value.toVariant(), true);
            unprocessed++;
        }
    } else if (title == "CLUT") {
        if (new_key == "Enable" && old_key == "Enable" && op_type == "") {
            if (path_list[0] == "ISP_CLUT_ATTR_S.Enable") {
                item->setData(value.toVariant(), true);
                processed++;
            } else if (path_list[0] == "ISP_CLUT_SATURATION_ATTR_S.Enable") {
                found--;
            }
        } else {
            item->setData(value.toVariant(), true);
            unprocessed++;
        }
    } else if (title == "YNR") {
        if ((new_key == "MotionCoringWgt" && old_key == "MotionCoringWgt" && op_type == "Auto") ||
            (new_key == "MotionYnrLut" && old_key == "MotionYnrLut" && op_type == "Auto") ||
            (new_key == "LumaStr" && old_key == "LumaStr" && op_type == "Auto"))
        {
            remapModeB(item, value);
            processed++;
        } else {
            item->setData(value.toVariant(), true);
            unprocessed++;
        }
    } else if (title == "CA") {
        if (new_key == "YRatioLut" && old_key == "YRatioLut" && op_type == "Auto") {
            remapModeB(item, value);
            processed++;
        } else {
            item->setData(value.toVariant(), true);
            unprocessed++;
        }
    } else if (title == "Exposure Info") {
        if ((new_key == "AERoute.RouteNode" && old_key == "AERoute.RouteNode" && op_type == "") ||
        (new_key == "AERouteEx.RouteExNode" && old_key == "AERouteEx.RouteExNode" && op_type == "") ||
        (new_key == "AERouteSF.RouteNode" && old_key == "AERouteSF.RouteNode" && op_type == "") ||
        (new_key == "AERouteSFEx.RouteExNode" && old_key == "AERouteSFEx.RouteExNode" && op_type == ""))
        {
            remapModeB(item, value);
            processed++;
        } else {
            item->setData(value.toVariant(), true);
            unprocessed++;
        }
    } else if (title == "Pub Attr") {
        if (new_key == "FrameRate" && old_key == "FrameRate" && op_type == "") {
            item->setData(value.toDouble() / 100.0, true);
            processed++;
        } else {
            item->setData(value.toVariant(), true);
            unprocessed++;
        }
    } else {
        item->setData(value.toVariant(), true);
        unprocessed++;
    }
}

void BasePageWidget::RemapValueB(BaseItemWidget *item, QJsonValue value, QString title, QString new_key, QString old_key, QString op_type, QStringList path_list, int &found, int &processed, int &unprocessed)
{
    if (title == "3DNR") {
        if ((new_key == "L2mIn0" && old_key.contains("L2mIn0") && op_type == "Manual") ||
        (new_key == "L2mIn1" && old_key.contains("L2mIn1") && op_type == "Manual") ||
        (new_key == "L2mOut0" && old_key.contains("L2mOut0") && op_type == "Manual") ||
        (new_key == "L2mOut1" && old_key.contains("L2mOut1") && op_type == "Manual") ||
        (new_key == "PrvMotion0" && old_key.contains("PrvMotion0") && op_type == "Manual") ||
        (new_key == "PrvMotion1" && old_key.contains("PrvMotion1") && op_type == "Manual") ||
        (new_key == "PrtctWgt0" && old_key.contains("PrtctWgt0") && op_type == "Manual") ||
        (new_key == "PrtctWgt1" && old_key.contains("PrtctWgt1") && op_type == "Manual") ||
        (new_key == "LowMtPrtInY" && old_key.contains("LowMtPrtInY") && op_type == "Manual") ||
        (new_key == "LowMtPrtOutY" && old_key.contains("LowMtPrtOutY") && op_type == "Manual") ||
        (new_key == "LowMtPrtInU" && old_key.contains("LowMtPrtInU") && op_type == "Manual") ||
        (new_key == "LowMtPrtOutU" && old_key.contains("LowMtPrtOutU") && op_type == "Manual") ||
        (new_key == "LowMtPrtInV" && old_key.contains("LowMtPrtInV") && op_type == "Manual") ||
        (new_key == "LowMtPrtOutV" && old_key.contains("LowMtPrtOutV") && op_type == "Manual") ||
        (new_key == "AdaptNrLumaStrIn" && old_key.contains("AdaptNrLumaStrIn") && op_type == "Manual") ||
        (new_key == "AdaptNrLumaStrOut" && old_key.contains("AdaptNrLumaStrOut") && op_type == "Manual") ||
        (new_key == "AdaptNrChromaStrIn" && old_key.contains("AdaptNrChromaStrIn") && op_type == "Manual") ||
        (new_key == "AdaptNrChromaStrOut" && old_key.contains("AdaptNrChromaStrOut") && op_type == "Manual"))
        {
            int index = old_key.right(1).toInt();
            QJsonArray value_array = item->getData().toJsonValue().toArray();
            value_array[index] = value;
            item->setData(QJsonValue(value_array), true);
            found++;
            processed++;
        }
    } else if (title == "AE Route") {
        if ((new_key == "AE Route.TotalNum" && old_key == "TotalNum" && op_type == "") ||
        (new_key == "AE RouteSF.TotalNum" && old_key == "TotalNumSF" && op_type == "") ||
        (new_key == "AE RouteEx.TotalNum" && old_key == "TotalExNum" && op_type == "") ||
        (new_key == "AE RouteExSF.TotalNum" && old_key == "TotalExNumSF" && op_type == ""))
        {
            item->setData(QJsonValue(value), true);
            found++;
            processed++;
        }
        else if ((new_key == "AE Route.RouteNode" && old_key == "RouteNode" && op_type == "") ||
        (new_key == "AE RouteSF.RouteNode" && old_key == "RouteNodeSF" && op_type == "") ||
        (new_key == "AE RouteEx.RouteExNode" && old_key == "RouteExNode" && op_type == "") ||
        (new_key == "AE RouteExSF.RouteExNode" && old_key == "RouteExNodeSF" && op_type == ""))
        {
            QJsonArray value_array;
            QJsonArray array_2d = value.toArray();
            int row = array_2d.count();
            for (int i = 0; i < row; i++) {
                int column = array_2d[i].toArray().count();
                for (int j = 0; j < column; j++)
                    value_array.append(array_2d[i].toArray()[j]);
            }

            item->setData(QJsonValue(value_array), true);
            found++;
            processed++;
        }
    } else if (title == "BLC") {
        if (new_key == "OffsetR" && old_key == "Black Level 1" && op_type == "Manual") {
            item->setData(QJsonValue(value.toArray()[0].toArray()[0]), true);
            found++;
            processed++;
        } else if (new_key == "OffsetGr" && old_key == "Black Level 1" && op_type == "Manual") {
            item->setData(QJsonValue(value.toArray()[1].toArray()[0]), true);
            found++;
            processed++;
        } else if (new_key == "OffsetGb" && old_key == "Black Level 1" && op_type == "Manual") {
            item->setData(QJsonValue(value.toArray()[2].toArray()[0]), true);
            found++;
            processed++;
        } else if (new_key == "OffsetB" && old_key == "Black Level 1" && op_type == "Manual") {
            item->setData(QJsonValue(value.toArray()[3].toArray()[0]), true);
            found++;
            processed++;
        } else if (new_key == "OffsetR" && old_key == "Black Level 1" && op_type == "Auto") {
            item->setData(QJsonValue(value.toArray()[0]), true);
            found++;
            processed++;
        } else if (new_key == "OffsetGr" && old_key == "Black Level 1" && op_type == "Auto") {
            item->setData(QJsonValue(value.toArray()[1]), true);
            found++;
            processed++;
        } else if (new_key == "OffsetGb" && old_key == "Black Level 1" && op_type == "Auto") {
            item->setData(QJsonValue(value.toArray()[2]), true);
            found++;
            processed++;
        } else if (new_key == "OffsetB" && old_key == "Black Level 1" && op_type == "Auto") {
            item->setData(QJsonValue(value.toArray()[3]), true);
            found++;
            processed++;
        } else if (new_key == "OffsetR2" && old_key == "Black Level 2" && op_type == "Manual") {
            item->setData(QJsonValue(value.toArray()[0].toArray()[0]), true);
            found++;
            processed++;
        } else if (new_key == "OffsetGr2" && old_key == "Black Level 2" && op_type == "Manual") {
            item->setData(QJsonValue(value.toArray()[1].toArray()[0]), true);
            found++;
            processed++;
        } else if (new_key == "OffsetGb2" && old_key == "Black Level 2" && op_type == "Manual") {
            item->setData(QJsonValue(value.toArray()[2].toArray()[0]), true);
            found++;
            processed++;
        } else if (new_key == "OffsetB2" && old_key == "Black Level 2" && op_type == "Manual") {
            item->setData(QJsonValue(value.toArray()[3].toArray()[0]), true);
            found++;
            processed++;
        } else if (new_key == "OffsetR2" && old_key == "Black Level 2" && op_type == "Auto") {
            item->setData(QJsonValue(value.toArray()[0]), true);
            found++;
            processed++;
        } else if (new_key == "OffsetGr2" && old_key == "Black Level 2" && op_type == "Auto") {
            item->setData(QJsonValue(value.toArray()[1]), true);
            found++;
            processed++;
        } else if (new_key == "OffsetGb2" && old_key == "Black Level 2" && op_type == "Auto") {
            item->setData(QJsonValue(value.toArray()[2]), true);
            found++;
            processed++;
        } else if (new_key == "OffsetB2" && old_key == "Black Level 2" && op_type == "Auto") {
            item->setData(QJsonValue(value.toArray()[3]), true);
            found++;
            processed++;
        }
    } else if (title == "CCM") {
        if ((new_key == "ISOActEnable" && old_key == "ISOActEn" && op_type == "Auto") ||
            (new_key == "TempActEnable" && old_key == "TempActEn" && op_type == "Auto"))
        {
            item->setData(value.toVariant(), true);
            found++;
            processed++;
        }
    } else if (title == "DCI") {
        if (new_key == "DciGamma" && old_key == "DciGamma x 100" && op_type == "") {
            item->setData(value.toVariant(), true);
            found++;
            processed++;
        }
    } else if (title == "DPC") {
        if (new_key == "Enable" && old_key == "Static Enable" && op_type == "") {
            if (path_list[0] == "ISP_DP_STATIC_ATTR_S.Enable") {
                item->setData(value.toVariant(), true);
                found++;
                processed++;
            }
        }
    } else if (title == "DRC") {
        if (new_key == "ContrastDarkMinThrd" && old_key == "ContrastDarkCurve" && op_type == "Manual") {
            item->setData(value[0], true);
            found++;
            processed++;
        } else if (new_key == "ContrastDarkMaxThrd" && old_key == "ContrastDarkCurve" && op_type == "Manual") {
            item->setData(value[1], true);
            found++;
            processed++;
        } else if (new_key == "ContrastDarkMinWeight" && old_key == "ContrastDarkCurve" && op_type == "Manual") {
            item->setData(value[2], true);
            found++;
            processed++;
        } else if (new_key == "ContrastDarkMaxWeight" && old_key == "ContrastDarkCurve" && op_type == "Manual") {
            item->setData(value[3], true);
            found++;
            processed++;
        } else if (new_key == "ContrastBrightMinThrd" && old_key == "ContrastBrightCurve" && op_type == "Manual") {
            item->setData(value[0], true);
            found++;
            processed++;
        } else if (new_key == "ContrastBrightMaxThrd" && old_key == "ContrastBrightCurve" && op_type == "Manual") {
            item->setData(value[1], true);
            found++;
            processed++;
        } else if (new_key == "ContrastBrightMinWeight" && old_key == "ContrastBrightCurve" && op_type == "Manual") {
            item->setData(value[2], true);
            found++;
            processed++;
        } else if (new_key == "ContrastBrightMaxWeight" && old_key == "ContrastBrightCurve" && op_type == "Manual") {
            item->setData(value[3], true);
            found++;
            processed++;
        }
    } else if (title == "Exposure attr") {
        if (new_key == "ByPass" && old_key == "Bypass" && op_type == "") {
            item->setData(value.toVariant(), true);
            found++;
            processed++;
        } else  if (new_key == "DebugMode" && old_key == "Debug Mode" && op_type == "") {
            item->setData(value.toVariant(), true);
            found++;
            processed++;
        } else  if (new_key == "ISONumRange.Max" && old_key == "ISORange.Max" && op_type == "") {
            item->setData(value.toVariant(), true);
            found++;
            processed++;
        } else  if (new_key == "ISONumRange.Min" && old_key == "ISORange.Min" && op_type == "") {
            item->setData(value.toVariant(), true);
            found++;
            processed++;
        } else  if (new_key == "EVBias" && old_key == "EvBias" && op_type == "") {
            item->setData(value.toVariant(), true);
            found++;
            processed++;
        } else  if (new_key == "EnableFaceAE" && old_key == "FaceAE" && op_type == "") {
            item->setData(value.toVariant(), true);
            found++;
            processed++;
        }
    } else if (title == "Gamma") {
        if (new_key == "CurveType" && old_key == "Mode" && op_type == "") {
            item->setData(value.toVariant(), true);
            found++;
            processed++;
        } else if (new_key == "Table" && old_key == "User Define Table" && op_type == "") {
            item->setData(value.toVariant(), true);
            found++;
            processed++;
        } else if (new_key == "sRGB Table" && old_key == "sRGB Table" && op_type == "") {
            item->setData(value.toVariant(), true);
            found++;
            processed++;
        } else if (new_key == "GammaTabNum" && old_key == "Table Number" && op_type == "") {
            item->setData(value.toVariant(), true);
            found++;
            processed++;
        } else if (new_key == "GammaTab[0].Tbl" && old_key == "Auto Gamma Table0" && op_type == "") {
            item->setData(value.toVariant(), true);
            found++;
            processed++;
        } else if (new_key == "GammaTab[1].Tbl" && old_key == "Auto Gamma Table1" && op_type == "") {
            item->setData(value.toVariant(), true);
            found++;
            processed++;
        } else if (new_key == "GammaTab[2].Tbl" && old_key == "Auto Gamma Table2" && op_type == "") {
            item->setData(value.toVariant(), true);
            found++;
            processed++;
        } else if (new_key == "GammaTab[3].Tbl" && old_key == "Auto Gamma Table3" && op_type == "") {
            item->setData(value.toVariant(), true);
            found++;
            processed++;
        } else if (new_key == "GammaTab[4].Tbl" && old_key == "Auto Gamma Table4" && op_type == "") {
            item->setData(value.toVariant(), true);
            found++;
            processed++;
        } else if (new_key == "GammaTab[0].Lv" && old_key == "Table0 LV" && op_type == "") {
            item->setData(value.toVariant(), true);
            found++;
            processed++;
        } else if (new_key == "GammaTab[1].Lv" && old_key == "Table1 LV" && op_type == "") {
            item->setData(value.toVariant(), true);
            found++;
            processed++;
        } else if (new_key == "GammaTab[2].Lv" && old_key == "Table2 LV" && op_type == "") {
            item->setData(value.toVariant(), true);
            found++;
            processed++;
        } else if (new_key == "GammaTab[3].Lv" && old_key == "Table3 LV" && op_type == "") {
            item->setData(value.toVariant(), true);
            found++;
            processed++;
        } else if (new_key == "GammaTab[4].Lv" && old_key == "Table4 LV" && op_type == "") {
            item->setData(value.toVariant(), true);
            found++;
            processed++;
        }
    } else if (title == "LSC") {
        if (new_key == "LscGainLut Size" && old_key == "LscGainLut.Size" && op_type == "") {
            item->setData(value.toVariant(), true);
            found++;
            processed++;
        } else if (new_key == "ColorTemperature" && old_key == "LscGainLut[0].ColorTemp" && op_type == "") {
            QJsonArray value_array = item->getData().toJsonValue().toArray();
            value_array[0] = value;
            item->setData(QJsonValue(value_array), true);
            found++;
            processed++;
        } else if (new_key == "ColorTemperature" && old_key == "LscGainLut[1].ColorTemp" && op_type == "") {
            QJsonArray value_array = item->getData().toJsonValue().toArray();
            value_array[1] = value;
            item->setData(QJsonValue(value_array), true);
            found++;
            processed++;
        } else if (new_key == "ColorTemperature" && old_key == "LscGainLut[2].ColorTemp" && op_type == "") {
            QJsonArray value_array = item->getData().toJsonValue().toArray();
            value_array[2] = value;
            item->setData(QJsonValue(value_array), true);
            found++;
            processed++;
        } else if (new_key == "ColorTemperature" && old_key == "LscGainLut[3].ColorTemp" && op_type == "") {
            QJsonArray value_array = item->getData().toJsonValue().toArray();
            value_array[3] = value;
            item->setData(QJsonValue(value_array), true);
            found++;
            processed++;
        } else if (new_key == "ColorTemperature" && old_key == "LscGainLut[4].ColorTemp" && op_type == "") {
            QJsonArray value_array = item->getData().toJsonValue().toArray();
            value_array[4] = value;
            item->setData(QJsonValue(value_array), true);
            found++;
            processed++;
        } else if (new_key == "ColorTemperature" && old_key == "LscGainLut[5].ColorTemp" && op_type == "") {
            QJsonArray value_array = item->getData().toJsonValue().toArray();
            value_array[5] = value;
            item->setData(QJsonValue(value_array), true);
            found++;
            processed++;
        } else if (new_key == "ColorTemperature" && old_key == "LscGainLut[6].ColorTemp" && op_type == "") {
            QJsonArray value_array = item->getData().toJsonValue().toArray();
            value_array[6] = value;
            item->setData(QJsonValue(value_array), true);
            found++;
            processed++;
        }
    } else if (title == "Statistics Config") {
        // AE Config
        if (new_key == "AECfg.HisStatisticsEnable" && old_key == "HisStatisticsEnable" && op_type == "") {
            item->setData(value.toVariant(), true);
            found++;
            processed++;
        } else if (new_key == "AECfg.Crop[0].Enable" && old_key == "AE.Crop.Enable" && op_type == "") {
            item->setData(value.toVariant(), true);
            found++;
            processed++;
        } else if (new_key == "AECfg.Crop[0].X" && old_key == "AE.Crop.X" && op_type == "") {
            item->setData(value.toVariant(), true);
            found++;
            processed++;
        } else if (new_key == "AECfg.Crop[0].Y" && old_key == "AE.Crop.Y" && op_type == "") {
            item->setData(value.toVariant(), true);
            found++;
            processed++;
        } else if (new_key == "AECfg.Crop[0].W" && old_key == "AE.Crop.W" && op_type == "") {
            item->setData(value.toVariant(), true);
            found++;
            processed++;
        } else if (new_key == "AECfg.Crop[0].H" && old_key == "AE.Crop.H" && op_type == "") {
            item->setData(value.toVariant(), true);
            found++;
            processed++;
        } else if (new_key == "AECfg.Weight" && old_key == "Weight" && op_type == "") {
            remapModeB(item, value);
            found++;
            processed++;
        } else if (new_key == "AECfg.StatisticsShiftBits" && old_key == "AEStatisticsShiftBits" && op_type == "") {
            item->setData(value.toVariant(), true);
            found++;
            processed++;
        } else if (new_key == "AECfg.RGain" && old_key == "RGain" && op_type == "") {
            item->setData(value.toVariant(), true);
            found++;
            processed++;
        } else if (new_key == "AECfg.GGain" && old_key == "GGain" && op_type == "") {
            item->setData(value.toVariant(), true);
            found++;
            processed++;
        } else if (new_key == "AECfg.BGain" && old_key == "BGain" && op_type == "") {
            item->setData(value.toVariant(), true);
            found++;
            processed++;
        // Focus Config
        } else if (new_key == "FocusCfg.Config.Enable" && old_key == "Enable" && op_type == "") {
            item->setData(value.toVariant(), true);
            found++;
            processed++;
        } else if (new_key == "FocusCfg.Config.Hwnd" && old_key == "Hwnd" && op_type == "") {
            item->setData(value.toVariant(), true);
            found++;
            processed++;
        } else if (new_key == "FocusCfg.Config.Vwnd" && old_key == "Vwnd" && op_type == "") {
            item->setData(value.toVariant(), true);
            found++;
            processed++;
        } else if (new_key == "FocusCfg.Config.Crop.Enable" && old_key == "AF.Crop.Enable" && op_type == "") {
            item->setData(value.toVariant(), true);
            found++;
            processed++;
        } else if (new_key == "FocusCfg.Config.Crop.X" && old_key == "AF.Crop.X" && op_type == "") {
            item->setData(value.toVariant(), true);
            found++;
            processed++;
        } else if (new_key == "FocusCfg.Config.Crop.Y" && old_key == "AF.Crop.Y" && op_type == "") {
            item->setData(value.toVariant(), true);
            found++;
            processed++;
        } else if (new_key == "FocusCfg.Config.Crop.W" && old_key == "AF.Crop.W" && op_type == "") {
            item->setData(value.toVariant(), true);
            found++;
            processed++;
        } else if (new_key == "FocusCfg.Config.Crop.H" && old_key == "AF.Crop.H" && op_type == "") {
            item->setData(value.toVariant(), true);
            found++;
            processed++;
        } else if (new_key == "FocusCfg.Config.RawCfg.PreGammaEn" && old_key == "RawCfg.PreGammaEn" && op_type == "") {
            item->setData(value.toVariant(), true);
            found++;
            processed++;
        } else if (new_key == "FocusCfg.Config.RawCfg.PreGammaTable" && old_key == "RawCfg.PreGammaTable" && op_type == "") {
            item->setData(value.toVariant(), true);
            found++;
            processed++;
        } else if (new_key == "FocusCfg.Config.PreFltCfg.PreFltEn" && old_key == "PreFltCfg.PreFltEn" && op_type == "") {
            item->setData(value.toVariant(), true);
            found++;
            processed++;
        } else if (new_key == "FocusCfg.Config.HFltShift" && old_key == "HFltShift" && op_type == "") {
            item->setData(value.toVariant(), true);
            found++;
            processed++;
        } else if (new_key == "FocusCfg.Config.HighLumaTh" && old_key == "HighLumaTh" && op_type == "") {
            item->setData(value.toVariant(), true);
            found++;
            processed++;
        } else if (new_key == "FocusCfg.Config.HVFltLpCoeff" && old_key == "HVFltLpCoeff" && op_type == "") {
            item->setData(value.toVariant(), true);
            found++;
            processed++;
        } else if (new_key == "FocusCfg.HParam_FIR0.HFltHpCoeff" && old_key == "HParam_FIR0.HFltHpCoeff" && op_type == "") {
            item->setData(value.toVariant(), true);
            found++;
            processed++;
        } else if (new_key == "FocusCfg.HParam_FIR1.HFltHpCoeff" && old_key == "HParam_FIR1.HFltHpCoeff" && op_type == "") {
            item->setData(value.toVariant(), true);
            found++;
            processed++;
        } else if (new_key == "FocusCfg.VParam_FIR.VFltHpCoeff" && old_key == "VParam_FIR.VFltHpCoeff" && op_type == "") {
            item->setData(value.toVariant(), true);
            found++;
            processed++;
        } else if (new_key == "FocusCfg.Config.ThLow" && old_key == "ThLow" && op_type == "") {
            item->setData(value.toVariant(), true);
            found++;
            processed++;
        } else if (new_key == "FocusCfg.Config.ThHigh" && old_key == "ThHigh" && op_type == "") {
            item->setData(value.toVariant(), true);
            found++;
            processed++;
        } else if (new_key == "FocusCfg.Config.GainLow" && old_key == "GainLow" && op_type == "") {
            item->setData(value.toVariant(), true);
            found++;
            processed++;
        } else if (new_key == "FocusCfg.Config.GainHigh" && old_key == "GainHigh" && op_type == "") {
            item->setData(value.toVariant(), true);
            found++;
            processed++;
        } else if (new_key == "FocusCfg.Config.SlopLow" && old_key == "SlopLow" && op_type == "") {
            item->setData(value.toVariant(), true);
            found++;
            processed++;
        } else if (new_key == "FocusCfg.Config.SlopHigh" && old_key == "SlopHigh" && op_type == "") {
            item->setData(value.toVariant(), true);
            found++;
            processed++;
        // AWB Config
        } else if (new_key == "WBCfg.BlackLevel" && old_key == "BlackLevel" && op_type == "") {
            item->setData(value.toVariant(), true);
            found++;
            processed++;
        } else if (new_key == "WBCfg.WhiteLevel" && old_key == "WhiteLevel" && op_type == "") {
            item->setData(value.toVariant(), true);
            found++;
            processed++;
        } else if (new_key == "WBCfg.Crop.Enable" && old_key == "AWB.Crop.Enable" && op_type == "") {
            item->setData(value.toVariant(), true);
            found++;
            processed++;
        } else if (new_key == "WBCfg.Crop.X" && old_key == "AWB.Crop.X" && op_type == "") {
            item->setData(value.toVariant(), true);
            found++;
            processed++;
        } else if (new_key == "WBCfg.Crop.Y" && old_key == "AWB.Crop.Y" && op_type == "") {
            item->setData(value.toVariant(), true);
            found++;
            processed++;
        } else if (new_key == "WBCfg.Crop.W" && old_key == "AWB.Crop.W" && op_type == "") {
            item->setData(value.toVariant(), true);
            found++;
            processed++;
        } else if (new_key == "WBCfg.Crop.H" && old_key == "AWB.Crop.H" && op_type == "") {
            item->setData(value.toVariant(), true);
            found++;
            processed++;
        }
    } else if (title == "WB Attr") {
        if (new_key == "DebugMode" && old_key == "Debug Mode" && op_type == "") {
            item->setData(value.toVariant(), true);
            found++;
            processed++;
        } else if (new_key == "Grgain" && old_key == "Ggain" && op_type == "") {
            item->setData(value.toVariant(), true);
            found++;
            processed++;
        } else if (new_key == "Gbgain" && old_key == "Ggain" && op_type == "") {
            item->setData(value.toVariant(), true);
            found++;
            processed++;
        } else if (new_key == "ColorTemperature" && old_key == "ColorTemp[0]" && op_type == "") {
            QJsonArray value_array = item->getData().toJsonValue().toArray();
            value_array[0] = value;
            item->setData(QJsonValue(value_array), true);
            found++;
            processed++;
        } else if (new_key == "ColorTemperature" && old_key == "ColorTemp[1]" && op_type == "") {
            QJsonArray value_array = item->getData().toJsonValue().toArray();
            value_array[1] = value;
            item->setData(QJsonValue(value_array), true);
            found++;
            processed++;
        } else if (new_key == "ColorTemperature" && old_key == "ColorTemp[2]" && op_type == "") {
            QJsonArray value_array = item->getData().toJsonValue().toArray();
            value_array[2] = value;
            item->setData(QJsonValue(value_array), true);
            found++;
            processed++;
        } else if (new_key == "AvgRgain" && old_key == "AvgRgain[0]" && op_type == "") {
            QJsonArray value_array = item->getData().toJsonValue().toArray();
            value_array[0] = value;
            item->setData(QJsonValue(value_array), true);
            found++;
            processed++;
        } else if (new_key == "AvgRgain" && old_key == "AvgRgain[1]" && op_type == "") {
            QJsonArray value_array = item->getData().toJsonValue().toArray();
            value_array[1] = value;
            item->setData(QJsonValue(value_array), true);
            found++;
            processed++;
        } else if (new_key == "AvgRgain" && old_key == "AvgRgain[2]" && op_type == "") {
            QJsonArray value_array = item->getData().toJsonValue().toArray();
            value_array[2] = value;
            item->setData(QJsonValue(value_array), true);
            found++;
            processed++;
        } else if (new_key == "AvgBgain" && old_key == "AvgBgain[0]" && op_type == "") {
            QJsonArray value_array = item->getData().toJsonValue().toArray();
            value_array[0] = value;
            item->setData(QJsonValue(value_array), true);
            found++;
            processed++;
        } else if (new_key == "AvgBgain" && old_key == "AvgBgain[1]" && op_type == "") {
            QJsonArray value_array = item->getData().toJsonValue().toArray();
            value_array[1] = value;
            item->setData(QJsonValue(value_array), true);
            found++;
            processed++;
        } else if (new_key == "AvgBgain" && old_key == "AvgBgain[2]" && op_type == "") {
            QJsonArray value_array = item->getData().toJsonValue().toArray();
            value_array[2] = value;
            item->setData(QJsonValue(value_array), true);
            found++;
            processed++;
        } else if (new_key == "AlgType" && old_key == "AWB.AlgType" && op_type == "") {
            if (path_list[0] == "ISP_WB_ATTR_S.stAuto.enAlgType") {
                item->setData(value.toVariant(), true);
                found++;
                processed++;
            }
        } else if (new_key == "ZoneWt" && old_key == "ZoneWt" && op_type == "") {
            remapModeB(item, value);
            found++;
            processed++;
        } else if (new_key == "InOrOut.GreenEnhanceEn" && old_key == "InOrOut.bGreenEnhanceEn" && op_type == "") {
            item->setData(value.toVariant(), true);
            found++;
            processed++;
        }
    } else if (title == "Y Sharpen" || title == "PreSharpen") {
        if ((new_key == "MotionShtGainIn" && old_key.contains("MotionShtGainIn") && op_type == "Manual") ||
            (new_key == "MotionShtGainOut" && old_key.contains("MotionShtGainOut") && op_type == "Manual"))
        {
            int index = old_key.right(1).toInt();
            QJsonArray value_array = item->getData().toJsonValue().toArray();
            value_array[index] = value;
            item->setData(QJsonValue(value_array), true);
            found++;
            processed++;
        }
    } else if (title == "CLUT") {
        if (new_key == "Enable" && old_key == "SbyS.Enable" && op_type == "") {
            if (path_list[0] == "ISP_CLUT_SATURATION_ATTR_S.Enable") {
                item->setData(value.toVariant(), true);
                found++;
                processed++;
            }
        } else if (new_key == "OpType" && old_key == "SbyS.OpType" && op_type == "") {
            if (path_list[0] == "ISP_CLUT_SATURATION_ATTR_S.enOpType") {
                item->setData(value.toVariant(), true);
                found++;
                processed++;
            }
        } else if ((new_key == "SatIn" && old_key.contains("SatIn") && op_type == "Manual") ||
            (new_key == "SatOut" && old_key.contains("SatOut") && op_type == "Manual")) {
            int index = old_key.right(1).toInt();
            QJsonArray value_array = item->getData().toJsonValue().toArray();
            value_array[index] = value;
            item->setData(QJsonValue(value_array), true);
            found++;
            processed++;
        } else if (new_key == "SatIn" && old_key == "SatCurve" && op_type == "Auto") {
            QJsonArray value_array;
            QJsonArray array_2d = value.toArray();
            for (int i = 0; i < 4; i++) {
                int column = array_2d[i].toArray().count();
                for (int j = 0; j < column; j++)
                    value_array.append(array_2d[i].toArray()[j]);
            }

            item->setData(QJsonValue(value_array), true);
            found++;
            processed++;
        } else if (new_key == "SatOut" && old_key == "SatCurve" && op_type == "Auto") {
            QJsonArray value_array;
            QJsonArray array_2d = value.toArray();
            int row = array_2d.count();
            for (int i = 4; i < row; i++) {
                int column = array_2d[i].toArray().count();
                for (int j = 0; j < column; j++)
                    value_array.append(array_2d[i].toArray()[j]);
            }

            item->setData(QJsonValue(value_array), true);
            found++;
            processed++;
        }
    } else if (title == "VO Gamma") {
        if (new_key == "Table" && old_key == "VO Gamma Table" && op_type == "") {
            QJsonArray new_value_array;
            QJsonArray old_value_array = value.toArray();
            for (int i = 0; i < 256; i+=4)
                new_value_array.append(old_value_array[i]);

            new_value_array.append(255);
            item->setData(QJsonValue(new_value_array), true);
            found++;
            processed++;
        }
    } else if (title == "CSC") {
        if (new_key == "UserMatrx.UserCscCoef" && old_key == "CscMagtrx" && op_type == "") {
            QJsonArray value_array;
            QJsonArray array_2d = value.toArray();
            int row = array_2d.count();
            for (int i = 0; i < row; i++) {
                for (int j = 0; j < 3; j++)
                    value_array.append(array_2d[i].toArray()[j]);
            }

            item->setData(QJsonValue(value_array), true);
            found++;
            processed++;
        } else if (new_key == "UserMatrx.UserCscOffset" && old_key == "CscMagtrx" && op_type == "") {
            QJsonArray value_array;
            QJsonArray array_2d = value.toArray();
            int row = array_2d.count();
            for (int i = 0; i < row; i++) {
                value_array.append(array_2d[i].toArray()[3]);
            }

            item->setData(QJsonValue(value_array), true);
            found++;
            processed++;
        }
    } else if (title == "IRIS") {
        if ((new_key == "MIAttr.HoldValue" && old_key == "HoldValue" && op_type == "") ||
            (new_key == "MIAttr.IrisFNO" && old_key == "IrisFNO" && op_type == ""))
        {
            item->setData(value.toVariant(), true);
            found++;
            processed++;
        }
    } else if (title == "WB Info") {
        if (new_key == "Grgain" && old_key == "Ggain" && op_type == "") {
            item->setData(value.toVariant(), true);
            found++;
            processed++;
        } else if (new_key == "Gbgain" && old_key == "Ggain" && op_type == "") {
            item->setData(value.toVariant(), true);
            found++;
            processed++;
        }
    } else if (title == "Exposure Info") {
        if (new_key == "ExposureIsMAX" && old_key == "ExposureIsMax" && op_type == "") {
            item->setData(value.toVariant(), true);
            found++;
            processed++;
        } else if (new_key == "PirisFNO" && old_key == "PirisFno" && op_type == "") {
            item->setData(value.toVariant(), true);
            found++;
            processed++;
        }
    }
}

void BasePageWidget::RemapValueC(BaseItemWidget *item, QJsonValue &value, QString title, QString new_key, QString old_key, QString new_optype, QString old_optype, QStringList path_list, int &found, int &processed, int &unprocessed)
{
    if (title == "Demosaic") {
        if (new_key == "DetailSmoothEnable" && old_key == "DetailSmoothEnable" && new_optype == "Manual" && old_optype == "") {
            item->setData(value.toVariant(), true);
            found++;
            processed++;
        }
    } else if (title == "Exposure attr") {
        if (new_key == "GainType" && old_key == "GainType" && new_optype == "" && old_optype == "Manual") {
            if (path_list[0] == "ISP_EXPOSURE_ATTR_S.stManual.enGainType") {
                item->setData(value.toVariant(), true);
                found++;
                processed++;
            }
        } else if (new_key == "GainType" && old_key == "GainType" && new_optype == "" && old_optype == "Auto") {
            if (path_list[0] == "ISP_EXPOSURE_ATTR_S.stAuto.enGainType") {
                item->setData(value.toVariant(), true);
                found++;
                processed++;
            }
        }
    }
}

void BasePageWidget::RemapValueD(BaseItemWidget *item, QJsonValue &value, QString title, QString new_key, QString old_key, QString new_optype, QString old_optype, QStringList path_list, int &found, int &processed, int &unprocessed)
{
    if (title == "Noise Profile") {
        if (new_key == "CalibrationCoef" && new_optype == "" && old_optype == "Auto") {
            int j = 0, k = 0;
            QJsonArray old_value_array = value.toArray();
            QJsonArray new_value_array = item->getData().toJsonValue().toArray();
            if (old_key == "Slope B") {
                j = 0; k = 0;
            } else if (old_key == "Slope Gb") {
                j = 1; k = 0;
            } else if (old_key == "Slope Gr") {
                j = 2; k = 0;
            } else if (old_key == "Slope R") {
                j = 3; k = 0;
            } else if (old_key == "Intercept B") {
                j = 0; k = 1;
            } else if (old_key == "Intercept Gb") {
                j = 1; k = 1;
            } else if (old_key == "Intercept Gr") {
                j = 2; k = 1;
            } else if (old_key == "Intercept R") {
                j = 3; k = 1;
            }

            for (int i = 0; i < 16; i++) {
                int index = i * 4 * 2 + j * 2 + k;
                new_value_array[index] = old_value_array[i];
            }

            item->setData(QJsonValue(new_value_array), true);
            found++;
            processed++;
        }
    }
}
