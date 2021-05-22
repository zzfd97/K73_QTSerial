#ifndef JWSERIALTHREAD_H
#define JWSERIALTHREAD_H

#include <QObject>
#include "jwserial.h"
#include <QStringList>
#include <QThread>
#include <QObject>
#include <QThread>
#include <QWaitCondition>
#include <QMutex>
#include <QDebug>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QRunnable>

class jwSerialThread:public QObject
{
    Q_OBJECT
public:
    explicit jwSerialThread();
    jwSerialThread(QSerialPort *com ,QObject *parant);
    jwSerialThread(QString port,QString baud);
    bool jwSerialPort(QString port,QString baud);
    bool jwSerialPortOpen();
    bool findSeialDeviceInfo(QStringList &str);
    void serialThreadStart(void);
    void run();
    void jwSerialThreadCreate();
    bool jwSerialisOpen(void);
signals:
    void sendResulttoGUI(QByteArray Data);
 public slots:

    void doDataSend(const QByteArray data);
    void doDataReceive(void);//耗时动作

    void jwSerialConfig(QString portname);
 private:
    QSerialPort *mserila;
    QObject *mParent;
    QString serialport ;
    int baud;
    int runnaleid;
};

#endif // JWSERIALTHREAD_H
