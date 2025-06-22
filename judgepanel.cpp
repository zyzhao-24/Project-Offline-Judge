#include "judgepanel.h"
#include "ui_judgepanel.h"

JudgePanel::JudgePanel(QWidget *parent, Contest* contestptr)
    : QMainWindow(parent)
    , ui(new Ui::JudgePanel)
    , contest(contestptr)
{
    ui->setupUi(this);
    refresh();
}

JudgePanel::~JudgePanel()
{
    delete ui;
}

QColor ScoreCol(double tot,double mx) {
    if(mx<=0) return DescriptionCol(_NA);
    if(tot<=0) return DescriptionCol(_wa);
    if(tot>=mx) return DescriptionCol(_ok);
    return DescriptionCol(_partially);
}

void JudgePanel::refresh() {
    ui->ScoreWID->clear();
    probnames=contest->problems.keys();
    contestantids=contest->participants.keys();
    ui->ScoreWID->setColumnCount(probnames.size()+2);
    ui->ScoreWID->setHorizontalHeaderLabels((QStringList()<<tr("ID"))+probnames<<tr("Total"));
    ui->ScoreWID->setRowCount(contestantids.size());

    for(int index=0;index<contestantids.size();index++) {
        QString ctid=contestantids[index];
        ui->ScoreWID->setItem(index,0,new QTableWidgetItem(ctid));
        if(judgeinfo.contains(contestantids[index])) {
            double cttotal=0.0,ctmax=0.0;
            QMap<QString,JudgeInfo> probinfo=judgeinfo[ctid];
            for(int ind2=1;ind2<=probnames.size();ind2++) {
                QString probname=probnames[ind2-1];
                double probtotal=0.0,probmax=0.0;
                for(Testdata::Subtask subtask:contest->problems[probname].testdata.subtasks) {
                    probmax+=subtask.score;
                }
                ctmax+=probmax;
                if(probinfo.contains(probname)) {
                    JudgeInfo caseinfo=probinfo[probname];
                    for(Testdata::Subtask subtask:contest->problems[probname].testdata.subtasks) {
                        double subtasktotal=1.0;
                        for(int caseid:subtask.cases) {
                            if(caseinfo.results.contains(caseid)) {
                                subtasktotal=qMin(subtasktotal,caseinfo.results[caseid].score);
                            } else {
                                subtasktotal=0.0;
                                break;
                            }
                        }
                        probtotal+=subtasktotal*subtask.score;
                    }
                    cttotal+=probtotal;
                    ui->ScoreWID->setItem(index,ind2,new QTableWidgetItem(QString::number(probtotal,'g',2)+"/"+QString::number(probmax,'g',2)));
                    ui->ScoreWID->item(index,ind2)->setForeground(QBrush(ScoreCol(probtotal,probmax)));
                } else {
                    ui->ScoreWID->setItem(index,ind2,new QTableWidgetItem("0.00/"+QString::number(probmax,'g',2)));
                    ui->ScoreWID->item(index,ind2)->setForeground(QBrush(DescriptionCol(_ce)));
                }
            }
            ui->ScoreWID->setItem(index,probnames.size()+1,new QTableWidgetItem(QString::number(cttotal,'g',2)+"/"+QString::number(ctmax,'g',2)));
            ui->ScoreWID->item(index,probnames.size()+1)->setForeground(QBrush(ScoreCol(cttotal,ctmax)));
        } else {
            for(int ind2=1;ind2<=probnames.size();ind2++) {
                ui->ScoreWID->setItem(index,ind2,new QTableWidgetItem("N/A"));
                ui->ScoreWID->item(index,ind2)->setForeground(QBrush(DescriptionCol(_NA)));
            }
            ui->ScoreWID->setItem(index,probnames.size()+1,new QTableWidgetItem("N/A"));
            ui->ScoreWID->item(index,probnames.size()+1)->setForeground(QBrush(DescriptionCol(_NA)));
        }
    }
}

void JudgePanel::closeEvent(QCloseEvent *event) {
    emit ExitWin();
}
