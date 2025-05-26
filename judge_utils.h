#ifndef JUDGE_UTILS_H
#define JUDGE_UTILS_H

#include "procexelib.h"
#include <QColor>
#include "ctsettings.h"

enum TResult {
    _ok = 0,
    _wa = 1,
    _pe = 2,
    _fail = 3,
    _dirt = 4,
    _points = 5,
    _unexpected_eof = 8,
    _ce = 9,
    _tle =10,
    _mle = 11,
    _re = 12,
    _pending=14,
    _NA = 15,
    _partially = 16
};

#define _pc(exitCode) (TResult(_partially + (exitCode)))

static const QString JudgeDescriptionStr(TResult result) {
    switch(result) {
    case _ok:
        return "Accepted";
        break;
    case _wa:
        return "Wrong Answer";
        break;
    case _pe:
    case _dirt:
    case _unexpected_eof:
        return "Presentation Error";
        break;
    case _points:
    case _partially:
        return "Partially Correct";
        break;
    case _fail:
        return "FAIL";
        break;
    case _ce:
        return "Compilation Error";
        break;
    case _tle:
        return "Time Limit Exceeded";
        break;
    case _mle:
        return "Memory Limit Exceeded";
        break;
    case _re:
        return "Runtime Error";
        break;
    case _pending:
        return "Pending";
        break;
    case _NA:
        return "N/A";
        break;
    default:
        return "Invalid";
        break;
    }
}

static const QColor JudgeDescriptionCol(TResult result) {
    switch(result) {
    case _ok:
        return QColor::fromRgb(50,205,50);
        break;
    case _wa:
    case _pe:
    case _dirt:
    case _unexpected_eof:
        return QColor::fromRgb(139,0,18);
        break;
    case _points:
    case _partially:
        return QColor::fromRgb(0,0,205);
        break;
    case _fail:
        return QColor::fromRgb(139,0,18);
        break;
    case _ce:
        return QColor::fromRgb(0,191,255);
        break;
    case _tle:
    case _mle:
    case _re:
        return QColor::fromRgb(139,0,18);
        break;
    case _pending:
        return QColor::fromRgb(0,0,205);
        break;
    default:
        return QColor::fromRgb(169,169,169);
        break;
    }
}

static TResult CompileOp(QObject* parent,
                         const QString& cmd,
                         const QStringList& inputs,
                         const QString& output,
                         const QStringList& params,
                         const QString& probpath,
                         QString& cplLog) {
    QString path=probpath;
    QStringList args;
    if(!path.endsWith("/")) path.append("/");
    if(cmd == "g++.exe" || cmd =="gcc.exe") {
        for(QString input:inputs) args<<path+input;
        args<<"-o"<<path+output;
        args.append(params);
    } else {
        cplLog="FAIL compiler not supported";
        return _fail;
    }
    QProcess compilation(parent);
    compilation.setProgram(cmd);
    compilation.setArguments(args);
    compilation.setWorkingDirectory(probpath);
    compilation.start();
    if(!compilation.waitForStarted(500)) {
        compilation.terminate();
        cplLog="FAIL cannot start compiler";
        return _fail;
    }
    if(!compilation.waitForFinished(30000)) {
        compilation.terminate();
        cplLog="FAIL compilation exceeded time limit";
        return _fail;
    }
    int ret=compilation.exitCode();
    if(compilation.exitStatus()==QProcess::ExitStatus::CrashExit) {
        cplLog="FAIL compiler crashed";
        return _fail;
    }
    else if(ret==0) {
        cplLog="ok compilation success";
        return _ok;
    }
    else {
        cplLog="compilation error exit code "+QString::number(ret)+"\n"+QString::fromLocal8Bit(compilation.readAllStandardError());
        return _ce;
    }
}

static QPair<TResult,double> parseVerdict(const QString& msg) {
    QStringList lst=msg.split(" ");
    if(lst[0]=="ok") return {_ok,1};
    if(lst[0]=="wrong") {
        if(lst[1]=="answer") return {_wa,0};
        if(lst[1]=="output" && lst[2]=="format") return {_pe,0};
    }
    if(lst[0]=="FAIL") return {_fail,0};
    if(lst[0]=="point") {
        QString score_info=lst[1];
        if(score_info.contains("=")) score_info=score_info.split("=")[1];
        bool ok;
        double score=score_info.toDouble(&ok);
        if(ok) {
            if(score>=1) return {_ok,1};
            if(score<=0) return {_wa,0};
            return {_points,score};
        }
        else return {_fail,0};
    }
    if(lst[0]=="unexpected"&&lst[1]=="eof") return {_unexpected_eof,0};
    if(lst[0]=="partially"&&lst[1]=="correct") {
        QString score_info=lst[2];
        if(score_info.front()=="(") score_info.removeFirst();
        if(score_info.back()==")") score_info.removeLast();
        bool ok;
        int partial=score_info.toInt(&ok);
        if(ok) {
            if(partial>=200) return {_ok,1};
            if(partial<=0) return {_wa,0};
            return {_points,partial/200.0};
        }
        else return {_fail,0};
    }
    return {_fail,0};
}

class JudgeOp {
    struct _proc {
        TProcess* process=nullptr;
        bool isInteractor=false;
        TPipe* PTPipe=nullptr;
    };
    QList<_proc> procs;
    QList<TProcess> interactors;
    unsigned int _tl{0},_ml{0};
    bool _enable_limit{false};
public:
    JudgeOp(unsigned int timeLimit_ms,unsigned int memLimit_MiB): _tl(timeLimit_ms),_ml(memLimit_MiB),_enable_limit(true) {}
    JudgeOp(): _tl(0),_ml(0),_enable_limit(false) {}
    ~JudgeOp() {
        for(_proc proc_struct:procs) {
            if(proc_struct.process!=nullptr) proc_struct.process->stop();
        }
    }
    JudgeOp(const JudgeOp&) =delete;
    JudgeOp(JudgeOp&&) =delete;

    void addProcess(TProcess* process,bool isInteractor=false) {
        if(process!=nullptr) procs.push_back({process,isInteractor,nullptr});
    }
    void addInteractor(const QString& exepath,
                       const QString& interactor,
                       const QString& inputfile,
                       const QString& outputfile,
                       const QString& ansfile=NULL
                       )
    {
        QString path=exepath;
        if(!path.endsWith("/")) path.append("/");
        if(ansfile.isEmpty()) interactors.push_back(TProcess(path+interactor,QStringList()<<inputfile<<outputfile));
        else interactors.push_back(TProcess(path+interactor,QStringList()<<inputfile<<outputfile<<ansfile));
        interactors.back().setWorkingDirectory(path);
        procs.push_back({&interactors.back(),true,nullptr});
    }
    void clear() {
        procs.clear();
    }
    void resetLimit() {
        _tl=0;
        _ml=0;
        _enable_limit=false;
    }
    void setLimit(unsigned int timeLimit_ms,unsigned int memLimit_MiB) {
        _tl=timeLimit_ms;
        _ml=memLimit_MiB;
        _enable_limit=true;
    }

    unsigned int getTime_ms() {
        unsigned int sum=0;
        for(_proc proc_struct:procs) {
            if(proc_struct.process!=nullptr) sum+=proc_struct.process->getTime_ms();
        }
        return sum;
    }
    unsigned int getMemory_MiB() {
        unsigned int sum=0;
        for(_proc proc_struct:procs) {
            if(proc_struct.process!=nullptr) sum+=proc_struct.process->getMemory_MiB();
        }
        return sum;
    }

    bool startAll() {
        for(_proc proc_struct:procs) {
            if(proc_struct.process!=nullptr)
                if(!proc_struct.process->start()) return false;
        }
        return true;
    }

    void stopAll() {
        for(_proc proc_struct:procs) {
            if(proc_struct.process!=nullptr) proc_struct.process->stop();
        }
    }

    bool isRunning() {
        for(_proc proc_struct:procs)
            if(proc_struct.process!=nullptr)  {
                if(proc_struct.process->getProcessState()==TProcess::Running) return true;
            }
        return false;
    }

    QPair<TResult,double> operator()(QString& judgeLog,QObject* parent=nullptr) {
        QList<TPipe> interactorPipes;
        for(_proc &proc_struct:procs)
            if(proc_struct.process!=nullptr && proc_struct.isInteractor) {
                interactorPipes.push_back(TPipe());
                proc_struct.process->setStandardErrorHandle(interactorPipes.back().hWrite);
                proc_struct.PTPipe=&interactorPipes.back();
            }

        if(!startAll()) {
            judgeLog="FAIL cannot start programs";
            stopAll();
            return {_fail,0};
        }
        while(isRunning()) {
            if(_enable_limit) {
                if(getTime_ms()>_tl*1.2) {
                    judgeLog="time limit exceeded";
                    stopAll();
                    return {_tle,0};
                }
                if(getMemory_MiB()>_ml*1.2) {
                    judgeLog="memory limit exceeded";
                    stopAll();
                    return {_mle,0};
                }
            }
        }
        for(_proc& proc_struct:procs)
            if(proc_struct.process!=nullptr)
                if(proc_struct.process->getExitStatus()==TProcess::CrashExit) {
                    if(proc_struct.isInteractor) {
                        judgeLog="FAIL interactor crashed with exit code "+QString::number(proc_struct.process->getExitCode());
                        return {_fail,0};
                    } else {
                        judgeLog="Runtime Error program crashed with exit code "+QString::number(proc_struct.process->getExitCode());
                        return {_re,0};
                    }
                }
        double scorePercent=1;
        judgeLog="ok judge complete";

        for(_proc& proc_struct:procs)
            if(proc_struct.process!=nullptr&&proc_struct.isInteractor) {
                THandleDevice device(&proc_struct.PTPipe->hRead,QIODevice::ReadOnly,parent);
                QTextStream stream(&device);
                QString message=stream.readAll();
                auto verdict=parseVerdict(message);
                if(scorePercent>verdict.second) scorePercent=verdict.second;
                if(scorePercent>=verdict.second) judgeLog=message;
                if(verdict.second<=0) return {verdict.first,0};
            }
        if(scorePercent<=0) return {_wa,0};
        if(scorePercent>=1) return {_ok,1};
        return {_points,scorePercent};
    }
};

static QPair<TResult,double> CheckOp(QObject* parent,
                                    const QString& exepath,
                                    const QString& checker,
                                    const QString& inputfile,
                                    const QString& outputfile,
                                    const QString& ansfile,
                                    QString& chkLog,
                                    int timeout=1000)
{
    QString path=exepath;
    QStringList args;
    args<<inputfile<<outputfile<<ansfile;
    if(!path.endsWith("/")) path.append("/");
    QProcess check(parent);
    check.setProgram(path+checker);
    check.setArguments(args);
    check.setWorkingDirectory(path);
    check.start();
    if(!check.waitForStarted(500)) {
        check.terminate();
        chkLog="FAIL cannot start checker";
        return {_fail,0};
    }
    if(!check.waitForFinished(timeout*6/5)) {
        check.terminate();
        chkLog="FAIL checker exceeded time limit";
        return {_fail,0};
    }
    if(check.exitStatus()==QProcess::ExitStatus::CrashExit) {
        chkLog="FAIL checker crashed";
        return {_fail,0};
    }
    chkLog=QString::fromLocal8Bit(check.readAllStandardError());
    return parseVerdict(chkLog);
}

#endif // JUDGE_UTILS_H
