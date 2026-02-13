#include "clut_online_page_widget.hpp"
#include "main_window.hpp"
#include <QVBoxLayout>
#include <QtCharts>
#include <QColor>

ClutOnlinePageWidget::ClutOnlinePageWidget(QString module_id, QWidget *parent)
    : BasePageWidget(module_id, parent)
{
    initData();
    initUI();
    main_layout->setStretch(0, 8);
    main_layout->setStretch(1, 1);
    clut_page = MainWindow::getInstance()->getPageWidget(QString("CLUT"));
}

void ClutOnlinePageWidget::initLeftArea()
{
    initOnlineWidgets();

    QVBoxLayout *wraper_layout = new QVBoxLayout();
    wraper_layout->setAlignment(Qt::AlignTop);

    for (CLUTOnlineItemWidget *clut_online_item : clut_online_item_list) {
        wraper_layout->addWidget(clut_online_item);
        wraper_layout->setSpacing(20);
    }

    QWidget *widget = new QWidget;
    widget->setLayout(wraper_layout);
    scroll_area = new QScrollArea();
    scroll_area->setWidget(widget);
    main_layout->addWidget(scroll_area);
}

void ClutOnlinePageWidget::initOnlineWidgets()
{
    QString module = property("module_id").toString();
    QJsonObject obj = GlobalData::getInstance()->getPageInfo(module);
    clut_page = MainWindow::getInstance()->getPageWidget(QString("CLUT"));
    QList<BaseItemWidget *> base_item_list = clut_page->getBaseItemList();

    if (obj.contains("ready")) {
        setReadyFlag(obj["ready"].toBool());
    }

    for (int i = 0; i < base_item_list.size(); i++) {
        BaseItemWidget *item = base_item_list[i];
        QStringList path_list = item->getPathList();
        for (int j = 0; j < path_list.count(); j++) {
            if (path_list[j] == "ISP_CLUT_HSL_ATTR_S.HByH") {
                hbyh_matrix = (UniversalMatrixItemWidget *) item;
                clut_online_item_list.append(new CLUTOnlineItemWidget(module, hbyh_matrix->getTitle(), hbyh_matrix));
            } else if (path_list[j] == "ISP_CLUT_HSL_ATTR_S.SByH") {
                sbyh_matrix = (UniversalMatrixItemWidget *) item;
                clut_online_item_list.append(new CLUTOnlineItemWidget(module, sbyh_matrix->getTitle(), sbyh_matrix));
            } else if (path_list[j] == "ISP_CLUT_HSL_ATTR_S.LByH") {
                lbyh_matrix = (UniversalMatrixItemWidget *) item;
                clut_online_item_list.append(new CLUTOnlineItemWidget(module, lbyh_matrix->getTitle(), lbyh_matrix));
            } else if (path_list[j] == "ISP_CLUT_HSL_ATTR_S.HByS") {
                hbys_matrix = (UniversalMatrixItemWidget *) item;
                clut_online_item_list.append(new CLUTOnlineItemWidget(module, hbys_matrix->getTitle(), hbys_matrix));
            } else if (path_list[j] == "ISP_CLUT_HSL_ATTR_S.SByS") {
                sbys_matrix = (UniversalMatrixItemWidget *) item;
                clut_online_item_list.append(new CLUTOnlineItemWidget(module, sbys_matrix->getTitle(), sbys_matrix));
            } else if (path_list[j] == "ISP_CLUT_HSL_ATTR_S.LByS") {
                lbys_matrix = (UniversalMatrixItemWidget *) item;
                clut_online_item_list.append(new CLUTOnlineItemWidget(module, lbys_matrix->getTitle(), lbys_matrix));
            } else if (path_list[j] == "ISP_CLUT_HSL_ATTR_S.HByL") {
                hbyl_matrix = (UniversalMatrixItemWidget *) item;
                clut_online_item_list.append(new CLUTOnlineItemWidget(module, hbyl_matrix->getTitle(), hbyl_matrix));
            } else if (path_list[j] == "ISP_CLUT_HSL_ATTR_S.SByL") {
                sbyl_matrix = (UniversalMatrixItemWidget *) item;
                clut_online_item_list.append(new CLUTOnlineItemWidget(module, sbyl_matrix->getTitle(), sbyl_matrix));
            } else if (path_list[j] == "ISP_CLUT_HSL_ATTR_S.LByL") {
                lbyl_matrix = (UniversalMatrixItemWidget *) item;
                clut_online_item_list.append(new CLUTOnlineItemWidget(module, lbyl_matrix->getTitle(), lbyl_matrix));
            }
        }
    }
}

void ClutOnlinePageWidget::initCenterArea() {}

void ClutOnlinePageWidget::readData()
{
    clut_page->readData();
}

void ClutOnlinePageWidget::writeData()
{
    clut_page->writeData();
}
