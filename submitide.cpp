#include "submitide.h"
#include "ui_submitide.h"

SubmitIDE::SubmitIDE(QWidget *parent)
    : QWidget(parent)
    , judge(nullptr,false)
    , ui(new Ui::SubmitIDE)
{
    ui->setupUi(this);
    connect(&judge,&JudgingWidget::allComplete,this,&SubmitIDE::set_tested);
}

SubmitIDE::~SubmitIDE()
{
    delete ui;
}

void SubmitIDE::refresh() {
    editors.clear();
    currentinfo=*judgeinfo;
    ui->IDEGroup->clear();
    for(Problem::Utility util : problem.utils) {
        if(util.category == Problem::Utility::FileCategory::submission &&
            util.filetype == Problem::Utility::FileType::code) {
            QWidget* codewid=new QWidget(this);
            codewid->setFixedSize(ui->IDEGroup->size()-QSize({5,30}));
            codewid->show();
            ui->IDEGroup->addTab(codewid,util.filename);
            QString suffix="";
            if(util.filename.contains("."))
                suffix=util.filename.split(".").back();
            QCodeEdit* codetext=new QCodeEdit(currentinfo.pans[util.filename],codewid,suffix);
            editors[util.filename]=codetext;
            codetext->setFixedSize(codewid->size());
            codetext->show();
        }
        else if(util.category == Problem::Utility::FileCategory::submission &&
                util.filetype == Problem::Utility::FileType::snippet) {
            QWidget* codewid=new QWidget(this);
            codewid->setFixedSize(ui->IDEGroup->size()-QSize({5,30}));
            codewid->show();
            ui->IDEGroup->addTab(codewid,util.filename);
            QString suffix="";
            for(Problem::Utility tplutil : problem.utils)
                if(tplutil.category == Problem::Utility::FileCategory::submission &&
                    tplutil.filetype == Problem::Utility::FileType::templ &&
                    tplutil.filename.contains(".")) {
                    QString templcontent=FileOp::read(ctPath+problem.name+".probdata/"+tplutil.filename+templ);
                    if(Codetpl::get_snippets(templcontent).contains(util.filename)) {
                        suffix=tplutil.filename.split(".").back();
                        break;
                    }
                }
            QCodeEdit* codetext=new QCodeEdit(currentinfo.pans[util.filename],codewid,suffix);
            editors[util.filename]=codetext;
            codetext->setFixedSize(codewid->size());
            codetext->show();
        }
        else if(util.category == Problem::Utility::FileCategory::submission &&
                   util.filetype == Problem::Utility::FileType::plain)
        for(int caseid=1;caseid<=problem.testdata.ncase;caseid++) {
            QWidget* codewid=new QWidget(this);
            codewid->setFixedSize(ui->IDEGroup->size()-QSize({5,30}));
            codewid->show();
            ui->IDEGroup->addTab(codewid,get_filename_with_id(util.filename,caseid));
            QCodeEdit* codetext=new QCodeEdit(currentinfo.pans[get_filename_with_id(util.filename,caseid)],codewid);
            editors[get_filename_with_id(util.filename,caseid)]=codetext;
            codetext->setFixedSize(codewid->size());
            codetext->show();
        }
    }

}

void SubmitIDE::set_tested() {
    testmutex.lock();
    tested=true;
    testmutex.unlock();
}

void SubmitIDE::on_testbtn_clicked() {
    QDateTime start_time = contest->start_time;
    QDateTime end_time = contest->end_time;
    if(contest->start_enabled && start_time > QDateTime::currentDateTime()) {
        QMessageBox::warning(NULL, "warning", tr("Contest has not started yet!"), QMessageBox::Yes, QMessageBox::Yes);
        this->close();
        return;
    }
    if(contest->end_enabled && QDateTime::currentDateTime() > end_time){
        QMessageBox::warning(NULL, "warning", tr("Contest has already ended!"), QMessageBox::Yes, QMessageBox::Yes);
        this->close();
        return;
    }
    testmutex.lock();
    tested=false;
    testmutex.unlock();
    for(QString filename:editors.keys()) {
        currentinfo.pans[filename]=editors[filename]->text();
    }
    judge.show();
    judge.prepareJudge(problem, currentinfo.id,ctPath,currentinfo.pans);
    for(int caseid:problem.testdata.subtasks[0].cases)
        judge.runJudge(problem,currentinfo.id,currentinfo.pans,caseid);
    judge.startJudge();
}

void SubmitIDE::on_submitbtn_clicked() {
    QDateTime start_time = contest->start_time;
    QDateTime end_time = contest->end_time;
    if(contest->start_enabled && start_time > QDateTime::currentDateTime()) {
        QMessageBox::warning(NULL, "warning", tr("Contest has not started yet!"), QMessageBox::Yes, QMessageBox::Yes);
        this->close();
        return;
    }
    if(contest->end_enabled && QDateTime::currentDateTime() > end_time){
        QMessageBox::warning(NULL, "warning", tr("Contest has already ended!"), QMessageBox::Yes, QMessageBox::Yes);
        this->close();
        return;
    }
    testmutex.lock();
    bool testedstatus=tested;
    testmutex.unlock();
    if(!testedstatus) {
        QMessageBox::warning(NULL, "warning", tr("Cannot submit problem before testing!"), QMessageBox::Yes, QMessageBox::Yes);
        return;
    }
    *judgeinfo=currentinfo;
    this->close();
}

void SubmitIDE::closeEvent(QCloseEvent* event) {
    judge.close();
}
