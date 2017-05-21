#ifndef PORTFOLIOOPTIMISATIONWINDOW_H
#define PORTFOLIOOPTIMISATIONWINDOW_H

#include <QMainWindow>
#include <miscdef.h>
#include <fincalculator.h>

class PortfolioOptimisationWindow : public QMainWindow
{
    Q_OBJECT
private:
    FinCalculator *calculator;
    QString dataFile;
    int fetchedRowsCount;
    QString inDateFormat;
    int numberStatesInChain;

    void calcPiXi();
public:
    explicit PortfolioOptimisationWindow(QWidget *parent = 0);

signals:

public slots:
};

#endif // PORTFOLIOOPTIMISATIONWINDOW_H
