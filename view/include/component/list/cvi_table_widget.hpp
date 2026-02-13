#ifndef CVI_TABLE_WIDGET_HPP
#define CVI_TABLE_WIDGET_HPP

#include <QTableWidget>
#include <QDropEvent>


class CviTableWidget : public QTableWidget
{
    Q_OBJECT
public:
    explicit CviTableWidget(int rows, int columns, QWidget *parent = nullptr);

signals:
     void rowSwaped(int from, int to);
protected:
     void dropEvent(QDropEvent *event) override;

};

#endif // CVI_TABLE_WIDGET_HPP
