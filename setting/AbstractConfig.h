#ifndef ABSTRACTCONFIG_H
#define ABSTRACTCONFIG_H
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
class AbstractConfig : public QObject
{
    Q_OBJECT
public:
    virtual QString ConfigName() = 0;
    virtual AbstractConfig *CreateDefaultConfig() = 0;

    void Save();
    void Load();
    bool isDefault();
protected:
    AbstractConfig(QObject *parent = nullptr):QObject(parent) {}
    virtual ~AbstractConfig() {}

    QJsonObject Serialize(const QObject *obj);
    void Deserialize(QObject *clas, const QJsonObject &data);
    void SaveJsonToFile(const QString &filePath, const QString &json);
    QByteArray readJsonFromFile(const QString &filePath);
    QDir getConfigDir();
private:

};

#endif // ABSTRACTCONFIG_H
