#ifndef PORTFOLIOOPTIMISATIONWINDOW_H
#define PORTFOLIOOPTIMISATIONWINDOW_H

#include <QMainWindow>
#include <miscdef.h>
#include "fincalculator.h"
#include "recalgcalc.h"
#include "qcustomplot.h"
#include "plotbymodel.h"

class PortfolioOptimisationWindow : public QMainWindow
{
    Q_OBJECT
private:
    FinCalculator *calculator;
    RecAlgCalc *recAlgCalculator;
    QString dataFile;
    int fetchedRowsCount;
    QString inDateFormat;
    int numberStatesInChain;
    QStandardItemModel *rawModel;

    QVector<double> ksi;
    QVector<QVector<double> > xi;
    QVector<double> pi;
    QVector<double> discretePrices;
    double Xo;
    double rate;

    QVector<PortfolioParam> simulate(int t,double a,double b,double rate,double roh);

    void calcPiXi();

    void makePlots(QVector<PortfolioParam> portfs);
public:
    explicit PortfolioOptimisationWindow(QWidget *parent = 0);

signals:

public slots:
};

#endif // PORTFOLIOOPTIMISATIONWINDOW_H
