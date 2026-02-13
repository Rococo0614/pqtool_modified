#ifndef REGISTER_TOOL_WINDOW_HPP
#define REGISTER_TOOL_WINDOW_HPP

#include <QMainWindow>
#include <QComboBox>
#include <QPlainTextEdit>
#include <QLineEdit>
#include <QPushButton>
#include <QMap>
#include <QList>
#include <QCheckBox>

struct RegisterInfo {
    QString name;
    int bitStart;
    int bitEnd;
    int defaultValue;
    QString baseAddr;
    QString comment;
    QString fullAddr;
    QString offset;
};

class RegToolWindow : public QMainWindow {
    Q_OBJECT

public:
    RegToolWindow(QWidget *parent = nullptr);
    ~RegToolWindow() override = default;

private slots:
    void importJson();
    void onModuleChanged(const QString &moduleName);
    void onRegChanged(const QString &regName);
    void onGetClicked();
    void onSetClicked();
    void onModuleSearchChanged(const QString &searchText);
    void onRegSearchChanged(const QString &searchText);
    void onModuleCompletionSelected(const QString &text);
    void onRegCompletionSelected(const QString &text);
    void handleGetStatus(int id);
    void handleSetStatus(int id);
    void handleTimeOut();
    void onValueTextChanged(const QString &text);
    void onGetRadixChanged();
    void onSetRadixChanged();

private:
    void setupUI();
    void initEvent();
    void setupCompleters();
    void updateModuleCompleter(const QStringList &items);
    void updateRegCompleter(const QStringList &items);
    void disableComponents();
    void enableComponents();
    void updateGetValueDisplay();

    QComboBox *moduleCombo;
    QComboBox *regCombo;
    QPlainTextEdit *regInfoText;
    QLineEdit *getvalueEdit;
    QLineEdit *setvalueEdit;
    QPushButton *getBtn;
    QPushButton *setBtn;
    QPushButton *importBtn;
    QTimer *timer;

    QLineEdit *moduleSearchEdit;
    QLineEdit *regSearchEdit;
    QCompleter *moduleCompleter;
    QCompleter *regCompleter;
    QPushButton *moduleSearchBtn;
    QPushButton *regSearchBtn;

    QPushButton *getRadixBtn;
    QPushButton *setRadixBtn;
    bool getIsHex;
    bool setIsHex;

    QMap<QString, QString> modules;
    QMap<QString, QList<RegisterInfo>> regMap;
    QList<QString> allModuleNames;
    QList<QString> currentRegNames;

    bool get_ok_flag {0};
    uint32_t reg_32_bit_value {0};
    uint32_t current_get_value {0};
};

#endif // REGISTER_TOOL_WINDOW_HPP
