#ifndef SEARCH_BOX_WIDGET_HPP
#define SEARCH_BOX_WIDGET_HPP

#include "qmainwindow.h"
#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QBoxLayout>

class SearchBoxWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SearchBoxWidget(QWidget *parent = nullptr);
    void initUI();

private:
    QPushButton *pButton = nullptr;
    QLineEdit *pLineEdit = nullptr;
    QBoxLayout *main_layout;
    QMainWindow *main_window;

signals:
    void sigSearchItem(QString item);

private slots:
    void startSearch();

};

#endif
