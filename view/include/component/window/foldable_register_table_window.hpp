#ifndef FOLDABLE_REGISTER_TABLE_WINDOW_HPP
#define FOLDABLE_REGISTER_TABLE_WINDOW_HPP

#include <QMainWindow>
#include <QGridLayout>
#include <QPushButton>
#include <QLabel>
#include <QKeyEvent>

class FoldableRegisterTableWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit FoldableRegisterTableWindow(QWidget *parent = nullptr);
    ~FoldableRegisterTableWindow();
    void setWidget(QWidget *widget);
    void setDesp(QString desp);

protected:
    void keyReleaseEvent(QKeyEvent *event) override;

private:
    void initUI();
    void initMenu();
    void retranslateUi();
    void refresh();
    void updateToggleBtnLabel();

signals:
    void sigToggle(bool fold_state);
    void sigExportFile(QString file_path);
    void sigImportFile(QString file_path);

private slots:
    void handleToggleEvent();
    void handleImportFile();
    void handleExportFile();


private:
    QWidget *main_widget;
    QGridLayout *main_layout;
    bool table_fold_state;
    QPushButton *toggle_btn;
    QPushButton *import_btn;
    QPushButton *export_btn;
    QLabel *desp_label;
};

#endif // FOLDABLE_REGISTER_TABLE_WINDOW_HPP
