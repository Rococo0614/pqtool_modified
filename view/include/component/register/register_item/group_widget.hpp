#ifndef GROUPITEMWIDGET_H
#define GROUPITEMWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>
#include <QPushButton>
#include "base_item_widget.hpp"

class GroupItemWidget : public QWidget
{
    Q_OBJECT
public:
    explicit GroupItemWidget(QString title, QWidget *parent = nullptr);

    void addWidget(QWidget *widget);
    void addPageWidget(QWidget *widget, QWidget *page);
    void addLayout(QLayout *layout);
    void setRegSection(QString reg_section);
    void updateVisible();
    void setLevel(int level);
    int getLevel();
    static QString getTempDirPath();
    void setConfigSetHidden();

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QString getConfigFilePath(int idx);

protected slots:
    void handleSaveConfigSet(int idx);
    void handleLoadConfigSet(int idx);

protected:
    QLabel *title_label;
    QWidget *group_widget;
    QVBoxLayout *group_layout;
    QPushButton *config_set_btn;
    QString reg_section;
    QList<BaseItemWidget *> base_item_list;
    int level{0};
};

#endif // GROUPITEMWIDGET_H
