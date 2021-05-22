#ifndef JWPLOT_H
#define JWPLOT_H

#include <QObject>
#include <QWidget>
#include <QTimer>
#include <QTime>
#include <jwPlot/qcustomplot.h>
//#include <jwPlot/jwplot.h>

class jwPlot:public QObject
{

    public:
    jwPlot(QWidget *mwight);
    void CreatePlot();
    void setupRealtimeDataDemo(QCustomPlot *customPlot);
    private:
    QCustomPlot *customPlot;

private slots:
  void realtimeDataSlot();
  void bracketDataSlot();
  void screenShot();
  void allScreenShots();

private:
  QTimer dataTimer;
  QCPItemTracer *itemDemoPhaseTracer;
  int currentDemoIndex;
  QWidget *mwight;
};


#endif // JWPLOT_H
