#include "judgingdialog.h"
#include "ui_judgingdialog.h"

JudgingDialog::JudgingDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::JudgingDialog)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_QuitOnClose, false);
    widget.show();
    connect(&widget,SIGNAL(ExitWin()),this,SLOT(close()));
}

JudgingDialog::~JudgingDialog()
{
    delete ui;
}

void JudgingDialog::on_StopBTN_clicked()
{
    widget.available=false;
    ui->ContinueBTN->setEnabled(true);
    ui->StopBTN->setEnabled(false);
}


void JudgingDialog::on_ContinueBTN_clicked()
{
    widget.available=true;
    ui->ContinueBTN->setEnabled(false);
    ui->StopBTN->setEnabled(true);
}

