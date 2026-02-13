#include "cv_line_edit.hpp"

CvLineEdit::CvLineEdit(QWidget* parent) : QLineEdit (parent)
{

}

void CvLineEdit::focusInEvent(QFocusEvent *event)
{
    QLineEdit::focusInEvent(event);
}

void CvLineEdit::focusOutEvent(QFocusEvent *event)
{
    QLineEdit::focusOutEvent(event);

    emit sigLostFocus(this->text());
}
