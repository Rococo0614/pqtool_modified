#ifndef FS_WDR_PAGE_HPP
#define FS_WDR_PAGE_HPP

#include "base_page_widget.hpp"

class FSWDRPageWidget : public BasePageWidget
{
    Q_OBJECT

public:
    explicit FSWDRPageWidget(QWidget *parent = nullptr);

protected:
    virtual void initLeftArea() override;
    virtual void initCenterArea() override;
    void initChart();

private:
    QLineEdit *longThr_edit;
    QLineEdit *shortThr_edit;
    QLineEdit *maxWeight_edit;
    QLineEdit *minWeight_edit;

    QComboBox *iso_box;
    QPushButton *import_btn;
    QPushButton *export_btn;
};

#endif // FS_WDR_PAGE_HPP
