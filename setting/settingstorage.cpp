#include "settingstorage.h"
SettingStorage *SettingStorage::_instance;
QMutex SettingStorage::_mutex;

SettingStorage::SettingStorage(QObject *parent) : QObject(parent)
{
    for(int i=0;i<3;i++)
        _lastPressets[i] = nullptr;

    _configDir = QDir(QDir::cleanPath(QCoreApplication::applicationDirPath()+QDir::separator()+"config"));
    if(!_configDir.exists())
    {
        QDir().mkdir(_configDir.path());
    }
    _pressetFileInfo.setFile(_configDir,"presset.json");
    LoadSettings();
}


void SettingStorage::removePressetConfig(QString pressetName)
{
    if(_pressets.contains(pressetName))
    {
        _pressets.remove(pressetName);
    }
}

PressetConfig *SettingStorage::getPressetConfig(QString pressetName)
{
    if(_pressets.contains(pressetName))
    {
        return _pressets[pressetName];
    }
    return nullptr;
}

PressetConfig *SettingStorage::getFirstPresset()
{
    if(_pressets.count()>0)
    {
        return _pressets.first();
    }
    return nullptr;
}

PressetConfig *SettingStorage::getFirsLogic(Logic logic)
{
    PressetConfig *presset = nullptr;
    if(_pressets.count()>0)
    {
        foreach(const QString &pressetName,_pressets.keys())
        {
            if(_pressets[pressetName]->Logic() == ((int)logic))
            {
                presset = _pressets[pressetName];
            }
        }
    }

    if(presset == nullptr)
    {
        presset = new PressetConfig();
        presset->setMass(0);
        presset->setTemp(0);
        presset->setPause(0);
        presset->setReverse(0);
        presset->setSwiperPulse(0);
        presset->setClipperPulse(0);
        presset->setFirstPortion(0);
        presset->setLogic(static_cast<int>(logic));
    }

    return presset;
}

PressetConfig *SettingStorage::getLastPresset(Logic logic)
{
    return _lastPressets[(int)logic];
}

void SettingStorage::setLastPresset(PressetConfig *lastPresset)
{
    _lastPressets[lastPresset->Logic()] = lastPresset;
    SaveSettings();
}

PressetConfig *SettingStorage::createPressetConfig(QString pressetName)
{

    if(!_pressets.contains(pressetName))
    {
        _pressets.insert(pressetName, new PressetConfig(this));
        _pressets[pressetName]->setPressetName(pressetName);
    }
    return _pressets[pressetName];
}

void SettingStorage::addPresset(PressetConfig *presset)
{
    addPresset(presset, presset->PressetName());
}

void SettingStorage::addPresset(PressetConfig *presset, QString name)
{
    if(!_pressets.contains(name))
    {
        _pressets.insert(name, presset->Copy());
    }
    else
    {
        _pressets[name] = presset;
    }
}

QList<QString> SettingStorage::getPressetsList()
{
    return _pressets.keys();
}

QList<PressetConfig *> SettingStorage::Pressets()
{
    return _pressets.values();
}



QJsonObject SettingStorage::Serialize(const QObject *obj)
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

void SettingStorage::Deserialize(QObject *clas, const QJsonObject &data)
{

    foreach(const QString & key, data.keys())
    {
        QJsonValue val = data[key];
        QVariant var = val.toVariant();
        clas->setProperty(key.toUtf8(),var);
    }
}

void SettingStorage::SaveSettings()
{
    //Save pressets.json
    QJsonObject pressets;
    QJsonArray array;
    foreach(const QString &pressetName, _pressets.keys())
    {
        QJsonObject obj = Serialize(_pressets[pressetName]);

        array.append(obj);
    }

    pressets["Pressets"] = array;

    QJsonDocument pressetsDoc(pressets);

    SaveJsonToFile(_pressetFileInfo.filePath(),pressetsDoc.toJson(QJsonDocument::Indented));

    for(int i=0;i<3;i++)
    {
        if(_lastPressets[i] != nullptr)
        {
            QJsonDocument lastPressetDoc(Serialize(_lastPressets[i]));
            SaveJsonToFile(_configDir.path()+QDir::separator()+"LastPresset" + QString::number(i) + ".json", lastPressetDoc.toJson(QJsonDocument::Indented));
        }
    }
    //Save modbus.json
    //QJsonDocument modbusDoc = QJsonDocument(Serialize(_modbus));

    //SaveJsonToFile(_modbusFileInfo.filePath(),modbusDoc.toJson(QJsonDocument::Indented));

    LoadSettings();
}

void SettingStorage::addConfig(AbstractConfig *config)
{
    if(!_configs.contains(config->ConfigName()))
    {
        _configs.insert(config->ConfigName(),config);
        config->Save();
    }
}

AbstractConfig *SettingStorage::getConfig(QString configName)
{
    if(_configs.contains(configName))
        return _configs[configName];
    else
        return nullptr;
}

void SettingStorage::LoadSettings()
{
    QJsonDocument pressetsDoc = QJsonDocument::fromJson(readJsonFromFile(_pressetFileInfo.filePath()));

    QJsonArray array = pressetsDoc.object()["Pressets"].toArray();

    foreach(const QJsonValue & obj, array)
    {
        PressetConfig *cfg = new PressetConfig();
        Deserialize(cfg,obj.toObject());
        _pressets.insert(cfg->PressetName(),cfg);
    }
    for(int i=0;i<3;i++)
    {
        QString lastPressetFile = _configDir.path()+QDir::separator()+"LastPresset" + QString::number(i) + ".json";
        if(QFile::exists(lastPressetFile))
        {
            QJsonDocument lastPressetDoc = QJsonDocument::fromJson(readJsonFromFile(lastPressetFile));
            //_lastPressets = new PressetConfig*[3];
            _lastPressets[i] = new PressetConfig(this);
            Deserialize(_lastPressets[i],lastPressetDoc.object());
        }
    }
    //Read modbus config
    // QJsonDocument modbusDoc = QJsonDocument::fromJson(readJsonFromFile(_modbusFileInfo.filePath()));

    //if(!_modbus)
    //     _modbus = new ModbusConfig(this);

    // Deserialize(_modbus,modbusDoc.object());
}

void SettingStorage::SaveJsonToFile(const QString &filePath, const QString &json)
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

QByteArray SettingStorage::readJsonFromFile(const QString &filePath)
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

