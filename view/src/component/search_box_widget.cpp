#include "search_box_widget.hpp"
#include <QVBoxLayout>

const int WIDGET_MAX_WIDTH = 180;

SearchBoxWidget::SearchBoxWidget(QWidget *parent) : QWidget(parent)
{
    initUI();
}

void SearchBoxWidget::initUI()
{
    QBoxLayout *main_layout = new QHBoxLayout(this);
    pLineEdit = new QLineEdit();
    pButton   = new QPushButton();
    pButton->setStyleSheet("QPushButton{image:url(:/icons/search.png); background:transparent;}");
    pLineEdit->setStyleSheet("border:2px groove gray;border-radius:10px;padding:2px 4px");
    main_layout->addStretch();
    main_layout->addWidget(pLineEdit);
    main_layout->addWidget(pButton);
    this->setMaximumWidth(WIDGET_MAX_WIDTH);

    connect(pButton, &QPushButton::clicked, this, &SearchBoxWidget::startSearch);
    connect(pLineEdit, SIGNAL(returnPressed()), this, SLOT(startSearch()));
}

void SearchBoxWidget::startSearch()
{
    QString item = pLineEdit->text();
    if (!item.isEmpty()) {
        emit sigSearchItem(item);
    }
}
