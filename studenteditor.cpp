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
        IDE->submission = &submission;
        IDE->maindir = maindir;
        IDE->show();
    }
}

bool StudentInfoValidation(Contest contest,QString usrname, QString pwd){
    if(contest.participants.find(usrname)!=contest.participants.end()){
        auto p = contest.participants.find(usrname);
        if(pwd == p->pwd){
            return true;
        }
        else return false;
    }
    else return false;
}

void StudentEditor::set_time(){
    if(contest.start_enabled ){
        ui->StartTime->setDateTime(contest.start_time);
    }
    if(contest.end_enabled ){
        ui->EndTime->setDateTime(contest.end_time);
    }
}

void StudentEditor::load_prob(){
    for(auto p = contest.problems.begin();p!=contest.problems.end();p++){
        ui->Problist->addItem(p->name);
    }
}

void StudentEditor::on_loadcontestbtn_clicked(){

    QString filePath = QFileDialog::getOpenFileName(nullptr, tr("选择比赛设置文件"), "", tr("比赛设置文件 (*.stuinfo)"));
    if (!filePath.isEmpty()) {
        QFile loadf(filePath);
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
        Pack = doc.object();
        Packer::createDirectoryFromJson(Pack,maindir);
        if (doc.isObject()) {
            QJsonArray contents = Pack["contents"].toArray();
            bool found = false;

            for(const auto & item : contents){
                QJsonObject fileObj = item.toObject();
                if (fileObj["type"].toString() == "file" && fileObj["name"].toString().contains(".ctinfo")) {

                    QString base64Data = fileObj["data"].toString();
                    QByteArray decodedData = QByteArray::fromBase64(base64Data.toUtf8());

                    QJsonDocument dataDoc = QJsonDocument::fromJson(decodedData);
                    if (dataDoc.isObject()) {  // 确保解析成功
                        QJsonObject dataObj = dataDoc.object();
                        contest.loadJsonObj(dataObj);
                        found = true;
                        break;
                    } else {
                        QMessageBox::warning(NULL, "错误", "比赛设置文件格式错误！" ,QMessageBox::Ok, QMessageBox::Ok);
                        return;
                    }
                }
            }

            if (found) {
                QMessageBox::information(NULL, "信息", tr("比赛设置导入成功！"),
                                         QMessageBox::Yes, QMessageBox::Yes);
                ui->contestname->setText(contest.name);
                access = false;
                set_time();
                load_prob();
            } else {
                QMessageBox::warning(NULL, "错误", "未找到比赛设置文件！",
                                     QMessageBox::Ok, QMessageBox::Ok);
            }
        } else {
            QMessageBox::warning(NULL, "错误", "主文件格式错误！",
                                 QMessageBox::Ok, QMessageBox::Ok);
        }
    }
    //decrypt ctinfo
    for(auto p :contest.problems){
        for(auto util : p.utils){
            qDebug()<<util.filename<<" ";
        }
    }
}

void StudentEditor::view(){
    if(access){
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
            problem.toStudentPack();

            QJsonArray contents = Pack["contents"].toArray();
            bool found = false;
            for (const auto & item : contents) {
                QJsonObject dirObj = item.toObject();

                // 查找 .probdata 目录
                if (dirObj["type"].toString() == "directory" &&
                    dirObj["name"].toString() == prob_name + ".probdata") {

                    QJsonArray probFiles = dirObj["contents"].toArray();
                    found = true;

                    // 遍历 .probdata 目录下的文件
                    for (const auto & fileItem : probFiles) {
                        QJsonObject fileObj = fileItem.toObject();

                        // 查找 content.pdf 文件
                        if (fileObj["name"].toString() == "content.pdf") {
                            QString base64Data = fileObj["data"].toString(); // 从文件对象获取data

                            // 解码Base64数据
                            QByteArray pdfData = QByteArray::fromBase64(base64Data.toUtf8());
                            if (pdfData.isEmpty()) {
                                QMessageBox::warning(this, "警告", "解码后的PDF数据为空，请检查Base64编码是否正确。");
                                return;
                            }
                            QTemporaryFile tmppdf;
                            if(tmppdf.open()){
                                tmppdf.write(pdfData);
                                tmppdf.close();
                            }
                            QString filePath = tmppdf.fileName();
                            QFile pdf(filePath);
                            pdfdoc->load(filePath);
                        }
                    }
                    break;
                }
            }

            if (!found) {
                QMessageBox::warning(this, "警告", "未找到 " + prob_name + ".probdata 文件夹");
            }
            QMessageBox::warning(this, "成功", "PDF文档准备完成。");
            pdfview->setDocument(pdfdoc);
            pdfview->show();
        }
    }

    else{
        QMessageBox::warning(this, "提示", "请先完成登录再查看题目");
    }
}
void StudentEditor::on_viewbtn_clicked(){
    view();
}

void StudentEditor::on_loginbtn_clicked(){
    //Read username and pwd
    QString usrname(ui->usrnamelte->text());
    QString pwd(ui->codelte->text());
    //if valid, Load Student info
    if(StudentInfoValidation(contest,usrname,pwd)){
        submission.usrname = usrname;
        submission.id = contest.participants.find(usrname) ->id;
        submission.pwd = pwd;
        QMessageBox::information(nullptr, "信息提示", "登陆成功");
        access = true;
    }
    else{
        QMessageBox::information(nullptr, "信息提示", "登陆失败，请检查比赛设置");
        access = false;
    }
    //refresh
}

void StudentEditor::on_returnbtn_clicked(){
    father->show();
    this->destroy();
}

void StudentEditor::on_savebtn_clicked(){
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

