#ifndef CONFIG_SET_WIDGET_HPP
#define CONFIG_SET_WIDGET_HPP

#include <QWidget>
#include <QGridLayout>

class ConfigSetWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ConfigSetWidget(QWidget *widget = nullptr);
    void initUI();

signals:
    void sigSaveConfigSet(int idx);
    void sigLoadConfigSet(int idx);

public slots:
    void toggleVisible();

private:
    QGridLayout *main_layout;
};

#endif // CONFIG_SET_WIDGET_HPP
