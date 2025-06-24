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
}

StudentEditor::~StudentEditor()
{
    delete ui;
    if (pdfview) {
        delete pdfview;
    }
    if (pdfdoc) {
        delete pdfdoc;
    }
}

void StudentEditor::on_submitbtn_clicked(){
    if(problem.name == ""){
        QMessageBox::warning(this, "警告", "请选择作答题目。");
        return;
    }
    else{
        SubmitIDE* IDE = new SubmitIDE();
        IDE->father = this;
        IDE->problem = problem;
        IDE->judgeinfo = &judgeinfo;
        IDE->maindir = maindir;
        IDE->show();
    }
}


void StudentEditor::set_time(){
    if(contest.start_enabled ){
        ui->StartTime->setDateTime(contest.start_time);
    }
    if(contest.end_enabled){
        ui->EndTime->setDateTime(contest.end_time);
    }
}

void StudentEditor::load_prob(){
    for(auto p = contest.problems.begin();p!=contest.problems.end();p++){
        ui->Problist->addItem(p->name);
    }
}

void StudentEditor::set_name(){
    ui->contestname->setText(contest.name);
}
void StudentEditor::view(){
        if(pdfview) {
            delete pdfview;
        }
        if(pdfdoc)
        {
            delete pdfdoc;
        }
        pdfview=new QPdfView(ui->PDF);
        pdfview->setPageMode(QPdfView::PageMode::MultiPage);
        pdfview->setZoomMode(QPdfView::ZoomMode::FitToWidth);
        ui->PDF->setWidget(pdfview);
        pdfdoc = new QPdfDocument(ui->PDF);
        pdfview->setDocument(pdfdoc);
        pdfview->hide();
        if(ui->Problist->selectedItems().size()!=1) return;
        QString prob_name=ui->Problist->selectedItems().constFirst()->text();
        if(contest.problems.contains(prob_name)){
            problem = contest.problems[prob_name];
            // problem.toStudentPack();
            QBuffer buffer(&problem.contentpdf);
            buffer.open(QIODevice::ReadOnly);
            pdfdoc->load(&buffer);
            pdfview->setDocument(pdfdoc);
            pdfview->show();
        }
        QMessageBox::warning(this, "成功", "PDF文档准备完成。");
        pdfview->setDocument(pdfdoc);
        pdfview->show();
}
void StudentEditor::on_viewbtn_clicked(){
    view();
}

void StudentEditor::on_returnbtn_clicked(){
    father->show();
    this->destroy();
}

void StudentEditor::on_savebtn_clicked(){
    judgeinfo.id = id;
    judgeinfo.pwd = pwd;
    submission.JsonSubmission();
    QFile Submit(maindir + contest.name +" "+ submission.usrname + ".ans");
    if(Submit.open(QIODevice::WriteOnly | QIODevice::Truncate)){
        QJsonDocument studentfile;
        studentfile.setObject(submission.SubmissionObj);
        QTextStream stream2(&Submit);
        stream2 <<(studentfile.toJson());
        Submit.close();
    }
    QMessageBox::information(nullptr, "信息提示", "保存答案成功");
}

