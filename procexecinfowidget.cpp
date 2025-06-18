#include "procexecinfowidget.h"
#include "ui_procexecinfowidget.h"

ProcExecInfoWidget::ProcExecInfoWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ProcExecInfoWidget)
{
    ui->setupUi(this);
}

ProcExecInfoWidget::~ProcExecInfoWidget()
{
    delete ui;
}

void ProcExecInfoWidget::refresh() {
    if(exec_info->isInteractor) ui->interactchkBox->setCheckState(Qt::Checked);
    else  ui->interactchkBox->setCheckState(Qt::Unchecked);
    ui->procnameTXT->setText(exec_info->proc_name);
    ui->IntercomboBox->setCurrentText(exec_info->interpreter);
    ui->innone->setChecked(exec_info->stdin_id==-1);
    ui->infile->setChecked(exec_info->stdin_id==-2);
    ui->inpipe->setChecked(exec_info->stdin_id>=0);
    ui->outnone->setChecked(exec_info->stdout_id==-1);
    ui->outfile->setChecked(exec_info->stdout_id==-2);
    ui->outpipe->setChecked(exec_info->stdout_id>=0);
    ui->infnamepipeTXT->setText(exec_info->stdin_file);
    ui->outfnamepipeTXT->setText(exec_info->stdout_file);
}

void ProcExecInfoWidget::on_procnameTXT_editingFinished()
{
    exec_info->proc_name=ui->procnameTXT->text();
}


void ProcExecInfoWidget::on_interactchkBox_checkStateChanged(const Qt::CheckState &arg1)
{
    if(arg1==Qt::Checked)
        exec_info->isInteractor=1;
    else exec_info->isInteractor=0;
}


void ProcExecInfoWidget::on_paramsTXT_editingFinished()
{
    exec_info->params=parseCombinedArgString(ui->paramsTXT->text());
}


void ProcExecInfoWidget::on_IntercomboBox_currentTextChanged(const QString &arg1)
{
    exec_info->interpreter=arg1;
}

void ProcExecInfoWidget::on_refreshBTN_clicked()
{
    exec_info->proc_name=ui->procnameTXT->text();
    if(ui->interactchkBox->checkState()==Qt::Checked)
        exec_info->isInteractor=1;
    else exec_info->isInteractor=0;
    exec_info->params=parseCombinedArgString(ui->paramsTXT->text());
    exec_info->interpreter=ui->IntercomboBox->currentText();
    if(ui->innone->isChecked()) {
        exec_info->stdin_id=-1;
        exec_info->stdin_file="";
    } else if(ui->infile) {
        exec_info->stdin_id=-2;
        exec_info->stdin_file=ui->infnamepipeTXT->text();
    } else {
        bool ok;
        exec_info->stdin_id=ui->infnamepipeTXT->text().toInt(&ok);
        if(ok) {
            exec_info->stdin_file=ui->infnamepipeTXT->text();
        } else {
            exec_info->stdin_id=-1;
            exec_info->stdin_file="";
        }
    }
    if(ui->outnone->isChecked()) {
        exec_info->stdout_id=-1;
        exec_info->stdout_file="";
    } else if(ui->outfile) {
        exec_info->stdout_id=-2;
        exec_info->stdout_file=ui->outfnamepipeTXT->text();
    } else {
        bool ok;
        exec_info->stdout_id=ui->outfnamepipeTXT->text().toInt(&ok);
        if(ok) {
            exec_info->stdout_file=ui->outfnamepipeTXT->text();
        } else {
            exec_info->stdout_id=-1;
            exec_info->stdout_file="";
        }
    }
    refresh();
}
