#ifndef PORTFOLIOOPTIMISATIONWINDOW_H
#define PORTFOLIOOPTIMISATIONWINDOW_H

#include <QMainWindow>
#include <miscdef.h>
#include "fincalculator.h"
#include "recalgcalc.h"
#include "qcustomplot.h"
#include "plotbymodel.h"

#include <QSpinBox>
#include <QLineEdit>
#include <QPushButton>

class PortfolioOptimisationWindow : public QMainWindow
{
    Q_OBJECT
private:
    FinCalculator *calculator;
    RecAlgCalc *recAlgCalculator;
    QString dataFile,dataStatusErr,dataStatusOk;
    int fetchedRowsCount;
    QString inDateFormat;
    QStandardItemModel *rawModel;
    int prec;
    QDoubleValidator *doubleValidator;
    QLabel *fileStatusLabel;
    QTableWidget *selectedPortfoliosTable;
    QStandardItemModel *chainModel;
    PlotByModel *chainPlot;
    QTableWidget *allPortfoliosTable;

    void createMainPaneComponents(QGroupBox *parentPane);
    void createResultPaneComponents(QGroupBox *parentPane);
    void portfoliosParamToGrid(QTableWidget *table, QVector<PortfolioParam> porfs, QStringList headers);
    void makeMarkovChainPlot();
    void createExplainPaneComponents(QGroupBox *explainPane);

    QVector<double> ksi;
    QVector<QVector<double> > xi;
    QVector<double> pi;
    QVector<double> discretePrices;

    QSpinBox *tEdit;
    QSpinBox *nEdit;
    QSpinBox *rateEdit;

    QLineEdit *capitalEdit;
    QLineEdit *aEdit;
    QLineEdit *bEdit;
    QLineEdit *resEdit;

    QVector<PortfolioParam> simulate(int t, double a, double b, double rate, double roh, double initialInvestment);
    void calcPiXi(QStandardItemModel *rawModel,int numberStatesInChain);
    void makePaPbPlots(QVector<PortfolioParam> portfs);
    QVector<PortfolioParam> portfs;

private slots:
    void calcButtonClicked();
    void loadButtonClicked();
    void PaPbPlotButtonClicked();
    void markovPlotButtonClicked();
    void mathEPlotButtonClicked();
    void mathDPlotButtonClicked();
    void mathE_DspacePlotButtonClicked();
    void mathPa_PbSpacePlotButtonClicked();

public:
    explicit PortfolioOptimisationWindow(QWidget *parent = 0);

public slots:

signals:


};

#endif // PORTFOLIOOPTIMISATIONWINDOW_H
