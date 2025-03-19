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
    static bool isValidPwd(const QString& Str) {
        if(Str.isEmpty()) return true;
        for(int _index=0;_index<Str.size();_index++) {
            if(Str[_index].isLetterOrNumber()||Str[_index]=='_') continue;
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
    static void rename(const QString& oldPath, const QString& newPath)
    {
        QDir dirOld(oldPath);
        dirOld.rename(oldPath, newPath);
    }
    static void remove(const QString& dirPath)
    {
        QDir dirItem(dirPath);
        dirItem.removeRecursively();
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
        if(fromFile==targetFile) return false;
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
    static bool exists(const QString& dirPath) {
        QFileInfo fileItem(dirPath);
        return fileItem.isFile();
    }
};

const QString ctinfo=".ctinfo";//suffix ctinfo
const QString probcontent="content.pdf";//problem content file

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

    QJsonObject JsonProblemObj() {
        QJsonObject ProblemObj;
        ProblemObj.insert("name",name);
        ProblemObj.insert("time_limit_ms",time_limit_ms);
        ProblemObj.insert("mem_limit_MiB",mem_limit_MiB);
        ProblemObj.insert("type",ProbTypeName[(int) type]);
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
        return 0;
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
        QString EndTimeStr=TimeSetObj["start_time"].toString();
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
            start_time=QDateTime::fromString(StartTimeStr,Qt::DateFormat::ISODate);
        }
        if(EndTimeStr=="disabled") {
            end_enabled=false;
        } else {
            if(!StrVal::isValidIsoDate(EndTimeStr)) return -5;
            end_time=QDateTime::fromString(EndTimeStr,Qt::DateFormat::ISODate);
        }
        load_pa_csv(Participantcsv);
        loadProblemArray(ProblemArray);
        return 0;
    }
};



#endif // CTSETTINGS_H
