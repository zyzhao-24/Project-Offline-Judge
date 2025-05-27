#include "dataconfigwidget.h"
#include "ui_dataconfigwidget.h"

DataConfigWidget::DataConfigWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::DataConfigWidget)
{
    ui->setupUi(this);
}

DataConfigWidget::~DataConfigWidget()
{
    delete ui;
}

void DataConfigWidget::refresh() {
    if(setting->validation==true) {
        ui->ValidatorCBOX->setCheckState(Qt::Checked);
        ui->ValidatorTXT->setText(setting->validator);
    } else {
        ui->ValidatorCBOX->setCheckState(Qt::Unchecked);
        ui->ValidatorTXT->setText("");
    }
    QStringList gencmdtxt;
    for(Testdata::GenCmd cmd:setting->generator) {
        QString cmdtxt=(cmd.genorstd.contains(" ")?"\""+cmd.genorstd+"\"":cmd.genorstd);
        if(!cmd.interpreter.isEmpty()) cmdtxt.push_front((cmd.interpreter.contains(" ")?"\""+cmd.interpreter+"\"":cmd.interpreter)+" ");
        for(QString param:cmd.params) cmdtxt.push_back(" "+(param.contains(" ")?"\""+param+"\"":param));
        if(!cmd.inputFile.isEmpty()) cmdtxt.push_back(" <"+(cmd.inputFile.contains(" ")?"\""+cmd.inputFile+"\"":cmd.inputFile));
        cmdtxt.push_back(" >");
        if(cmd.multigen) {
            cmdtxt.push_back("{");
            QString nums="";
            int serst=-1,prev=-1;
            std::sort(cmd.cases.begin(),cmd.cases.end());
            bool cmm=0;
            for(int caseid:cmd.cases) {
                if(caseid>problem->testdata.ncase) break;
                if(caseid!=prev+1) {
                    if(serst>=1&&prev>=1) {
                        if(cmm) {
                            nums.append(",");
                        } else cmm=1;
                        if(serst==prev) nums.append(QString::number(prev));
                        else if(serst+1==prev) nums.append(QString::number(serst)+","+QString::number(prev));
                        else nums.append(QString::number(serst)+"-"+QString::number(prev));
                    }
                    serst=prev=caseid;
                }
                prev=caseid;
            }
            if(serst>=1&&prev>=1) {
                if(cmm) {
                    nums.append(",");
                } else cmm=1;
                if(serst==prev) nums.append(QString::number(prev));
                else if(serst+1==prev) nums.append(QString::number(serst)+","+QString::number(prev));
                else nums.append(QString::number(serst)+"-"+QString::number(prev));
            }
            if(nums.isEmpty()) continue;
            cmdtxt.push_back(nums);
            cmdtxt.push_back("}");
        } else {
            cmdtxt.push_back(QString::number(cmd.cases[0]));
        }
        gencmdtxt.append(cmdtxt);
    }
    ui->GenCmdTXT->setPlainText(gencmdtxt.join("\n"));
}

void DataConfigWidget::on_ValidatorCBOX_checkStateChanged(const Qt::CheckState &arg1)
{
    if(arg1==Qt::Checked) {
        ui->ValidatorTXT->setEnabled(true);
    }
    else {
        ui->ValidatorTXT->clear();
        ui->ValidatorTXT->setEnabled(false);
    }
}


void DataConfigWidget::on_pushButton_clicked()
{
    if(ui->ValidatorCBOX->isChecked()) {
        if(problem->cpl_settings.contains(ui->ValidatorTXT->text())) {
            setting->validation=true;
            setting->validator=ui->ValidatorTXT->text();
        }
    } else {
        setting->validation=false;
        setting->validator="";
    }
    QStringList cmds=ui->GenCmdTXT->toPlainText().split("\n");
    setting->generator.clear();
    for(QString cmd:cmds) {
        Testdata::GenCmd info;

        QStringList cmdall=parseCombinedArgString(cmd);
        if(cmdall.isEmpty()) continue;
        QStringList params;

        if(cmdall[0]=="python"||cmdall[0]=="python.exe") {
            info.interpreter=cmdall[0];
            cmdall.pop_front();
            if(cmdall.isEmpty()) continue;
        }

        info.genorstd=cmdall[0];
        cmdall.pop_front();

        bool valid=true;

        for(QString part:cmdall) {
            if(part.isEmpty()) {continue;}
            if(part[0]=="<")
            {
                info.inputFile=part.mid(1);
                if(info.inputFile.isEmpty()) {valid=false;break;}
            }
            else if(part[0]==">")
            {
                QString cases=part.mid(1);
                if(cases.isEmpty()) {valid=false;break;}
                if(cases.front()=="{"&&cases.back()=="}") {
                    info.multigen=true;
                    info.cases.clear();
                    cases.removeFirst();
                    cases.removeLast();
                    for(QString valrange:cases.split(",")) {
                        QStringList sted=valrange.split("-");
                        bool isnum;
                        if(sted.size()!=1&&sted.size()!=2) continue;
                        int val1=sted[0].toInt(&isnum);
                        if(!isnum) continue;
                        if(sted.size()==1) {
                            if(val1>=1&&val1<=problem->testdata.ncase&&!info.cases.contains(val1)) info.cases.append(val1);
                        } else {
                            int val2=sted[1].toInt(&isnum);
                            if(!isnum) continue;
                            for(int val=val1;val<=val2;val++) {
                                if(val>=1&&val<=problem->testdata.ncase&&!info.cases.contains(val)) info.cases.append(val);
                            }
                        }
                    }
                    if(info.cases.isEmpty()) {valid=false;break;}
                    std::sort(info.cases.begin(),info.cases.end());
                } else {
                    bool isnum;
                    info.multigen=false;
                    info.cases={cases.toInt(&isnum)};
                    if(!isnum)  {valid=false;break;}
                }
            }
            else params.append(part);
        }
        info.params=params;
        if(info.cases.isEmpty()) valid=false;
        if(info.multigen&&(!info.inputFile.isEmpty())) valid=false;
        if(valid) setting->generator.append(info);
    }
    problem->validationSuccess=false;
    refresh();
}

