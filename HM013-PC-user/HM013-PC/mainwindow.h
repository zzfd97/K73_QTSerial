#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "appthreadhjw.h"
#include <QMainWindow>
#include <QPropertyAnimation>
#include "fileprotocolparsehjw.h"
#include <QFileDialog>
#include <QDir>
#include <QList>
#include "qlist.h"
#include "plothjw.h"
#include "jwserialthread.h"
#include <QThread>
#include <QTimer>
#include <QTime>
#include <jwplot/qcustomplot.h>
#include <QThreadPool>
#include <QThread>
#include <QSerialPort>
#include <QSerialPortInfo>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void CreateBlackImage(void);
    void mainGUISendData(void);
    void CreatePlot(void);
    void CreatWaveplot(void);
    QString ByteArrayToHexString(QByteArray data);
    QByteArray HexStringToByteArray(QString HexString);
signals:
    void serialDataSend(const QByteArray data);//通知串口接收数据
    void serialDataRead(void);
    //////////////////////////////////////////////////

public slots:
    void hanldeResults(QString &result);
    void on_serial_show(QByteArray data);//接收串口数据
    ///////////////////////////////////////////////////
private slots:
    void on_pushButton_2_clicked();

    void on_dial_valueChanged(int value);

    void on_pushButton_clicked();

    void timerUpdate(void);

    void LableTextShow(void);

    void on_widget_windowIconTextChanged(const QString &iconText);

    void ThreadTxt(int value);

    void on_dial_2_valueChanged(int value);

    void on_dial_3_valueChanged(int value);

    void on_dial_4_valueChanged(int value);

    void on_pushButton_8_clicked();

    bool find_SerialDevice(void);
    void on_comboBox_2_activated(const QString &arg1);

    void on_pushButton_7_clicked();

    void realtimeDataSlot();

    void on_tabWidget_currentChanged(int index);

    void on_comboBox_7_activated(const QString &arg1);

    void on_pushButton_9_clicked();

    void on_pushButton_10_clicked();

private:
    QTimer *timer;
    Ui::MainWindow *ui;
    QPropertyAnimation   *pAnimation;
    ///////////////////////////////
    Plothjw              *plot;
    FileProtocolParsehjw *m_file;

    QTimer*         dataTimer;
    /////////////////////////////////////////////
    QThreadPool     *pMainThread;

    QCustomPlot     *mCustombar;
    QString         serialportname;
    QStringList     mserialportList;

    QThread         mainThread;
    QThread         PlotThread;
    QSerialPort     mSerialPort;
    jwSerialThread  *m_serial;

    QByteArray       mByteSendData;
};
#endif // MAINWINDOW_H
