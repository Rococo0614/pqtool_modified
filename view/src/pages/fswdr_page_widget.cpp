#include "fswdr_page_widget.hpp"
#include "global_data.hpp"

FSWDRPageWidget::FSWDRPageWidget(QWidget *parent) : BasePageWidget(MODULE_NAME_FSWDR, parent)
{
    initData();
    initUI();
    main_layout->setStretch(0, 8);
    main_layout->setStretch(1, 1);
}

void FSWDRPageWidget::initLeftArea()
{
    initGroupLeftArea();
}

void FSWDRPageWidget::initCenterArea()
{
    //initChart();
}

void FSWDRPageWidget::initChart()
{
    /*QLabel *head_label = new QLabel(tr("WDRCombineCurve"));
    QVBoxLayout *v_layout = new QVBoxLayout;
    v_layout->addWidget(head_label);
    QString module = getModuleName(static_cast<MODULE_ID>(property("module_id").toInt()));

    ///chart = new ISOChartItemWidget(module, "curve");
    //chart->setInterpolationNum(2);
    //chart->setOpType(OP_TYPE_AUTO);

    QList<SecKeyType> reg_section_key_list;
    reg_section_key_list.append(SecKeyType(module, "curve"));
    //chart->setRegSectionKeyList(std::move(reg_section_key_list));

    //v_layout->addWidget(chart);

    QLabel *longThr_label = new QLabel(tr("LongThr"));
    longThr_edit = new QLineEdit;
    longThr_edit->setMaximumWidth(150);
    QLabel *shortThr_label = new QLabel(tr("ShortThr"));
    shortThr_edit = new QLineEdit;
    shortThr_edit->setMaximumWidth(150);
    QLabel *maxWeight_label = new QLabel(tr("MaxWeight"));
    maxWeight_edit = new QLineEdit;
    maxWeight_edit->setMaximumWidth(150);
    QLabel *minWeight_label = new QLabel(tr("minWeight"));
    minWeight_edit = new QLineEdit;
    minWeight_edit->setMaximumWidth(150);

    QWidget *w = new QWidget;
    QGridLayout *grid_layout = new QGridLayout(w);
    grid_layout->addWidget(longThr_label, 0, 0);
    grid_layout->addWidget(longThr_edit, 0, 1, Qt::AlignLeft);
    grid_layout->addWidget(maxWeight_label, 0, 2);
    grid_layout->addWidget(maxWeight_edit, 0, 3, Qt::AlignLeft);
    grid_layout->addWidget(shortThr_label, 1, 0);
    grid_layout->addWidget(shortThr_edit, 1, 1, Qt::AlignLeft);
    grid_layout->addWidget(minWeight_label, 1, 2);
    grid_layout->addWidget(minWeight_edit, 1, 3, Qt::AlignLeft);
    //chart->setEditPanelWidget(w);

    main_layout->addLayout(v_layout);*/
}
