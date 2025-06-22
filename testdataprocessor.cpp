#include "testdataprocessor.h"
#include "ui_testdataprocessor.h"

ValidationThread::ValidationThread(QObject* parent):QObject(parent) {}
ValidationThread::~ValidationThread() {}
void ValidationThread::runValidation(Problem* problem,QString probPath) {
    emit ValidationResults(_pending,{},"N/A");
    QApplication::processEvents();
    QString log;
    for(Problem::CompileSetting cplset:problem->cpl_settings) {
        if(cplset.precompile) {
            TResult cplres=CompileOp(this,cplset.compiler,cplset.inputs,cplset.output,parseCombinedArgString(cplset.params),probPath,log);
            emit ValidationResults(_pending,cplset.output,"N/A",log);
            if(cplres!=_ok) {
                emit ValidationResults(cplres,cplset.output,"N/A",log);
                return;
            }
        }
    }

    QList<Testdata::TDSetting> tdsetvec(problem->testdata.settings.values());
    std::sort(tdsetvec.begin(),tdsetvec.end(),[&](Testdata::TDSetting& op1,Testdata::TDSetting& op2){return op1.index<op2.index;});
    for(Testdata::TDSetting tdset:tdsetvec) {
        for(Testdata::GenCmd cmd:tdset.generator) {
            TResult genres=GenOp(this,cmd.interpreter,cmd.genorstd,cmd.params,cmd.inputFile,tdset.filename,probPath,log,cmd.multigen,cmd.cases,qMax(2*problem->time_limit_ms,2000));
            if(cmd.multigen) {
                emit ValidationResults(_pending,tdset.filename,"N/A",log);
                if(genres!=_ok) {
                    emit ValidationResults(genres,tdset.filename,"N/A",log);
                    return;
                }
            } else {
                QFile tdfile(probPath+"testdata/"+get_filename_with_id(tdset.filename,cmd.cases[0]));
                QString content="N/A";
                if (tdfile.open(QIODevice::ReadOnly))
                {
                    QTextStream tdstream(&tdfile);
                    content=tdstream.readAll();
                    tdfile.close();
                }
                emit ValidationResults(_pending,get_filename_with_id(tdset.filename,cmd.cases[0]),content,log);
                if(genres!=_ok) {
                    emit ValidationResults(genres,get_filename_with_id(tdset.filename,cmd.cases[0]),content,log);
                    return;
                }
            }
        }
    }
    for(int i=1;i<=problem->testdata.ncase;i++)
        for(Testdata::TDSetting tdset:tdsetvec)
            if(!FileOp::exists(probPath+"testdata/"+get_filename_with_id(tdset.filename,i))) {
                emit ValidationResults(_fail,get_filename_with_id(tdset.filename,i),"N/A","FAIL filecheck:file does not exist");
                return;
            }
    for(Testdata::TDSetting tdset:tdsetvec)
        if(tdset.validation)
            for(int i=1;i<=problem->testdata.ncase;i++) {
                TResult valres=ValidateOp(this,tdset.validator,tdset.filename,i,probPath,log,qMax(2*problem->time_limit_ms,2000));
                QFile tdfile(probPath+"testdata/"+get_filename_with_id(tdset.filename,i));

                QString content="N/A";
                if (tdfile.open(QIODevice::ReadOnly))
                {
                    QTextStream tdstream(&tdfile);
                    content=tdstream.readAll();
                    tdfile.close();
                }

                emit ValidationResults(_pending,get_filename_with_id(tdset.filename,i),content,log);

                if(valres!=_ok) {
                    emit ValidationResults(valres,get_filename_with_id(tdset.filename,i),content,log);
                    return;
                }
            }
    QThread::msleep(300);
    emit ValidationResults(_ok,{},{},"ok validation complete");
    return;
}

TestDataProcessor::TestDataProcessor(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::TestDataProcessor)
{
    ui->setupUi(this);
    valThreadContainer=new QThread;
    valThread=new ValidationThread;
    valThread->moveToThread(valThreadContainer);

    connect(valThreadContainer,&QThread::finished,valThread,&QObject::deleteLater);
    connect(valThread,&ValidationThread::ValidationResults,this,&TestDataProcessor::HandleResults);
    valThreadContainer->start();
    connect(this, &TestDataProcessor::startValidation, valThread, &ValidationThread::runValidation);
}

TestDataProcessor::~TestDataProcessor()
{
    delete ui;
}

void TestDataProcessor::show_subtsk_tree() {
    ui->NCaseSpBOX->setValue(problem->testdata.ncase);
    ui->SubtskTWID->clear();
    QTreeWidgetItem *samples=new QTreeWidgetItem({tr("Sample")});
    ui->SubtskTWID->addTopLevelItem(samples);
    std::sort(problem->testdata.subtasks[0].cases.begin(),problem->testdata.subtasks[0].cases.end());
    for(int case_id:problem->testdata.subtasks[0].cases) {
        QTreeWidgetItem *casewid=new QTreeWidgetItem({tr("Case ")+QString::number(case_id)});
        samples->addChild(casewid);
        for(Problem::Utility util:problem->utils) if(util.filetype==Problem::Utility::FileType::plain) {
            if(util.category==Problem::Utility::FileCategory::testdata) {
                QTreeWidgetItem *tdatawid=new QTreeWidgetItem({tr("Testdata file: ")+get_filename_with_id(util.filename,case_id)});
                casewid->addChild(tdatawid);
            } else if(util.category==Problem::Utility::FileCategory::submission) {
                QTreeWidgetItem *pasubwid=new QTreeWidgetItem({tr("Participant submission: ")+get_filename_with_id(util.filename,case_id)});
                casewid->addChild(pasubwid);
            }
        }
    }
    int score_tot=0;
    for(int index=1;index<problem->testdata.subtasks.size();index++) {
        QTreeWidgetItem *subtsk=new QTreeWidgetItem({tr("Subtask ")+QString::number(index)+": "+QString::number(problem->testdata.subtasks[index].score)+tr("pt")});
        ui->SubtskTWID->addTopLevelItem(subtsk);
        std::sort(problem->testdata.subtasks[index].cases.begin(),problem->testdata.subtasks[index].cases.end());
        score_tot+=problem->testdata.subtasks[index].score;
        for(int case_id:problem->testdata.subtasks[index].cases) {
            QTreeWidgetItem *casewid=new QTreeWidgetItem({tr("Case ")+QString::number(case_id)});
            subtsk->addChild(casewid);
            for(Problem::Utility util:problem->utils) if(util.filetype==Problem::Utility::FileType::plain) {
                if(util.category==Problem::Utility::FileCategory::testdata) {
                    QTreeWidgetItem *tdatawid=new QTreeWidgetItem({tr("Testdata file: ")+get_filename_with_id(util.filename,case_id)});
                    casewid->addChild(tdatawid);
                } else if(util.category==Problem::Utility::FileCategory::submission) {
                    QTreeWidgetItem *pasubwid=new QTreeWidgetItem({tr("Participant submission: ")+get_filename_with_id(util.filename,case_id)});
                    casewid->addChild(pasubwid);
                }
            }
        }
    }
    ui->TotScoreTXT->setText(QString::number(score_tot));
}

void TestDataProcessor::refresh_testdata_settings() {
    for(Problem::Utility util:problem->utils)
        if(util.category==Problem::Utility::FileCategory::testdata&&util.filetype==Problem::Utility::FileType::plain)
            if(!problem->testdata.settings.contains(util.filename))
                problem->testdata.settings[util.filename]=Testdata::TDSetting{(int) problem->testdata.settings.size()+1
                                                                                ,util.filename,false,"",{}};

    int index=0;
    QVector<Testdata::TDSetting> settingvec(problem->testdata.settings.values());
    std::sort(settingvec.begin(),settingvec.end(),
              [&](const Testdata::TDSetting& op1,const Testdata::TDSetting& op2)->bool {return op1.index<op2.index;}
              );
    for(QVector<Testdata::TDSetting>::Iterator setting=settingvec.begin();setting!=settingvec.end();setting++) {
        if(!problem->utils.contains(setting->filename))
        {
            problem->testdata.settings.remove(setting->filename);
            setting=settingvec.erase(setting)-1;
        }
        else if(problem->utils[setting->filename].category!=Problem::Utility::FileCategory::testdata||
                 problem->utils[setting->filename].filetype!=Problem::Utility::FileType::plain)
        {
            problem->testdata.settings.remove(setting->filename);
            setting=settingvec.erase(setting)-1;
        }
        else
        {
            problem->testdata.settings[setting->filename].index=index;
            setting->index=index;
            index++;
        }
    }

    ui->DataConfTWID->clear();
    for(Testdata::TDSetting setting:settingvec)  {
        DataConfigWidget* wid=new DataConfigWidget;
        wid->problem=problem;
        wid->setting=&problem->testdata.settings[setting.filename];
        wid->refresh();
        ui->DataConfTWID->addTab(wid,setting.filename);
    }
}


void TestDataProcessor::on_CrSubtskBTN_clicked()
{
    int score=QInputDialog::getInt(this,tr("Input subtask score"),tr("score:"),0,1,200);
    if(score>0) problem->testdata.subtasks.append(Testdata::Subtask{score,{}});
    show_subtsk_tree();
}


void TestDataProcessor::on_AddCaseBTN_clicked()
{
    QList<int> selsub;
    for(int subtsk=0;subtsk<ui->SubtskTWID->topLevelItemCount();subtsk++) {
        if(ui->SubtskTWID->topLevelItem(subtsk)->isSelected()) selsub.append(subtsk);
    }
    if(selsub.empty()) return;
    QString cases=QInputDialog::getText(this,tr("Input case id(example:1,3-5)"),tr("id:"));
    QList<int> caseids;
    for(QString caseinfo:cases.split(",")) {
        QStringList sted=caseinfo.split("-");
        bool isnum;
        if(sted.size()!=1&&sted.size()!=2) return;
        int val1=sted[0].toInt(&isnum);
        if(!isnum) return;
        if(sted.size()==1) {
            if(val1>=1&&val1<=problem->testdata.ncase) caseids.append(val1);
        } else {
            int val2=sted[1].toInt(&isnum);
            if(!isnum) return;
            for(int val=val1;val<=val2;val++) {
                if(val>=1&&val<=problem->testdata.ncase) caseids.append(val);
            }
        }
    }
    for(int subtsk:selsub) {
        for(int caseid:caseids) if(!problem->testdata.subtasks[subtsk].cases.contains(caseid)) problem->testdata.subtasks[subtsk].cases.append(caseid);
    }
    show_subtsk_tree();
}


void TestDataProcessor::on_NCaseSpBOX_valueChanged(int arg1)
{
    problem->testdata.ncase=arg1;
    for(int i=0;i<problem->testdata.subtasks.size();i++) {
        for(QList<int>::Iterator it=problem->testdata.subtasks[i].cases.begin();it!=problem->testdata.subtasks[i].cases.end();it++) {
            if(*it>arg1) it=problem->testdata.subtasks[i].cases.erase(it)-1;
        }
    }
    problem->validationSuccess=false;
    show_subtsk_tree();
}


void TestDataProcessor::on_RemSubtskBTN_clicked()
{
    for(int subtsk=ui->SubtskTWID->topLevelItemCount()-1;subtsk>=1;subtsk--) {
        if(ui->SubtskTWID->topLevelItem(subtsk)->isSelected()) problem->testdata.subtasks.removeAt(subtsk);
    }
    show_subtsk_tree();
}


void TestDataProcessor::on_RemCaseBTN_clicked()
{
    for(int subtsk=0;subtsk<ui->SubtskTWID->topLevelItemCount();subtsk++) {
        for(int index=ui->SubtskTWID->topLevelItem(subtsk)->childCount()-1;index>=0;index--) {
            if(ui->SubtskTWID->topLevelItem(subtsk)->child(index)->isSelected()) problem->testdata.subtasks[subtsk].cases.removeAt(index);
        }
    }
    show_subtsk_tree();
}


void TestDataProcessor::on_DataConfTWID_currentChanged(int index)
{
    for(int ind=0;ind<ui->DataConfTWID->count();ind++) {
        QString name=ui->DataConfTWID->tabText(ind);
        if(!problem->testdata.settings.contains(name)) refresh_testdata_settings();
        else problem->testdata.settings[name].index=ind;
    }
}


void TestDataProcessor::on_RefreshBTN_clicked()
{
    show_subtsk_tree();
    refresh_testdata_settings();
    ui->VerdictTXT->setTextColor(DescriptionCol(_NA));
    ui->VerdictTXT->setText(DescriptionStr(_NA));
    ui->VerdictTXT->setAlignment(Qt::Alignment::enum_type::AlignCenter);
    ui->FileTXT->clear();
    ui->FileContentTXT->clear();
    ui->ValLogTXT->clear();
}


void TestDataProcessor::on_RunValBTN_clicked()
{
    if(pending) return;
    pending=true;
    emit startValidation(problem,probPath);
    return;
}

void TestDataProcessor::HandleResults(TResult result,QString file,QString content,QString log) {
    if(result==_ok) problem->validationSuccess=true;
    if(result!=_pending) pending=false;
    ui->VerdictTXT->setTextColor(DescriptionCol(result));
    ui->VerdictTXT->setText(DescriptionStr(result));
    ui->VerdictTXT->setAlignment(Qt::Alignment::enum_type::AlignCenter);
    ui->FileTXT->setText(file);
    ui->FileContentTXT->setPlainText(content);
    ui->ValLogTXT->setPlainText(log);
    QApplication::processEvents();
}

void TestDataProcessor::closeEvent(QCloseEvent *event) {
    emit ExitWin();
}
