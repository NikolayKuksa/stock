#include "mainwindow.h"
#include "portfoliooptimisationwindow.h"
#include <QApplication>
#include <QDebug>

#include "recalgcalc.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    //PortfolioOptimisationWindow opw;
    //opw.show();
/*
    QVector<double> ksi;
    ksi.push_back(0.2);
    ksi.push_back(-0.1);
    QVector<double> pi;
    pi.push_back(0.3);
    pi.push_back(0.7);
    RecAlgCalc rc(ksi,pi);
    /*
    rc.setRange(40,100);
    qDebug()<<"a=40 b=100";
    qDebug()<<1-rc.calcPa(100,5);
    qDebug()<<1-rc.calcPa(100,10);
    qDebug()<<1-rc.calcPa(100,15);
    qDebug()<<1-rc.calcPa(100,20);
    qDebug()<<"___";
    rc.setRange(60,100);
    qDebug()<<"a=60 b=100";
    qDebug()<<1-rc.calcPa(100,5);
    qDebug()<<1-rc.calcPa(100,10);
    qDebug()<<1-rc.calcPa(100,15);
    qDebug()<<1-rc.calcPa(100,20);
    qDebug()<<"___";
    rc.setRange(80,100);
    qDebug()<<"a=80 b=100";
    qDebug()<<1-rc.calcPa(100,5);
    qDebug()<<1-rc.calcPa(100,10);
    qDebug()<<1-rc.calcPa(100,15);
    qDebug()<<1-rc.calcPa(100,20);
    qDebug()<<"___";
    rc.setRange(100,100);
    qDebug()<<"a=100 b=100";
    qDebug()<<1-rc.calcPa(100,5);
    qDebug()<<1-rc.calcPa(100,10);
    qDebug()<<1-rc.calcPa(100,15);
    qDebug()<<1-rc.calcPa(100,20);
    qDebug()<<"___";
    rc.setRange(120,100);
    qDebug()<<"a=120 b=100";
    qDebug()<<1-rc.calcPa(100,5);
    qDebug()<<1-rc.calcPa(100,10);
    qDebug()<<1-rc.calcPa(100,15);
    qDebug()<<1-rc.calcPa(100,20);
    qDebug()<<"___";
*/
    /*
    qDebug()<<"___";
    rc.setRange(80,120);
    qDebug()<<"a=80 b=120";
    qDebug()<<rc.calcPab(100,5);
    qDebug()<<rc.calcPab(100,10);
    qDebug()<<rc.calcPab(100,15);
    qDebug()<<rc.calcPab(100,20);
    qDebug()<<"___";

    qDebug()<<"___";
    rc.setRange(80,140);
    qDebug()<<"a=80 b=140";
    qDebug()<<rc.calcPab(100,5);
    qDebug()<<rc.calcPab(100,10);
    qDebug()<<rc.calcPab(100,15);
    qDebug()<<rc.calcPab(100,20);
    qDebug()<<"___";

    qDebug()<<"___";
    rc.setRange(60,140);
    qDebug()<<"a=60 b=140";
    qDebug()<<rc.calcPab(100,5);
    qDebug()<<rc.calcPab(100,10);
    qDebug()<<rc.calcPab(100,15);
    qDebug()<<rc.calcPab(100,20);
    qDebug()<<"___";
    */
    /*
    rc.setRange(60,140);
    qDebug()<<"a=60 b=140";
    qDebug()<<rc.calcPb(100,5);
    qDebug()<<rc.calcPb(100,10);
    qDebug()<<rc.calcPb(100,15);
    qDebug()<<rc.calcPb(100,20);
    qDebug()<<"___";
    */
    /*QVector<double> ksi;
    ksi.push_back(0.1);
    ksi.push_back(-0.3);
    ksi.push_back(0.7);
    ksi.push_back(0.2);
    QVector<double> pi;
    pi.push_back(0.55);
    pi.push_back(0.25);
    pi.push_back(0.015);
    pi.push_back(0.185);

    qDebug()<<"E="<<mathE(ksi,pi);
    RecAlgCalc rc(ksi,pi);
    double a=100;
    double b=120;
    double x0=100;
    rc.setRange(a,b);
    qDebug()<<"Pa";
    qDebug()<<rc.calcPa(x0,5);
    qDebug()<<rc.calcPa(x0,10);
    qDebug()<<rc.calcPa(x0,15);
    qDebug()<<rc.calcPa(x0,20);
    qDebug()<<rc.calcPa(x0,25);
    qDebug()<<rc.calcPa(x0,30);

    qDebug()<<"Pb";
    qDebug()<<rc.calcPb(x0,5);
    qDebug()<<rc.calcPb(x0,10);
    qDebug()<<rc.calcPb(x0,15);
    qDebug()<<rc.calcPb(x0,20);
    qDebug()<<rc.calcPb(x0,25);
    qDebug()<<rc.calcPb(x0,30);*/

    return 0;
    //return a.exec();
}
