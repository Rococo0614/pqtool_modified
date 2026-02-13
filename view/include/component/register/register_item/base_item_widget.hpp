#ifndef BASEITEMWIDGET_H
#define BASEITEMWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>
#include <QVariant>
#include <QPushButton>

#include "i_observer.hpp"

class BaseItemWidget : public QWidget, public IObserver
{
    Q_OBJECT
public:
    explicit BaseItemWidget(QString title, QWidget *parent = nullptr);
    explicit BaseItemWidget(QString module, QString title, QWidget *parent = nullptr);

    QString getKey() const;
    QString getTitle() const;
    void setDesp(QString desp);
    QString getDesp() const;
    void setGroup(QString group);
    QString getGroup() const;
    void uiReset();
    void setLevel(int level);
    int getLevel();
    void updateVisible();
    virtual void updateHighlight(bool bHiglight);
    QString getValueType() const;
    void setValueType(QString value_type);
    void setModule(QString module);
    QString getModule() const;
    void setLabelWidth(int w, bool is_extended=false);
    void setLTKey(SecKeyType &lt_key);
    void setMTKey(SecKeyType &mt_key);
    void setStructure(QString structure);

    virtual void initSubWidgets();
    virtual void setChmod(int mod) override;
    virtual void setParam(QVariant params);
    virtual QVariant getData() const;
    virtual QJsonObject getJsonData() const;
    virtual void setData(QVariant, bool sync_to_global = false);
    virtual void refreshData(const SecKeyType &reg_section_key) override;
    virtual void setValueMappingVec(QVector<QVariantList> &&value_mapping_vec);
    virtual void setOpType(QString op_type) override;
    virtual QString getOpType() const override;
    virtual void handleUpdateTheme(bool dark_theme_enabled);

    void addPath(QString path) { this->path_list.append(path); }
    QStringList getPathList() { return this->path_list; }
    virtual QJsonValue getJsonValue(QString path) const;
    virtual QVariant getDataJsonrpc(QString path) const;
    virtual void setDataJsonrpc(QString path, QVariant data, bool sync_to_global = false);
    virtual QJsonArray getSaveData() const;

protected:
    virtual void syncValueChanged(QVariant value);
    void paintEvent(QPaintEvent *event) override;
    void changeEvent(QEvent *event) override;
    // get ui item value by value mapping
    QVariant getValueByValueMapping(QVariantList &&values) const;
    // get reg value by reg's idx through value mapping table (for mapping to multiple regs)
    QVariant getRegValueByValueMapping(QVariant value, SecKeyType &&reg_section_key) const;
    // get reg values by value mapping table (for value is array)
    QVariantList getRegValueListByValueMapping(QVariant value) const;
    virtual void retranslateUi();

signals:
    void sigValueChanged(QVariant value);
    void sigWritePageData();

protected slots:
    void dataLoaded();
    void handleUiChanged();

protected:
    QHBoxLayout *layout;
    QHBoxLayout *icon_layout;
    QWidget *icon_widget;
    QPushButton *icon_button;
    QLabel *title_label{nullptr};
    QWidget *value_widget;
    QHBoxLayout *value_layout;
    bool value_changed;
    bool dark_theme{false};
    QVector<QVariantList> value_mapping_vec;
    QString module;
    QString title;
    QString desp;
    int level{0};
    QString value_type;
    QString group;
    QString structure;
    QString op_type;
    SecKeyType lt_key = QPair<QString, QString>();
    SecKeyType mt_key = QPair<QString, QString>();

    QStringList path_list;
};

#endif // BASEITEMWIDGET_H
