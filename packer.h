#ifndef PACKER_H
#define PACKER_H
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QByteArray>
#include <QDebug>
class Packer
{
public:
    //文件生成Json对象
    static QJsonObject fileToJson(const QString &filePath) ;

    // 递归将文件夹转换为JSON对象
    static QJsonObject directoryToJson(const QString &dirPath) ;
    // 生成包含文件/文件夹结构的JSON
    static QByteArray generateFileSystemJson(const QString &path) ;


    // 从JSON对象创建文件
    static bool createFileFromJson(const QJsonObject &fileObj, const QString &basePath) ;


    // 递归从JSON对象创建文件夹和文件
    static  bool createDirectoryFromJson(const QJsonObject &dirObj, const QString &basePath) ;
    // 从JSON数据重建文件系统;
    static bool restoreFileSystemFromJson(const QByteArray &jsonData, const QString &destinationPath) ;

    static QJsonObject findfile(QJsonObject dir);
};

#endif // PACKER_H
