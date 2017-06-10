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
    void makeParetoSpacePlot(QVector<double> x, QVector<double> y, QString xName, QString yName, QString title, QString lineType=QString("dot"));
    void makePaPbPlots();

    QString Pa,Pb,mathEtxt,riskFreeTxt,x_partTxt,deviationTxt;
    QVector<double> ksi;
    QVector<QVector<double> > xi;
    QVector<double> pi;
    QVector<double> discretePrices;

    QSpinBox *tEdit;
    QSpinBox *nEdit;
    QSpinBox *nPortfsEdit;
    QSpinBox *portfNumEdit;
    
    QLineEdit *rateEdit;
    QLineEdit *capitalEdit;
    QLineEdit *aEdit;
    QLineEdit *bEdit;
    QLineEdit *resEdit;

    QCheckBox *ECheckBox;
    QCheckBox *DCheckBox;
    QCheckBox *PaCheckBox;
    QCheckBox *PbCheckBox;


    QVector<PortfolioParam> simulate(int t, double a, double b, double rate, double roh, double initialInvestment);
    void calcPiXi(QStandardItemModel *rawModel,int numberStatesInChain);
    QVector<PortfolioParam> portfs;

    QVector<double> plotPas;
    QVector<double> plotPbs;
    QVector<double> plotRos;
    QVector<double> plotEs;
    QVector<double> plotDs;

private slots:
    void calcButtonClicked();
    void loadButtonClicked();
    void PaPbPlotButtonClicked();
    void markovPlotButtonClicked();
    void mathEPlotButtonClicked();
    void mathDPlotButtonClicked();
    void mathE_DspacePlotButtonClicked();
    void Pa_PbSpacePlotButtonClicked();
    void mathE_PaSpacePlotButtonClicked();
    void mathE_PbSpacePlotButtonClicked();
    void mathD_PaSpacePlotButtonClicked();
    void mathD_PbSpacePlotButtonClicked();
    void mathEmathDPlotButtonClicked();


public:
    explicit PortfolioOptimisationWindow(QWidget *parent = 0);

public slots:

signals:


};

#endif // PORTFOLIOOPTIMISATIONWINDOW_H
