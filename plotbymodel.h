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
public:
    PlotByModel(QString inDateFormat, QString outDateFormat, QString yAxisLabel, QWidget *parent = 0);
    void setDataModel(QStandardItemModel *model);
    void makePlot();

    ~PlotByModel();
};

#endif // FINANCIALPLOT_H
