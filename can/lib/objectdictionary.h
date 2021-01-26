#ifndef OBJECTDICTIONARY_H
#define OBJECTDICTIONARY_H

#include <QObject>

class ObjectDictionary : public QObject
{
    Q_OBJECT
public:
    explicit ObjectDictionary(QObject *parent = nullptr);

signals:

};

#endif // OBJECTDICTIONARY_H
