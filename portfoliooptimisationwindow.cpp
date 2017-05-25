#include "portfoliooptimisationwindow.h"

PortfolioOptimisationWindow::PortfolioOptimisationWindow(QWidget *parent) : QMainWindow(parent)
{
    calculator=new FinCalculator(this);
    recAlgCalculator=new RecAlgCalc(this);
    rawModel=Q_NULLPTR;
    inDateFormat="yyyy-mm-dd";
    fetchedRowsCount=0;
    chainModel=Q_NULLPTR;
    chainPlot=Q_NULLPTR;
    prec=6;
    double maxInPrice=9999999;
    dataStatusErr="<font color='red'>No loaded market data</font>";
    dataStatusOk="<font color='green'>Data is loaded</font>";

    QLocale loc(QLocale::English, QLocale::UnitedStates);
    loc.setNumberOptions(QLocale::RejectGroupSeparator);
    doubleValidator=new QDoubleValidator(0,maxInPrice, prec, this);
    doubleValidator->setLocale(loc);

    QWidget *backgroundWidget=new QWidget(this);
    QHBoxLayout *backgroundLayout=new QHBoxLayout(backgroundWidget);
    backgroundWidget->setLayout(backgroundLayout);
    this->setCentralWidget(backgroundWidget);

    QGroupBox *mainPane=new QGroupBox("Data for optimisation",backgroundWidget);
    createMainPaneComponents(mainPane);

    QGroupBox *resultPane=new QGroupBox("Optimal portfolios",backgroundWidget);
    createResultPaneComponents(resultPane);

    QGroupBox *explainPane=new QGroupBox("Explain Calculation",backgroundWidget);
    createExplainPaneComponents(explainPane);

    QSplitter *splitter=new QSplitter(backgroundWidget);
    backgroundLayout->addWidget(splitter);
    splitter->addWidget(mainPane);
    splitter->addWidget(resultPane);
    splitter->addWidget(explainPane);
}

void PortfolioOptimisationWindow::createMainPaneComponents(QGroupBox *parent)
{
    QGroupBox *mainPane=parent;
    QFormLayout *mainLayout=new QFormLayout;
    mainPane->setLayout(mainLayout);

    capitalEdit=new QLineEdit(mainPane);
    capitalEdit->setValidator(doubleValidator);
    mainLayout->addRow("Amount of investment:",capitalEdit);
    capitalEdit->setText("100.00");

    tEdit=new QSpinBox(mainPane);
    mainLayout->addRow("Investment period:",tEdit);
    tEdit->setValue(5);

    rateEdit=new QSpinBox(mainPane);
    mainLayout->addRow("Risk free rate (%):",rateEdit);
    rateEdit->setValue(7);

    /*nEdit=new QSpinBox(mainPane);
    QLabel *lbl=new QLabel(mainPane);
    lbl->setText("Minimum number of "
                 "<br>states in Markov Chain:<\br>");
    mainLayout->addRow(lbl,nEdit);
    nEdit->setValue(10);*/

    aEdit=new QLineEdit(mainPane);
    aEdit->setValidator(doubleValidator);
    mainLayout->addRow("Left boudary (a):",aEdit);
    aEdit->setText("100");

    bEdit=new QLineEdit(mainPane);
    bEdit->setValidator(doubleValidator);
    mainLayout->addRow("Right boudary (b):",bEdit);
    bEdit->setText("104");

    QPushButton *loadDataButton=new QPushButton(tr("Load data"),mainPane);
    fileStatusLabel=new QLabel(dataStatusErr,mainPane);
    mainLayout->addRow(loadDataButton,fileStatusLabel);
    connect(loadDataButton,SIGNAL(clicked()),this,SLOT(loadButtonClicked()));

    QPushButton *calcButton=new QPushButton("Calculate",mainPane);
    mainLayout->addWidget(calcButton);
    connect(calcButton,SIGNAL(clicked()),this,SLOT(calcButtonClicked()));

}

void PortfolioOptimisationWindow::createResultPaneComponents(QGroupBox *resultPane)
{
    QHBoxLayout *resultLayout=new QHBoxLayout;
    resultPane->setLayout(resultLayout);
    selectedPortfoliosTable=new QTableWidget;
    resultLayout->addWidget(selectedPortfoliosTable);
}

void PortfolioOptimisationWindow::createExplainPaneComponents(QGroupBox *explainPane)
{
    QHBoxLayout *explainLayout=new QHBoxLayout;
    explainPane->setLayout(explainLayout);
    allPortfoliosTable=new QTableWidget;
    explainLayout->addWidget(allPortfoliosTable);

    QVBoxLayout *explainButtonsLayout=new QVBoxLayout;
    QGroupBox *explainButtonsPane=new QGroupBox("Creating useful plots",explainPane);
    explainButtonsPane->setLayout(explainButtonsLayout);
    explainLayout->addWidget(explainButtonsPane);

    QPushButton *markovPlotButton=new QPushButton("Markov Chain",explainButtonsPane);
    explainButtonsLayout->addWidget(markovPlotButton);
    connect(markovPlotButton,SIGNAL(clicked()),this,SLOT(markovPlotButtonClicked()));

    QPushButton *PaPbPlotButton=new QPushButton("Pr_a, Pr_b",explainButtonsPane);
    explainButtonsLayout->addWidget(PaPbPlotButton);
    connect(PaPbPlotButton,SIGNAL(clicked()),this,SLOT(PaPbPlotButtonClicked()));


    QPushButton *mathEPlotButton=new QPushButton("Expectation",explainButtonsPane);
    explainButtonsLayout->addWidget(mathEPlotButton);
    connect(mathEPlotButton,SIGNAL(clicked()),this,SLOT(mathEPlotButtonClicked()));

    QPushButton *mathDPlotButton=new QPushButton("Deviation",explainButtonsPane);
    explainButtonsLayout->addWidget(mathDPlotButton);
    connect(mathDPlotButton,SIGNAL(clicked()),this,SLOT(mathDPlotButtonClicked()));

    QPushButton *mathE_DspacePlotButton=new QPushButton("E-D space",explainButtonsPane);
    explainButtonsLayout->addWidget(mathE_DspacePlotButton);
    connect(mathE_DspacePlotButton,SIGNAL(clicked()),this,SLOT(mathE_DspacePlotButtonClicked()));

    QPushButton *mathPa_PbSpacePlotButton=new QPushButton("Pa-Pb space",explainButtonsPane);
    explainButtonsLayout->addWidget(mathPa_PbSpacePlotButton);
    connect(mathPa_PbSpacePlotButton,SIGNAL(clicked()),this,SLOT(mathPa_PbSpacePlotButtonClicked()));
}

void PortfolioOptimisationWindow::makePaPbPlots(QVector<PortfolioParam> portfs)
{
    int n=portfs.length();
    QVector<double> pas(n);
    QVector<double> pbs(n);
    QVector<double> ros(n);
    QVector<double> Es(n);
    QVector<double> Ds(n);
    for(int i=0;i<n;i++)
    {
        pas[i]=portfs.at(i).Pa;
        pbs[i]=portfs.at(i).Pb;
        Es[i]=portfs.at(i).E;
        ros[i]=portfs.at(i).ro;
        Ds[i]=portfs.at(i).D;
    }
    QCustomPlot *PaPbPlot=new QCustomPlot();
    PaPbPlot->setLocale(QLocale(QLocale::English, QLocale::UnitedKingdom)); // period as decimal separator and comma as thousand separator
    PaPbPlot->legend->setVisible(true);
    PaPbPlot->xAxis->setLabel("Part of free risk investments");
    PaPbPlot->yAxis->setLabel("Probability");

    // add two new graphs and set their look:
    PaPbPlot->addGraph();
    PaPbPlot->graph(0)->setPen(QPen(Qt::blue)); // line color blue for first graph
    PaPbPlot->graph(0)->setName("Pa");

    PaPbPlot->addGraph();
    PaPbPlot->graph(1)->setName("Pb");
    PaPbPlot->graph(1)->setPen(QPen(Qt::red));

    PaPbPlot->graph(0)->setData(ros, pas,false);
    PaPbPlot->graph(1)->setData(ros, pbs,false);

    PaPbPlot->graph(0)->rescaleAxes();
    PaPbPlot->graph(1)->rescaleAxes(true);
    PaPbPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);

    PaPbPlot->resize(300,300);
    PaPbPlot->setWindowTitle(QString("Probabilities Pa and Pb"));
    PaPbPlot->show();
}

void PortfolioOptimisationWindow::calcPiXi(QStandardItemModel *rawModel,int numberStatesInChain)
{
    calculator->setModel(rawModel);
    calculator->recalculate(fetchedRowsCount);

    QVector<double> prices=calculator->getPrices();

    double minPrice=*std::min_element(prices.constBegin(), prices.constEnd());
    double maxPrice=*std::max_element(prices.constBegin(), prices.constEnd());
    int delta=static_cast<int>(maxPrice-minPrice);
    //qDebug()<<"delta "<<delta;

    int N=std::min(delta,numberStatesInChain);

    QVector<int> states;
    int step=delta/N;
    //qDebug()<<delta<<"  "<<N<<"  "<<step;
    states.push_back(static_cast<int>(minPrice));
    int i=0;
    while(states[i]<=maxPrice)
    {
        if(step==0)
            break;
        states.push_back(states.at(i)+step);
        i++;
    }
    //qDebug()<<"states: "<<states;

    double price;
    QVector<double> intPrices(fetchedRowsCount);
    for(int priceix=0;priceix<fetchedRowsCount;priceix++)
    {
        price=prices.at(priceix);
        for(int i=0;i<states.length()-1;i++)
            if(price<states.at(i+1) && price>=states.at(i))
            {
                intPrices[priceix]=states.at(i);
                break;
            }
    }

    //qDebug()<<intPrices;

    QVector<double> ksiByChain(fetchedRowsCount);
    double curIntPrice;
    for(int i=0;i<fetchedRowsCount-1;i++)
    {
        curIntPrice=intPrices.at(i);
        ksiByChain[i]=(intPrices.at(i+1)-curIntPrice)/curIntPrice;
    }

    //qDebug()<<ksiByChain;

    QSet<double> ksiSet=ksiByChain.toList().toSet();
    QVector<double> uniqueKsi=ksiSet.toList().toVector();


    QVector<double> pi(uniqueKsi.length());
    for(int i=0;i<uniqueKsi.length();i++)
        pi[i]=ksiByChain.count(uniqueKsi.at(i))/static_cast<double>(fetchedRowsCount);

    //qDebug()<<"Xi: "<<uniqueKsi;
    //qDebug()<<"Pi: "<<pi;

    this->ksi=uniqueKsi;
    this->pi=pi;
    this->discretePrices=intPrices;
    //this->xi.clear();
    //this->xi.push_back(this->ksi);

    double sum=0;
    for(int i=0;i<pi.length();i++)
        sum+=pi.at(i);
    qDebug()<<pi.length()<<"  "<<uniqueKsi.length()<<"  "<<sum;
}

QVector<PortfolioParam> PortfolioOptimisationWindow::simulate(int t,double a,double b,double rate,double roh,double initialInvestment)
{
    double ro=0.0;
    QVector<double> roVec;
    while(ro<=1.000001)
    {
        roVec.push_back(ro);
        ro+=roh;
    }

    QVector<double> curKsi(ksi.length());
    QVector<PortfolioParam> portfolios(roVec.length());
    double x0;
    for(int i=0;i<roVec.length();i++)
    {
        for(int j=0;j<ksi.length();j++)
            curKsi[j]=(1-roVec.at(i))*ksi.at(j)+roVec.at(i)*rate;
        xi.clear(); //we need this as recAlgCalculator
        xi.push_back(curKsi);   //likes QVector<QVector<double> >
        recAlgCalculator->setXiPi(xi,pi);
        recAlgCalculator->setRange(a,b);
        x0=initialInvestment;
        portfolios[i].ro=roVec.at(i);
        portfolios[i].Pa=recAlgCalculator->calcPa(x0,t);
        portfolios[i].Pb=recAlgCalculator->calcPb(x0,t);
        double mathEksi=mathE(curKsi,pi);
        portfolios[i].E=(1+mathEksi)*initialInvestment;
        portfolios[i].D=mathD(curKsi,pi,mathEksi);
        portfolios[i].toString();
    }
    return portfolios;
}

void PortfolioOptimisationWindow::loadButtonClicked()
{
    dataFile = QFileDialog::getOpenFileName(this,QString("Open File"));
    if(!dataFile.isEmpty())
    {
        if(rawModel!=Q_NULLPTR)
            delete rawModel;
        rawModel=new QStandardItemModel;
        fetchedRowsCount=modelFromCSV(rawModel,dataFile);
        if(fetchedRowsCount!=0)
            fileStatusLabel->setText(dataStatusOk);
        else
            fileStatusLabel->setText(dataStatusErr);
    }
}

void PortfolioOptimisationWindow::calcButtonClicked()
{

    if(fetchedRowsCount!=0){
        calcPiXi(rawModel,10);

        int t=tEdit->value();
        double a,b,rate,totalMoney;
        a=aEdit->text().toDouble();
        b=bEdit->text().toDouble();
        rate=rateEdit->value()/100.0;
        totalMoney=capitalEdit->text().toDouble();
        double roh=0.01;

        portfs=simulate(t,a,b,rate,roh,totalMoney);

        QVector<PortfolioParam> paretoSolution=getParetoSet(portfs);
        qDebug()<<paretoSolution.length();
        qDebug()<<portfs.length();
        QString pareto;
        for(int i=0;i<paretoSolution.length();i++)
            pareto.append(QString::number(paretoSolution.at(i).ro)).append("  ");


        QStringList headers;
        headers<<"x-part"<<"Expectation"<<"Deviation"<<"Pr_a"<<"Pr_b";

        portfoliosParamToGrid(allPortfoliosTable,portfs,headers);
        portfoliosParamToGrid(selectedPortfoliosTable,paretoSolution,headers);

    }
}

void PortfolioOptimisationWindow::makeMarkovChainPlot()
{
    if(chainModel!=Q_NULLPTR)
        delete chainModel;
    chainModel=cutModelChangeDirection(rawModel,QStringList("Date"),false);
    appendValuesToModel(chainModel,QString("Price/State"),discretePrices);
    QString dateFormat=QString("yyyy-MM-dd");
    QString plotDateFormat=QString("d. MMMM\nyyyy");
    if(chainPlot!=Q_NULLPTR)
        delete chainPlot;
    chainPlot=new PlotByModel(dateFormat,plotDateFormat,"Markov Chain",this);
    chainPlot->setDataModel(chainModel);
    chainPlot->setChain(true);
    chainPlot->setWindowTitle(QString("Markov chain by real data"));
    chainPlot->show();
    chainPlot->makePlot();
}

void PortfolioOptimisationWindow::portfoliosParamToGrid(QTableWidget *table, QVector<PortfolioParam> porfs, QStringList headers)
{
    table->clear();
    while(table->rowCount())
        table->removeRow(0);
    table->setColumnCount(headers.length());
    table->setHorizontalHeaderLabels(headers);
    QTableWidgetItem *cell;
    for(int i=0;i<porfs.length();i++)
    {
        table->insertRow(i);

        cell=new QTableWidgetItem(QString::number(porfs.at(i).ro));
        table->setItem(i,headers.indexOf("x-part"),cell);

        cell=new QTableWidgetItem(QString::number(porfs.at(i).E));
        table->setItem(i,headers.indexOf("Expectation"),cell);

        cell=new QTableWidgetItem(QString::number(porfs.at(i).D));
        table->setItem(i,headers.indexOf("Deviation"),cell);

        cell=new QTableWidgetItem(QString::number(porfs.at(i).Pa));
        table->setItem(i,headers.indexOf("Pr_a"),cell);

        cell=new QTableWidgetItem(QString::number(porfs.at(i).Pb));
        table->setItem(i,headers.indexOf("Pr_b"),cell);
    }
}

void PortfolioOptimisationWindow::markovPlotButtonClicked()
{
    if(discretePrices.length()!=0)
        makeMarkovChainPlot();
}

void PortfolioOptimisationWindow::PaPbPlotButtonClicked()
{
    makePaPbPlots(portfs);
}

void PortfolioOptimisationWindow::mathEPlotButtonClicked()
{
    int n=portfs.length();
    QVector<double> pas(n);
    QVector<double> pbs(n);
    QVector<double> ros(n);
    QVector<double> Es(n);
    QVector<double> Ds(n);
    for(int i=0;i<n;i++)
    {
        pas[i]=portfs.at(i).Pa;
        pbs[i]=portfs.at(i).Pb;
        Es[i]=portfs.at(i).E;
        ros[i]=portfs.at(i).ro;
        Ds[i]=portfs.at(i).D;
    }
    QCustomPlot *mathEPlot=new QCustomPlot();
    mathEPlot->setLocale(QLocale(QLocale::English, QLocale::UnitedKingdom)); // period as decimal separator and comma as thousand separator
    mathEPlot->xAxis->setLabel("Part of assets invested in free risk investments");
    mathEPlot->yAxis->setLabel("Mathematical Expectation");

    mathEPlot->addGraph();
    mathEPlot->graph()->setPen(QPen(Qt::red));
    mathEPlot->graph()->setData(ros, Es,false);
    mathEPlot->graph()->rescaleAxes();
    mathEPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);

    mathEPlot->resize(300,300);
    mathEPlot->setWindowTitle(QString("Mathematical Expectation of portfolio value"));
    mathEPlot->show();
}
void PortfolioOptimisationWindow::mathDPlotButtonClicked()
{
    int n=portfs.length();
    QVector<double> pas(n);
    QVector<double> pbs(n);
    QVector<double> ros(n);
    QVector<double> Es(n);
    QVector<double> Ds(n);
    for(int i=0;i<n;i++)
    {
        pas[i]=portfs.at(i).Pa;
        pbs[i]=portfs.at(i).Pb;
        Es[i]=portfs.at(i).E;
        ros[i]=portfs.at(i).ro;
        Ds[i]=portfs.at(i).D;
    }
    QCustomPlot *mathDPlot=new QCustomPlot();
    mathDPlot->setLocale(QLocale(QLocale::English, QLocale::UnitedKingdom)); // period as decimal separator and comma as thousand separator
    mathDPlot->xAxis->setLabel("Part of assets invested in free risk investments");
    mathDPlot->yAxis->setLabel("Deviation");

    mathDPlot->addGraph();
    mathDPlot->graph()->setPen(QPen(Qt::red));
    mathDPlot->graph()->setData(ros, Ds,false);
    mathDPlot->graph()->rescaleAxes();
    mathDPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);

    mathDPlot->resize(300,300);
    mathDPlot->setWindowTitle(QString("Deviation of portfolio value"));
    mathDPlot->show();
}

void PortfolioOptimisationWindow::mathE_DspacePlotButtonClicked()
{
    int n=portfs.length();
    QVector<double> pas(n);
    QVector<double> pbs(n);
    QVector<double> ros(n);
    QVector<double> Es(n);
    QVector<double> Ds(n);
    for(int i=0;i<n;i++)
    {
        pas[i]=portfs.at(i).Pa;
        pbs[i]=portfs.at(i).Pb;
        Es[i]=portfs.at(i).E;
        ros[i]=portfs.at(i).ro;
        Ds[i]=portfs.at(i).D;
    }
    QCustomPlot *spaceED=new QCustomPlot();
    spaceED->setLocale(QLocale(QLocale::English, QLocale::UnitedKingdom)); // period as decimal separator and comma as thousand separator
    spaceED->xAxis->setLabel("Matematical Expectation");
    spaceED->yAxis->setLabel("Deviation");

    spaceED->addGraph();
    spaceED->graph()->setPen(QPen(Qt::red));
    spaceED->graph()->setLineStyle(QCPGraph::lsNone);
    spaceED->graph()->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, 2));
    spaceED->graph()->setData(Es, Ds,false);
    spaceED->graph()->rescaleAxes();
    spaceED->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);

    spaceED->resize(300,300);
    spaceED->setWindowTitle(QString("Simulated portfolios in E-D pace-criteria"));
    spaceED->show();
}

void PortfolioOptimisationWindow::mathPa_PbSpacePlotButtonClicked()
{
    int n=portfs.length();
    QVector<double> pas(n);
    QVector<double> pbs(n);
    QVector<double> ros(n);
    QVector<double> Es(n);
    QVector<double> Ds(n);
    for(int i=0;i<n;i++)
    {
        pas[i]=portfs.at(i).Pa;
        pbs[i]=portfs.at(i).Pb;
        Es[i]=portfs.at(i).E;
        ros[i]=portfs.at(i).ro;
        Ds[i]=portfs.at(i).D;
    }
    QCustomPlot *spacePaPb=new QCustomPlot();
    spacePaPb->setLocale(QLocale(QLocale::English, QLocale::UnitedKingdom)); // period as decimal separator and comma as thousand separator
    spacePaPb->xAxis->setLabel("Pa");
    spacePaPb->yAxis->setLabel("Pb");

    spacePaPb->addGraph();
    spacePaPb->graph()->setPen(QPen(Qt::red));
    spacePaPb->graph()->setLineStyle(QCPGraph::lsNone);
    spacePaPb->graph()->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, 2));
    spacePaPb->graph()->setData(pas, pbs,false);
    spacePaPb->graph()->rescaleAxes();
    spacePaPb->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);

    spacePaPb->resize(300,300);
    spacePaPb->setWindowTitle(QString("Simulated portfolios in Pa-Pb space-criteria"));
    spacePaPb->show();
}
