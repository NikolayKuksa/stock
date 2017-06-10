#include "portfoliooptimisationwindow.h"

PortfolioOptimisationWindow::PortfolioOptimisationWindow(QWidget *parent) : QMainWindow(parent)
{
    calculator=new FinCalculator(this);
    recAlgCalculator=new RecAlgCalc(this);
    rawModel=Q_NULLPTR;
    inDateFormat="yyyy-MM-dd";
    fetchedRowsCount=0;
    chainModel=Q_NULLPTR;
    chainPlot=Q_NULLPTR;
    prec=6;
    double maxInPrice=9999999;
    dataStatusErr="<font color='red'>No loaded market data</font>";
    dataStatusOk="<font color='green'>Data is loaded</font>";
    Pa="P_down";
    Pb="P_up";
    mathEtxt="Expected return";
    riskFreeTxt="Risk free share (x)";
    x_partTxt=" x ";
    deviationTxt="Semi-Deviation";
    this->setWindowTitle("BS-portfolio analysis");


    QLocale loc(QLocale::English, QLocale::UnitedStates);
    loc.setNumberOptions(QLocale::RejectGroupSeparator);
    doubleValidator=new QDoubleValidator(0,maxInPrice, prec, this);
    doubleValidator->setLocale(loc);

    QWidget *backgroundWidget=new QWidget(this);
    QHBoxLayout *backgroundLayout=new QHBoxLayout(backgroundWidget);
    backgroundWidget->setLayout(backgroundLayout);
    this->setCentralWidget(backgroundWidget);

    QGroupBox *mainPane=new QGroupBox("Optimization parameters",backgroundWidget);
    createMainPaneComponents(mainPane);

    QGroupBox *resultPane=new QGroupBox("Pareto-efficient boundary (selected by 4 criteria)",backgroundWidget);
    createResultPaneComponents(resultPane);

    QGroupBox *explainPane=new QGroupBox("Explain Calculation (all simulated portfolios)",backgroundWidget);
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
    QVBoxLayout *mainLayout=new QVBoxLayout();
    QFormLayout *mainFormLayout=new QFormLayout;
    mainLayout->addLayout(mainFormLayout);
    mainPane->setLayout(mainLayout);

    capitalEdit=new QLineEdit(mainPane);
    capitalEdit->setValidator(doubleValidator);
    mainFormLayout->addRow("Amount of investment:",capitalEdit);
    capitalEdit->setText("100.00");

    tEdit=new QSpinBox(mainPane);
    mainFormLayout->addRow("Investment period:",tEdit);
    tEdit->setValue(5);

    rateEdit=new QLineEdit(mainPane);
    mainFormLayout->addRow("Risk free rate (%):",rateEdit);
    rateEdit->setValidator(doubleValidator);
    rateEdit->setText("0.5");

    /*nEdit=new QSpinBox(mainPane);
    QLabel *lbl=new QLabel(mainPane);
    lbl->setText("Minimum number of "
                 "<br>states in Markov Chain:<\br>");
    mainLayout->addRow(lbl,nEdit);
    nEdit->setValue(10);*/

    aEdit=new QLineEdit(mainPane);
    aEdit->setValidator(doubleValidator);
    mainFormLayout->addRow("Lower bound:",aEdit);
    aEdit->setText("100");

    bEdit=new QLineEdit(mainPane);
    bEdit->setValidator(doubleValidator);
    mainFormLayout->addRow("Target bound:",bEdit);
    bEdit->setText("103");

    portfNumEdit=new QSpinBox(mainPane);
    mainFormLayout->addRow("Number of portfolios:",portfNumEdit);
    portfNumEdit->setMinimum(11);
    portfNumEdit->setMaximum(1001);
    portfNumEdit->setValue(11);

    QPushButton *loadDataButton=new QPushButton(tr("Load data"),mainPane);
    fileStatusLabel=new QLabel(dataStatusErr,mainPane);
    mainFormLayout->addRow(loadDataButton,fileStatusLabel);
    connect(loadDataButton,SIGNAL(clicked()),this,SLOT(loadButtonClicked()));

    QGroupBox *criteriaPane=new QGroupBox(tr("Criteria for optimisation"),mainPane);
    QVBoxLayout *criteriaLayout=new QVBoxLayout();
    criteriaPane->setLayout(criteriaLayout);
    ECheckBox=new QCheckBox(mathEtxt,criteriaPane);
    criteriaLayout->addWidget(ECheckBox);
    DCheckBox=new QCheckBox(deviationTxt,criteriaPane);
    criteriaLayout->addWidget(DCheckBox);
    PaCheckBox=new QCheckBox(Pa,criteriaPane);
    criteriaLayout->addWidget(PaCheckBox);
    PbCheckBox=new QCheckBox(Pb,criteriaPane);
    criteriaLayout->addWidget(PbCheckBox);
    criteriaPane->setMaximumHeight(150);
    mainLayout->addWidget(criteriaPane);

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

    QPushButton *PaPbPlotButton=new QPushButton(Pa+QString(", ")+Pb,explainButtonsPane);
    explainButtonsLayout->addWidget(PaPbPlotButton);
    connect(PaPbPlotButton,SIGNAL(clicked()),this,SLOT(PaPbPlotButtonClicked()));


    QPushButton *mathEPlotButton=new QPushButton(mathEtxt,explainButtonsPane);
    explainButtonsLayout->addWidget(mathEPlotButton);
    connect(mathEPlotButton,SIGNAL(clicked()),this,SLOT(mathEPlotButtonClicked()));

    QPushButton *mathDPlotButton=new QPushButton(deviationTxt,explainButtonsPane);
    explainButtonsLayout->addWidget(mathDPlotButton);
    connect(mathDPlotButton,SIGNAL(clicked()),this,SLOT(mathDPlotButtonClicked()));

    QPushButton *mathEDPlotButton=new QPushButton(mathEtxt+QString("-")+deviationTxt,explainButtonsPane);
    explainButtonsLayout->addWidget(mathEDPlotButton);
    connect(mathEDPlotButton,SIGNAL(clicked()),this,SLOT(mathEmathDPlotButtonClicked()));

    QPushButton *mathE_DspacePlotButton=new QPushButton("E-D space",explainButtonsPane);
    explainButtonsLayout->addWidget(mathE_DspacePlotButton);
    connect(mathE_DspacePlotButton,SIGNAL(clicked()),this,SLOT(mathE_DspacePlotButtonClicked()));

    QPushButton *Pa_PbSpacePlotButton=new QPushButton(Pa+QString("-")+Pb+QString(" space"),explainButtonsPane);
    explainButtonsLayout->addWidget(Pa_PbSpacePlotButton);
    connect(Pa_PbSpacePlotButton,SIGNAL(clicked()),this,SLOT(Pa_PbSpacePlotButtonClicked()));

    QPushButton *mathE_PaSpacePlotButton=new QPushButton(mathEtxt+QString("-")+Pa+QString(" space"),explainButtonsPane);
    explainButtonsLayout->addWidget(mathE_PaSpacePlotButton);
    connect(mathE_PaSpacePlotButton,SIGNAL(clicked()),this,SLOT(mathE_PaSpacePlotButtonClicked()));

    QPushButton *mathE_PbSpacePlotButton=new QPushButton(mathEtxt+QString("-")+Pb+QString(" space"),explainButtonsPane);
    explainButtonsLayout->addWidget(mathE_PbSpacePlotButton);
    connect(mathE_PbSpacePlotButton,SIGNAL(clicked()),this,SLOT(mathE_PbSpacePlotButtonClicked()));

    QPushButton *mathD_PaSpacePlotButton=new QPushButton(deviationTxt+QString("-")+Pa+QString(" space"),explainButtonsPane);
    explainButtonsLayout->addWidget(mathD_PaSpacePlotButton);
    connect(mathD_PaSpacePlotButton,SIGNAL(clicked()),this,SLOT(mathD_PaSpacePlotButtonClicked()));

    QPushButton *mathD_PbSpacePlotButton=new QPushButton(deviationTxt+QString("-")+Pb+QString(" space"),explainButtonsPane);
    explainButtonsLayout->addWidget(mathD_PbSpacePlotButton);
    connect(mathD_PbSpacePlotButton,SIGNAL(clicked()),this,SLOT(mathD_PbSpacePlotButtonClicked()));
}

void PortfolioOptimisationWindow::makePaPbPlots()
{
    QCustomPlot *PaPbPlot=new QCustomPlot();
    PaPbPlot->setLocale(QLocale(QLocale::English, QLocale::UnitedKingdom)); // period as decimal separator and comma as thousand separator
    PaPbPlot->legend->setVisible(true);
    PaPbPlot->xAxis->setLabel("Part of free risk investments");
    PaPbPlot->yAxis->setLabel("Probability");

    // add two new graphs and set their look:
    PaPbPlot->addGraph();
    PaPbPlot->graph(0)->setPen(QPen(Qt::blue)); // line color blue for first graph
    PaPbPlot->graph(0)->setName(Pa);

    PaPbPlot->addGraph();
    PaPbPlot->graph(1)->setName(Pb);
    PaPbPlot->graph(1)->setPen(QPen(Qt::red));

    PaPbPlot->graph(0)->setData(plotRos, plotPas,false);
    PaPbPlot->graph(1)->setData(plotRos, plotPbs,false);

    PaPbPlot->graph(0)->rescaleAxes();
    PaPbPlot->graph(1)->rescaleAxes(true);
    PaPbPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);

    Qt::WindowFlags flags = PaPbPlot->windowFlags();
    PaPbPlot->setWindowFlags(flags | Qt::WindowStaysOnTopHint);

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
    //qDebug()<<pi.length()<<"  "<<uniqueKsi.length()<<"  "<<sum;
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
        portfolios[i].D=roundPrec(x0*x0*mathSemiD(curKsi,pi,mathEksi));
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
        if(forward==calcDirection(rawModel,inDateFormat))
            rawModel=reverseRowsOrder(rawModel);
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
        rate=rateEdit->text().toDouble()/100.0;
        totalMoney=capitalEdit->text().toDouble();
        double roh=roundPrec(1.0/(portfNumEdit->value()-1.0));//0.01;

        portfs=simulate(t,a,b,rate,roh,totalMoney);

        int n=portfs.length();
        plotPas=QVector<double>(n);
        plotPbs=QVector<double>(n) ;
        plotRos=QVector<double>(n);
        plotEs=QVector<double>(n) ;
        plotDs=QVector<double>(n);

        for(int i=0;i<n;i++)
        {
            plotPas[i]=portfs.at(i).Pa;
            plotPbs[i]=portfs.at(i).Pb;
            plotEs[i]=portfs.at(i).E;
            plotRos[i]=portfs.at(i).ro;
            plotDs[i]=portfs.at(i).D;
        }

        int nn=4;
        QVector<bool> lx(nn);

        lx[0]=ECheckBox->isChecked();
        lx[1]=DCheckBox->isChecked();
        lx[2]=PaCheckBox->isChecked();
        lx[3]=PbCheckBox->isChecked();
        bool calcFlag=false;
        //qDebug()<<lx;
        for(int i=0;i<nn;i++)
            if(lx.at(i))
            {
                calcFlag=true;
                break;
            }

        QStringList headers;
        headers<<x_partTxt<<mathEtxt<<deviationTxt<<Pa<<Pb;

        if(calcFlag)
        {
            QVector<PortfolioParam> paretoSolution=getParetoSet(portfs,lx);
            //qDebug()<<paretoSolution.length();
            //qDebug()<<portfs.length();
            QString pareto;
            for(int i=0;i<paretoSolution.length();i++)
                pareto.append(QString::number(paretoSolution.at(i).ro)).append("  ");
            portfoliosParamToGrid(selectedPortfoliosTable,paretoSolution,headers);
        }
        portfoliosParamToGrid(allPortfoliosTable,portfs,headers);
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
    Qt::WindowFlags flags = chainPlot->windowFlags();
    chainPlot->setWindowFlags(flags | Qt::WindowStaysOnTopHint);
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
        table->setItem(i,headers.indexOf(x_partTxt),cell);

        cell=new QTableWidgetItem(QString::number(porfs.at(i).E));
        table->setItem(i,headers.indexOf(mathEtxt),cell);

        cell=new QTableWidgetItem(QString::number(porfs.at(i).D));
        table->setItem(i,headers.indexOf(deviationTxt),cell);

        cell=new QTableWidgetItem(QString::number(porfs.at(i).Pa));
        table->setItem(i,headers.indexOf(Pa),cell);

        cell=new QTableWidgetItem(QString::number(porfs.at(i).Pb));
        table->setItem(i,headers.indexOf(Pb),cell);
    }
}

void PortfolioOptimisationWindow::markovPlotButtonClicked()
{
    if(discretePrices.length()!=0)
        makeMarkovChainPlot();
}

void PortfolioOptimisationWindow::PaPbPlotButtonClicked()
{
    makePaPbPlots();
}

void PortfolioOptimisationWindow::mathEPlotButtonClicked()
{
    QString title(mathEtxt+QString(" of portfolio value"));
    makeParetoSpacePlot(plotRos,plotEs,riskFreeTxt,mathEtxt,title,"line");
}
void PortfolioOptimisationWindow::mathDPlotButtonClicked()
{
    QString title(deviationTxt+QString(" of portfolio value"));
    makeParetoSpacePlot(plotRos,plotDs,riskFreeTxt,deviationTxt,title,"line");
}

void PortfolioOptimisationWindow::mathEmathDPlotButtonClicked()
{
    QCustomPlot *EDPlot=new QCustomPlot();
    EDPlot->setLocale(QLocale(QLocale::English, QLocale::UnitedKingdom)); // period as decimal separator and comma as thousand separator
    EDPlot->legend->setVisible(true);
    EDPlot->xAxis->setLabel("Part of free risk investments");
    EDPlot->yAxis->setLabel("Return/Risk");

    // add two new graphs and set their look:
    EDPlot->addGraph();
    EDPlot->graph(0)->setPen(QPen(Qt::blue)); // line color blue for first graph
    EDPlot->graph(0)->setName(mathEtxt);

    EDPlot->addGraph();
    EDPlot->graph(1)->setName(deviationTxt);
    EDPlot->graph(1)->setPen(QPen(Qt::red));

    EDPlot->graph(0)->setData(plotRos, plotEs,false);
    EDPlot->graph(1)->setData(plotRos, plotDs,false);

    EDPlot->graph(0)->rescaleAxes();
    EDPlot->graph(1)->rescaleAxes(true);
    EDPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);

    Qt::WindowFlags flags = EDPlot->windowFlags();
    EDPlot->setWindowFlags(flags | Qt::WindowStaysOnTopHint);

    EDPlot->resize(300,300);
    EDPlot->setWindowTitle(QString("Expected return and Semi-Deviation (risk)"));
    EDPlot->show();

}

void PortfolioOptimisationWindow::mathE_DspacePlotButtonClicked()
{
    QString title(QString("Simulated portfolios in E-D space-criteria"));
    makeParetoSpacePlot(plotEs,plotDs,mathEtxt,deviationTxt,title);
}

void PortfolioOptimisationWindow::Pa_PbSpacePlotButtonClicked()
{
    QString title(QString("Simulated portfolios in ")+Pa+QString("-")+Pb+QString(" space-criteria"));
    makeParetoSpacePlot(plotPas,plotPbs,Pa,Pb,title);
}

void PortfolioOptimisationWindow::mathE_PaSpacePlotButtonClicked()
{
    QString title(QString("Simulated portfolios in ")+mathEtxt+QString("-")+Pa+QString(" space-criteria"));
    makeParetoSpacePlot(plotEs,plotPas,mathEtxt,Pa,title);
}

void PortfolioOptimisationWindow::mathE_PbSpacePlotButtonClicked()
{
    QString title(QString("Simulated portfolios in ")+mathEtxt+QString("-")+Pb+QString(" space-criteria"));
    makeParetoSpacePlot(plotEs,plotPbs,mathEtxt,Pb,title);
}

void PortfolioOptimisationWindow::mathD_PaSpacePlotButtonClicked()
{
    QString title(QString("Simulated portfolios in ")+deviationTxt+QString("-")+Pa+QString(" space-criteria"));
    makeParetoSpacePlot(plotDs,plotPas,deviationTxt,Pa,title);
}

void PortfolioOptimisationWindow::mathD_PbSpacePlotButtonClicked()
{
    QString title(QString("Simulated portfolios in ")+deviationTxt+QString("-")+Pb+QString(" space-criteria"));
    makeParetoSpacePlot(plotDs,plotPbs,deviationTxt,Pb,title);
}

void PortfolioOptimisationWindow::makeParetoSpacePlot(QVector<double> x,QVector<double> y, QString xName,QString yName,QString title,QString lineType)
{
    QCustomPlot *spacePaPb=new QCustomPlot();
    spacePaPb->setLocale(QLocale(QLocale::English, QLocale::UnitedKingdom)); // period as decimal separator and comma as thousand separator
    spacePaPb->xAxis->setLabel(xName);
    spacePaPb->yAxis->setLabel(yName);

    spacePaPb->addGraph();
    spacePaPb->graph()->setPen(QPen(Qt::red));
    //dot line type is default, when lineType isn't specified
    if(lineType=="dot")
    {
        spacePaPb->graph()->setLineStyle(QCPGraph::lsNone);
        spacePaPb->graph()->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, 2));
    }
    spacePaPb->graph()->setData(x, y,false);
    spacePaPb->graph()->rescaleAxes();
    spacePaPb->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);

    Qt::WindowFlags flags = spacePaPb->windowFlags();
    spacePaPb->setWindowFlags(flags | Qt::WindowStaysOnTopHint);
    spacePaPb->resize(300,300);
    spacePaPb->setWindowTitle(title);
    spacePaPb->show();


}
