#ifndef JUDGEPANEL_H
#define JUDGEPANEL_H

#include <QMainWindow>
#include <QFileDialog>
#include "judgingwidget.h"
#include "ctsettings.h"
#include "judgingwidget.h"

namespace Ui {
class JudgePanel;
}

class JudgePanel : public QMainWindow
{
    Q_OBJECT

public:
    explicit JudgePanel(QWidget *parent = nullptr);
    ~JudgePanel();

    void refresh();
    void closeEvent(QCloseEvent *event);
    QString ctPath="";
    Contest* contest=nullptr;
    QStringList probnames,contestantids;
    QMap<QString,QMap<QString,JudgeInfo> > judgeinfo;
    JudgingWidget judgewid;
public: signals:
    void ExitWin();

public slots:
    void updateCplResult(const QString& participant,
                         const QString& problem,
                         TResult verdict,
                         const QString& log);
    void updateJudgeResult(const QString& participant,
                           const QString& problem,
                           int caseid,
                           TResult verdict,
                           double score,
                           unsigned int time,
                           unsigned int mem,
                           const QString& log);

private slots:
    void on_ExportBTN_clicked();

    void on_JudgeAllBTN_clicked();

    void on_JudgeBTN_clicked();

private:
    Ui::JudgePanel *ui;
};

#endif // JUDGEPANEL_H
