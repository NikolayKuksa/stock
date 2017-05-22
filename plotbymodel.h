#ifndef FINANCIALPLOT_H
#define FINANCIALPLOT_H

#include <QDialog>
#include <QVector>
#include <QHBoxLayout>
#include <QStandardItemModel>
#include <QString>
#include <algorithm>
#include <QStringList>

#include "qcustomplot.h"

class PlotByModel : public QDialog
{
    Q_OBJECT
private:
    QString inDateFormat;
    QString outDateFormat;
    QCustomPlot * customPlot;
    QStandardItemModel *dataModel;
    QVector<QColor> plotColors;
    QString yAxisLabel;
    int yScaleMaxPlus;
    int yScaleMinMinus;
    bool chain;
public:
    PlotByModel(QString inDateFormat, QString outDateFormat, QString yAxisLabel, QWidget *parent = 0);
    void setDataModel(QStandardItemModel *model);
    void makePlot();
    void setYscaleRange(int maxPlus,int minMinus);
    void setChain(bool);

    ~PlotByModel();
};

#endif // FINANCIALPLOT_H
