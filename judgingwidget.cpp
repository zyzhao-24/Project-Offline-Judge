#include "judgingwidget.h"
#include "ui_judgingwidget.h"

JudgingThread::JudgingThread(QObject* parent):QObject(parent) {}
JudgingThread::~JudgingThread() {}
void JudgingThread::setAbort(bool abort) {
    amutex.lock();
    _abort=abort;
    amutex.unlock();
}
void JudgingThread::cplRunner::StartJud(JudgingThread* parent,bool abort)
{
    parent->_cplsuccess=false;
    if(abort) {
        parent->emit CplResult(participant,problem.name,_NA,"N/A compilation stopped manually");
        return;
    }
    QString log="pending";
    if(!problem.toStudentPack()) {
        log="FAIL testdata not validated";
        parent->emit CplResult(participant,problem.name,_fail,log);
        return;
    }
    parent->emit CplResult(participant,problem.name,_pending,log);

    QString probdir=(ctdir.endsWith("/")?ctdir+problem.name+".probdata"+"/":ctdir+"/"+problem.name+".probdata"+"/");
    QString judgedir=QDir().absolutePath()+"/_JUDGE/";
    FolderOp::remove(judgedir);
    FolderOp::create(judgedir);
    FolderOp::create(judgedir+"testdata/");
    FolderOp::copy(probdir+"testdata/",judgedir+"testdata/",true);
    for(Problem::Utility util:problem.utils){
        if(util.category==Problem::Utility::FileCategory::builtin||util.category==Problem::Utility::FileCategory::resource) {
            FileOp::copy(probdir+util.filename,judgedir+util.filename,true);
        }
        if(util.category==Problem::Utility::FileCategory::submission)
        {
            if(util.filetype==Problem::Utility::FileType::code) {
                if(pa_files.isEmpty()) {
                    log="compilation error empty submission";
                    parent->emit CplResult(participant,problem.name,_ce,log);
                    return;
                } else if(pa_files.contains(util.filename)) {
                    FileOp::write(judgedir+util.filename,pa_files[util.filename]);
                } else {
                    log="compilation error file not exist: "+util.filename;
                    parent->emit CplResult(participant,problem.name,_ce,log);
                    return;
                }
            }
            if(util.filetype==Problem::Utility::FileType::snippet) {
                if(pa_files.isEmpty()) {
                    log="compilation error empty submission";
                    parent->emit CplResult(participant,problem.name,_ce,log);
                    return;
                } else if(!pa_files.contains(util.filename)) {
                    log="compilation error file not exist: "+util.filename;
                    parent->emit CplResult(participant,problem.name,_ce,log);
                    return;
                }
            }
            if(util.filetype==Problem::Utility::FileType::templ) {
                QString filecontent=FileOp::read(probdir+util.filename+".tpl");
                filecontent=Codetpl::fill_in(filecontent,pa_files);
                FileOp::write(judgedir+util.filename,filecontent);
            }
        }
    }
    for(Problem::CompileSetting cplsetting:problem.cpl_settings) {
        TResult verdict=CompileOp(parent,cplsetting.compiler,cplsetting.inputs,cplsetting.output,parseCombinedArgString(cplsetting.params),judgedir,log);
        if(verdict!=_ok) {
            parent->emit CplResult(participant,problem.name,verdict,log);
            return;
        }
    }
    log="ok compilation success";
    parent->_cplsuccess=true;
    parent->emit CplResult(participant,problem.name,_ok,log);
    return;
}
void JudgingThread::judRunner::StartJud(JudgingThread* parent,bool abort)
{
    if(abort) {
        parent->emit JudgeResult(participant,problem.name,caseid,_NA,0,0,0,"N/A judgement stopped manually");
        return;
    }
    QString log="pending";

    QString judgedir=QDir().absolutePath()+"/_JUDGE/";
    if(caseid<=0||caseid>problem.testdata.ncase) {
        log="FAIL invalid case id";
        return;
    }
    if(problem.student&&(!problem.testdata.subtasks[0].cases.contains(caseid))) {
        log="FAIL invalid case id";
        return;
    }
    if(!parent->_cplsuccess) {
        log="compilation error unsuccessful compilation";
        parent->emit JudgeResult(participant,problem.name,caseid,_ce,0,0,0,log);
        return;
    }
    if(!FolderOp::exists(judgedir)) {
        log="FAIL judge folder unprepared";
        parent->emit JudgeResult(participant,problem.name,caseid,_fail,0,0,0,log);
        return;
    }
    parent->emit JudgeResult(participant,problem.name,caseid,_pending,0,0,0,log);
    for(Problem::Utility util:problem.utils) {
        if(util.category==Problem::Utility::FileCategory::submission&&util.filetype==Problem::Utility::FileType::plain) {
            if(!pa_files.contains(get_filename_with_id(util.filename,caseid))) {
                log="wrong answer file not exist: "+get_filename_with_id(util.filename,caseid);
                parent->emit JudgeResult(participant,problem.name,caseid,_wa,0,0,0,log);
                return;
            } else {
                FileOp::write(judgedir+util.filename,pa_files[get_filename_with_id(util.filename,caseid)]);
            }
        }
        if(util.category==Problem::Utility::FileCategory::testdata) {
            FileOp::copy(judgedir+"testdata/"+get_filename_with_id(util.filename,caseid),
                         judgedir+util.filename,caseid);
        }
    }

    for(QString file:problem.judge_proc.folder.keys()) {
        FolderOp::create(judgedir+problem.judge_proc.folder[file]);
        FileOp::copy(judgedir+file,judgedir+problem.judge_proc.folder[file]+"/"+file,true);
    }

    TPipe* pipes=new TPipe[problem.judge_proc.pipecount];
    QList<TProcess*> procs;
    QList<TFile*> files;
    JudgeOp judgeOp(problem.time_limit_ms,problem.mem_limit_MiB);
    for(JudgeProcess::process procinfo:problem.judge_proc.execute_info) {
        procs.append(new TProcess(judgedir+
                                      (problem.judge_proc.folder.contains(procinfo.proc_name)?
                                           problem.judge_proc.folder[procinfo.proc_name]+"/":"")
                                      +procinfo.proc_name,procinfo.params,procinfo.interpreter));
        procs.back()->setWorkingDirectory(judgedir+
                                          (problem.judge_proc.folder.contains(procinfo.proc_name)?
                                               problem.judge_proc.folder[procinfo.proc_name]+"/":""));
        if(procinfo.stdin_id==-2) {
            files.append(new TFile(judgedir+
                                       (problem.judge_proc.folder.contains(procinfo.stdin_file)?
                                            problem.judge_proc.folder[procinfo.stdin_file]+"/":"")
                                       +procinfo.stdin_file,true));
            procs.back()->setStandardInputHandle(files.back()->hFile);
        } else if(procinfo.stdin_id>=0&&procinfo.stdin_id<(int) problem.judge_proc.pipecount) {
            procs.back()->setStandardInputHandle(pipes[procinfo.stdin_id].hRead);
        }
        if(procinfo.stdout_id==-2) {
            files.append(new TFile(judgedir+
                                   (problem.judge_proc.folder.contains(procinfo.stdout_file)?
                                        problem.judge_proc.folder[procinfo.stdout_file]+"/":"")
                                   +procinfo.stdout_file));
            procs.back()->setStandardOutputHandle(files.back()->hFile);
        } else if(procinfo.stdout_id>=0&&procinfo.stdout_id<(int) problem.judge_proc.pipecount) {
            procs.back()->setStandardOutputHandle(pipes[procinfo.stdout_id].hWrite);
        }
        judgeOp.addProcess(procs.back(),procinfo.isInteractor);
    }
    QPair<TResult,double> res=judgeOp(log,parent);
    if(res.second<=0) {
        parent->emit JudgeResult(participant,problem.name,caseid,res.first,0,judgeOp.getTime_ms(),judgeOp.getMemory_MiB(),log);
        delete[] pipes;
        while(!procs.isEmpty()) {
            TProcess* proc=procs.back();
            procs.pop_back();
            delete proc;
        }
        while(!files.isEmpty()) {
            TFile* file=files.back();
            files.pop_back();
            delete file;
        }
        return;
    }

    for(QString file:problem.judge_proc.file_collect.keys()) {
        if(!FileOp::exists(judgedir+problem.judge_proc.file_collect[file]+"/"+file)) {
            log="wrong answer file not exists:"+file;
            parent->emit JudgeResult(participant,problem.name,caseid,_wa,0,judgeOp.getTime_ms(),judgeOp.getMemory_MiB(),log);
            delete[] pipes;
            while(!procs.isEmpty()) {
                TProcess* proc=procs.back();
                procs.pop_back();
                delete proc;
            }
            while(!files.isEmpty()) {
                TFile* file=files.back();
                files.pop_back();
                delete file;
            }
            return;
        }
        FileOp::copy(judgedir+problem.judge_proc.file_collect[file]+"/"+file,judgedir+file,true);
    }

    for(JudgeProcess::ChkCmd chkcmd:problem.judge_proc.checkersCmds) {
        QString chklog;
        QPair<TResult,double> chkres=CheckOp(parent,judgedir,chkcmd.checker,chkcmd.inputFile,chkcmd.outputFile,chkcmd.answerFile,chklog,qMax(3000,2*problem.time_limit_ms));
        if(chkres.second<=res.second) {
            res=chkres;
            log=chklog;
        }
        if(res.second<=0) {
            parent->emit JudgeResult(participant,problem.name,caseid,res.first,0,judgeOp.getTime_ms(),judgeOp.getMemory_MiB(),log);
            delete[] pipes;
            while(!procs.isEmpty()) {
                TProcess* proc=procs.back();
                procs.pop_back();
                delete proc;
            }
            while(!files.isEmpty()) {
                TFile* file=files.back();
                files.pop_back();
                delete file;
            }
            return;
        }
    }
    delete[] pipes;
    while(!procs.isEmpty()) {
        TProcess* proc=procs.back();
        procs.pop_back();
        delete proc;
    }
    while(!files.isEmpty()) {
        TFile* file=files.back();
        files.pop_back();
        delete file;
    }
    parent->emit JudgeResult(participant,problem.name,caseid,res.first,res.second,judgeOp.getTime_ms(),judgeOp.getMemory_MiB(),log);
    return;
}
void JudgingThread::addCplWaitlist(const Problem& problem,
                                   const QString& participant,
                                   const QString& ctdir,
                                   const QMap<QString,QString>& pa_files) {
    mutex.lock();
    tasks.push_back(new cplRunner{problem,participant,ctdir,pa_files});
    mutex.unlock();
}
void JudgingThread::addJudWaitlist(const Problem& problem,
                                   const QString& participant,
                                   const QMap<QString,QString>& pa_files,
                                   int caseid) {
    mutex.lock();
    tasks.push_back(new judRunner{problem,participant,pa_files,caseid});
    mutex.unlock();
}
void JudgingThread::startJud() {
    amutex.lock();
    if(_pendingflag) {amutex.unlock();return;}
    _pendingflag=true;
    amutex.unlock();
    while(!tasks.empty()) {
        amutex.lock();
        if(_abort) {
            while(!tasks.empty()) {
                tasks.front()->StartJud(this,true);
                tasks.pop_front();
            }
            amutex.unlock();
            break;
        }
        amutex.unlock();
        mutex.lock();
        JudgeRunner* runner=tasks.front();
        tasks.pop_front();
        if(!runner->cpl()) mutex.unlock();
        runner->StartJud(this);
        if(runner->cpl()) mutex.unlock();
        delete runner;
    }
    amutex.lock();
    _pendingflag=false;
    amutex.unlock();
}

JudgingWidget::JudgingWidget(QWidget *parent,bool controllable)
    : QWidget(parent)
    , ui(new Ui::JudgingWidget)
{
    ui->setupUi(this);
    judThreadContainer=new QThread;
    judThread=new JudgingThread;
    judThread->moveToThread(judThreadContainer);

    connect(judThreadContainer,&QThread::finished,judThread,&QObject::deleteLater);
    connect(judThread,&JudgingThread::CplResult,this,&JudgingWidget::showCplResult);
    connect(judThread,&JudgingThread::JudgeResult,this,&JudgingWidget::showJudResult);
    judThreadContainer->start();
    connect(this,&JudgingWidget::startJudgeSig, judThread,&JudgingThread::startJud);

    if(!controllable) {
        ui->ContinueBTN->hide();
        ui->StopBTN->hide();
    }
    setAttribute(Qt::WA_QuitOnClose, false);
}

JudgingWidget::~JudgingWidget()
{
    delete ui;
}
void JudgingWidget::clrResult() {
    ui->judgeTWid->headerItem()->treeWidget()->clear();
}
void JudgingWidget::showCplResult(const QString& participant,
                               const QString& problem,
                               TResult verdict,
                               const QString& log)
{
    if(verdict!=_pending && verdict!=_NA) emit cplComplete(participant,problem,verdict,log);
    QList<QTreeWidgetItem*> pa_find=ui->judgeTWid->findItems(participant,Qt::MatchFlag::MatchExactly,0);

    QTreeWidgetItem* paitem;
    if(pa_find.isEmpty()) {
        paitem=new QTreeWidgetItem(ui->judgeTWid,{participant});
        ui->judgeTWid->addTopLevelItem(paitem);
    } else paitem=pa_find.back();

    for(int indprob=0;indprob<paitem->childCount();indprob++) {
        if(paitem->child(indprob)->text(0)==problem) paitem->removeChild(paitem->child(indprob--));
    }

    QTreeWidgetItem* probitem;
    if(verdict==_ok)
        probitem=new QTreeWidgetItem(paitem,{problem,
                                              "Compilation Success",
                                              "",
                                              "",
                                              "",
                                              log});
    else probitem=new QTreeWidgetItem(paitem,{problem,
                                              DescriptionStr(verdict),
                                              "",
                                              "",
                                              "",
                                              log});

    probitem->setForeground(1,QBrush(DescriptionCol(verdict)));
    probitem->setTextAlignment(1,Qt::AlignCenter);
    probitem->setTextAlignment(2,Qt::AlignCenter);
    probitem->setTextAlignment(3,Qt::AlignCenter);
    probitem->setTextAlignment(4,Qt::AlignCenter);
    paitem->addChild(probitem);
    ui->judgeTWid->expandAll();
    QApplication::processEvents();
}
void JudgingWidget::showJudResult(const QString& participant,
                               const QString& problem,
                               int caseid,
                               TResult verdict,
                               double score,
                               unsigned int time,
                               unsigned int mem,
                               const QString& log)
{
    if(caseid<0||caseid>999) return;
    if(verdict!=_pending && verdict!=_NA) emit judgeComplete(participant,problem,caseid,verdict,score,time,mem,log);
    QList<QTreeWidgetItem*> pa_find=ui->judgeTWid->findItems(participant,Qt::MatchFlag::MatchExactly,0);
    QTreeWidgetItem* paitem;
    if(pa_find.isEmpty()) {
        paitem=new QTreeWidgetItem(ui->judgeTWid,{participant});
        ui->judgeTWid->addTopLevelItem(paitem);
    }
    else paitem=pa_find.back();

    QTreeWidgetItem* probitem=nullptr;
    for(int indprob=0;indprob<paitem->childCount();indprob++) {
        if(paitem->child(indprob)->text(0)==problem) {
            probitem=paitem->child(indprob);
        }
    }
    if(probitem==nullptr) {
        probitem=new QTreeWidgetItem(paitem,{problem});
        paitem->addChild(probitem);
    }

    for(int indcase=0;indcase<probitem->childCount();indcase++) {
        if(probitem->child(indcase)->text(0)==
            (caseid<100?(caseid<10?"Case 00":"Case 0"):"Case ")+QString::number(caseid)
        ) probitem->removeChild(probitem->child(indcase--));
    }

    QTreeWidgetItem* caseitem;
    caseitem = new QTreeWidgetItem(probitem,{(caseid<100?(caseid<10?"Case 00":"Case 0"):"Case ")+QString::number(caseid),
                                                            DescriptionStr(verdict),
                                                            QString::number(score,'g',3),
                                                            QString::number(time),
                                                            QString::number(mem),
                                                            log});
    caseitem->setForeground(1,QBrush(DescriptionCol(verdict)));
    caseitem->setTextAlignment(1,Qt::AlignCenter);
    caseitem->setTextAlignment(2,Qt::AlignCenter);
    caseitem->setTextAlignment(3,Qt::AlignCenter);
    caseitem->setTextAlignment(4,Qt::AlignCenter);
    probitem->addChild(caseitem);
    probitem->sortChildren(0,Qt::SortOrder::AscendingOrder);
    ui->judgeTWid->expandAll();
    QApplication::processEvents();
}
void JudgingWidget::prepareJudge(const Problem& problem,
                             const QString& participant,
                             const QString& ctdir,
                             const QMap<QString,QString>& pa_files)
{
    this->judThread->addCplWaitlist(problem,participant,ctdir,pa_files);
}
void JudgingWidget::runJudge(const Problem& problem,
                               const QString& participant,
                               const QMap<QString,QString>& pa_files,
                               int caseid)
{
    this->judThread->addJudWaitlist(problem,participant,pa_files,caseid);
}
void JudgingWidget::startJudge() {
    emit startJudgeSig();
}

void JudgingWidget::on_StopBTN_clicked()
{
    judThread->setAbort(true);
    ui->StopBTN->setEnabled(false);
}

void JudgingWidget::on_ContinueBTN_clicked()
{
    judThread->setAbort(false);
    startJudge();
    ui->StopBTN->setEnabled(true);
}

void JudgingWidget::on_ClearBTN_clicked()
{
    clrResult();
}

void JudgingWidget::on_judgeTWid_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
    QMessageBox::information(NULL, "log", "Log:\n"+item->text(5), QMessageBox::Yes, QMessageBox::Yes);
}

