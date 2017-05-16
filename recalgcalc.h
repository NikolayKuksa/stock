#ifndef RECALGCALC_H
#define RECALGCALC_H

#include <QObject>
#include <miscdef.h>

class RecAlgCalc : public QObject
{
    Q_OBJECT
public:
    explicit RecAlgCalc(QVector<double> ksi, QVector<double> pi, QObject *parent = 0);
    void setRange(double left,double right);
    double calcPa(double initialPrice, int t);
    double calcPb(double initialPrice, int t);
    double calcPab(double initialPrice,int t);
signals:

public slots:

private:
    double a,b;
    QVector<double> pi;
    QVector<double> ksi;
    int n,T;
    QMap<QPair<double,int>,double> CACHEfi_ab;
    QMap<QPair<double,int>,double> CACHEfi_ainf;

    bool in(double val, double left, double right, bool bounderyIncluded=true);
    double fi_ab(double x, int t);
    double fi_ainf(double x, int t);
    double getKsi(int i,int t);

};

#endif // RECALGCALC_H
