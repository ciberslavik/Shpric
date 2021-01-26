#ifndef QSTORAGE_H
#define QSTORAGE_H

#include <QObject>
#include <QJsonObject>
#include <QJsonDocument>
class QStorage : public QObject
{
    Q_OBJECT
public:
    explicit QStorage(QObject *parent = nullptr);

signals:

};

#endif // QSTORAGE_H
