#include "register_edit.hpp"

RegisterEdit::RegisterEdit(QString title, QWidget *parent) :QWidget(parent)
{
    QVBoxLayout *main_layout = new QVBoxLayout(this);

    QGroupBox * groupbox = new QGroupBox();
    QVBoxLayout *layout = new QVBoxLayout(groupbox);
    groupbox->setMaximumSize(300, 80);
    title_label = new QLabel(title, this);
    title_label->setAlignment(Qt::AlignCenter);
    title_label->setMaximumSize(300, 40);
    title_label->setStyleSheet("background-color:rgb(220, 220, 220);");

    QHBoxLayout *hlayout = new QHBoxLayout();
    hlayout->setContentsMargins(0, 0, 0, 0);
    hlayout->setSpacing(0);
    prefix_label = new QLabel(this);
    prefix_label->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    prefix_label->setMaximumSize(20, 40);
    prefix_label->setStyleSheet("background-color:rgb(245, 245, 245);");
    value_edit = new QLineEdit(this);
    value_edit->setMaximumSize(280, 40);
    value_edit->setFrame(false);
    value_edit->setAlignment(Qt::AlignCenter);
    value_edit->setStyleSheet("background-color:rgb(245, 245, 245);");
    hlayout->addWidget(prefix_label, Qt::AlignLeft);
    hlayout->addWidget(value_edit, Qt::AlignLeft);
    connect(value_edit, &QLineEdit::editingFinished, this, &RegisterEdit::handleEditFinished);

    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    layout->addWidget(title_label);
    layout->addLayout(hlayout);
    main_layout->addWidget(groupbox);
}

void RegisterEdit::setData(QString data)
{
    if ((base == 2) || (base == 8) || (base == 10) || (base == 16)) {

        uint value = data.toUInt(nullptr, base);
        QString value_str = QString::number(value, base);
        if (value_str.size() > length) {
            value_str = value_str.left(length);
        } else {
            QString str;
            str.resize(length - value_str.size(), '0');
            value_str = str + value_str;
        }

        value_edit->setText(value_str);
    } else {
        value_edit->setText(data);
    }
}

QString RegisterEdit::getData()
{
    return value_edit->text();
}

QString RegisterEdit::getTitle()
{
    return title_label->text();
}

void RegisterEdit::setTitle(QString title)
{
    title_label->setText(title);
}

void RegisterEdit::handleEditFinished()
{
    if ((base == 2) || (base == 8) || (base == 10) || (base == 16)) {

        uint value = value_edit->text().toUInt(nullptr, base);
        QString value_str = QString::number(value, base);
        if (value_str.size() > length) {
            value_str = value_str.left(length);
        } else {
            QString str;
            str.resize(length - value_str.size(), '0');
            value_str = str + value_str;
        }

        value_edit->setText(value_str);
    }
}

void RegisterEdit::setEditProperty(QString prefix, int base, int length, bool readonly)
{
    prefix_label->setText(prefix);
    value_edit->setReadOnly(readonly);

    if (base == 2) {
        QRegExp regx(QString("[0-1]{1,%1}").arg(length));
        QValidator *validator = new QRegExpValidator(regx);
        value_edit->setValidator(validator);
        this->base = base;
        if (length > 0) {
            this->length = length;
        }
    } else if (base == 8) {
        QRegExp regx(QString("[0-7]{1,%1}").arg(length));
        QValidator *validator = new QRegExpValidator(regx);
        value_edit->setValidator(validator);
        this->base = base;
        if (length > 0) {
            this->length = length;
        }
    } else if (base == 10) {
        QRegExp regx(QString("[0-9]{1,%1}").arg(length));
        QValidator *validator = new QRegExpValidator(regx);
        value_edit->setValidator(validator);
        this->base = base;
        if (length > 0) {
            this->length = length;
        }
    } else if (base == 16) {
        QRegExp regx(QString("[A-Fa-f0-9]{1,%1}").arg(length));
        QValidator *validator = new QRegExpValidator(regx);
        value_edit->setValidator(validator);
        this->base = base;
        if (length > 0) {
            this->length = length;
        }
    }
}

void RegisterEdit::getEditProperty(QString *prefix, int *base, int *length)
{
    if (prefix) {
        *prefix = prefix_label->text();
    }

    if (base) {
        *base = this->base;
    }

    if (length) {
        *length = this->length;
    }
}
