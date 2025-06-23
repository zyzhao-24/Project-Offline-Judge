#include "contesteditor.h"
#include "ui_contesteditor.h"

#include "problemeditor.h"
#include "judgepanel.h"

ContestEditor::ContestEditor(QWidget *parent)
    : QMainWindow(parent)
    , panel(this)
    , ui(new Ui::ContestEditor)
{
    ui->setupUi(this);
    panel.contest=&contest;
}

ContestEditor::~ContestEditor()
{
    delete ui;
}

void ContestEditor::loadPath() {
    ui->ctpathtext->setText(ctPath);
}
void ContestEditor::setName() {
    QString displayName=ui->nametext->displayText();
    if(!StrVal::isValidName(displayName)) {
        QMessageBox::warning(NULL, "warning", tr("Invalid name! Name should NOT be empty and should only contain Letters, Numbers and Underscore \' _ \'!"), QMessageBox::Yes, QMessageBox::Yes);
        return;
    }
    contest.name=displayName;
}
void ContestEditor::loadName(){
    ui->nametext->setText(contest.name);
}
void ContestEditor::setTime() {
    if(ui->dstchkbox->isChecked()==false
        &&ui->dedchkbox->isChecked()==false
        &&ui->edtime->dateTime()<ui->sttime->dateTime()
        ) {
        QMessageBox::warning(NULL, "warning", tr("End time must be later than start time!"), QMessageBox::Yes, QMessageBox::Yes);
        return;
    }
    contest.start_time=ui->sttime->dateTime();
    contest.start_enabled=!ui->dstchkbox->isChecked();
    contest.end_time=ui->edtime->dateTime();
    contest.end_enabled=!ui->dedchkbox->isChecked();
}
void ContestEditor::loadTime() {
    ui->dstchkbox->setCheckState(contest.start_enabled?Qt::Unchecked:Qt::Checked);
    ui->dedchkbox->setCheckState(contest.end_enabled?Qt::Unchecked:Qt::Checked);
    ui->sttime->setDateTime(contest.start_time);
    ui->edtime->setDateTime(contest.end_time);
}
void ContestEditor::setPa() {
    contest.participants.clear();
    int row_cnt=ui->patabwid->rowCount();
    for(int index=0;index<row_cnt;index++) {
        Contest::Participant dat={"",""};
        if(ui->patabwid->item(index,0)) dat.id=ui->patabwid->item(index,0)->text();
        if(ui->patabwid->item(index,1)) dat.pwd=ui->patabwid->item(index,1)->text();
        if(!StrVal::isValidUsername(dat.id))
            continue;
        if(!StrVal::isValidPwd(dat.pwd))
            continue;
        contest.participants[dat.id]=dat;
    }
}
void ContestEditor::loadPa() {
    QVector <Contest::Participant> pavec(contest.participants.values());
    ui->patabwid->setRowCount(pavec.size());
    for(int index=0;index<pavec.size();index++) {
        QTableWidgetItem *idItem = new QTableWidgetItem(pavec[index].id);
        QTableWidgetItem *pwdItem = new QTableWidgetItem(pavec[index].pwd);
        ui->patabwid->setItem(index,0,idItem);
        ui->patabwid->setItem(index,1,pwdItem);
    }
}
void ContestEditor::loadProb() {
    QVector <Problem> probvec(contest.problems.values());
    ui->problistwid->clear();
    for(int index=0;index<probvec.size();index++) {
        ui->problistwid->addItem(probvec[index].name);
        if(!FolderOp::exists(ctPath+probvec[index].name+".probdata/")) {
            FolderOp::create(ctPath+probvec[index].name+".probdata/");
        }
        if(!FolderOp::exists(ctPath+probvec[index].name+".probdata/testdata/")) {
            FolderOp::create(ctPath+probvec[index].name+".probdata/testdata/");
        }
        if(!FileOp::exists(ctPath+probvec[index].name+".probdata/testlib.h")) {
            FileOp::copy(QCoreApplication::applicationDirPath()+"/testlib.h",ctPath+probvec[index].name+".probdata/testlib.h",true);
        }
    }
}

void ContestEditor::refresh() {
    loadPath();
    loadName();
    loadTime();
    loadPa();
    loadProb();
}
void ContestEditor::save() {
    QFile savef(ctPath+contest.name+ctinfo);
    if (!savef.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        QMessageBox::warning(NULL, "warning", tr("Failed while saving contest settings!"), QMessageBox::Yes, QMessageBox::Yes);
        return;
    }

    QJsonDocument doc;
    doc.setObject(contest.JsonContestObj());
    QTextStream stream(&savef);
    stream << Encryption::encrypt_data(doc.toJson());
    savef.close();

    // toStudent needed
}


void ContestEditor::on_refreshbtn_clicked()
{
    refresh();
}
void ContestEditor::on_savebtn_clicked()
{
    save();
}
void ContestEditor::on_returnbtn_clicked()
{
    father->show();
    this->destroy();
}

void ContestEditor::on_setnamebtn_clicked()
{
    setName();
}

void ContestEditor::on_dstchkbox_checkStateChanged(const Qt::CheckState &arg1)
{
    if(arg1==Qt::Checked)
        ui->sttime->setEnabled(false);

    if(arg1==Qt::Unchecked)
        ui->sttime->setEnabled(true);
}
void ContestEditor::on_dedchkbox_checkStateChanged(const Qt::CheckState &arg1)
{
    if(arg1==Qt::Checked)
        ui->edtime->setEnabled(false);

    if(arg1==Qt::Unchecked)
        ui->edtime->setEnabled(true);
}

void ContestEditor::on_settimebtn_clicked()
{
    setTime();
}
void ContestEditor::on_addbtn_clicked()
{
    ui->patabwid->setRowCount(ui->patabwid->rowCount()+1);
}
void ContestEditor::on_rembtn_clicked()
{
    ui->patabwid->removeRow(ui->patabwid->currentRow());
}
void ContestEditor::on_impbtn_clicked()
{
    QString pa_csv=QFileDialog::getOpenFileName(this, tr("Import From csv"), ctPath, tr("comma seperated values (*.csv)"));
    if(pa_csv=="") return;
    QVector<QString> lines;
    QString line;
    QFile csvf(pa_csv);
    if (!csvf.open(QIODevice::ReadOnly)) {
        QMessageBox::warning(NULL, "warning", tr("Failed while reading csv file!"), QMessageBox::Yes, QMessageBox::Yes);
        return;
    }
    QTextStream csvstream(&csvf);
    while(!csvstream.atEnd()) {
        line=csvstream.readLine();
        lines.push_back(line);
    }
    int skipped=0;
    for(int index=0;index<lines.size();index++) {
        if(lines[index].isEmpty()) continue;
        QVector<QString> info=lines[index].split(",");
        if(info.size()<2) {
            skipped++;
            continue;
        }
        if(!StrVal::isValidUsername(info[0])) {
            skipped++;
            continue;
        }
        if(!StrVal::isValidPwd(info[1])) {
            skipped++;
            continue;
        }
        ui->patabwid->setRowCount(ui->patabwid->rowCount()+1);
        QTableWidgetItem *idItem = new QTableWidgetItem(info[0]);
        QTableWidgetItem *pwdItem = new QTableWidgetItem(info[1]);
        ui->patabwid->setItem(ui->patabwid->rowCount()-1,0,idItem);
        ui->patabwid->setItem(ui->patabwid->rowCount()-1,1,pwdItem);
    }
    if(skipped) {
        QString warning_text=QString::number(skipped)+" "+tr("lines discarded!");
        QMessageBox::warning(NULL, "warning", warning_text.toStdString().c_str(), QMessageBox::Yes, QMessageBox::Yes);
    }
    csvf.close();
}
void ContestEditor::on_clearbtn_clicked()
{
    ui->patabwid->setRowCount(0);
}
void ContestEditor::on_expbtn_clicked()
{
    QString pa_csv=QFileDialog::getSaveFileName(this, tr("Export to csv"), ctPath, tr("comma seperated values (*.csv)"));
    if(pa_csv.isEmpty()) return;
    QFile csvf(pa_csv);
    if (!csvf.open(QIODevice::WriteOnly|QIODevice::Truncate)) {
        QMessageBox::warning(NULL, "warning", tr("Failed while writing csv file!"), QMessageBox::Yes, QMessageBox::Yes);
        return;
    }
    QTextStream csvstream(&csvf);
    int row_cnt=ui->patabwid->rowCount();
    for(int index=0;index<row_cnt;index++) {
        Contest::Participant dat={"",""};
        if(ui->patabwid->item(index,0)) dat.id=ui->patabwid->item(index,0)->text();
        if(ui->patabwid->item(index,1)) dat.pwd=ui->patabwid->item(index,1)->text();
        if(!StrVal::isValidUsername(dat.id))
            continue;
        if(!StrVal::isValidPwd(dat.pwd))
            continue;
        csvstream<<dat.id<<","<<dat.pwd<<Qt::endl;
    }
    csvf.close();
}
void ContestEditor::on_setbtn_clicked()
{
    setPa();
    loadPa();
}

void ContestEditor::on_addprobbtn_clicked()
{
    QString new_prob_name=ui->probnametext->text();
    if(!StrVal::isValidName(new_prob_name)) {
        QMessageBox::warning(NULL, "warning", tr("Invalid problem name!"), QMessageBox::Yes, QMessageBox::Yes);
        return;
    }
    if(contest.problems.contains(new_prob_name)) {
        QMessageBox::warning(NULL, "warning", tr("Problem name already exists!"), QMessageBox::Yes, QMessageBox::Yes);
        return;
    }
    contest.problems[new_prob_name]=Problem(new_prob_name);
    FolderOp::create(ctPath+new_prob_name+".probdata/");
    FolderOp::create(ctPath+new_prob_name+".probdata/testdata/");
    FileOp::copy(QCoreApplication::applicationDirPath()+"/testlib.h",ctPath+new_prob_name+".probdata/testlib.h",true);
    contest.problems[new_prob_name].utils["testlib.h"]={"testlib.h",Problem::Utility::FileCategory::builtin,Problem::Utility::FileType::code};
    loadProb();
}

void ContestEditor::on_renprobbtn_clicked()
{
    if(ui->problistwid->selectedItems().size()!=1) return;
    QString prev_prob_name=ui->problistwid->selectedItems().constFirst()->text();
    if(!contest.problems.contains(prev_prob_name)) return;
    QString new_prob_name=ui->probnametext->text();
    if(prev_prob_name==new_prob_name) return;
    if(!StrVal::isValidName(new_prob_name)) {
        QMessageBox::warning(NULL, "warning", tr("Invalid problem name!"), QMessageBox::Yes, QMessageBox::Yes);
        return;
    }
    if(contest.problems.contains(new_prob_name)) {
        QMessageBox::warning(NULL, "warning", tr("Problem name already exists!"), QMessageBox::Yes, QMessageBox::Yes);
        return;
    }
    contest.problems[prev_prob_name].name=new_prob_name;
    contest.problems[new_prob_name]=contest.problems[prev_prob_name];
    contest.problems.remove(prev_prob_name);
    FolderOp::rename(ctPath+prev_prob_name+".probdata/",ctPath+new_prob_name+".probdata/");
    loadProb();
}


void ContestEditor::on_remprobbtn_clicked()
{
    if(ui->problistwid->selectedItems().size()!=1) return;
    QString prev_prob_name=ui->problistwid->selectedItems().constFirst()->text();
    if(!contest.problems.contains(prev_prob_name)) return;
    contest.problems.remove(prev_prob_name);
    FolderOp::remove(ctPath+prev_prob_name+".probdata/");
    loadProb();
}


void ContestEditor::on_problistwid_currentTextChanged(const QString &currentText)
{
    ui->probnametext->setText(currentText);
}


void ContestEditor::on_confprobbtn_clicked()
{
    if(ui->problistwid->selectedItems().size()!=1) return;
    QString prob_name=ui->problistwid->selectedItems().constFirst()->text();
    if(!contest.problems.contains(prob_name)) return;
    loadProb();
    ProblemEditor* probEditor=new ProblemEditor;
    probEditor->conEditor=this;
    probEditor->problem=&contest.problems[prob_name];
    probEditor->probPath=ctPath+prob_name+".probdata/";
    probEditor->show();
    this->hide();
    connect(probEditor,&ProblemEditor::ExitWin,this,&ContestEditor::show);
}


void ContestEditor::on_gradebtn_clicked()
{
    panel.refresh();
    panel.show();
    this->hide();
    connect(&panel,&JudgePanel::ExitWin,this,&ContestEditor::show);
}
