#ifndef SUBMITIDE_H
#define SUBMITIDE_H

#include <QWidget>
#include <QMessageBox>
#include <QPlainTextEdit>
#include <QMutex>
#include "ctsettings.h"
#include "texthighlighter.h"
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
    Problem problem;
    JudgingWidget judge;
    QString ctPath;
    QList<TextHighlighter*> highlighters;
    QMap<QString, QPlainTextEdit*> editors;

    QMutex testmutex;
    bool tested = false;
    void refresh();
    void set_tested();
    void runtest();

private slots:
    void on_testbtn_clicked();

    void on_submitbtn_clicked();

private:
    Ui::SubmitIDE *ui;
};

#endif // SUBMITIDE_H
