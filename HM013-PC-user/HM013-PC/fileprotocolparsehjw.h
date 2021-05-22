#ifndef FILEPROTOCOLPARSEHJW_H
#define FILEPROTOCOLPARSEHJW_H
#include <QDir>
#include <QList>
#include <QMainWindow>
#include "usbhidhjw.h"

class FileProtocolParsehjw: public QMainWindow
{

public:
    FileProtocolParsehjw();
    ~FileProtocolParsehjw();
    bool FileOpen(void);
    QByteArray FileRead(void);
private:
    QDir* dir;
    QFileInfoList      qlist;
    QFileInfo          qfileinfo;
    QByteArray         mByteArry;
    USBHIDHJW          *musb;
};

#endif // FILEPROTOCOLPARSEHJW_H
