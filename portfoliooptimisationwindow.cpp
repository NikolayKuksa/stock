#include "portfoliooptimisationwindow.h"

PortfolioOptimisationWindow::PortfolioOptimisationWindow(QWidget *parent) : QMainWindow(parent)
{
    calculator=new FinCalculator(this);
    recAlgCalculator=new RecAlgCalc(this);
    inDateFormat="yyyy-mm-dd";
    numberStatesInChain=10;

    dataFile="International Business Machines Corporation (IBM)  28.03.2015-06.05.2017.csv";
    //dataFile="devtest.txt";

    calcPiXi();

    int t=5;//tEdit->value();
    double a,b;
    a=150;//aEdit->text().toDouble();
    b=160;//Edit->text().toDouble();
    rate=0.1;
    double roh=0.1;

    QVector<PortfolioParam> portfs= simulate(t,a,b,rate,roh);

}

void PortfolioOptimisationWindow::calcPiXi()
{
    QStandardItemModel *rawModel=new QStandardItemModel;
    int fetchedRowsCount=modelFromCSV(rawModel,dataFile);
    calculator->setModel(rawModel);
    calculator->recalculate(fetchedRowsCount);

    calcDirection(rawModel,inDateFormat);
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
    this->Xo=prices.last();
    this->xi.clear();
    this->xi.push_back(this->ksi);

    double sum=0;
    for(int i=0;i<pi.length();i++)
        sum+=pi.at(i);
    qDebug()<<pi.length()<<"  "<<uniqueKsi.length()<<"  "<<sum;
}

QVector<PortfolioParam> PortfolioOptimisationWindow::simulate(int t,double a,double b,double rate,double roh)
{
    double ro=0;
    QVector<double> roVec;
    while(ro<=1)
    {
        roVec.push_back(ro);
        ro+=roh;
    }
    QVector<double> curKsi(ksi.length());
    QVector<PortfolioParam> portfolios(roVec.length());
    double fixValue;
    for(int i=0;i<roVec.length();i++)
    {
        fixValue=rate*roVec.at(i);
        for(int j=0;j<ksi.length();j++)
            curKsi[j]=(1-roVec.at(i))*ksi.at(j);
        xi.clear();
        xi.push_back(curKsi);
        recAlgCalculator->setXiPi(xi,pi);
        recAlgCalculator->setRange(a,b);

        portfolios[i].ro=roVec.at(i);
        portfolios[i].Pa=recAlgCalculator->calcPa(Xo,t);
        portfolios[i].Pb=recAlgCalculator->calcPb(Xo,t);
        portfolios[i].E=mathE(curKsi,pi)+fixValue;
        portfolios[i].D=mathD(curKsi,pi,portfolios[i].E-fixValue);
        //portfolios[i].toString();
    }

    return portfolios;
}
