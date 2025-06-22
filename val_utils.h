#ifndef VAL_UTILS_H
#define VAL_UTILS_H

#include <QColor>
#include "ctsettings.h"

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
    if(!validation.waitForFinished(timeout)) {
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
