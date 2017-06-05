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
    //RecAlgWindow w;
    w.show();

    return a.exec();

    /*
    Q_ASSERT(cond), where cond is a boolean expression, writes the warning "ASSERT: 'cond' in file xyz.cpp, line 234" and exits if cond is false.
    Q_ASSERT_X(cond, where, what), where cond is a boolean expression, where a location, and what a message, writes the warning: "ASSERT failure in where: 'what', file xyz.cpp, line 234" and exits if cond is false.
    Q_CHECK_PTR(ptr), where ptr is a pointer. Writes the warning "In file xyz.cpp, line 234: Out of memory" and exits if ptr is 0.*/
    //PortfolioOptimisationWindow opw;
}
