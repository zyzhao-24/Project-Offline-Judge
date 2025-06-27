#ifndef SUBMITIDE_H
#define SUBMITIDE_H

#include <QWidget>
#include <QMessageBox>
#include <QMutex>
#include "ctsettings.h"
#include "qcodeedit.h"
#include "judgingwidget.h"

namespace Ui {
class SubmitIDE;
}

class SubmitIDE : public QWidget
{
    Q_OBJECT


public:
    explicit SubmitIDE(QWidget *parent = nullptr);
    ~SubmitIDE();

    JudgeInfo * judgeinfo;
    JudgeInfo currentinfo;
    Contest* contest;
    Problem problem;
    JudgingWidget judge;
    QString ctPath;
    QMap<QString, QCodeEdit*> editors;

    QMutex testmutex;
    bool tested = false;
    void refresh();
    void set_tested();
    void runtest();
protected:
    void closeEvent(QCloseEvent *event);
private slots:
    void on_testbtn_clicked();

    void on_submitbtn_clicked();

private:
    Ui::SubmitIDE *ui;
};

#endif // SUBMITIDE_H
