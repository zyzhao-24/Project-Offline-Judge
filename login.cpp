#include "login.h"
#include "ui_login.h"

login::login(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::login)
{
    ui->setupUi(this);
    ui->passwordline->setEchoMode(QLineEdit::Password);
}

login::~login()
{
    delete ui;
}

void login::refresh() {
    if(contest.start_enabled) {
        ui->starttimeline->setEnabled(true);
        ui->starttimeline->setDateTime(contest.start_time);
    } else {
        ui->starttimeline->setEnabled(false);
    }
    if(contest.end_enabled) {
        ui->endtimeline->setEnabled(true);
        ui->endtimeline->setDateTime(contest.end_time);
    } else {
        ui->endtimeline->setEnabled(false);
    }
}
void login::on_loginbtn_clicked(){
    refresh();
    if(!verified) {
        username = ui->usernameline->text();
        password = ui->passwordline->text();
        if(contest.participants.contains(username)){
            QString pwd = contest.participants[username].pwd;
            if(password != pwd) {
                QMessageBox::warning(NULL, "warning", tr("Wrong Password!"), QMessageBox::Yes, QMessageBox::Yes);
                return;
            }
        }
        else {
            QMessageBox::warning(NULL, "warning", tr("Participant ID does not exist!"), QMessageBox::Yes, QMessageBox::Yes);
            return;
        }
        verified=true;
        ui->usernameline->setReadOnly(true);
        ui->usernameline->setStyleSheet("color: rgb(0, 170, 0);");
        ui->passwordline->setReadOnly(true);
        ui->passwordline->setStyleSheet("color: rgb(0, 170, 0);");
        ui->loginbtn->setText(tr("Start Contest"));
        return;
    }
    QDateTime start_time = contest.start_time;
    QDateTime end_time = contest.end_time;
    if(contest.start_enabled && start_time > QDateTime::currentDateTime()) {
        QMessageBox::warning(NULL, "warning", tr("Contest has not started yet!"), QMessageBox::Yes, QMessageBox::Yes);
        return;
    }
    if(contest.end_enabled && QDateTime::currentDateTime() > end_time){
        QMessageBox::warning(NULL, "warning", tr("Contest has already ended!"), QMessageBox::Yes, QMessageBox::Yes);
        return;
    }
    contest.unpackFiles(sctPath);
    StudentEditor* Editor=new StudentEditor();
    Editor->ctPath = sctPath;
    Editor->contest = &contest;
    Editor->father = father;
    Editor->id = username;
    Editor->pwd = password;
    Editor->refresh();
    Editor->show();
    this->hide();
}

void login::on_homebtn_clicked()
{
    father->show();
    this->destroy();
}

