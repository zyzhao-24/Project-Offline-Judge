#ifndef JUDGINGWIDGET_H
#define JUDGINGWIDGET_H

#include <QWidget>
#include "judge_utils.h"
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
    TResult runJudge(JudgeProcess process,
                     const QString& ctdir,
                     const QString& padir,
                     const QString& problem,
                     const QString& participant,
                     int caseid);
protected:
    void closeEvent(QCloseEvent *event);
public: signals:
    void ExitWin();

private:
    Ui::JudgingWidget *ui;
};

#endif // JUDGINGWIDGET_H
