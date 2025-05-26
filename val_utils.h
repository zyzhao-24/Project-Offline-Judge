#ifndef VAL_UTILS_H
#define VAL_UTILS_H

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

static const QString ValDescriptionStr(TResult result) {
    switch(result) {
    case _ok:
        return "Accepted";
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

static const QColor ValDescriptionCol(TResult result) {
    switch(result) {
    case _ok:
        return QColor::fromRgb(50,205,50);
        break;
    case _fail:
        return QColor::fromRgb(139,0,18);
        break;
    case _ce:
        return QColor::fromRgb(0,191,255);
        break;
    case _tle:
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

static TResult GenOp(QObject* parent,
                         const QString& interpreter,// leave empty if none
                         const QString& gen_or_std,
                         const QStringList& params,
                         const QString& input,//leave empty if none
                         const QString& output,
                         const QString& probpath,
                         QString& genLog,
                         const bool& multigen,
                         QList<int> testcases,
                         int timeout=1000) {
    QString path=probpath;
    if(!path.endsWith("/")) path.append("/");
    if(testcases.isEmpty()) {
        genLog="FAIL no test case present";
        return _fail;
    }
    if(output.isEmpty()) {
        genLog="FAIL invalid output file";
        return _fail;
    }
    QStringList args(params);
    if((!multigen)&&testcases.size()>1) {
        genLog="FAIL multiple testcases without multigen";
        return _fail;
    }

    QProcess generation(parent);
    if(interpreter.isEmpty()) generation.setProgram(path+gen_or_std);
    else {
        generation.setProgram(interpreter);
        args.push_front(path+gen_or_std);
    }
    generation.setArguments(args);
    if((!multigen)&&(!input.isEmpty()))
        generation.setStandardInputFile(path+"testdata/"+get_filename_with_id(input,testcases[0]));
    if(!multigen)
        generation.setStandardOutputFile(path+"testdata/"+get_filename_with_id(output,testcases[0]));

    generation.setWorkingDirectory(path+"testdata/");
    generation.start();
    if(!generation.waitForStarted(500)) {
        generation.terminate();
        genLog="FAIL cannot start generator";
        return _fail;
    }
    if(!generation.waitForFinished(timeout*6/5)) {
        generation.terminate();
        genLog="time limit exceeded generation exceeded time limit";
        return _tle;
    }
    int ret=generation.exitCode();
    if(generation.exitStatus()==QProcess::ExitStatus::CrashExit) {
        genLog="FAIL generator crashed";
        return _fail;
    }
    else if(ret==0) {
        if(multigen) {
            for(int caseid:testcases) {
                if(!FileOp::rename(path+"testdata/"+QString::number(caseid),
                                  path+"testdata/"+get_filename_with_id(output,caseid),
                                  true)
                    ) {
                    genLog="FAIL file not found in case "+QString::number(caseid);
                    return _fail;
                }
            }
        }
        genLog="ok generation success";
        return _ok;
    }
    else {
        genLog=QString::fromLocal8Bit(generation.readAllStandardError());
        return _fail;
    }
}

static TResult ValidateOp(QObject* parent,
                         const QString& validator,
                         const QString& file,
                         const int& testcase,
                         const QString& probpath,
                         QString& valLog,
                         int timeout=1000) {
    QString path=probpath;
    QStringList args;
    if(!path.endsWith("/")) path.append("/");
    args<<"--testcase"<<QString::number(testcase);
    QProcess validation(parent);
    validation.setProgram(path+validator);
    validation.setArguments(args);
    validation.setStandardInputFile(path+"testdata/"+get_filename_with_id(file,testcase));
    validation.setWorkingDirectory(path+"testdata/");
    validation.start();
    if(!validation.waitForStarted(500)) {
        validation.terminate();
        valLog="FAIL cannot start validator";
        return _fail;
    }
    if(!validation.waitForFinished(timeout*6/5)) {
        validation.terminate();
        valLog="time limit exceeded validation exceeded time limit";
        return _tle;
    }
    int ret=validation.exitCode();
    if(validation.exitStatus()==QProcess::ExitStatus::CrashExit) {
        valLog="FAIL validator crashed";
        return _fail;
    }
    else if(ret==0) {
        valLog="ok validation success";
        return _ok;
    }
    else {
        valLog=QString::fromLocal8Bit(validation.readAllStandardError());
        return _fail;
    }
}

#endif // VAL_UTILS_H
