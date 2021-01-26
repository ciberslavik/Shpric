#ifndef FRAMEBASE_H
#define FRAMEBASE_H

#include <QWidget>

#include <setting/pressetconfig.h>

class FrameBase : public QWidget
{
    Q_OBJECT
public:
    explicit FrameBase(QWidget *parent = nullptr);

    void setPressetConfig(PressetConfig* config)
    {
        if(_pressetConfig!=nullptr)
        {
            if(!_pressetConfig->isEqual(config))
            {
                _pressetConfig = new PressetConfig(*config);
                onPressedConfigChanged();
            }
        }
        else
        {
            _pressetConfig = new PressetConfig(*config);
            onPressedConfigChanged();
        }
    }
    PressetConfig* getPresset(){return _pressetConfig;}

    virtual void setEnabled(bool enabled)=0;
    bool isEnabled(){return _isEnabled;}
signals:
    void PressetChanged();
protected slots:
    virtual void onPressedConfigChanged() = 0;
protected:
    PressetConfig* _pressetConfig;
    bool _isEnabled;
};

#endif // FRAMEBASE_H
