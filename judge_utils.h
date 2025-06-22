#ifndef JUDGE_UTILS_H
#define JUDGE_UTILS_H

#include "procexelib.h"
#include "ctsettings.h"
#include <QTime>

class JudgeOp {
    struct _proc {
        TProcess* process=nullptr;
        bool isInteractor=false;
        TPipe* PTPipe=nullptr;
    };
    QList<_proc> procs;
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
    TProcess* addProcess(TProcess* process,bool isInteractor=false) {
        if(process!=nullptr) procs.push_back({process,isInteractor,nullptr});
        return process;
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
        QList<TPipe*> interactorPipes;
        for(_proc &proc_struct:procs)
            if(proc_struct.process!=nullptr && proc_struct.isInteractor) {
                interactorPipes.push_back(new TPipe());
                proc_struct.process->setStandardErrorHandle(interactorPipes.back()->hWrite);
                proc_struct.PTPipe=interactorPipes.back();
            }

        if(!startAll()) {
            judgeLog="FAIL cannot start programs";
            stopAll();
            while(!interactorPipes.isEmpty()) {
                TPipe* pipe=interactorPipes.back();
                interactorPipes.pop_back();
                delete pipe;
            }
            return {_fail,0};
        }
        QElapsedTimer timer;
        timer.start();
        while(isRunning()) {
            if(_enable_limit) {
                if(getTime_ms()>_tl) {
                    judgeLog="time limit exceeded";
                    stopAll();
                    while(!interactorPipes.isEmpty()) {
                        TPipe* pipe=interactorPipes.back();
                        interactorPipes.pop_back();
                        delete pipe;
                    }
                    return {_tle,0};
                }
                if(getMemory_MiB()>_ml) {
                    judgeLog="memory limit exceeded";
                    stopAll();
                    while(!interactorPipes.isEmpty()) {
                        TPipe* pipe=interactorPipes.back();
                        interactorPipes.pop_back();
                        delete pipe;
                    }
                    return {_mle,0};
                }
                if(timer.elapsed()>_tl*5) {
                    judgeLog="FAIL program stuck while executing";
                    stopAll();
                    while(!interactorPipes.isEmpty()) {
                        TPipe* pipe=interactorPipes.back();
                        interactorPipes.pop_back();
                        delete pipe;
                    }
                    return {_fail,0};
                }
            }
        }
        for(_proc& proc_struct:procs)
            if(proc_struct.process!=nullptr)
                if(proc_struct.process->getExitStatus()==TProcess::CrashExit) {
                    if(proc_struct.isInteractor) {
                        judgeLog="FAIL interactor crashed with exit code "+QString::number(proc_struct.process->getExitCode());
                        while(!interactorPipes.isEmpty()) {
                            TPipe* pipe=interactorPipes.back();
                            interactorPipes.pop_back();
                            delete pipe;
                        }
                        return {_fail,0};
                    } else {
                        judgeLog="Runtime Error program crashed with exit code "+QString::number(proc_struct.process->getExitCode());
                        while(!interactorPipes.isEmpty()) {
                            TPipe* pipe=interactorPipes.back();
                            interactorPipes.pop_back();
                            delete pipe;
                        }
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
                if(verdict.second<=0) {
                    while(!interactorPipes.isEmpty()) {
                        TPipe* pipe=interactorPipes.back();
                        interactorPipes.pop_back();
                        delete pipe;
                    }
                    return {verdict.first,0};
                }
            }
        while(!interactorPipes.isEmpty()) {
            TPipe* pipe=interactorPipes.back();
            interactorPipes.pop_back();
            delete pipe;
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
    if(!check.waitForFinished(timeout)) {
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
