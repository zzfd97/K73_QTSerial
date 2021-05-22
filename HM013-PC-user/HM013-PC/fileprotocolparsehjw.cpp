#include "fileprotocolparsehjw.h"
#include <QFileDialog>
#include <QFileInfo>
#include <QMessageBox>
#include <QDateTime>
#include <QString>
#include <QTimer>
#include <QDir>
#include <QList>
#include <QFile>
#include "qlist.h"
#include "qdebug.h"

FileProtocolParsehjw::FileProtocolParsehjw()
{
    //musb=new USBHIDHJW;
}

FileProtocolParsehjw::~FileProtocolParsehjw()
{
    delete musb;
}

bool FileProtocolParsehjw::FileOpen(void)
{
    QString path=QFileDialog::getOpenFileName(NULL,QString::fromUtf8("选择文件"),"",QObject::tr("txt(*.txt)"));
    if(path=="")
    {
        //QMessageBox::information(this,QString::fromUtf8("提示"),QString::fromUtf8("选择文件失败无路径！"),"OK");
    }
    if(!path.isEmpty())
    {
        QFile *file = new QFile;
        file->setFileName(path);
        bool ok = file->open(QIODevice::ReadOnly|QIODevice::Text);
        if(ok)
        {
#if 0
            qDebug()<<"文件内容";
            QDataStream in(file);
            QTextStream is(file);
            while(!is.atEnd())
            {
                QString str=is.readLine();
                QStringList list=str.split('\t');
                QByteArray data=str.toUtf8();
                for(int i=0;i<list.size();i++){
                    QString s=list.at(i);
                    char*p=s.toUtf8().data();
                    qDebug()<<*p;
                }
            }
#else
            QByteArray arry;
            QByteArray arryline;
            char *ptr;
            while(file->atEnd()==false)
            {

                arryline=file->readLine();
                qDebug()<<"大小"<<arryline.size();
                arry+=arryline;
                //arryline=arryline.toHex();
                for(int i=0;i<arryline.size();i++){
                    ptr=arryline.data();
                    //qDebug()<<ptr[i];
                }
                //musb->USBWrite((unsigned char*)ptr,arryline.size());
                qDebug()<<"------\r\n 换行";
            }
            mByteArry=arry;
#endif
            file->close();
            delete file;
        }
        else
        {
            //QMessageBox::information(this,"错误信息","打开文件:" + file->errorString());
            return true;
        }
    }
    return false;
}
QByteArray FileProtocolParsehjw::FileRead(void)
{
    return  mByteArry;
}
