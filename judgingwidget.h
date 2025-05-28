#ifndef JUDGINGWIDGET_H
#define JUDGINGWIDGET_H

#include <QWidget>
namespace Ui {
class JudgingWidget;
}

class JudgingWidget : public QWidget
{
    Q_OBJECT

public:
    explicit JudgingWidget(QWidget *parent = nullptr);
    ~JudgingWidget();
    bool available=true;

protected:
    void closeEvent(QCloseEvent *event);
public: signals:
    void ExitWin();

private:
    Ui::JudgingWidget *ui;
};

#endif // JUDGINGWIDGET_H
