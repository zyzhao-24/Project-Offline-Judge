#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "contesteditor.h"
#include "studenteditor.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_createbtn_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Select contest path"), "./", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    //Get contest folder
    if(dir.isEmpty())
        return;
    //Invalid path

    ContestEditor* Editor=new ContestEditor();
    Editor->father=this;
    if(!dir.endsWith('/')) dir+="/";
    Editor->ctPath=dir;
    Editor->refresh();
    Editor->show();
    this->hide();
}


void MainWindow::on_editbtn_clicked()
{
    QString dirCTINFO = QFileDialog::getOpenFileName(this, tr("Select contest info file"), "./", tr("contest info (*.ctinfo)"));
    //get ctinfo file
    if(dirCTINFO.isEmpty())
        return;
    //Invalid Path

    QString dir=dirCTINFO.mid(0,dirCTINFO.lastIndexOf("/")+1);//get contest folder
    QFile ctinfof(dirCTINFO);
    if (!ctinfof.open(QIODevice::ReadOnly)) {
        QMessageBox::warning(NULL, "warning", tr("Failed while reading ctinfo file!"), QMessageBox::Yes, QMessageBox::Yes);
        return;
    }
    QTextStream ctinfoStream(&ctinfof);
    QString JsonText=/*Encryption::decrypt_data*/(ctinfoStream.readAll());
    ctinfof.close();

    QJsonParseError parseError;
    QJsonDocument document = QJsonDocument::fromJson(JsonText.toUtf8(), &parseError);
    if (parseError.error != QJsonParseError::NoError) {
        QMessageBox::warning(NULL, "warning", tr("Error while parsing ctinfo file!"), QMessageBox::Yes, QMessageBox::Yes);
        return;
    }
    QJsonObject ContestObj=document.object();


    ContestEditor* Editor=new ContestEditor();
    Editor->father=this;
    Editor->ctPath=dir;
    if(Editor->contest.loadJsonObj(ContestObj)!=0) {
        QMessageBox::warning(NULL, "warning", tr("Invalid ctinfo file!"), QMessageBox::Yes, QMessageBox::Yes);
        delete Editor;
        return;
    }
    Editor->contest.unpackFiles(dir);
    Editor->refresh();
    Editor->show();
    this->hide();
}


void MainWindow::on_loadbtn_clicked()
{
    QString dir;
    dir = QFileDialog::getExistingDirectory(nullptr,tr("新建提交文件夹"),"./", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks) + "/";
    if(dir=="/")return;
    StudentEditor* Editor=new StudentEditor();
    Editor->father = this;
    Editor->maindir = dir;
    Editor->show();
    this->hide();
}


