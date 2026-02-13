#include <QLineEdit>

class CvLineEdit: public QLineEdit
{
    Q_OBJECT
public:
    CvLineEdit(QWidget* parent = nullptr);

protected:
    void focusInEvent(QFocusEvent *event) override;
    void focusOutEvent(QFocusEvent *event) override;

signals:
    void sigGetFocus();
    void sigLostFocus(QString text);
};
