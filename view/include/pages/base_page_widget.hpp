#ifndef BASE_PAGE_WIDGET_H
#define BASE_PAGE_WIDGET_H

#include <QWidget>
#include <QHBoxLayout>

#include "group_widget.hpp"
#include "register_item_factory.hpp"
#include "module_define.hpp"
#include "status_log_widget.hpp"
#include "utils.hpp"

const int READ_WRITE_TIMEOUT = 6; // second

class BasePageWidget : public QWidget
{
    Q_OBJECT

public:
    explicit BasePageWidget(QString id, QWidget *parent = nullptr);

    QString getTitle();
    void setTitle(const QString &title);
    void clearDirty();
    virtual void updateUI();
    virtual void setStatusBusy(bool busy);
    static void setValueToGlobalData(const QString &section, const QString &key, QString value_str);
    QJsonArray GetModuleObjectToLocal();

    bool getReadyFlag();
    bool getPageOnlyReadProperty();
    void setReadyFlag(bool flag);
    void setBaseItemList(BaseItemWidget *widget);
    void insertBaseItemList(int idx, BaseItemWidget *widget);
    void disableButtons();
    void enableButtons();
    void disableAllPageButtons();
    void enableAllPageButtons();
    QList<BaseItemWidget *> getBaseItemList() { return base_item_list; }
    virtual QScrollArea *getPageScrollArea() {return scroll_area;}
    QStringList getStructList() { return struct_list; }
    QMap<QString, QString> getStructGetApiMap() { return struct_get_api_map; }
    QMap<QString, QString> getStructSetApiMap() { return struct_set_api_map; }

    QJsonArray GetPageSaveData();
    void readDataFromOldJsonFormat(QJsonArray obj);
    void readDataFromNewJsonFormat(QJsonArray obj);
    void RemapValueA(BaseItemWidget *item, QJsonValue &value, QString title, QString new_key, QString old_key, QString op_type, QStringList path_list, int &found, int &processed, int &unprocessed);
    void RemapValueB(BaseItemWidget *item, QJsonValue value, QString title, QString new_key, QString old_key, QString op_type, QStringList path_list, int &found, int &processed, int &unprocessed);
    void RemapValueC(BaseItemWidget *item, QJsonValue &value, QString title, QString new_key, QString old_key, QString new_optype, QString old_optype, QStringList path_list, int &found, int &processed, int &unprocessed);
    void RemapValueD(BaseItemWidget *item, QJsonValue &value, QString title, QString new_key, QString old_key, QString new_optype, QString old_optype, QStringList path_list, int &found, int &processed, int &unprocessed);
    void updateEvShowState(bool checked_state, QString iso_value, QString lv_value);
    bool getPageBusyState();
    void updatePageAutoWrite(bool checked_state);
    virtual QJsonObject GetModuleObjectToNetworkJsonrpc();
    void searchItem(int idx, QString& title);

public slots:
    virtual void readData();
    virtual void writeData();
    void handleAutoWriteChanged(bool checked_state);
    void handleEvShowChanged(bool checked_state);
    void pageWriteAll();
    void pageReadAll();
    void receiveLdcSize(int id);
    void sendLdcBinData(int id);
    virtual void paegUpdateTheme(bool dark_theme_enabled);
    virtual void handleOpTypeChanged(QVariant value);
    virtual void handleCopyApply();
    void doRecord();

signals:
    void sigWriteAll();
    void sigReadAll();
    void sigEnvirValue(bool checked_state);
    void sigStatusLog(QString message, LOG_LEVEL log_level = LOG_LEVEL::LOG_WARNING);

protected:
    virtual void initUI();
    virtual void initData();
    virtual void initLeftArea();
    virtual void initCenterArea();
    virtual void initRightArea();
    virtual void writeToIni();
    virtual void writeToNetwork();
    virtual QJsonObject GetModuleObjectToNetwork();

    void showEvent(QShowEvent *event) override;
    void changeEvent(QEvent *event) override;

    void initGroupLeftArea();
    QList<GroupItemWidget *> *initGroupWidgets();

    QJsonObject GetRegObjectToNetwork();
    void readFromIni();
    void readFromNetwork();
    void readRegFromNetwork();
    void readModuleFromNetwork();

    virtual void retranslateUi();
    void handleShortCut();
    int getGroupTitleMaxLen(const QJsonArray items_json);

    void initItemWidgets();
    MODULE_LDC_ATTR_S getModuleLDCAttr(QJsonObject module_obj);
    void sendLdcBinSize(LDC_MODULE_E module);
    void getViVpssLdcChnSize();
    void convertBinToCFiles(QByteArray &data, QString path);
    int sendLDCAttrToBoard(QJsonObject module_obj);
    int getLDCMeshTable(MODULE_LDC_ATTR_S ldc_attr);
    virtual void readModuleFromNetworkJsonrpc();
    virtual void writeToNetworkJsonrpc();
    virtual void initManuAutoWidgets();
    virtual void moveAuto2Manu(int index);
    virtual void autoManuConnect();
    virtual void autoManuDisconnect();
    virtual void recoverManuData();
    virtual bool checkIndex(UniversalMatrixItemWidget *item, int index);

protected:
    QString title;
    QPushButton *read_all_btn;
    QPushButton *write_all_btn;
    QPushButton *read_btn;
    QPushButton *write_btn;
    QPushButton *copy_apply_btn;
    QHBoxLayout *main_layout;
    QList<BaseItemWidget *> auto_item_list;
    QList<BaseItemWidget *> manu_item_list;
    QList<BaseItemWidget *> auto_iso_item_list;
    QList<BaseItemWidget *> auto_lv_item_list;
    QMap<BaseItemWidget *, BaseItemWidget *> auto_manu_item_map;
    QList<QVariant> manu_item_data;
    ComboBoxItemWidget *optype_widget{nullptr};
    QComboBox *copy_from_cbox{nullptr};
    bool manu_data_covered{false};
    int last_choice{-1};

private:
    QList<BaseItemWidget *> base_item_list;
    bool ready_flag {false};
    bool ro_page{false};
    bool page_state{false};
    QLabel *iso_label{nullptr};
    QLabel *lv_label{nullptr};
    QScrollArea *scroll_area;
    QCheckBox *ldc_bin_send_checkbox{nullptr};
    QCheckBox *env_value_checkbox{nullptr};
    QCheckBox *auto_write_checkbox{nullptr};
    SIZE_S vi_size;
    SIZE_S vpss_size;

    QStringList struct_list;
    QMap<QString, QString> struct_get_api_map;
    QMap<QString, QString> struct_set_api_map;
    QMap<QString, QJsonObject> struct_mapping_map;
};

#endif // BASE_PAGE_WIDGET_H
