#include "submitide.h"
#include "ui_submitide.h"

SubmitIDE::SubmitIDE(QWidget *parent, Problem prob)
    : QWidget(parent)
    , judge(nullptr,false)
    , ui(new Ui::SubmitIDE)
{
    ui->setupUi(this);
    highlighter = new IDEHighlighter(ui->IDE->document());
    ui->IDEGroup->clear();

}

SubmitIDE::~SubmitIDE()
{
    delete ui;
    highlighter = nullptr;
}

void SubmitIDE::submit(){
    for(Problem::Utility util : problem.utils){
        if(util.category == Problem::Utility::FileCategory::submission){
            QString answer_name = util.filename;
            QString code;
            QWidget * wid = ui->IDEGroup->widget(0);
            IDEGroupWidget* textEdit = qobject_cast<IDEGroupWidget*>(wid);
            code = textEdit->getText();
            if(code == ""){
                QMessageBox::warning(this, "警告", "代码文本为空/代码上传失败");
            }
            submission->answers[answer_name] = code;
        }
    }
}

void SubmitIDE::on_testbtn_clicked(){

    submit();
    judge.show();
    judge.prepareJudge(problem, submission->id,maindir,submission->answers);
    for(int caseid:problem.testdata.subtasks[0].cases)
        judge.runJudge(problem,submission->id,submission->answers,caseid);
    judge.startJudge();
}

void SubmitIDE::on_submitbtn_clicked(){
    submit();
    QMessageBox::warning(this, "成功", "代码提交成功 ");
}

void SubmitIDE::on_returnbtn_clicked(){
    this->hide();
}

void SubmitIDE::on_initbtn_clicked(){
    ui->IDEGroup->clear();
    for(Problem::Utility util : problem.utils){
        if(util.category == Problem::Utility::FileCategory::submission &&
            (util.filetype == Problem::Utility::FileType::code ||
             util.filetype == Problem::Utility::FileType::snippet)){
            IDEGroupWidget * wid = new IDEGroupWidget();
            ui->IDEGroup->addTab(wid,util.filename);
        }
    }
}
