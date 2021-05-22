#ifndef LICENSEHJW_H
#define LICENSEHJW_H

#include <QProcess>
#include <QObject>
#include "qsettings.h"
#include <QChar>
#include "qmutex.h"
#include "qfile.h"
#include "qtimer.h"
#include "qdatetime.h"
#include "qapplication.h"
#include "qmessagebox.h"
class Licensehjw
{
public:
    Licensehjw();
    QString getDiskNum();
    QString getCpuId();
    QString getCpuName(void);
    QString getWMIC(const QString &cmd);
    QString getValiedDate(void);
    QString toXOREncryptUncrypt(QString src, const QChar key);
    QString str;
};

#endif // LICENSEHJW_H
