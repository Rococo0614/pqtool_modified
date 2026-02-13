#ifndef CLUT_ONLINE_PAGE_WIDGET_H
#define CLUT_ONLINE_PAGE_WIDGET_H

#include "base_page_widget.hpp"
#include "clut_online_item_widget.hpp"
#include <QChartView>
#include <QLineSeries>
#include <QValueAxis>
#include <QGridLayout>
#include <QLabel>

QT_CHARTS_USE_NAMESPACE

class ClutOnlinePageWidget : public BasePageWidget
{
    Q_OBJECT

public:
    explicit ClutOnlinePageWidget(QString module_id, QWidget *parent = nullptr);

public slots:
    virtual void readData() override;
    virtual void writeData() override;

protected:
    void initLeftArea() override;
    void initCenterArea() override;

private:
    void initOnlineWidgets();

private:
    QScrollArea *scroll_area;
    BasePageWidget *clut_page;
    UniversalMatrixItemWidget *hbyh_matrix{nullptr};
    UniversalMatrixItemWidget *sbyh_matrix{nullptr};
    UniversalMatrixItemWidget *lbyh_matrix{nullptr};
    UniversalMatrixItemWidget *hbys_matrix{nullptr};
    UniversalMatrixItemWidget *sbys_matrix{nullptr};
    UniversalMatrixItemWidget *lbys_matrix{nullptr};
    UniversalMatrixItemWidget *hbyl_matrix{nullptr};
    UniversalMatrixItemWidget *sbyl_matrix{nullptr};
    UniversalMatrixItemWidget *lbyl_matrix{nullptr};
    QList<CLUTOnlineItemWidget *> clut_online_item_list;
};

#endif // CLUT_ONLINE_PAGE_WIDGET_H
