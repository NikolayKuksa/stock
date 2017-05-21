#ifndef PORTFOLIOOPTIMISATIONWINDOW_H
#define PORTFOLIOOPTIMISATIONWINDOW_H

#include <QMainWindow>
#include <miscdef.h>
#include "fincalculator.h"
#include "recalgcalc.h"

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

    QVector<double> ksi;
    QVector<QVector<double> > xi;
    QVector<double> pi;
    QVector<double> discretePrices;
    double Xo;
    double rate;

    QVector<PortfolioParam> simulate(int t,double a,double b,double rate,double roh);

    void calcPiXi();
public:
    explicit PortfolioOptimisationWindow(QWidget *parent = 0);

signals:

public slots:
};

#endif // PORTFOLIOOPTIMISATIONWINDOW_H
