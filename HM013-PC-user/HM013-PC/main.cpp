#include "mainwindow.h"
#include <QCryptographicHash>
#include <QApplication>
#include "qdebug.h"
#include "qsettings.h"
#include <QFile>
#include <QString>

#include "qstring.h"
#include "hidapi.h"
#include <QThread>
#include <enterwindow.h>
#include <QtMultimedia/QAudioDeviceInfo>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMessageBox>
//#include "usbhidhjw.h"
#include "jwblue.h"

#define printf qDebug


int main(int argc, char *argv[])
{
    QApplication sdk(argc, argv);
    QFile file(":black.qss"); // 指定路径
        if(file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            sdk.setStyleSheet(file.readAll());
            file.close();
    }
    //MainWindow w;
    EnterWindow enterw;
    enterw.show();
    MainWindow w;
    w.show();
    w.setWindowTitle("K73_xiaomi");





    return sdk.exec();
}
