#include "login.h"
#include "ui_login.h"

login::login(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::login)
{
    ui->setupUi(this);

}

login::~login()
{
    delete ui;
}

void login::on_loginbtn_clicked(){
    qDebug()<<dir;
    if (!dir.isEmpty()) {
        QFile loadf(dir);
        if (!loadf.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QMessageBox::warning(NULL, "warning", tr("无法打开所选文件！"), QMessageBox::Yes, QMessageBox::Yes);
            return;
        }
        QTextStream in(&loadf);
        QString encryptedData = in.readAll();
        loadf.close();

        QString decryptedData = Encryption::decrypt_data(encryptedData);
        //文件解密后需要再根据内容进行解包
        QJsonDocument doc = QJsonDocument::fromJson(decryptedData.toUtf8());
        contest.loadJsonObj(doc.object());
    }
    if(contest.start_enabled ){
        ui->starttimeline->setDateTime(contest.start_time);
    }
    if(contest.end_enabled ){
        ui->endtimeline->setDateTime(contest.end_time);
    }
    QMessageBox::warning(NULL, "warning", tr("1"), QMessageBox::Yes, QMessageBox::Yes);
    bool access_ready = false;
    bool info_access = false;
    bool time_access = false;
    QString username, password;
    username = ui->usernameline->text();
    password = ui->passwordline->text();
    qDebug()<<username<<" "<<password;
    if(contest.participants.find(username)!=contest.participants.end()){
        auto p = contest.participants.find(username);
        if(password == p->pwd){
            info_access = true;
            QMessageBox::warning(NULL, "warning", tr("2"), QMessageBox::Yes, QMessageBox::Yes);
        }
        else
            QMessageBox::warning(NULL, "warning", tr("密码错误X_X"), QMessageBox::Yes, QMessageBox::Yes);
    }
    else
        QMessageBox::warning(NULL, "warning", tr("用户不存在"), QMessageBox::Yes, QMessageBox::Yes);
    time = QDateTime::currentDateTime();
    QDateTime start_time = contest.start_time;
    QDateTime end_time = contest.end_time;
    if(contest.start_enabled && contest.end_enabled){
        if(start_time <= time && time <= end_time){
            time_access = true;
        }
        else
            QMessageBox::warning(NULL, "warning", tr("不在比赛时间内"), QMessageBox::Yes, QMessageBox::Yes);
    }
    else if(contest.start_enabled){
        if(start_time <= time){
            time_access = true;
        }
        QMessageBox::warning(NULL, "warning", tr("不在比赛时间内"), QMessageBox::Yes, QMessageBox::Yes);
    }
    else if(contest.end_enabled){
        if(time <= end_time){
            time_access = true;
        }
        QMessageBox::warning(NULL, "warning", tr("不在比赛时间内"), QMessageBox::Yes, QMessageBox::Yes);
    }
    else
            time_access = true;
    if(info_access && time_access) {access_ready = true;
    QMessageBox::warning(NULL, "warning", tr("3"), QMessageBox::Yes, QMessageBox::Yes);}
    if(access_ready){
        QMessageBox::warning(NULL, "warning", tr("登陆成功"), QMessageBox::Yes, QMessageBox::Yes);
        StudentEditor* Editor=new StudentEditor();
        // Editor->father = this;
        Editor->maindir = dir;
        Editor->contest = contest;
        Editor->show();
        this->hide();
    }


}
