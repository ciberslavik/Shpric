#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QGridLayout>
#include <QMainWindow>
#include "frames/standbyframe.h"
#include "machine.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void setMachine(Machine *machine);
signals:
    //void
private slots:
    void onEnableOperation();
    void onDisableOperation();
    void onSettingCalled();
    void onReturn();
    void onCycleDone();
private:
    Ui::MainWindow *ui;

    QMetaObject::Connection _enableOpConn;
    QMetaObject::Connection _configDoneConn;
    QMetaObject::Connection _configCalledConn;
    QWidget *_currentFrame;
    QWidget *_prevFrame;
    QGridLayout *_currentGrid;

    Machine *_machine;


    QWidget *_mainFrame;
    QWidget *_selectSettingFrame;
    QWidget *_stbyFrame;

    void createMainFrame();
    void createStbyFrame();
    void createSelectSettingFrame();

    void showMainFrame();
    void showStbyFrame();
    void showSelectSettingFrame();
    void setCurrentFrame(QWidget *frame);
};

#endif // MAINWINDOW_H
