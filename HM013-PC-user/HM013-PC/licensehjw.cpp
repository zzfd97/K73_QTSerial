#include "licensehjw.h"



Licensehjw::Licensehjw()
{
    QSettings settings("HKEY_CURRENT_USER\\Software\\aaa",QSettings::NativeFormat);
    QString value = settings.value("1more", "error").toString();//读
    int im=value.toInt()+1;
    QVariant i(QString::number(im));
    settings.setValue("1more", i);//写
    str=value;
}

QString Licensehjw::toXOREncryptUncrypt(QString src, const QChar key)
{
    for(int i = 0; i < src.count(); i++) {
        src[i] = src.at(i).toLatin1() ^ key.toLatin1()+15;
    }
    return src;
}
QString Licensehjw::getValiedDate(void)
{
    return str;//读
}
QString Licensehjw::getWMIC(const QString &cmd)
{
    //获取cpu名称：wmic cpu get Name
    //获取cpu核心数：wmic cpu get NumberOfCores
    //获取cpu线程数：wmic cpu get NumberOfLogicalProcessors
    //查询cpu序列号：wmic cpu get processorid
    //查询主板序列号：wmic baseboard get serialnumber
    //查询BIOS序列号：wmic bios get serialnumber
    //查看硬盘：wmic diskdrive get serialnumber
    QProcess p;
    p.start(cmd);
    p.waitForFinished();
    QString result = QString::fromLocal8Bit(p.readAllStandardOutput());
    QStringList list = cmd.split(" ");
    result = result.remove(list.last(), Qt::CaseInsensitive);
    result = result.replace("\r", "");
    result = result.replace("\n", "");
    result = result.simplified();
    return result;
}

QString Licensehjw::getCpuName(void)
{
    return getWMIC("wmic cpu get name");
}

QString Licensehjw::getCpuId()
{
    return getWMIC("wmic cpu get processorid");
}

QString Licensehjw::getDiskNum()
{
    return getWMIC("wmic diskdrive where index=0 get serialnumber");
}
