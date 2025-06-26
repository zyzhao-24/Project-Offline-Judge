#include "studenteditor.h"
#include "ui_studenteditor.h"
#include "submitide.h"

#include <QMessageBox>
#include <QFileDialog>

StudentEditor::StudentEditor(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::StudentEditor)
{
    ui->setupUi(this);
    pdfview=new QPdfView(ui->PDF);
    pdfview->setPageMode(QPdfView::PageMode::MultiPage);
    pdfview->setZoomMode(QPdfView::ZoomMode::FitToWidth);
    ui->PDF->setWidget(pdfview);
    pdfdoc = new QPdfDocument(ui->PDF);
    pdfview->setDocument(pdfdoc);
}

StudentEditor::~StudentEditor()
{
    delete pdfview;
    delete pdfdoc;
    delete ui;
}

void StudentEditor::refresh() {
    ui->ID->setText(id);
    ui->contestname->setText(contest->name);
    if(contest->start_enabled){
        ui->StartTime->setEnabled(true);
        ui->StartTime->setDateTime(contest->start_time);
    } else ui->StartTime->setEnabled(false);
    if(contest->end_enabled){
        ui->EndTime->setEnabled(true);
        ui->EndTime->setDateTime(contest->end_time);
    } else ui->EndTime->setEnabled(false);
    ui->Problist->clear();
    for(auto p = contest->problems.begin();p!=contest->problems.end();p++) {
        ui->Problist->addItem(p->name);
    }
    if(!contest->problems.contains(curProb)) {
        ui->probnametext->clear();
        ui->timelimtext->clear();
        ui->memlimtext->clear();
        ui->probtypetext->clear();
        pdfview->hide();
    }
    else {
        ui->probnametext->setText(contest->problems[curProb].name);
        ui->timelimtext->setText(QString::number(contest->problems[curProb].time_limit_ms));
        ui->memlimtext->setText(QString::number(contest->problems[curProb].mem_limit_MiB));
        ui->probtypetext->setText(ProbTypeName[int(contest->problems[curProb].type)]);
        QBuffer buffer(&contest->problems[curProb].contentpdf);
        buffer.open(QIODevice::ReadOnly);
        pdfdoc->load(&buffer);
        pdfview->setDocument(pdfdoc);
        pdfview->show();
        update();
    }
}

void StudentEditor::on_submitbtn_clicked(){
    QDateTime start_time = contest->start_time;
    QDateTime end_time = contest->end_time;
    if(contest->start_enabled && start_time > QDateTime::currentDateTime()) {
        QMessageBox::warning(NULL, "warning", tr("Contest has not started yet!"), QMessageBox::Yes, QMessageBox::Yes);
        return;
    }
    if(contest->end_enabled && QDateTime::currentDateTime() > end_time){
        QMessageBox::warning(NULL, "warning", tr("Contest has already ended!"), QMessageBox::Yes, QMessageBox::Yes);
        return;
    }

    if(!contest->problems.contains(curProb)) {
        QMessageBox::warning(NULL, "warning", tr("Problem not selected!"), QMessageBox::Yes, QMessageBox::Yes);
        return;
    }
    else {
        judgeinfo[curProb].id=id;
        judgeinfo[curProb].pwd=pwd;
        judgeinfo[curProb].problem=curProb;
        IDE.problem = contest->problems[curProb];
        IDE.contest = contest;
        IDE.judgeinfo = &judgeinfo[curProb];
        IDE.ctPath = ctPath;
        IDE.refresh();
        IDE.show();
    }
}

void StudentEditor::on_returnbtn_clicked(){
    father->show();
    IDE.close();
    this->destroy();
}

void StudentEditor::on_savebtn_clicked(){
    QString sspPath=QFileDialog::getSaveFileName(this, tr("Save submission pack"), ctPath+id+sspack, tr("student submission pack (*.sspack)"));
    if(sspPath.isEmpty())
        return;

    QFile savef(sspPath);
    if (!savef.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        QMessageBox::warning(NULL, "warning", tr("Failed while saving submission pack!"), QMessageBox::Yes, QMessageBox::Yes);
        return;
    }

    QJsonDocument doc;
    doc.setObject(JudgeInfo::packInfoList(judgeinfo.values()));
    QTextStream stream(&savef);
    stream << Encryption::encrypt_data(doc.toJson());
    savef.close();
}


void StudentEditor::on_Problist_itemClicked(QListWidgetItem *item)
{
    curProb=item->text();
    refresh();
}

void StudentEditor::closeEvent(QCloseEvent* event) {
    IDE.close();
}
