#include <QLabel>

class RegionLabel: public QLabel
{
    Q_OBJECT
public:
    RegionLabel(QWidget* parent = nullptr);

protected:
    virtual void mouseReleaseEvent(QMouseEvent * ev);

signals:
    void clicked();
};
