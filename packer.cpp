#include "packer.h"

#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QByteArray>
#include <QDebug>

// 将单个文件转换为JSON对象

QJsonObject Packer::fileToJson(const QString &filePath) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "无法打开文件:" << filePath;
        return QJsonObject();
    }

    QByteArray fileData = file.readAll();
    file.close();

    QFileInfo fileInfo(filePath);

    QJsonObject fileObj;
    fileObj["type"] = "file";
    fileObj["name"] = fileInfo.fileName();
    fileObj["data"] = QString(fileData.toBase64());
    fileObj["size"] = fileInfo.size();
    fileObj["permissions"] = (int)file.permissions();

    return fileObj;
}

// 递归将文件夹转换为JSON对象
QJsonObject Packer::directoryToJson(const QString &dirPath) {
    QDir dir(dirPath);
    QFileInfo dirInfo(dirPath);

    QJsonObject dirObj;
    dirObj["type"] = "directory";
    dirObj["name"] = dirInfo.fileName();
    dirObj["permissions"] = (int)QFile(dirPath).permissions();

    QJsonArray contentsArray;

    foreach (const QFileInfo &item, dir.entryInfoList(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot)) {
        if (item.isDir()) {
            contentsArray.append(directoryToJson(item.absoluteFilePath()));
        } else {
            contentsArray.append(fileToJson(item.absoluteFilePath()));
        }
    }

    dirObj["contents"] = contentsArray;
    return dirObj;
}

// 生成包含文件/文件夹结构的JSON
QByteArray Packer::generateFileSystemJson(const QString &path) {
    QFileInfo pathInfo(path);
    QJsonObject rootObj;

    if (pathInfo.isDir()) {
        rootObj = directoryToJson(path);
    } else {
        rootObj = fileToJson(path);
    }

    QJsonDocument doc(rootObj);
    return doc.toJson(QJsonDocument::Indented);
}

// 修复后的 restoreFileSystemFromJson 函数
bool Packer::restoreFileSystemFromJson(const QByteArray &jsonData, const QString &destinationPath) {
    QJsonDocument doc = QJsonDocument::fromJson(jsonData);
    if (doc.isNull()) {
        qWarning() << "无效的JSON数据";
        return false;
    }

    QJsonObject rootObj = doc.object();

    // 确保目标路径存在
    QDir().mkpath(destinationPath);

    if (rootObj["type"].toString() == "file") {
        return createFileFromJson(rootObj, destinationPath);
    } else if (rootObj["type"].toString() == "directory") {
        return createDirectoryFromJson(rootObj, destinationPath);
    }

    qWarning() << "未知的JSON对象类型";
    return false;
}

// 改进的 createFileFromJson 函数
bool Packer::createFileFromJson(const QJsonObject &fileObj, const QString &basePath) {
    QString fileName = basePath + "/" + fileObj["name"].toString();
    QByteArray fileData = QByteArray::fromBase64(fileObj["data"].toString().toUtf8());

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly)) {
        qWarning() << "无法创建文件:" << fileName;
        return false;
    }

    file.write(fileData);
    file.close();

    // 设置文件权限
    if (fileObj.contains("permissions")) {
        file.setPermissions((QFile::Permissions)fileObj["permissions"].toInt());
    }

    return true;
}

// 改进的 createDirectoryFromJson 函数
bool Packer::createDirectoryFromJson(const QJsonObject &dirObj, const QString &basePath) {
    QString dirName = basePath + "/" + dirObj["name"].toString();
    QDir dir;

    if (!dir.mkpath(dirName)) {
        qWarning() << "无法创建目录:" << dirName;
        return false;
    }

    // 设置目录权限
    if (dirObj.contains("permissions")) {
        QFile dirFile(dirName);
        dirFile.setPermissions((QFile::Permissions)dirObj["permissions"].toInt());
    }

    QJsonArray contents = dirObj["contents"].toArray();  // 注意这里应该是"contents"而不是"answer"
    for (const QJsonValue &item : contents) {
        QJsonObject itemObj = item.toObject();
        if (itemObj["type"].toString() == "file") {
            if (!createFileFromJson(itemObj, dirName)) {
                return false;
            }
        } else if (itemObj["type"].toString() == "directory") {
            if (!createDirectoryFromJson(itemObj, dirName)) {
                return false;
            }
        }
    }

    return true;
}
