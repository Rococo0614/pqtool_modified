#include "rgn_label.hpp"

RegionLabel::RegionLabel(QWidget* parent) : QLabel (parent)
{

}

void RegionLabel::mouseReleaseEvent(QMouseEvent * ev)
{
    QLabel::mouseReleaseEvent(ev);
    emit clicked();
}
