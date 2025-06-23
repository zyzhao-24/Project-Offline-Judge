#include "judgesetting.h"
#include "ui_judgesetting.h"
#include <QSpinBox>
#include <QPushButton>
#include <QVBoxLayout>

JudgeSetting::JudgeSetting(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::JudgeSetting)
{
    ui->setupUi(this);
}

JudgeSetting::~JudgeSetting()
{
    delete ui;
}

void JudgeSetting::refresh() {
    ui->folderfnComboBox->clear();
    ui->chkComboBox->clear();
    for(Problem::Utility util:problem->utils) {
        if(util.category==Problem::Utility::FileCategory::builtin||util.category==Problem::Utility::FileCategory::resource)
            ui->folderfnComboBox->addItem(util.filename);
        if(util.category==Problem::Utility::FileCategory::testdata&&util.filetype==Problem::Utility::FileType::plain)
            ui->folderfnComboBox->addItem(util.filename);
        if(util.category==Problem::Utility::FileCategory::submission) {
            if(util.filetype==Problem::Utility::FileType::templ)
                ui->folderfnComboBox->addItem(util.filename.left(util.filename.lastIndexOf(".tpl")));
            if(util.filetype==Problem::Utility::FileType::code||util.filetype==Problem::Utility::FileType::plain)
                ui->folderfnComboBox->addItem(util.filename);
        }
    }
    for(Problem::CompileSetting cplset:problem->cpl_settings) {
        bool valid=true;
        for(QString inputf:cplset.inputs) {
            if(problem->utils[inputf].category==Problem::Utility::FileCategory::testdata) valid=false;
        }
        if(valid) {
            ui->folderfnComboBox->addItem(cplset.output);
            ui->chkComboBox->addItem(cplset.output);
        }
    }
    ui->folderLWid->clear();
    for(QString file:problem->judge_proc.folder.keys()) {
        ui->folderLWid->addItem(problem->judge_proc.folder[file]+"/"+file);
    }
    ui->clLWid->clear();
    for(QString file:problem->judge_proc.file_collect.keys()) {
        ui->clLWid->addItem(problem->judge_proc.file_collect[file]+"/"+file);
    }
    ui->pipecntSpBox->setValue(problem->judge_proc.pipecount);
    ui->checkerLWid->clear();
    for(JudgeProcess::ChkCmd chkcmd:problem->judge_proc.checkersCmds) {
        ui->checkerLWid->addItem(chkcmd.checker+" "+chkcmd.inputFile+" "+chkcmd.outputFile+" "+chkcmd.answerFile);
    }
    ui->tabWidget->clear();
    for(int i=0;i<problem->judge_proc.execute_info.size();i++) {
        JudgeProcess::process& exe_proc=problem->judge_proc.execute_info[i];
        ProcExecInfoWidget* execinfowid=new ProcExecInfoWidget(this);
        execinfowid->exec_info=&exe_proc;
        execinfowid->problem=problem;
        execinfowid->refresh();
        ui->tabWidget->addTab(execinfowid,tr("Process ")+QString::number(i+1));
    }
}

void JudgeSetting::on_folderaddBTN_clicked()
{
    QString filename,foldername;
    filename=ui->folderfnComboBox->currentText();
    foldername=ui->folderTXT->text();
    if(!filename.isEmpty()&&!foldername.isEmpty())
    {
        problem->judge_proc.folder.insert(filename,foldername);
        ui->folderLWid->addItem(foldername+'/'+filename);
    }
}


void JudgeSetting::on_folderremBTN_clicked()
{
    if(ui->folderLWid->selectedItems().isEmpty()) return;
    QStringList str=ui->folderLWid->selectedItems()[0]->text().split("/");
    if(str.size()<2) return;
    QString filename=str[1],foldername=str[0];
    if(problem->judge_proc.folder.contains(filename)) {
        auto i=problem->judge_proc.folder.find(filename);
        problem->judge_proc.folder.erase(i);
    }
    refresh();
}

void JudgeSetting::on_folderLWid_itemClicked(QListWidgetItem *item)
{
    QStringList str=item->text().split('/');
    if(str.size()<2) return;
    ui->folderfnComboBox->setCurrentText(str[1]);
    ui->folderTXT->setText(str[0]);
}

void JudgeSetting::on_claddBTN_clicked()
{
    QString filename,foldername;
    filename=ui->clfilenmeTXT->text();
    foldername=ui->clfolderTXT->text();
    if(!filename.isEmpty()&&!foldername.isEmpty())
    {
        problem->judge_proc.file_collect.insert(filename,foldername);
    }
    refresh();
}


void JudgeSetting::on_clremBTN_clicked()
{
    if(ui->clLWid->selectedItems().isEmpty()) return;
    QStringList str=ui->clLWid->selectedItems()[0]->text().split("/");
    if(str.size()<2) return;
    QString filename=str[1],foldername=str[0];
    if(problem->judge_proc.file_collect.contains(filename)) {
        auto i=problem->judge_proc.file_collect.find(filename);
        problem->judge_proc.file_collect.erase(i);
    }
    refresh();
}


void JudgeSetting::on_clLWid_itemClicked(QListWidgetItem *item)
{
    QStringList str=item->text().split('/');
    if(str.size()<2) return;
    ui->clfilenmeTXT->setText(str[1]);
    ui->clfolderTXT->setText(str[0]);
}

void JudgeSetting::on_pipecntSpBox_valueChanged(int arg1)
{
    problem->judge_proc.pipecount=arg1;
}


void JudgeSetting::on_exeaddBTN_clicked()
{
    problem->judge_proc.execute_info.append(JudgeProcess::process());
    ProcExecInfoWidget* execinfowid=new ProcExecInfoWidget(this);
    execinfowid->exec_info=&problem->judge_proc.execute_info.back();
    ui->tabWidget->addTab(execinfowid,tr("Process ")+QString::number(ui->tabWidget->count()+1));

}


void JudgeSetting::on_exeremBTN_clicked()
{
    problem->judge_proc.execute_info.removeAt(ui->tabWidget->currentIndex());
    ui->tabWidget->removeTab(ui->tabWidget->currentIndex());
}


void JudgeSetting::on_chkaddBTN_clicked()
{
    QString checkername=ui->chkComboBox->currentText();
    QString inputFilename=ui->chkinputTXT->text();
    QString outputFilename=ui->chkoutputTXT->text();
    QString answerFilename=ui->chkansTXT->text();
    JudgeProcess::ChkCmd *Chk=new JudgeProcess::ChkCmd();
    Chk->checker=checkername;
    Chk->answerFile=answerFilename;
    Chk->inputFile=inputFilename;
    Chk->outputFile=outputFilename;
    problem->judge_proc.checkersCmds.append(*Chk);
    ui->checkerLWid->addItem(checkername+" "+inputFilename+" "+outputFilename+" "+answerFilename);
}


void JudgeSetting::on_chkremBTN_clicked()
{
    if(ui->checkerLWid->selectedItems().isEmpty()) return;
    QStringList str=ui->checkerLWid->selectedItems()[0]->text().split(" ");
    if(str.size()<4) return;
    QString checkername=str[0];
    QString inputFilename=str[1];
    QString outputFilename=str[2];
    QString answerFilename=str[3];
    for(auto i=problem->judge_proc.checkersCmds.begin();i!=problem->judge_proc.checkersCmds.end();i++)
        if(i->checker==checkername&&i->inputFile==inputFilename&&i->outputFile==outputFilename&&i->answerFile==answerFilename)
        {
            problem->judge_proc.checkersCmds.erase(i);
            break;
        }
    refresh();
}


void JudgeSetting::on_checkerLWid_itemClicked(QListWidgetItem *item)
{
    QStringList qlist;
    qlist=item->text().split(" ");
    if(qlist.size()==4)
    {
        QString checkername=qlist[0];
        QString inputFilename=qlist[1];
        QString outputFilename=qlist[2];
        QString answerFilename=qlist[3];
        ui->chkComboBox->setCurrentText(checkername);
        ui->chkinputTXT->setText(inputFilename);
        ui->chkoutputTXT->setText(outputFilename);
        ui->chkansTXT->setText(answerFilename);
    }
}

void JudgeSetting::closeEvent(QCloseEvent* event) {
    emit ExitWin();
}
