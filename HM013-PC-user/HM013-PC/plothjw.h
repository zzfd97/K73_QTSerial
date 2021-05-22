#ifndef PLOTHJW_H
#define PLOTHJW_H

#include <QtWidgets/QWidget>

#include <QtMultimedia/QAudioDeviceInfo>
#include <QtMultimedia/QAudioInput>
#include <QtCharts/QChartGlobal>
#include <QtWidgets/QVBoxLayout>
#include <QtCharts/QLineSeries>
#include <QtCharts/QChartView>
#include <QtCharts/QValueAxis>
#include "xyseriesiodevice.h"
#include <QtCharts/QChart>
#include <QVBoxLayout>
#include <QAudioInput>

QT_CHARTS_BEGIN_NAMESPACE
class QLineSeries;
class QChart;
QT_CHARTS_END_NAMESPACE

QT_CHARTS_USE_NAMESPACE

class XYSeriesIODevice;

QT_BEGIN_NAMESPACE
class QAudioInput;
class QAudioDeviceInfo;
QT_END_NAMESPACE

class Plothjw
{
public:
    Plothjw();
    ~Plothjw();
    explicit Plothjw(const QAudioDeviceInfo &deviceInfo, QWidget *parent = nullptr);
private:
    XYSeriesIODevice *m_device = nullptr;
    QChart *m_chart;
    QLineSeries *m_series ;
    QAudioInput *m_audioInput = nullptr;
};

#endif // PLOTHJW_H
