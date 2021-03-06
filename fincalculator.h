#ifndef FINCALCULATOR_H
#define FINCALCULATOR_H

#include <QObject>
#include <QDebug>
#include <QString>
#include <QStandardItemModel>
#include <QVector>
#include <miscdef.h>

#include "miscdef.h"

class FinCalculator : public QObject
{
    Q_OBJECT
public:
    explicit FinCalculator(QObject *parent = 0);

    void setModel(QStandardItemModel *inModel);
    QStandardItemModel *getModel();
    QVector<double> getPrices();
    QVector<double> getReturns();

private:
    QStandardItemModel *rawModel;
    QStandardItemModel *outModel;

    QString inDateFormat;

    ModelDirection directionSetting;
    QVector<double> prices;
    QVector<double> returns;
    QVector<double> calcPrices();
    QVector<double> calcReturns();




signals:
    void recalculated(int rowCount);

public slots:
    void recalculate(int rowCount);
};

#endif // FINCALCULATOR_H
