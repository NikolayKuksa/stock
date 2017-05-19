#ifndef RECALGCALC_H
#define RECALGCALC_H

#include <QDialog>
#include <QProgressBar>
#include <QHBoxLayout>

#include <miscdef.h>


class RecAlgCalc : public QDialog
{
    Q_OBJECT
public:
    explicit RecAlgCalc(QWidget *parent = 0);
    void setRange(double left,double right);
    double calcPa(double initialPrice, int t);
    double calcPb(double initialPrice, int t);
    double calcPab(double initialPrice,int t);
    void setXiPi(QVector<QVector<double> > xi,QVector<double> pi);
    void myShow();
signals:
    updateProgress(int);


public slots:

private:
    QWidget *mother;
    double a,b;
    QVector<double> pi;
    QVector<QVector<double> >ksi;
    int n,T;
    QMap<QPair<double,int>,double> CACHEfi_ab;
    QMap<QPair<double,int>,double> CACHEfi_ainf;

    QProgressBar *calcProgressBar;

    bool in(double val, double left, double right, bool bounderyIncluded=true);
    double fi_ab(double x, int t);
    double fi_ainf(double x, int t);
    double getKsi(int i,int t);

private slots:
    void updateProgressBar(int value);
};

#endif // RECALGCALC_H
