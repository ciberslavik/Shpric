#ifndef CONFIGFRAMEBASE_H
#define CONFIGFRAMEBASE_H

#include <QWidget>

#include <setting/AbstractConfig.h>

class ConfigFrameBase : public QWidget
{
    Q_OBJECT

public:
    explicit ConfigFrameBase(AbstractConfig *config, QWidget *parent = nullptr);
    ~ConfigFrameBase();
protected slots:
    void SaveConfig();
signals:
    void Return();
protected:
    AbstractConfig *_curentConfig;


private:

};

#endif // CONFIGFRAMEBASE_H
