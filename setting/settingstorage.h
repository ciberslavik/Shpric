#ifndef SETTINGSTORAGE_H
#define SETTINGSTORAGE_H

#include <QObject>
#include <QJsonDocument>
#include <QVariantMap>
#include <QMetaProperty>
#include <QMetaType>
#include <QJsonArray>
#include <QJsonValue>
#include <QJsonObject>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QCoreApplication>
#include <QMessageBox>
#include <QMutex>
//#include "modbusconfig.h"
#include "AbstractConfig.h"
#include "pressetconfig.h"
#include "../utils/LoggingCategories.h"

class SettingStorage : public QObject
{
    Q_OBJECT
private:
    static SettingStorage *_instance;
    static QMutex _mutex;
public:

    static SettingStorage *instance()
    {
        if (_instance == nullptr)
        {
            _mutex.lock();
            if (_instance == nullptr)
            {
                _instance = new SettingStorage();
            }
        }
        _mutex.unlock();
        return _instance;
    }
    SettingStorage(SettingStorage &other) = delete;
    void operator=(const SettingStorage &) = delete;

    void removePressetConfig(QString pressetName);
    PressetConfig *getPressetConfig(QString pressetName);
    PressetConfig *getFirstPresset();
    PressetConfig *getFirsLogic(Logic logic);
    PressetConfig *getLastPresset();
    void setLastPresset(PressetConfig *lastPresset);
    PressetConfig *createPressetConfig(QString pressetName);
    void addPresset(PressetConfig *presset);
    void addPresset(PressetConfig *presset, QString name);
    QList<QString> getPressetsList();
    QList<PressetConfig *> Pressets();
    void SaveSettings();

    void addConfig(AbstractConfig *config);
    AbstractConfig *getConfig(QString configName);
signals:
private:

    explicit SettingStorage(QObject *parent = nullptr);

    QJsonObject Serialize(const QObject *obj);
    void Deserialize(QObject *clas, const QJsonObject &data);

    void LoadSettings();

    void SaveJsonToFile(const QString &filePath,const QString &json);
    QByteArray readJsonFromFile(const QString &filePath);
    QDir _configDir;

    QMap<QString, AbstractConfig*> _configs;

    QFileInfo _pressetFileInfo;



    QMap<QString, PressetConfig*> _pressets;
    //ModbusConfig *_modbus = nullptr;
    PressetConfig *_lastPresset;
};

#endif // SETTINGSTORAGE_H
