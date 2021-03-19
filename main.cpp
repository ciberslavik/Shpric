#include "mainwindow.h"

#include <QApplication>
#include <QDateTime>
#include "setting/ioserialconfig.h"
#include "setting/settingstorage.h"
#include <setting/machineconfig.h>
#include <IO/iobase.h>
#include <IO/iofake.h>
#include <IO/iomodbus.h>
#include <can/StoberServo.h>
#include <can/fakeservo.h>
#include <QLoggingCategory>

// Умный указатель на файл логирования
QScopedPointer<QFile>   m_logFile;

// Объявляение обработчика
void messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg);

void InitConfig()
{


}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    InitConfig();

    MainWindow w;

    // Устанавливаем файл логирования,
    QDir logDir = QDir(QDir::cleanPath(QCoreApplication::applicationDirPath()+QDir::separator()+"log"));
    QFileInfo *logFileInfo = new QFileInfo(logDir.path()+QDir::separator()+"Machine.log");
    if(!logDir.exists())
    {
        logDir.mkpath(logDir.path());
    }
    if(logFileInfo->exists())
    {
        QFile::remove(logFileInfo->path());
    }
    m_logFile.reset(new QFile(logDir.path()+QDir::separator()+"Machine.log"));
    // Открываем файл логирования
    m_logFile.data()->open((QFile::Append | QFile::Text));
    // Устанавливаем обработчик
    qInstallMessageHandler(messageHandler);


    SerialConfig machineSerialConfig;
    machineSerialConfig.Load();
    MachineConfig machineConfig;
    machineConfig.Load();

    SettingStorage::instance()->addConfig(&machineSerialConfig);
    SettingStorage::instance()->addConfig(&machineConfig);

    //IO Module Init
    SerialConfig *ioConfig = static_cast<SerialConfig*>
            (SettingStorage::instance()->getConfig("IOSerialConfig"));

    IOModbus ioModule(ioConfig, &w);
    ioModule.Init(ioConfig->CycleTime());
    ioModule.Start();


    //    IOFake ioModule;

    //ioModule.FCLogicOn(true);
    //CAN Module init

//    StoberServo servo((IOBase*)&ioModule, &w);
//    servo.setSlaveId(1);
//    servo.Connect();

    //ioModule.FCLogicOn(true);
    //FakeServo servo(&ioModule, &w);

    //Machine Init

//    MachineConfig *mConfig = static_cast<MachineConfig*>
//            (SettingStorage::instance()->getConfig("MachineConfig"));
    //MachineController *controller = new MachineController(mConfig, (IOBase*)&ioModule, &servo);
   // machine.InitMachine(controller);

//    w.setMachine(&machine);
    w.showMaximized();
    return a.exec();
}
// Реализация обработчика
void messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    //    bool isNumber = false;
    //    msg.toInt(&isNumber,10);
    //    if(isNumber)
    //        return;

    // Открываем поток записи в файл
    if(m_logFile.data()->size() >= 1000000)
        m_logFile.data()->reset();

    QTextStream out(m_logFile.data());
    // Записываем дату записи
    // out << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz ");
    // По типу определяем, к какому уровню относится сообщение
    //    switch (type)
    //    {
    //    case QtInfoMsg:     out << "INF "; break;
    //    case QtDebugMsg:    out << "DBG "; break;
    //    case QtWarningMsg:  out << "WRN "; break;
    //    case QtCriticalMsg: out << "CRT "; break;
    //    case QtFatalMsg:    out << "FTL "; break;
    //    }
    // Записываем в вывод категорию сообщения и само сообщение
    if(msg.length() > 0)
        out << context.category << ": " << msg << endl;

    out.flush();    // Очищаем буферизированные данные
}
