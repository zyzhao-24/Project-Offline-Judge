#include "judgepanel.h"
#include "ui_judgepanel.h"

JudgePanel::JudgePanel(QWidget *parent)
    : QMainWindow(parent)
    , judgewid(nullptr)
    , ui(new Ui::JudgePanel)
{
    ui->setupUi(this);
    connect(&judgewid,&JudgingWidget::cplComplete,this,&JudgePanel::updateCplResult);
    connect(&judgewid,&JudgingWidget::judgeComplete,this,&JudgePanel::updateJudgeResult);
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
                if(probinfo.contains(probname)&&probinfo[probname].cplres.Verdict==_ok) {
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
                    ui->ScoreWID->setItem(index,ind2,new QTableWidgetItem(QString::number(probtotal,'f',2)+"/"+QString::number(probmax,'f',2)));
                    ui->ScoreWID->item(index,ind2)->setForeground(QBrush(ScoreCol(probtotal,probmax)));
                } else {
                    ui->ScoreWID->setItem(index,ind2,new QTableWidgetItem("0.00/"+QString::number(probmax,'f',2)));
                    ui->ScoreWID->item(index,ind2)->setForeground(QBrush(DescriptionCol(_ce)));
                }
            }
            ui->ScoreWID->setItem(index,probnames.size()+1,new QTableWidgetItem(QString::number(cttotal,'f',2)+"/"+QString::number(ctmax,'f',2)));
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

void JudgePanel::on_ExportBTN_clicked()
{
    probnames=contest->problems.keys();
    contestantids=contest->participants.keys();
    QString csvPath=QFileDialog::getSaveFileName(this, tr("Export to csv"), "./", tr("comma seperated values (*.csv)"));
    if(csvPath.isEmpty()) return;
    QFile csvf(csvPath);
    if (!csvf.open(QIODevice::WriteOnly|QIODevice::Truncate)) {
        QMessageBox::warning(NULL, "warning", tr("Failed while writing csv file!"), QMessageBox::Yes, QMessageBox::Yes);
        return;
    }

    QTextStream csvstream(&csvf);
    csvstream<<tr("ID")<<","<<probnames.join(',')<<","<<tr("Total")<<Qt::endl;
    for(int index=0;index<contestantids.size();index++) {
        QString ctid=contestantids[index];
        csvstream<<ctid<<",";
        if(judgeinfo.contains(contestantids[index])) {
            double cttotal=0.0;
            QMap<QString,JudgeInfo> probinfo=judgeinfo[ctid];
            for(int ind2=1;ind2<=probnames.size();ind2++) {
                QString probname=probnames[ind2-1];
                double probtotal=0.0;
                if(probinfo.contains(probname)&&probinfo[probname].cplres.Verdict==_ok) {
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
                }
                csvstream<<probtotal<<",";
            }
            csvstream<<cttotal<<Qt::endl;
        } else {
            for(int ind2=1;ind2<=probnames.size();ind2++) csvstream<<0<<",";
            csvstream<<0<<Qt::endl;
        }
    }
}

void JudgePanel::updateCplResult(const QString& participant,
                     const QString& problem,
                     TResult verdict,
                     const QString& log) {
    judgeinfo[participant][problem].id=participant;
    judgeinfo[participant][problem].problem=problem;
    judgeinfo[participant][problem].cplres={verdict,log};
    judgeinfo[participant][problem].results.clear();
    refresh();
}
void JudgePanel::updateJudgeResult(const QString& participant,
                       const QString& problem,
                       int caseid,
                       TResult verdict,
                       double score,
                       unsigned int time,
                       unsigned int mem,
                       const QString& log) {
    judgeinfo[participant][problem].id=participant;
    judgeinfo[participant][problem].problem=problem;
    judgeinfo[participant][problem].results[caseid]={caseid,score,verdict,time,mem,log};
    refresh();
}

void JudgePanel::on_JudgeAllBTN_clicked()
{
    probnames=contest->problems.keys();
    contestantids=contest->participants.keys();
    for(QString probname:probnames)
        for(QString ctid:contestantids) {
            judgewid.prepareJudge(contest->problems[probname],ctid,ctPath,judgeinfo[ctid][probname].pans);
            for(int index=1;index<=contest->problems[probname].testdata.ncase;index++)
                judgewid.runJudge(contest->problems[probname],ctid,judgeinfo[ctid][probname].pans,index);
        }
    judgewid.show();
    judgewid.startJudge();
}


void JudgePanel::on_JudgeBTN_clicked()
{
    probnames=contest->problems.keys();
    contestantids=contest->participants.keys();
    QList<QTableWidgetItem*> selitems=ui->ScoreWID->selectedItems();
    for(auto* item:selitems) {
        if(item->row()<0||item->row()>=contestantids.size()) continue;
        if(item->column()<1||item->column()>probnames.size()) continue;
        QString ctid=contestantids[item->row()];
        QString probname=probnames[item->column()-1];
        judgewid.prepareJudge(contest->problems[probname],ctid,ctPath,judgeinfo[ctid][probname].pans);
        for(int index=1;index<=contest->problems[probname].testdata.ncase;index++)
            judgewid.runJudge(contest->problems[probname],ctid,judgeinfo[ctid][probname].pans,index);
    }
    judgewid.show();
    judgewid.startJudge();
}

