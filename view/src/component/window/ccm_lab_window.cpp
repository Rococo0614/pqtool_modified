#include "ccm_lab_window.hpp"
#include "raw_image_format_dialog.hpp"
#include "utils.hpp"
#include "value_validator.hpp"
#include "global_data.hpp"
#include "main_window.hpp"

#include <QHeaderView>
#include <QLabel>
#include <QCloseEvent>
#include <QFileDialog>
#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include <QMessageBox>

const int MIN_WINDOW_WIDTH = 760;
const int MIN_WINDOW_HEIGHT = 660;


CCMLabWindow::CCMLabWindow(QWidget *parent) :
    QMainWindow(parent)
{
    setWindowTitle(tr("LAB Customization"));
    setWindowFlags(Qt::WindowCloseButtonHint);
    setFixedSize(MIN_WINDOW_WIDTH, MIN_WINDOW_HEIGHT);
    initUI();
    SetDefaultLab();
    initEvent();
}

void CCMLabWindow::initUI()
{
    main_widget = new QWidget();
    main_layout = new QVBoxLayout();

    labImg = new CcmLabImageWindow();

    color_group = new QGroupBox(tr("Standard Color(In CIELab)"));
    QVBoxLayout *color_layout = new QVBoxLayout;

    QHBoxLayout *import_layout = new QHBoxLayout;
    QLabel *import_lab = new QLabel("Import From:");
    QPushButton *text_btn = new QPushButton( "Text File");
    connect(text_btn, &QPushButton::clicked, this, &CCMLabWindow::ImportLabFile);
    QPushButton *image_btn = new QPushButton( "Image");
    connect(image_btn, &QPushButton::clicked, this, &CCMLabWindow::ImportImage);
    import_layout->addWidget(import_lab);
    import_layout->addWidget(text_btn);
    import_layout->addWidget(image_btn);
    import_layout->addStretch(1);

    lab_table = new QTableWidget(16, 8);
    for (int i=0;i<8;i++) {
        lab_table->setColumnWidth(i,60);
        lab_table->setRowHeight(i,15);
    }
    for (int i=0;i<16;i++) {
        for (int j=0;j<8;j++) {
            QLineEdit *lab_edit = new QLineEdit;
            lab_edit->setFrame(false);
            lab_edit->setAlignment(Qt::AlignCenter);
            if (i%4==0) {
                lab_edit->setReadOnly(true);
            }
            if (j==0) {
                lab_edit->setReadOnly(true);
                lab_edit->setText(QString("Row %1").arg(i/4+1));
                lab_edit->setStyleSheet(QString("background-color:rgb(245, 245, 245)"));
            } else if (j==1) {
                int k = i % 4;
                switch (k) {
                case 0:
                    lab_edit->setText("Sample");
                    break;
                case 1:
                    lab_edit->setText("L");
                    break;
                case 2:
                    lab_edit->setText("a*");
                    break;
                case 3:
                    lab_edit->setText("b*");
                    break;
                }
                lab_edit->setReadOnly(true);
                lab_edit->setStyleSheet(QString("background-color:rgb(245, 245, 245)"));
            } else {
                int k = i % 4;
                switch (k) {
                case 1:
                    lab_edit->setMaxLength(8);
                    connect(lab_edit,&QLineEdit::editingFinished,[=]{
                        double data = lab_edit->text().toDouble();
                        data = qBound(0.0, data, 100.0);
                        lab_edit->setText(QString::number(data));
                        emit sigLabChange(i,j);
                    });
                    break;
                case 2:
                case 3:
                    lab_edit->setMaxLength(8);
                    connect(lab_edit,&QLineEdit::editingFinished,[=]{
                        double data = lab_edit->text().toDouble();
                        data = qBound(-128.0, data, 127.0);
                        lab_edit->setText(QString::number(data));
                        emit sigLabChange(i,j);
                    });
                    break;
                }
            }
            lab_table->setCellWidget(i,j,lab_edit);
        }
    }
    lab_table->setHorizontalHeaderLabels({tr(""), tr(""), tr("Column 1"), tr("Column 2"),
        tr("Column 3"), tr("Column 4"), tr("Column 5"), tr("Column 6")});
    lab_table->verticalHeader()->hide();
    lab_table->horizontalHeader()->setStyleSheet("QHeaderView::section{background:WhiteSmoke}");
    lab_table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    lab_table->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    color_layout->addLayout(import_layout);
    color_layout->addWidget(lab_table);
    color_group->setLayout(color_layout);

    QHBoxLayout *opt_layout = new QHBoxLayout;
    QPushButton *export_btn = new QPushButton( "Export LAB");
    connect(export_btn, &QPushButton::clicked, this, &CCMLabWindow::ExportLabFile);
    QPushButton *ok_btn = new QPushButton( "OK");
    connect(ok_btn, &QPushButton::clicked,[=]{
        emit sigLabOk();
        this->close();
    });
    QPushButton *cancel_btn = new QPushButton( "Cancel");
    connect(cancel_btn, &QPushButton::clicked, this, &CCMLabWindow::close);
    opt_layout->addStretch(1);
    opt_layout->addWidget(export_btn);
    opt_layout->addWidget(ok_btn);
    opt_layout->addWidget(cancel_btn);

    main_layout->addWidget(color_group);
    main_layout->addLayout(opt_layout);

    setCentralWidget(main_widget);
    centralWidget()->setLayout(main_layout);
}

void CCMLabWindow::initEvent()
{
    connect(this, &CCMLabWindow::sigLabChange, this, &CCMLabWindow::HandleLabChange);
    connect(labImg, &CcmLabImageWindow::sigLabImgOk, this, &CCMLabWindow::HandleImgLab);
}

void CCMLabWindow::closeEvent(QCloseEvent *event)
{
    shared_ptr<NetworkClientHelper> network_helper = NetworkClientHelper::getInstance();
    event->accept();
    QMainWindow::closeEvent(event);
    //TODO handle close event
}

void CCMLabWindow::GetLab(double *l, double *a, double *b)
{
    if ((l == nullptr) || (a == nullptr) || (b == nullptr)) {
        return;
    }

    int k = 0;
    for (int i=0;i<16;i++) {
        for (int j=2;j<8;j++) {
            if (i%4 == 1) {
                QLineEdit *lab_item_l = (QLineEdit *)lab_table->cellWidget(i,j);
                QLineEdit *lab_item_a = (QLineEdit *)lab_table->cellWidget(i+1,j);
                QLineEdit *lab_item_b = (QLineEdit *)lab_table->cellWidget(i+2,j);
                l[k] = lab_item_l->text().toDouble();
                a[k] = lab_item_a->text().toDouble();
                b[k] = lab_item_b->text().toDouble();
                k++;
            }
        }
    }
}

void CCMLabWindow::HandleImgLab()
{
    if (labImg == nullptr) {
        return;
    }
    labImg->GetImgRgb(red, green, blue);
    for (int i=0;i<24;i++) {
        utils::RgbToLab(red[i], green[i], blue[i], &lab_ref_l[i], &lab_ref_a[i], &lab_ref_b[i]);
    }
    SetLabTable(lab_ref_l, lab_ref_a, lab_ref_b, red, green, blue);
}

void CCMLabWindow::keyReleaseEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape) {
        this->close();
    }
}

void CCMLabWindow::SetLabTable(double *l, double *a, double *b, double *red, double *green, double *blue)
{
    int k=0, t=0, m=0, n=0;
    for (int i=0;i<16;i++) {
        for (int j=2;j<8;j++) {
            QLineEdit *lab_item = (QLineEdit *)lab_table->cellWidget(i,j);
            if (i%4==0) {
                int temp_r = (int)(red[n] + 0.5);
                int temp_g = (int)(green[n] + 0.5);
                int temp_b = (int)(blue[n] + 0.5);
                temp_r = qBound(0, temp_r, 255);
                temp_g = qBound(0, temp_g, 255);
                temp_b = qBound(0, temp_b, 255);
                lab_item->setStyleSheet(QString("background-color:rgb(%1, %2, %3)").arg(temp_r).arg(temp_g).arg(temp_b));
                qDebug("row:%d red[%d]:%d,green[%d]:%d,blue[%d]:%d\n", i,n,temp_r,n,temp_g,n,temp_b);
                n++;
            } else {
                switch (i%4) {
                    case 1:
                        lab_item->setText(QString::number(l[k]));
                        k++;
                        break;
                    case 2:
                        lab_item->setText(QString::number(a[t]));
                        t++;
                        break;
                    case 3:
                        lab_item->setText(QString::number(b[m]));
                        m++;
                        break;
                }
            }
        }
    }
}

void CCMLabWindow::SetLabItem(int row, int col)
{
    double l,a,b,red,green,blue;
    switch (row%4) {
        case 1: {
            QLineEdit *lab_item_l = (QLineEdit *)lab_table->cellWidget(row,col);
            QLineEdit *lab_item_a = (QLineEdit *)lab_table->cellWidget(row+1,col);
            QLineEdit *lab_item_b = (QLineEdit *)lab_table->cellWidget(row+2,col);
            QLineEdit *lab_item_rgb = (QLineEdit *)lab_table->cellWidget(row-1,col);
            l = lab_item_l->text().toDouble();
            a = lab_item_a->text().toDouble();
            b = lab_item_b->text().toDouble();
            utils::LabToRgb(l, a, b, &red, &green, &blue);
            int temp_r = (int)(red + 0.5);
            int temp_g = (int)(green + 0.5);
            int temp_b = (int)(blue + 0.5);
            temp_r = qBound(0, temp_r, 255);
            temp_g = qBound(0, temp_g, 255);
            temp_b = qBound(0, temp_b, 255);
            lab_item_rgb->setStyleSheet(QString("background-color:rgb(%1, %2, %3)").arg(temp_r).arg(temp_g).arg(temp_b));
            break;
        }
        case 2: {
            QLineEdit *lab_item_l = (QLineEdit *)lab_table->cellWidget(row-1,col);
            QLineEdit *lab_item_a = (QLineEdit *)lab_table->cellWidget(row,col);
            QLineEdit *lab_item_b = (QLineEdit *)lab_table->cellWidget(row+1,col);
            QLineEdit *lab_item_rgb = (QLineEdit *)lab_table->cellWidget(row-2,col);
            l = lab_item_l->text().toDouble();
            a = lab_item_a->text().toDouble();
            b = lab_item_b->text().toDouble();
            utils::LabToRgb(l, a, b, &red, &green, &blue);
            int temp_r = (int)(red + 0.5);
            int temp_g = (int)(green + 0.5);
            int temp_b = (int)(blue + 0.5);
            temp_r = qBound(0, temp_r, 255);
            temp_g = qBound(0, temp_g, 255);
            temp_b = qBound(0, temp_b, 255);
            lab_item_rgb->setStyleSheet(QString("background-color:rgb(%1, %2, %3)").arg(temp_r).arg(temp_g).arg(temp_b));
            break;
        }
        case 3: {
            QLineEdit *lab_item_l = (QLineEdit *)lab_table->cellWidget(row-2,col);
            QLineEdit *lab_item_a = (QLineEdit *)lab_table->cellWidget(row-1,col);
            QLineEdit *lab_item_b = (QLineEdit *)lab_table->cellWidget(row,col);
            QLineEdit *lab_item_rgb = (QLineEdit *)lab_table->cellWidget(row-3,col);
            l = lab_item_l->text().toDouble();
            a = lab_item_a->text().toDouble();
            b = lab_item_b->text().toDouble();
            utils::LabToRgb(l, a, b, &red, &green, &blue);
            int temp_r = (int)(red + 0.5);
            int temp_g = (int)(green + 0.5);
            int temp_b = (int)(blue + 0.5);
            temp_r = qBound(0, temp_r, 255);
            temp_g = qBound(0, temp_g, 255);
            temp_b = qBound(0, temp_b, 255);
            lab_item_rgb->setStyleSheet(QString("background-color:rgb(%1, %2, %3)").arg(temp_r).arg(temp_g).arg(temp_b));
            break;
        }
    }
}

void CCMLabWindow::ExportLabFile()
{
    QString path = QFileDialog::getSaveFileName(this, tr("Save File"), "lab.txt", "Text files (*.txt)");

    if (path.isEmpty()) {
        return;
    }


    QFile labfile(path);
    if (!labfile.open(QIODevice::WriteOnly)) {
        return;
    }

    QString data = "";
    for (int i=0;i<16;i++) {
        for (int j=2;j<8;j++) {
            if (i%4 == 1) {
                QLineEdit *lab_item_l = (QLineEdit *)lab_table->cellWidget(i,j);
                QLineEdit *lab_item_a = (QLineEdit *)lab_table->cellWidget(i+1,j);
                QLineEdit *lab_item_b = (QLineEdit *)lab_table->cellWidget(i+2,j);
                data += lab_item_l->text();
                data += "	";
                data += lab_item_a->text();
                data += "	";
                data += lab_item_b->text();
                data += "\n";
            }
        }
    }

    labfile.write(data.toStdString().c_str());
    labfile.close();
    QMessageBox message(QMessageBox::NoIcon, "Notice", "Export lab text File success!");
    message.exec();
}

void CCMLabWindow::ImportLabFile()
{
    qDebug("CCMLabWindow::ImportLabFile\n");
    QString file_path = QFileDialog::getOpenFileName(this, tr("Open File"), "", tr("Text files (*.txt)"));
    QFile input_file(file_path);
    if(!input_file.open(QIODevice::ReadOnly)) {
        qDebug("Import file :%s error", qUtf8Printable(file_path));
        return;
    }

    QString data = "";
    QTextStream in(&input_file);
    int n = 0;
    while (!in.atEnd())
    {
       data = in.readLine();
       QList<QVariant> data_list = utils::stringToList<QVariant>(data, "	");
       if ((data_list.size() == 3) && (n < 24)) {
           lab_ref_l[n] = data_list[0].toDouble();
           lab_ref_a[n] = data_list[1].toDouble();
           lab_ref_b[n] = data_list[2].toDouble();
           n++;
       }
    }

    for (int i=0;i<24;i++) {
        utils::LabToRgb(lab_ref_l[i], lab_ref_a[i], lab_ref_b[i], &red[i], &green[i], &blue[i]);
    }
    SetLabTable(lab_ref_l, lab_ref_a, lab_ref_b, red, green, blue);

    input_file.close();
}

void CCMLabWindow::ImportImage()
{
    qDebug("CCMLabWindow::ImportImage\n");
    QString img_path = QFileDialog::getOpenFileName(this, tr("Open File"), "", "*.jpg *.bmp");
    
    if (img_path.isEmpty()) {
        return;
    }

    QFileInfo file(img_path);
    if (!file.exists()) {
        QMessageBox::warning(this, tr("Error!"), tr("Can't find image file!"), QMessageBox::Ok);
        return;
    }

    labImg->show();
    labImg->activateWindow();
    labImg->OpenImage(img_path);
}

void CCMLabWindow::HandleLabChange(int row, int col)
{
    qDebug("HandleLabChange,row:%d,col:%d\n", row,col);
    SetLabItem(row,col);
}

void CCMLabWindow::SetDefaultLab()
{
    qDebug("CCMLabWindow::SetDefaultLab()\n");

    double l[24] = {37.54, 65.20, 50.37, 43.13, 55.34, 71.36, 61.37, 40.71, 49.86,
                    30.15, 72.44, 70.92, 29.62, 55.64, 40.55, 80.98, 51.01, 52.12,
                    96.54, 81.27, 66.79, 50.87, 35.68, 20.48};

    double a[24] = {12.02, 14.82, -1.57, -14.63, 11.45, -32.72, 32.88, 16.91, 45.93,
                    24.91, -27.46, 15.58, 21.43, -40.76, 49.97, -1.04, 49.88, -24.61,
                    -0.69, -0.61, -0.65, -0.06, -0.22, 0.05};

    double b[24] = {13.33, 17.55, -21.43, 22.12, -25.29, 1.64, 55.16, -45.09, 13.88,
                    -22.61, 58.47, 66.54, -49.03, 33.27, 25.46, 80.03, -16.93, -26.18,
                    1.35, -0.24, -0.43, -0.25, -1.21, -0.97};

    for (int i=0;i<24;i++) {
        utils::LabToRgb(l[i], a[i], b[i], &red[i], &green[i], &blue[i]);
    }
    SetLabTable(l, a, b, red, green, blue);

}


