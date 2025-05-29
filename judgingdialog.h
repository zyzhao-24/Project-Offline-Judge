#ifndef JUDGINGDIALOG_H
#define JUDGINGDIALOG_H

#include <QDialog>
#include "judgingwidget.h"
namespace Ui {
class JudgingDialog;
}

class JudgingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit JudgingDialog(QWidget *parent = nullptr);
    ~JudgingDialog();
    JudgingWidget widget;
protected:
    void closeEvent(QCloseEvent *event);
public: signals:
    void ExitWin();
private slots:
    void on_StopBTN_clicked();

    void on_ContinueBTN_clicked();

private:
    Ui::JudgingDialog *ui;
};

#endif // JUDGINGDIALOG_H
