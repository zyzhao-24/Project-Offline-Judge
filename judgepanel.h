#ifndef JUDGEPANEL_H
#define JUDGEPANEL_H

#include <QMainWindow>
#include "judgingwidget.h"
#include "ctsettings.h"
namespace Ui {
class JudgePanel;
}

class JudgePanel : public QMainWindow
{
    Q_OBJECT

public:
    explicit JudgePanel(QWidget *parent = nullptr, Contest* contestptr = nullptr);
    ~JudgePanel();

    void refresh();
    void closeEvent(QCloseEvent *event);

    QStringList probnames,contestantids;
    QMap<QString,QMap<QString,JudgeInfo> > judgeinfo;
public: signals:
    void ExitWin();
private slots:

private:
    Ui::JudgePanel *ui;
    Contest* contest;
};

#endif // JUDGEPANEL_H
