#include "mainwindow.h"
#include "importviewdatawindow.h"
#include "recalgwindow.h"
#include "portfoliooptimisationwindow.h"

#include <QApplication>
#include <QDebug>
#include <QFileDialog>


#include "recalgcalc.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    PortfolioOptimisationWindow w;
    //MainWindow w;
    //ImportViewDataWindow w;
    w.show();

    return a.exec();

    /*
    Q_ASSERT(cond), where cond is a boolean expression, writes the warning "ASSERT: 'cond' in file xyz.cpp, line 234" and exits if cond is false.
    Q_ASSERT_X(cond, where, what), where cond is a boolean expression, where a location, and what a message, writes the warning: "ASSERT failure in where: 'what', file xyz.cpp, line 234" and exits if cond is false.
    Q_CHECK_PTR(ptr), where ptr is a pointer. Writes the warning "In file xyz.cpp, line 234: Out of memory" and exits if ptr is 0.*/
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

    rc.setRange(40,100);
    qDebug()<<"a=40 b=100";
    qDebug()<<1-rc.calcPa(100,5);
    qDebug()<<1-rc.calcPa(100,10);
    qDebug()<<1-rc.calcPa(100,15);
    qDebug()<<1-rc.calcPa(100,20);
   */


}
