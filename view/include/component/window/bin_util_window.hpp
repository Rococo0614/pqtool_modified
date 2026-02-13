#ifndef BIN_UTIL_WINDOW_HPP
#define BIN_UTIL_WINDOW_HPP

#include <QMainWindow>
#include <QGridLayout>
#include <QLabel>
#include <QComboBox>
#include <QPixmap>
#include <QGroupBox>
#include <QTextEdit>
#include <QCheckBox>

class BinUtilWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit BinUtilWindow(QWidget *parent = nullptr);
    void initMenu();
    void initUI();
    void initEvent();
    void refresh();
    void convertBinToCFiles(QByteArray &data, QString path);

private:
    QGroupBox *createImportExportGroupbox();
    QGroupBox *createBinFixGroupbox();
    QGroupBox *setBinFileModeGroupbox();

protected:
    void keyReleaseEvent(QKeyEvent *event) override;
    void closeEvent(QCloseEvent *event) override;

private slots:
    void handleBinFixClick();
    void handleImportClick();
    void handleExportClick();
    void handleGetTime();

    void handleReceiveBinFile(QString cmd);
    void handleReceiveBinMessage(int id);

private:
    QWidget *main_widget;
    QVBoxLayout *main_layout;
    QGroupBox *import_export_group;
    QGroupBox *bin_fix_group;

    QComboBox *bin_type_combox;
    QLineEdit *author_edit;
    QTextEdit *desp_edit;
    QLineEdit *time_edit;
    QString binPath;
};

#endif //BIN_UTIL_WINDOW_HPP
