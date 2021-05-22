#include "jwserialthread.h"
#include "qdebug.h"


jwSerialThread::jwSerialThread()
{

}

jwSerialThread::jwSerialThread(QSerialPort *com ,QObject *parant):mserila(com),QObject(parant)
{
    mParent=parant;
}

jwSerialThread::jwSerialThread(QString port,QString baud)
{

}

bool jwSerialThread::jwSerialPortOpen(void)
{
#if 0
    if(!mserila->open(QIODevice::ReadWrite)){

    }else{

    }
    connect(mserila,SIGNAL(readRead()),this,SLOT(doDataReceive()));
#endif
}

bool jwSerialThread::jwSerialPort(QString port,QString baud)
{
    return false;
}
void jwSerialThread::jwSerialConfig(QString portname)
{

}

bool jwSerialThread::jwSerialisOpen(void)
{
    mserila->clear();
    mserila->close();
    return mserila->isOpen();
}

bool jwSerialThread::findSeialDeviceInfo(QStringList &str)
{
    QStringList list_avail_serialport;
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        //ui->portComboBox->addItem(qspinfo.portName());
        qDebug()<<"串口名:"<<info.portName();
        qDebug()<<"描述信息:"<<info.description();
        qDebug()<<"制造商:"<<info.manufacturer();
        QSerialPort serial;
        serial.setPort(info);
        if (serial.open(QIODevice::ReadWrite))
        {
        list_avail_serialport.append(serial.portName());
        serial.close();
        }

    }

    #if 0
    serialPort->setPortName(PortName);
    serialPort->setBaudRate(QSerialPort::Baud19200); //设置波特率
    serialPort->setParity(QSerialPort::NoParity);    //设置校验位
    serialPort->setDataBits(QSerialPort::Data8);     //设置数据位
    serialPort->setStopBits(QSerialPort::OneStop);   //设置停止位
    serialPort->open(QIODevice::ReadWrite);
    #endif
    qDebug()<<list_avail_serialport;
    str=list_avail_serialport;
    return false;
}

void jwSerialThread::doDataSend(const QByteArray data)
{
    mserila->write(data);
}

void jwSerialThread::jwSerialThreadCreate()
{

}
void jwSerialThread::serialThreadStart()
{
    mserila->setBaudRate(QSerialPort::Baud115200,QSerialPort::AllDirections);
    mserila->setDataBits(QSerialPort::Data8);
    mserila->setFlowControl(QSerialPort::NoFlowControl);
    mserila->setParity(QSerialPort::NoParity);
    mserila->setStopBits(QSerialPort::OneStop);
    //connect(mserila,SIGNAL(sendResulttoGUI()),this,SLOT(doDataReceive()));
}


void jwSerialThread::run()
{
    for(int i=0;i<10;i++){

        QThread::sleep(111);
    }
}

void jwSerialThread::doDataReceive()
{
   QString resultstr;
   if(mserila!=nullptr){
        QByteArray buffer=mserila->readAll();
        QString resultstr=buffer;
        emit sendResulttoGUI(buffer);
    }
    qDebug()<<resultstr<<"线程ID："<<QThread::currentThreadId();

}
