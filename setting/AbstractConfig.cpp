#include "AbstractConfig.h"

#include <QCoreApplication>


void AbstractConfig::Save()
{
    QDir configDir = getConfigDir();
    QJsonDocument configDoc = QJsonDocument(Serialize(this));
    QFileInfo configFileInfo(configDir,ConfigName()+".json");
    SaveJsonToFile(configFileInfo.filePath(),configDoc.toJson(QJsonDocument::Indented));
}

void AbstractConfig::Load()
{
    QDir configDir = getConfigDir();
    QFileInfo configFileInfo(configDir,ConfigName()+".json");
    QJsonDocument configDoc = QJsonDocument::fromJson(readJsonFromFile(configFileInfo.filePath()));
    Deserialize(this, configDoc.object());
}

QJsonObject AbstractConfig::Serialize(const QObject *obj)
{
    QVariantMap map;

    for(int i=0; i<obj->metaObject()->propertyCount(); ++i)
    {
       if((obj->metaObject()->property(i).isStored())&&(strcmp(obj->metaObject()->property(i).name(), "objectName")))
       {
          map.insert(obj->metaObject()->property(i).name(),obj->metaObject()->property(i).read(obj));
       }
    }
    return QJsonDocument::fromVariant(map).object();
}

void AbstractConfig::Deserialize(QObject *clas, const QJsonObject &data)
{
    foreach(const QString & key, data.keys())
    {
        QJsonValue val = data[key];
        QVariant var = val.toVariant();
        clas->setProperty(key.toUtf8(),var);
    }
}

void AbstractConfig::SaveJsonToFile(const QString &filePath, const QString &json)
{
    QFileInfo jsonFileInfo(filePath);
    QDir::setCurrent(jsonFileInfo.path());
    QFile jsonFile(jsonFileInfo.filePath());
    if(jsonFile.open(QIODevice::WriteOnly))
    {
        jsonFile.write(json.toStdString().c_str());
    }
    jsonFile.close();
}

QByteArray AbstractConfig::readJsonFromFile(const QString &filePath)
{
    if(QFileInfo::exists(filePath))
    {
        QFileInfo jsonFileInfo(filePath);
        QDir::setCurrent(jsonFileInfo.path());
        QFile jsonFile(jsonFileInfo.filePath());
        if(jsonFile.open(QIODevice::ReadOnly))
        {
            return jsonFile.readAll();
        }
        jsonFile.close();
    }
    return QByteArray();
}

QDir AbstractConfig::getConfigDir()
{
    QDir configDir = QDir(QDir::cleanPath(QCoreApplication::applicationDirPath()+QDir::separator()+"config"));
    if(!configDir.exists())
    {
        QDir().mkdir(configDir.path());
    }
    return configDir;
}
