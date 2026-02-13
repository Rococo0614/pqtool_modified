#include "binary_item_widget.hpp"
#include "global_data.hpp"
#include <QRadioButton>
#include <QVariantMap>

const int ENABLE_BTN_ID = 1;
const int DISABLE_BTN_ID = 2;

BinaryItemWidget::BinaryItemWidget(QString module, QString title, QWidget *parent) :
    BaseItemWidget(module, title, parent)
{
    initValueWidget();
    retranslateUi();

    QList<class BinaryItemWidget *> binary_item_ls;
    if (map_module_binaryItems.contains(module))
    {
        binary_item_ls = map_module_binaryItems.value(module);
        binary_item_ls.append(this);
        map_module_binaryItems[module] = binary_item_ls;
    } else {
        binary_item_ls.append(this);
        map_module_binaryItems.insert(module, binary_item_ls);
    }
}

BinaryItemWidget::~BinaryItemWidget() {
    if (map_module_binaryItems.contains(module)) {
        QList<BinaryItemWidget *>& binary_item_ls = map_module_binaryItems[module];

        if (binary_item_ls.contains(this)) {
            binary_item_ls.removeAll(this);
            if (binary_item_ls.isEmpty()) {
                map_module_binaryItems.remove(module);
            }
        }
    }
}

void BinaryItemWidget::setValue(bool enabled)
{
    int id = (enabled) ? ENABLE_BTN_ID : DISABLE_BTN_ID;
    if (id == current_id) return;

    current_id = id;
    QAbstractButton *btn = btn_group->button(current_id);
    btn->click();
    btn->setChecked(true);

    syncModuleValueChanged(enabled);

    syncValueChanged(static_cast<int>(enabled));
}

void BinaryItemWidget::initValueWidget()
{
    btn_group = new QButtonGroup;
    enable_btn = new QRadioButton();
    disable_btn = new QRadioButton();
    disable_btn->setChecked(true);
    current_id = DISABLE_BTN_ID;
    btn_group->addButton(enable_btn, ENABLE_BTN_ID);
    btn_group->addButton(disable_btn, DISABLE_BTN_ID);

    value_layout->addWidget(enable_btn);
    value_layout->addWidget(disable_btn);

    connect(btn_group, QOverload<int>::of(&QButtonGroup::buttonClicked),
            this, &BinaryItemWidget::valueChanged);
    connect(this, &BinaryItemWidget::sigValueChanged,
            this, &BinaryItemWidget::handleUiChanged);
}

void BinaryItemWidget::syncModuleValueChanged(bool enabled)
{
    QMap<QString, QPair<QString, QString>> topTitle_moduleTitle_map =
        GlobalData::getInstance()->getTopModuleTitleMap();

    if (topTitle_moduleTitle_map.isEmpty()) {
        return;
    }

    QStringList page_ready_list = GlobalData::getInstance()->getPageReadyList();
    QString current_module = this->module;
    QString current_title = this->title;
    QString path = this->path_list[0];
    QString ref_module, ref_title;
    bool is_find_ref = false;

    QString module_switch_page_name = "ModuleSwitch";

    if (!page_ready_list.contains(module_switch_page_name)) {
        module_switch_page_name = "Top";
    }

    if (current_module == module_switch_page_name) {
        if (topTitle_moduleTitle_map.contains(current_title)) {
            QPair<QString, QString> ref_pair = topTitle_moduleTitle_map[current_title];
            ref_module = ref_pair.first;
            ref_title = ref_pair.second;
        }
    } else {
        for (auto& top_title : topTitle_moduleTitle_map.keys()) {
            QString tmp_module = topTitle_moduleTitle_map[top_title].first;
            QString tmp_title = topTitle_moduleTitle_map[top_title].second;

            if (tmp_module.toUpper() == current_module.toUpper() && tmp_title == current_title) {
                ref_module = module_switch_page_name;
                ref_title = top_title;
                break;
            }
        }
    }

    if (ref_module.isEmpty() || ref_title.isEmpty()) {
        return;
    }

    if (map_module_binaryItems.contains(ref_module)) {
        QList<BinaryItemWidget *>Qbinary_items_ls = map_module_binaryItems[ref_module];
        for (auto& Qbinary_item : Qbinary_items_ls) {
             if (Qbinary_item->title == ref_title) {
                 is_find_ref = true;
                 Qbinary_item->setValue(enabled);
                 break;
             }
        }
    } else if (page_ready_list.contains(ref_module)) {
        // get and set page_data
        QJsonObject obj = GlobalData::getInstance()->getPageData(ref_module);

        if (!obj.contains("data")) {
            return;
        }

        QJsonObject page_data = obj["data"].toObject();
        QStringList struct_list = page_data.keys();

        if (obj.isEmpty()) {
            return;
        }
        for (int i = 0; i < struct_list.size(); ++i) {
            QString struct_id = struct_list[i];
            QJsonObject group_data = page_data[struct_id].toObject();
            QStringList keys = group_data.keys();
            for (int j = 0; j < keys.size(); ++j) {
                QString path_candicate = keys[j];

                QStringList path_parts = path_candicate.split(".");

                if (path_parts.isEmpty()) {
                    continue;
                }

                if (path_parts.last() == ref_title) {
                    QJsonObject item_obj = group_data[path_candicate].toObject();
                    item_obj["VALUE"] = static_cast<int>(enabled);
                    group_data.insert(path_candicate, item_obj);
                    page_data.insert(struct_id, group_data);
                    obj.insert("data", page_data);
                    // update page data
                    GlobalData::getInstance()->setPageData(ref_module, obj);
                    is_find_ref = true;
                    break;
                }
            }
            if (is_find_ref) {
                break;
            }
        }
    }

    if (is_find_ref) {
        QString debug_info = current_module + ": " + current_title + " -> " + ref_module + ":" + ref_title;
        qDebug("%s", qUtf8Printable(debug_info));
    }
}

void BinaryItemWidget::valueChanged(int id)
{
    bool enabled = (id == ENABLE_BTN_ID) ? true : false;
    if (id == current_id) return;

    current_id = id;

    syncModuleValueChanged(enabled);

    if (current_id == ENABLE_BTN_ID)
        syncValueChanged(static_cast<int>(true));
    else
        syncValueChanged(static_cast<int>(false));
}

QVariant BinaryItemWidget::getData() const
{
    return (ENABLE_BTN_ID ==  btn_group->checkedId()) ? 1 : 0;
}

void BinaryItemWidget::setData(QVariant data, bool sync_to_global)
{
    if (data == getData()) {
        return;
    }

    if (sync_to_global == false) {
        btn_group->blockSignals(true);
    }

    setValue(data.toBool());

    if (sync_to_global == false) {
        btn_group->blockSignals(false);
    }
}

void BinaryItemWidget::setParam(QVariant params)
{
    QMap<QString, QVariant> m = params.toMap();
    if (m.count() == 0) {
        return;
    }

    text_enable = m["text_enable"].toString();
    if (!text_enable.isEmpty())
        enable_btn->setText(text_enable);

    text_disable = m["text_disable"].toString();
    if (!text_disable.isEmpty())
        disable_btn->setText(text_disable);
}

void BinaryItemWidget::retranslateUi()
{
    enable_btn->setText(tr(text_enable.toLocal8Bit()));
    disable_btn->setText(tr(text_disable.toLocal8Bit()));
}

void BinaryItemWidget::setChmod(int mod)
{
    BaseItemWidget::setChmod(mod);

    bool disabled = (mod == MOD_RO);
    enable_btn->setDisabled(disabled);
    disable_btn->setDisabled(disabled);
}

