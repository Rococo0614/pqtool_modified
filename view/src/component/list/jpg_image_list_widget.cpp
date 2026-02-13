#include <QMenuBar>
#include <QLabel>
#include <QHeaderView>
#include <QCheckBox>
#include <QDir>
#include <QFileDialog>

#include "global_data.hpp"
#include "jpg_image_list_widget.hpp"
#include "isp_info_dialog.hpp"

#define MAX_WIN_WIDTH (400)
#define MAX_WIN_HEIGHT (400)

JpgImageListWidget::JpgImageListWidget(QWidget *parent) : QWidget(parent)
{
    initUI();
    initEvent();
    retranslateUi();
}

void JpgImageListWidget::addTableItems(QList<JpgImageListItem *> list)
{
    int n = image_list.size();
    for (int i = 0; i < list.size(); i++) {
        addTableItem(list[i]);
    }

    if ((image_list.size() > n) && (n > 0)) {
        emit imageListChanged(ROW_ADD_CAPTURE);
    } else if ((image_list.size() > n) && (n == 0)) {
        emit imageListChanged(LIST_IMPORT_CAPTURE);
    }
}

void JpgImageListWidget::addTableItem(JpgImageListItem *item)
{
    if (!item) {
        return;
    }

    int index = image_list.size();
    this->image_list.append(item);

    image_table_widget->setRowCount(image_list.size());

    QTableWidgetItem *name_item = new QTableWidgetItem(item->getFileName());

    image_table_widget->setItem(index, 0, name_item);

    QString str = QString::number(item->getRMS()) + "/" + QString::number(item->getRMSTotal());
    QTableWidgetItem *rms_item = new QTableWidgetItem(str);
    image_table_widget->setItem(index, 1, rms_item);

    QPushButton *del_btn = new QPushButton(tr("X"));
    del_btn->setMaximumWidth(30);
    image_table_widget->setCellWidget(index, 2, del_btn);
    connect(del_btn, &QPushButton::clicked, this, [=]{del_one();});
}

void JpgImageListWidget::del_one()
{
    int row = image_table_widget->currentRow();
    image_table_widget->removeRow(row);
    image_list.removeAt(row);
    if (image_list.isEmpty()) {
        emit imageListChanged(DEL_ALL);
    } else {
        emit imageListChanged(DEL_ONE);
    }
}

void JpgImageListWidget::del_all()
{
    while(image_table_widget->rowCount() != 0) {
        image_table_widget->removeRow(0);
    }
    image_list.clear();
    emit imageListChanged(DEL_ALL);
}

QList<JpgImageListItem *> JpgImageListWidget::getJpgImageList()
{
    return this->image_list;
}

void JpgImageListWidget::initEvent()
{
    connect(image_table_widget, &CviTableWidget::rowSwaped, this, &JpgImageListWidget::updateImageList);
}

void JpgImageListWidget::updateImageList(int from, int to)
{
    qDebug("updateImageList,from:%d, to:%d", from, to);
    if (from < 0 || from > image_list.size() || to < 0 || to > image_list.size())
        return;
    image_list.move(from, to);
    for (int i =0;i<image_list.size();i++) {
        qDebug("name:%s", image_list[i]->getFileName().toStdString().c_str());
    }
    emit imageListChanged(ROW_SWAP);
}

void JpgImageListWidget::initUI()
{
    this->setFixedSize(MAX_WIN_WIDTH, MAX_WIN_HEIGHT);
    main_layout = new QVBoxLayout(this);

    QHBoxLayout *ctrl_layout = new QHBoxLayout;
    QLabel *image_format_label = new QLabel;
    image_format_label->setText(tr("Format:"));
    image_format_label->setFixedSize(60, 30);
    image_format_box = new QComboBox();
    image_format_box->setFixedSize(80, 30);
    QStringList str_list = {".jpg", ".png", ".bmp"};
    image_format_box->addItems(str_list);
    image_format_box->setCurrentIndex(0);
    QPushButton *open_btn = new QPushButton(tr("Open"));
    open_btn->setFixedSize(80,30);
    connect(open_btn, &QPushButton::clicked, this, [=]{
        importImages();
    });
    QPushButton *clean_btn = new QPushButton(tr("Clean"));
    clean_btn->setFixedSize(80,30);
    connect(clean_btn, &QPushButton::clicked, this, [=]{
        del_all();
    });
    ctrl_layout->addWidget(open_btn, 0, Qt::AlignLeft);
    ctrl_layout->addWidget(clean_btn, 0, Qt::AlignLeft);
    ctrl_layout->addWidget(image_format_label, 0, Qt::AlignRight);
    ctrl_layout->addWidget(image_format_box, 0, Qt::AlignRight);

    image_table_widget = new CviTableWidget(0, 3);
    image_table_widget->setColumnWidth(0, 200);
    image_table_widget->setColumnWidth(1, 150);
    image_table_widget->setColumnWidth(2, 30);
    image_table_widget->clearContents();

    main_layout->addLayout(ctrl_layout);
    main_layout->addWidget(image_table_widget);
    setLayout(main_layout);
}

void JpgImageListWidget::importImages(QString dirPath)
{
    QString path;

    if (dirPath.isEmpty()) {
        path = QFileDialog::getExistingDirectory(this, "Please Select image dir!", "./");
        if (path.isEmpty()){
            return;
        }
    } else {
        path = dirPath;
    }

    QDir dir(path);
    dir.setFilter(QDir::Files | QDir::NoSymLinks);
    dir.setNameFilters({QString("*") + image_format_box->currentText()});
    QFileInfoList fileList = dir.entryInfoList();
    if (fileList.isEmpty()) {
        return;
    }

    int n = image_list.size();

    for (int i = 0; i < fileList.size(); i++) {
        QString file_path = fileList[i].absoluteFilePath();
        int j;
        for (j = 0; j < n; j++) {
            if (file_path == image_list[i]->getFilePath()) {
                break;
            }
        }
        if (j >= n) {
            JpgImageListItem *item = new JpgImageListItem(file_path);
            addTableItem(item);
        }
    }

    if ((image_list.size() > n) && (n > 0)) {
        emit imageListChanged(ROW_ADD_LOCAL);
    } else if ((image_list.size() > n) && (n == 0)) {
        emit imageListChanged(LIST_IMPORT_LOCAL);
    }
}

void JpgImageListWidget::updateRMS(double rms, QList<double> *list)
{
    for (int i = 0; i < list->size(); i++) {
        image_list[i]->setRMS(list->at(i));
        image_list[i]->setRMSTotal(rms);
        QTableWidgetItem *rms_item = image_table_widget->item(i, 1);
        QString str = QString::number(list->at(i)) + "/" + QString::number(rms);
        rms_item->setText(str);
    }
}

void JpgImageListWidget::retranslateUi()
{
     this->image_table_widget->setHorizontalHeaderLabels({tr("File"), tr("rms(this/total)"), tr("Del")});
}
