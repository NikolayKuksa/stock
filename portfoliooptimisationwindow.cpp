#include "portfoliooptimisationwindow.h"

PortfolioOptimisationWindow::PortfolioOptimisationWindow(QWidget *parent) : QMainWindow(parent)
{
    calculator=new FinCalculator(this);
    recAlgCalculator=new RecAlgCalc(this);
    inDateFormat="yyyy-mm-dd";
    numberStatesInChain=10;

    //dataFile="International Business Machines Corporation (IBM)  28.03.2015-06.05.2017.csv";
    dataFile="devtest.txt";

    calcPiXi();

    QStandardItemModel *chainModel=cutModelChangeDirection(rawModel,QStringList("Date"),false);
    appendValuesToModel(chainModel,QString("Price/State"),discretePrices);
    qDebug()<<discretePrices.length();

    QString dateFormat=QString("yyyy-MM-dd");
    QString plotDateFormat=QString("d. MMMM\nyyyy");
    PlotByModel *chainPlot=new PlotByModel(dateFormat,plotDateFormat,"Markov Chain",this);

    chainPlot->setDataModel(chainModel);
    chainPlot->setChain(true);
    chainPlot->setWindowTitle(QString("Markov chain by real data"));
    chainPlot->show();
    chainPlot->makePlot();

    int t=5;//tEdit->value();
    double a,b;
    a=100;//aEdit->text().toDouble();
    b=104;//Edit->text().toDouble();
    rate=0.1;Xo=100;
    double roh=0.1;

    /*QVector<PortfolioParam> portfs=simulate(t,a,b,rate,roh);
    makePlots(portfs);

    QVector<PortfolioParam> paretoSolution=getParetoSet(portfs);
    qDebug()<<paretoSolution.length();
    qDebug()<<portfs.length();
    for(int i=0;i<paretoSolution.length();i++)
        qDebug()<<paretoSolution.at(i).ro;*/


}

void PortfolioOptimisationWindow::makePlots(QVector<PortfolioParam> portfs)
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

void PortfolioOptimisationWindow::calcPiXi()
{
    rawModel=new QStandardItemModel;
    int fetchedRowsCount=modelFromCSV(rawModel,dataFile);
    calculator->setModel(rawModel);
    calculator->recalculate(fetchedRowsCount);

    //calcDirection(rawModel,inDateFormat);
    QVector<double> prices=calculator->getPrices();
    //QVector<double> returns=calculator->getReturns();

    /*for(int i=0;i<fetchedRowsCount;i++)
        qDebug()<<"Price: "<<prices.at(i)<<"   Return:"<<returns.at(i);*/

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
    //this->Xo=prices.last();
    this->xi.clear();
    this->xi.push_back(this->ksi);

    double sum=0;
    for(int i=0;i<pi.length();i++)
        sum+=pi.at(i);
    qDebug()<<pi.length()<<"  "<<uniqueKsi.length()<<"  "<<sum;
}

QVector<PortfolioParam> PortfolioOptimisationWindow::simulate(int t,double a,double b,double rate,double roh)
{
    double ro=0,x0;
    QVector<double> roVec;
    while(ro<=1)
    {
        roVec.push_back(ro);
        ro+=roh;
    }
    QVector<double> curKsi(ksi.length());
    QVector<PortfolioParam> portfolios(roVec.length());
    double fixReturn;
    for(int i=0;i<roVec.length();i++)
    {
        fixReturn=rate*roVec.at(i);
        for(int j=0;j<ksi.length();j++)
            curKsi[j]=(1-roVec.at(i))*ksi.at(j);
        xi.clear();
        xi.push_back(curKsi);
        recAlgCalculator->setXiPi(xi,pi);
        recAlgCalculator->setRange(a-roVec.at(i)*Xo,b-roVec.at(i)*Xo);

        x0=Xo*(1-roVec.at(i));
        portfolios[i].ro=roVec.at(i);
        portfolios[i].Pa=recAlgCalculator->calcPa(x0,t);
        portfolios[i].Pb=recAlgCalculator->calcPb(x0,t);
        portfolios[i].E=mathE(curKsi,pi)+fixReturn;
        portfolios[i].D=mathD(curKsi,pi,portfolios[i].E-fixReturn);
        portfolios[i].toString();
    }

    return portfolios;
}
