#ifndef CTSETTINGS_H
#define CTSETTINGS_H

#include <QFile>
#include <QDir>
#include <QFileInfo>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonValue>
#include <QJsonParseError>
#include <QDateTime>
#include <QtCore>
#include <QCryptographicHash>
#include <QDebug>
#include "Qaesencryption/qaesencryption.h"
#include <QByteArray>
#include "packer.h"

const QString keyStr="cbweuiverfuiwwryw8rew8df897wrwrqeftqwgqw78qwdq7qwtqw7etqw79dqwgd";//Key for Encryption, change to your need
//Class for Encryption
class Encryption {
public:
    Encryption() = delete;
    static QString encrypt_data(const QString& dec_data) {
        QByteArray key=QCryptographicHash::hash(keyStr.toLocal8Bit(),QCryptographicHash::Sha256);
        QAESEncryption encry(QAESEncryption::AES_256, QAESEncryption::ECB, QAESEncryption::PKCS7);
        QByteArray enc_data_ba = encry.encode(dec_data.toUtf8(), key);
        QString enc_data = QString::fromLatin1(enc_data_ba.toBase64());
        return enc_data;
    }
    static QString decrypt_data(const QString& enc_data) {
        QByteArray key=QCryptographicHash::hash(keyStr.toLocal8Bit(),QCryptographicHash::Sha256);
        QAESEncryption encry(QAESEncryption::AES_256, QAESEncryption::ECB, QAESEncryption::PKCS7);
        QByteArray dec_data_ba = encry.decode(QByteArray::fromBase64(enc_data.toLatin1()), key);
        QString dec_data = QString::fromLatin1(QAESEncryption::RemovePadding(dec_data_ba, QAESEncryption::PKCS7));
        return dec_data;
    }
    static QByteArray encrypt_bytearray(const QByteArray& dec_data) {
        QByteArray key=QCryptographicHash::hash(keyStr.toLocal8Bit(),QCryptographicHash::Sha256);
        QAESEncryption encry(QAESEncryption::AES_256, QAESEncryption::ECB, QAESEncryption::PKCS7);
        return encry.encode(dec_data, key);
    }
    static QByteArray decrypt_bytearray(const QByteArray& enc_data) {
        QByteArray key=QCryptographicHash::hash(keyStr.toLocal8Bit(),QCryptographicHash::Sha256);
        QAESEncryption encry(QAESEncryption::AES_256, QAESEncryption::ECB, QAESEncryption::PKCS7);
        return encry.decode(enc_data, key);
    }
};

//class for QString format validation
class StrVal {
public:
    StrVal() = delete;
    static bool isValidName(const QString& Str) {
        if(Str.isEmpty()) return false;
        for(int _index=0;_index<Str.size();_index++) {
            if(Str[_index].isLetterOrNumber()||Str[_index]=='_') continue;
            else return false;
        }
        return true;
    }
    static bool isValidIsoDate(const QString& Str) {
        return QDateTime::fromString(Str,Qt::DateFormat::ISODate).isValid();
    }
    static bool isValidUsername(const QString& Str) {
        if(Str.isEmpty()) return false;
        for(int _index=0;_index<Str.size();_index++) {
            if(Str[_index].isLetterOrNumber()||Str[_index]=='_'||Str[_index]=='.'||Str[_index]=='@') continue;
            else return false;
        }
        return true;
    }
    static bool isValidPwd(const QString& Str) {
        if(Str.isEmpty()) return true;
        for(int _index=0;_index<Str.size();_index++) {
            if(Str[_index].isLetterOrNumber()||Str[_index]=='_'||Str[_index]=='.'||Str[_index]=='@'||Str[_index]=='~'||Str[_index]=='%'||Str[_index]=='$'||Str[_index]=='#'||Str[_index]=='!'||Str[_index]=='*') continue;
            else return false;
        }
        return true;
    }
    static bool isValidFileName(const QString& Str) {
        if(Str.isEmpty()) return false;
        for(int _index=0;_index<Str.size();_index++) {
            if(Str[_index].isLetterOrNumber()||Str[_index]=='_'||Str[_index]=='.') continue;
            else return false;
        }
        return true;
    }
};

//class for folder operation(create,remove,rename and duplicate)
class FolderOp {
public:
    FolderOp() = delete;
    static void create(const QString& folderPath)
    {
        QDir dir(folderPath);
        if (!dir.exists())
        {
            dir.mkdir(folderPath);
        }
    }
    static bool copy(const QString& fromDir, const QString& toDir, bool coverFileIfExist)
    {
        if(fromDir==toDir) return true;
        QDir sourceDir(fromDir);
        QDir targetDir(toDir);

        if (!targetDir.exists())
        {
            if (!targetDir.mkdir(targetDir.absolutePath())) return false;
        }

        QFileInfoList fileInfoList = sourceDir.entryInfoList();
        foreach(QFileInfo fileInfo, fileInfoList)
        {
            if (fileInfo.fileName() == "." || fileInfo.fileName() == "..") continue;

            if (fileInfo.isDir())
            {
                if (!copy(fileInfo.filePath(),
                                targetDir.filePath(fileInfo.fileName()),
                                coverFileIfExist))
                    return false;
            }
            else
            {
                if (coverFileIfExist && targetDir.exists(fileInfo.fileName()))
                {
                    targetDir.remove(fileInfo.fileName());
                }
                if (!QFile::copy(fileInfo.filePath(), targetDir.filePath(fileInfo.fileName())))
                {
                    return false;
                }
            }
        }
        return true;
    }
    static bool rename(const QString& oldPath, const QString& newPath)
    {
        QDir dirOld(oldPath);
        return dirOld.rename(oldPath, newPath);
    }
    static bool remove(const QString& dirPath)
    {
        QDir dirItem(dirPath);
        return dirItem.removeRecursively();
    }
    static bool exists(const QString& dirPath)
    {
        QDir dirItem(dirPath);
        return dirItem.exists();
    }
};

//class for file operation
class FileOp {
public:
    FileOp() = delete;
    static bool copy(const QString& fromDir, const QString& toDir, bool coverFileIfExist)
    {
        QFileInfo fromFile(fromDir);
        QFileInfo targetFile(toDir);
        if(fromFile==targetFile) return true;
        if(!fromFile.isFile()) return false;
        if(targetFile.isFile()) {
            if(coverFileIfExist) {
                if(!QFile::remove(toDir)) {
                    return false;
                }
            }
            else
                return false;
        }
        return QFile::copy(fromDir,toDir);
    }
    static bool rename(const QString& fromDir, const QString& toDir, bool coverFileIfExist) {
        QFileInfo fromFile(fromDir);
        QFileInfo targetFile(toDir);
        if(fromFile==targetFile) return true;
        if(!fromFile.isFile()) return false;
        if(targetFile.isFile()) {
            if(coverFileIfExist) {
                if(!QFile::remove(toDir)) {
                    return false;
                }
            }
            else
                return false;
        }
        if(QFile::copy(fromDir,toDir)) {
            QFile::remove(fromDir);
            return true;
        } else return false;
    }
    static bool exists(const QString& dirPath) {
        QFileInfo fileItem(dirPath);
        return fileItem.isFile();
    }
    static bool remove(const QString& dirPath) {
        return QFile::remove(dirPath);
    }
    static bool write(const QString& dirPath,const QString fileContent) {
        QFile outf(dirPath);
        if(!outf.open(QFile::WriteOnly)) return false;
        else {
            QTextStream stream(&outf);
            stream<<fileContent;
        }
        outf.close();
        return true;
    }
    static QString read(const QString& dirPath) {
        QFile outf(dirPath);
        QString result;
        if(!outf.open(QFile::ReadOnly)) return result;
        else {
            QTextStream stream(&outf);
            result=stream.readAll();
        }
        outf.close();
        return result;
    }
};

/*
 * whenever you want to insert a snippet, write <snippet filename="filename"> in your file;
 * to fill in a template file, write all snippets in a QMap:name->snippet file content, and pass it in function "fill_in"
 */
class Codetpl {
public:
    Codetpl() =delete;
    static QVector<QString> get_snippets(const QString& templatefile) {
        QVector<QString> FileNames={};
        int pos1=0,pos2=0;
        while(pos2>=0&&pos2<templatefile.size()) {
            pos1=templatefile.indexOf("<snippet filename=\"",pos2);
            if(pos1==-1) break;
            pos2=templatefile.indexOf("\">",pos1);
            if(pos2==-1) pos2=templatefile.size();
            FileNames.push_back(templatefile.sliced(pos1+19,pos2-pos1-19));
        }
        return FileNames;
    }
    static bool is_valid(const QString& templatefile) {
        QVector<QString> FileNames=get_snippets(templatefile);
        if(FileNames.empty()) return false;
        for(int i=0;i<FileNames.size();i++) {
            if(!StrVal::isValidFileName(FileNames[i])) return false;
        }
        return true;
    }
    static QString fill_in(const QString& templatefile,const QMap<QString,QString>& snippetfiles) {// need testing
        QString filefill="";
        int pos1=0,pos2=0;
        while(pos2>=0&&pos2<templatefile.size()) {
            pos1=templatefile.indexOf("<snippet filename=\"",pos2);
            if(pos1==-1) {
                if(pos2==0) filefill+=templatefile;
                else filefill+=templatefile.last(templatefile.size()-pos2-2);
                break;
            }
            filefill+=templatefile.sliced(pos2,pos1-pos2);
            pos2=templatefile.indexOf("\">",pos1);
            if(pos2==-1) pos2=templatefile.size();
            QString filename=templatefile.sliced(pos1+19,pos2-pos1-19);
            if(snippetfiles.contains(filename)) {
                filefill+=snippetfiles[filename];
            } else {
                filefill+=templatefile.sliced(pos1,pos2-pos1+2);
            }
        }
        return filefill;
    }
};

static const QString get_filename_with_id(const QString& filename,int case_id) {
    if(!filename.contains(".")) return filename+QString::number(case_id);
    else {
        QString name=filename;
        name.insert(name.lastIndexOf("."),QString::number(case_id));
        return name;
    }
}

static QStringList parseCombinedArgString(const QString &program)
{
    QStringList args;
    QString tmp;
    int quoteCount = 0;
    bool inQuote = false;

    // handle quoting. tokens can be surrounded by double quotes
    // "hello world". three consecutive double quotes represent
    // the quote character itself.
    for (int i = 0; i < program.size(); ++i) {
        if (program.at(i) == QLatin1Char('"')) {
            ++quoteCount;
            if (quoteCount == 3) {
                // third consecutive quote
                quoteCount = 0;
                tmp += program.at(i);
            }
            continue;
        }
        if (quoteCount) {
            if (quoteCount == 1)
                inQuote = !inQuote;
            quoteCount = 0;
        }
        if (!inQuote && program.at(i).isSpace()) {
            if (!tmp.isEmpty()) {
                args += tmp;
                tmp.clear();
            }
        } else {
            tmp += program.at(i);
        }
    }
    if (!tmp.isEmpty())
        args += tmp;

    return args;
}

const QString ctinfo=".ctinfo";//suffix ctinfo
const QString probcontent="content.pdf";//problem content file
const QString templ=".tpl";//suffix tpl for template file
const QString stuinfo = ".stuinfo";

const QString ProbTypeName[6]={
    "custom",
    "traditional",
    "submit_answer",
    "fill_in",
    "interactive",
    "communicative"
};
const QMap<QString,int> ProbTypeID = {
    {"custom",0},
    {"traditional",1},
    {"submit_answer",2},
    {"fill_in",3},
    {"interactive",4},
    {"communicative",5}
};

class JudgeProcess {
public:
    QMap<QString,QString> folder;
    unsigned int pipecount;
    struct process{
        bool isInteractor;
        QString interpreter;
        QString proc_name;
        int stdin_id=-1,stdout_id=-1;//not connected:-1,file:-2,pipe:0-(pipecount-1)
        QString stdin_file,stdout_file;
        QStringList params;
    };
    QMap<QString,QString> file_collect;
    QList<process> execute_info;
    struct ChkCmd {
        QString checker;
        QString inputFile;
        QString outputFile;
        QString answerFile;
    };
    QList<ChkCmd> checkersCmds;

    QJsonObject JsonJuProcObj() {
        QJsonObject JuProcObj;
        QJsonArray pathArray;
        for(QString filename:folder.keys()) {
            pathArray.append(QJsonObject({{"filename",filename},{"folder",folder[filename]}}));
        }
        JuProcObj.insert("folder",pathArray);
        JuProcObj.insert("pipe_count",int(pipecount));
        QJsonArray collectArray;
        for(QString filename:file_collect.keys()) {
            pathArray.append(QJsonObject({{"filename",filename},{"folder",file_collect[filename]}}));
        }
        JuProcObj.insert("collect",collectArray);
        QJsonArray exeArray;
        for(process proc:execute_info) {
            QJsonArray paramArray;
            for(QString str:proc.params) paramArray.append(str);
            exeArray.append( QJsonObject({{"interactor",proc.isInteractor},{"interpreter",proc.interpreter},{"name",proc.proc_name},
                             {"stdin",proc.stdin_id},{"stdin_file",proc.stdin_file},
                             {"stdout",proc.stdout_id},{"stdout_file",proc.stdout_file},{"params",paramArray}}) );
        }
        JuProcObj.insert("execution",exeArray);
        QJsonArray chkArray;
        for(ChkCmd chk:checkersCmds) {
            chkArray.append(QJsonObject({{"checker",chk.checker},{"input",chk.inputFile},{"output",chk.outputFile},{"answer",chk.answerFile}}));
        }
        JuProcObj.insert("checker",chkArray);
        return JuProcObj;
    }
    int load_form_JsonObj(const QJsonObject& JuProcObj) {
        if(JuProcObj.contains("folder")) {
            QJsonArray pathArray=JuProcObj["folder"].toArray();
            for(int ind=0;ind<pathArray.size();ind++) {
                QJsonObject pathObj=pathArray[ind].toObject();
                folder[pathObj["filename"].toString()]=pathObj["folder"].toString();
            }
        }
        if(JuProcObj.contains("collect")) {
            QJsonArray colArray=JuProcObj["collect"].toArray();
            for(int ind=0;ind<colArray.size();ind++) {
                QJsonObject colObject=colArray[ind].toObject();
                file_collect[colObject["file"].toString()]=colObject["folder"].toString();
            }
        }
        pipecount=JuProcObj["pipe_count"].toInt(0);
        if(!JuProcObj.contains("execution")) return -1;
        QJsonArray exeArray=JuProcObj["execution"].toArray();
        execute_info.clear();
        for(int ind=0;ind<exeArray.size();ind++) {
            QJsonObject exeObj=exeArray[ind].toObject();
            QJsonArray paramArray=exeObj["params"].toArray();
            QStringList params;
            for(int ind2=0;ind2<paramArray.size();ind2++) params.append(paramArray[ind2].toString());
            execute_info.push_back({
                exeObj["interactor"].toBool(),
                exeObj["interpreter"].toString(),
                exeObj["name"].toString(),
                exeObj["stdin"].toInt(-1),
                exeObj["stdout"].toInt(-1),
                exeObj["stdin_file"].toString(),
                exeObj["stdout_file"].toString(),
                params
            });
        }
        if(!JuProcObj.contains("checker")) return -2;
        QJsonArray chkArray=JuProcObj["checker"].toArray();
        checkersCmds.clear();
        for(int ind=0;ind<chkArray.size();ind++) {
            QJsonObject chkObj=chkArray[ind].toObject();
            checkersCmds.push_back({
                chkObj["checker"].toString(),
                chkObj["input"].toString(),
                chkObj["output"].toString(),
                chkObj["answer"].toString()
            });
        }
        return 0;
    }
};

class Testdata {
public:
    int ncase;
    Testdata(const int& numcase=1):ncase(numcase) {}
    struct Subtask {
        int score=10;
        QList<int> cases;
    };
    QList<Subtask> subtasks={{0,{}}};//subtasks[0] denotes samples
    struct GenCmd {
        QString interpreter;
        QString genorstd;
        QStringList params;
        QString inputFile;
        bool multigen;
        QList<int> cases;
    };
    struct TDSetting {
        int index;
        QString filename;
        bool validation;
        QString validator;
        QList<GenCmd> generator;
    };
    QMap<QString,TDSetting> settings;

    QJsonObject JsonTDataObj() {
        QJsonObject TDataObj;
        TDataObj.insert("num_cases",ncase);
        QJsonArray SubtaskArray;
        for(Subtask subtsk:subtasks) {
            QJsonObject SubtaskObj;
            SubtaskObj.insert("score",subtsk.score);
            QJsonArray CasesArr;
            for(int caseid:subtsk.cases) CasesArr.append(caseid);
            SubtaskObj.insert("cases",CasesArr);
            SubtaskArray.append(SubtaskObj);
        }
        TDataObj.insert("subtasks",SubtaskArray);
        QJsonArray SettingArray;
        for(TDSetting setting:settings) {
            QJsonObject SettingObj;
            SettingObj.insert("index",setting.index);
            SettingObj.insert("filename",setting.filename);
            SettingObj.insert("validation",setting.validation);
            SettingObj.insert("validator",setting.validator);
            QJsonArray GenArray;
            for(GenCmd gen:setting.generator) {
                QJsonObject GenObj;
                GenObj.insert("interpreter",gen.interpreter);
                GenObj.insert("gen_or_std",gen.genorstd);
                GenObj.insert("input_file",gen.inputFile);
                GenObj.insert("multigen",gen.multigen);
                QJsonArray ParamArray;
                for(QString param:gen.params) ParamArray.append(param);
                GenObj.insert("params",ParamArray);
                QJsonArray CaseArray;
                for(int caseid:gen.cases) CaseArray.append(caseid);
                GenObj.insert("cases",CaseArray);
                GenArray.append(GenObj);
            }
            SettingObj.insert("generator",GenArray);
            SettingArray.append(SettingObj);
        }
        TDataObj.insert("settings",SettingArray);
        return TDataObj;
    }
    int load_form_JsonObj(const QJsonObject& TDataObj) {
        if(!TDataObj.contains("num_cases")) return -1;
        if(!TDataObj.contains("subtasks")) return -1;
        if(!TDataObj.contains("settings")) return -1;

        ncase=TDataObj["num_cases"].toInt(1);

        QJsonArray SubtaskArray=TDataObj["subtasks"].toArray();
        for(int subind=0;subind<SubtaskArray.count();subind++) {
            QJsonObject SubtaskObj=SubtaskArray[subind].toObject();
            Subtask subtsk;
            subtsk.score=SubtaskObj["score"].toInt(0);
            QJsonArray CaseArray=SubtaskObj["cases"].toArray();
            for(int caseind=0;caseind<CaseArray.count();caseind++) {
                subtsk.cases.append(CaseArray[caseind].toInt(1));
            }
            if(subind==0) subtasks[0].cases=subtsk.cases;
            else subtasks.append(subtsk);
        }

        QJsonArray SettingArray=TDataObj["settings"].toArray();
        for(int setind=0;setind<SettingArray.size();setind++) {
            QJsonObject SettingObj=SettingArray[setind].toObject();
            TDSetting tdset;
            tdset.index=SettingObj["index"].toInt(setind);
            tdset.filename=SettingObj["filename"].toString();
            tdset.validation=SettingObj["validation"].toBool();
            tdset.validator=SettingObj["validator"].toString();
            QJsonArray GenArray=SettingObj["generator"].toArray();
            for(int genind=0;genind<GenArray.count();genind++) {
                QJsonObject GenObj=GenArray[genind].toObject();
                GenCmd cmd;
                cmd.interpreter=GenObj["interpreter"].toString();
                cmd.genorstd=GenObj["gen_or_std"].toString();
                cmd.inputFile=GenObj["input_file"].toString();
                cmd.multigen=GenObj["multigen"].toBool();
                QJsonArray ParamArray=GenObj["params"].toArray();
                for(int pind=0;pind<ParamArray.count();pind++) {
                    cmd.params.append(ParamArray[pind].toString());
                }
                QJsonArray CaseArray=GenObj["cases"].toArray();
                for(int caseind=0;caseind<CaseArray.count();caseind++) {
                    cmd.cases.append(CaseArray[caseind].toInt(1));
                }
                tdset.generator.append(cmd);
            }
            settings[tdset.filename]=tdset;
        }
        return 0;
    }
};

class Problem {
public:
    Problem() {}
    Problem(const QString& ProbName) {name=ProbName;}
    Problem(const QJsonObject& ProblemObj) {loadJsonObj(ProblemObj);}
    QString name="new_problem";
    int time_limit_ms=1000;
    int mem_limit_MiB=256;
    enum class ProblemType {
        custom=0,
        traditional=1,
        submit_answer=2,
        fill_in=3,
        interactive=4,
        communicative=5
    } type = ProblemType::custom;

    struct Utility {
        QString filename;
        enum class FileCategory{
            resource=0,
            testdata=1,
            submission=2,
            builtin=3
        } category;
        enum class FileType{
            plain=0,
            code=1,
            templ=2,
            snippet=3
        } filetype;
    };

    struct CompileSetting {
        bool precompile;
        QString compiler;
        QStringList inputs;
        QString output;
        QString params;
    };

    QMap<QString,Utility> utils;
    QMap<QString,CompileSetting> cpl_settings;

    Testdata testdata;
    bool validationSuccess=false;
    bool student=false;
    JudgeProcess judge_proc;

    QJsonObject JsonProblemObj() {
        QJsonObject ProblemObj;
        ProblemObj.insert("name",name);
        ProblemObj.insert("time_limit_ms",time_limit_ms);
        ProblemObj.insert("mem_limit_MiB",mem_limit_MiB);
        ProblemObj.insert("type",ProbTypeName[(int) type]);
        QJsonArray Utilities;
        QVector<Utility> utilsvec(utils.values());
        QJsonArray CompileSettings;
        QVector<CompileSetting> cplvec(cpl_settings.values());
        for(int _index=0;_index<utilsvec.size();_index++) {
            QJsonObject util;
            util.insert("filename",utilsvec[_index].filename);
            util.insert("category",(int) utilsvec[_index].category);
            util.insert("type",(int) utilsvec[_index].filetype);
            Utilities.append(util);
        }
        if(!Utilities.isEmpty()) ProblemObj.insert("utils",Utilities);
        for(int _index=0;_index<cplvec.size();_index++) {
            QJsonObject cpls;
            cpls.insert("precompile",cplvec[_index].precompile);
            cpls.insert("compiler",cplvec[_index].compiler);
            cpls.insert("inputs",cplvec[_index].inputs.join('|'));
            cpls.insert("output",cplvec[_index].output);
            cpls.insert("params",cplvec[_index].params);
            CompileSettings.append(cpls);
        }
        if(!CompileSettings.isEmpty()) ProblemObj.insert("cpl_settings",CompileSettings);
        ProblemObj.insert("testdata",testdata.JsonTDataObj());
        if(validationSuccess) ProblemObj.insert("validated",true);
        ProblemObj.insert("judge",judge_proc.JsonJuProcObj());
        return ProblemObj;
    }
    int loadJsonObj(const QJsonObject& ProblemObj) {
        if(!ProblemObj.contains("name")) return -1;
        if(!ProblemObj.contains("time_limit_ms")) return -1;
        if(!ProblemObj.contains("mem_limit_MiB")) return -1;
        if(!ProblemObj.contains("type")) return -1;

        name=ProblemObj["name"].toString();
        time_limit_ms=ProblemObj["time_limit_ms"].toInt(1000);
        mem_limit_MiB=ProblemObj["mem_limit_MiB"].toInt(1000);
        type=ProblemType(ProbTypeID[ProblemObj["type"].toString()]);
        if(ProblemObj.contains("utils")) {
            QJsonArray Utilities=ProblemObj["utils"].toArray();
            for(int _index=0;_index<Utilities.count();_index++) {
                QJsonObject util=Utilities[_index].toObject();
                if(!util.contains("filename")) continue;
                if(!util.contains("category")) continue;
                if(!util.contains("type")) continue;
                QString filename=util["filename"].toString();
                Utility::FileCategory category=Utility::FileCategory(util["category"].toInt());
                Utility::FileType ftype=Utility::FileType(util["type"].toInt());

                utils[filename]={filename,category,ftype};
            }
        }

        if(ProblemObj.contains("cpl_settings")) {
            QJsonArray CompileSettings=ProblemObj["cpl_settings"].toArray();
            for(int _index=0;_index<CompileSettings.count();_index++) {
                QJsonObject cplset=CompileSettings[_index].toObject();
                if(!cplset.contains("precompile")) continue;
                if(!cplset.contains("compiler")) continue;
                if(!cplset.contains("inputs")) continue;
                if(!cplset.contains("output")) continue;
                if(!cplset.contains("params")) continue;
                bool precompile=cplset["precompile"].toBool();
                QString compiler=cplset["compiler"].toString("g++.exe");
                QStringList inputs=cplset["inputs"].toString().split("|");
                QString output=cplset["output"].toString();
                QString params=cplset["params"].toString();
                cpl_settings[output]={precompile,compiler,inputs,output,params};
            }
        }

        if(ProblemObj.contains("testdata")) testdata.load_form_JsonObj(ProblemObj["testdata"].toObject());

        if(ProblemObj.contains("validated")) validationSuccess=true;
        else validationSuccess=false;

        if(ProblemObj.contains("judge")) judge_proc.load_form_JsonObj(ProblemObj["judge"].toObject());
        return 0;
    }

    bool toStudentPack() {
        if(!validationSuccess) return false;
        testdata.settings.clear();
        for(auto it=utils.begin();it!=utils.end();) {
            if(it->filetype==Problem::Utility::FileType::code&&it->category==Problem::Utility::FileCategory::testdata) it=utils.erase(it);
            else it++;
        }
        for(auto it=cpl_settings.begin();it!=cpl_settings.end();) {
            bool invalid=false;
            for(const QString& src:it->inputs) if(!utils.contains(src)) invalid=true;
            if(invalid) it=cpl_settings.erase(it);
            else it++;
        }
        student=true;
        return true;
    }
};

class Contest {
public:
    Contest() {}
    Contest(const QString ContestName) {name=ContestName;}
    Contest(const QJsonObject& ContestObj) {loadJsonObj(ContestObj);}
    QString name="new_contest";
    QDateTime start_time=QDateTime::currentDateTime(),
        end_time=QDateTime::currentDateTime().addSecs(7200);//contest time
    bool start_enabled=false,end_enabled=false;//contest time enabled?

    //participant data
    struct Participant {
        QString id="";
        QString pwd="";
    };
    QMap<QString,Participant> participants;//all data of participants are stored here(id of participants must be unique)
    //get csv formatted QString for participants
    QString get_pa_csv() {
        QString csvStr="";
        QVector <Participant> _palist(participants.values());
        for(int _index=0;_index<_palist.size();_index++)
            csvStr+=_palist[_index].id+","+_palist[_index].pwd+"\n";
        return csvStr;
    }
    //load csv formatted QString to participants
    int load_pa_csv(const QString& csvStr) {
        participants.clear();
        if(csvStr.isEmpty()) return 0;
        int error=0;
        QVector<QString> _palist=csvStr.split("\n");
        for(int _index=0;_index<_palist.size();_index++) {
            if(_palist[_index].isEmpty()) continue;
            QVector<QString> _painfo=_palist[_index].split(",");
            if(_painfo.size()<2) {
                error++;
                continue;
            }
            if(!StrVal::isValidName(_painfo[0])) {
                error++;
                continue;
            }
            if(!StrVal::isValidPwd(_painfo[1])) {
                error++;
                continue;
            }
            participants[_painfo[0]]=Participant{_painfo[0],_painfo[1]};
        }
        return error;
    }

    QMap<QString,Problem> problems;
    QJsonArray JsonProblemArray() {
        QVector<Problem> _prlist(problems.values());
        QJsonArray ProblemArray;
        for(int _index=0;_index<_prlist.size();_index++) {
            QJsonObject ProblemObject=_prlist[_index].JsonProblemObj();
            ProblemArray.append(ProblemObject);
        }
        return ProblemArray;
    }
    int loadProblemArray(const QJsonArray& ProblemArray) {
        problems.clear();
        for(int _index=0;_index<ProblemArray.count();_index++) {
            Problem curProb(ProblemArray[_index].toObject());
            if(!StrVal::isValidName(curProb.name)) return -1;
            if(problems.contains(curProb.name)) return -2;
            problems[curProb.name]=curProb;
        }
        return 0;
    }
    QJsonObject JsonContestObj() {
        QJsonObject ContestObj;
        ContestObj.insert("name",name);
        QJsonObject TimeSetObj;
        if(start_enabled)
            TimeSetObj.insert("start_time",start_time.toString(Qt::DateFormat::ISODate));
        else
            TimeSetObj.insert("start_time","disabled");
        if(end_enabled)
            TimeSetObj.insert("end_time",end_time.toString(Qt::DateFormat::ISODate));
        else
            TimeSetObj.insert("end_time","disabled");

        ContestObj.insert("time_setting",TimeSetObj);
        ContestObj.insert("participant",get_pa_csv());
        ContestObj.insert("problem",JsonProblemArray());
        return ContestObj;
    }
    int loadJsonObj(const QJsonObject& ContestObj) {
        if(!ContestObj.contains("name")) return -1;
        if(!ContestObj.contains("time_setting")) return -2;
        QJsonObject TimeSetObj=ContestObj["time_setting"].toObject();
        if(!TimeSetObj.contains("start_time")) return -2;
        if(!TimeSetObj.contains("end_time")) return -2;
        QString StartTimeStr=TimeSetObj["start_time"].toString();
        QString EndTimeStr=TimeSetObj["end_time"].toString();
        if(!ContestObj.contains("participant")) return -3;
        QString Participantcsv=ContestObj["participant"].toString();
        if(!ContestObj.contains("problem")) return -4;
        QJsonArray ProblemArray=ContestObj["problem"].toArray();

        name=ContestObj["name"].toString();
        if(!StrVal::isValidName(name)) return -4;

        if(StartTimeStr=="disabled") {
            start_enabled=false;
        } else {
            if(!StrVal::isValidIsoDate(StartTimeStr)) return -5;
            start_enabled=true;
            start_time=QDateTime::fromString(StartTimeStr,Qt::DateFormat::ISODate);
        }

        if(EndTimeStr=="disabled") {
            end_enabled=false;
        } else {
            if(!StrVal::isValidIsoDate(EndTimeStr)) return -5;
            end_enabled=true;
            end_time=QDateTime::fromString(EndTimeStr,Qt::DateFormat::ISODate);
        }

        load_pa_csv(Participantcsv);
        return loadProblemArray(ProblemArray);
    }
};
class Submission{
public:
    Submission(){}
    QString usrname="";//Student Basic info
    QString id="";
    QString pwd="";
    QMap<QString,QString> answers;
    QJsonObject SubmissionObj;//Final upload jsonfile
    void JsonSubmission(){
        QJsonObject submission;
        submission.insert("usrname",usrname);
        submission.insert("id",id);
        submission.insert("pwd",pwd);
        QJsonObject anss;
        for(auto it = answers.begin();it!=answers.end();it++){
            anss[it.key()] = it.value();
        }
        submission.insert("answer",anss);
        SubmissionObj = submission;
    }
    //应该能够一次上传一个contest的多个问题的答案
};



#endif // CTSETTINGS_H
