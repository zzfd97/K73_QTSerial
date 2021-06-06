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

#include "qcustomplot.h"
#include <QThreadPool>
#include <QThread>
#include <QSerialPort>
#include <QSerialPortInfo>
#include "jwserial.h"
#include "jwserialthread.h"
#include <callback.h>
#include <QBluetoothSocket>
#include <QBluetoothUuid>
#include <QBluetoothAddress>
#include <QIODevice>
#include <QBluetoothDeviceInfo>
#include <qbluetoothdevicediscoveryagent.h>
#include <QBluetoothLocalDevice>

#include <windows.h>
#include <WinUser.h>
#include <Dbt.h>
#include <devguid.h>
#include <SetupAPI.h>
#include <InitGuid.h>
#include <QMouseEvent>
#include <QByteArray>
#include <QDebug>
#include <QTimer>
#include <QMessageBox>
#include <QDateTime>



QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void CreateSerialportWindow(void);
    void CreateBlackImage(void);
    void mainGUISendData(void);
    void CreatePlot(void);
    void CreatWaveplot(void);
    bool find_SerialDevice(void);
    void bluetoothDataSend(QString str);
    void Serialport_SendData(void);
    void stringTohexString(QString& str, QString& hexStr);
    int hexStringToString(QString& hexStr, QString& str);
    int hexStringToHexArray(QString& hexStr, QByteArray& arr);
    int hexArrayToString(QByteArray& hexArr, QString& str);

signals:
    void serialDataSend(const QByteArray data);//通知串口接收数据
    void serialDataRead(void);
    //////////////////////////////////////////////////
    void sig_exit();
    void sig_deviceChanged(int flag);

public slots:
    void hanldeResults(QString &result);
    void on_serial_show(QByteArray data);//接收串口数据
    void readyRead();
    void on_deviceChanged(int flag);
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

    void on_pushButton_7_clicked();

    void realtimeDataSlot();

    void on_pushButton_9_clicked();

    void on_pushButton_10_clicked();

    //////////////////////////////////////////////////////////
    void discoverBlueTooth(QBluetoothDeviceInfo info);
    void discoveryFinished();
    //void scanFinished();
    void readBluetoothDataEvent();
    void bluetoothConnectedEvent();

    void serialport_receivedata();

    void on_chSendNewline_stateChanged(int arg1);

    void on_pushButton_11_clicked();

    void on_m_recvHex_clicked();

    void on_m_sendHex_clicked();

public :
    // 串口
    int m_pressMouse;
    QPoint m_startPos;
    QPoint m_windowPos;
    int m_recvHex;
    int m_sendHex;
    int m_sendTimer;
    int m_sendNewline;
    int m_sendTimerId;
    int m_rxCnt;
    int m_txCnt;
    // 状态栏相关


    void showMessage(const char* str);

    void on_btnOpen_clicked();

    void on_btnSaveRecv_clicked();

    void on_btnClearRecv_clicked();

    void on_ckRecvHex_stateChanged(int arg1);

    void on_ckTimestamp_stateChanged(int arg1);

    void on_btnSend_clicked();

    void on_btnClearSend_clicked();

    void on_chSendHex_stateChanged(int arg1);

    void on_ckSendTimer_stateChanged(int arg1);


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

    QByteArray       mByteSendData;
    QBluetoothLocalDevice *localDevice;
    QString BTaddress;		// 记录MAC地址
    QBluetoothSocket *socket;
};
#endif // MAINWINDOW_H
