#include "recalgcalc.h"

RecAlgCalc::RecAlgCalc(QVector<double> ksi, QVector<double> pi, QObject *parent) : QObject(parent)
{
    this->ksi=ksi;
    this->pi=pi;
    n=ksi.length();
}

void RecAlgCalc::setRange(double left, double right)
{
    a=left;
    b=right;
    CACHEfi_ab.clear();
    CACHEfi_ainf.clear();
}

bool RecAlgCalc::in(double val, double left, double right, bool bounderyIncluded)
{
    if(bounderyIncluded)
    {
        if(val>=left)
            if(val<=right)
                return true;
    }
    else
    {
        if(val>left)
            if(val<right)
                return true;
    }
    return false;
}

double RecAlgCalc::getKsi(int i, int t)
{
    return ksi.at(i);
}

double RecAlgCalc::fi_ab(double x, int t)
{
    QPair<double,int> arg;
    arg.first=x;
    arg.second=t;
    bool isInCache=CACHEfi_ab.contains(arg);

    if(isInCache)
        return CACHEfi_ab.value(arg);
    if(t==0)
    {
        if(in(x,a,b))
            return 1;
        else
            return 0;
    }
    else
    {
        QVector<double> fi(n);
        for(int i=0;i<n;i++)
        {
            double rate=1+getKsi(i,T-t);
            fi[i]=fi_ab(x*rate,t-1);
        }
        double fiRes=mathE(fi,pi);
        if(!isInCache)
            CACHEfi_ab.insert(arg,fiRes);
        return fiRes;
    }
}

double RecAlgCalc::fi_ainf(double x, int t)
{
    QPair<double,int> arg;
    arg.first=x;
    arg.second=t;
    bool isInCache=CACHEfi_ainf.contains(arg);

    if(isInCache)
        return CACHEfi_ainf.value(arg);
    if(t==0)
    {
        if(x>=a)
            return 1;
        else
            return 0;
    }
    else
    {
        QVector<double> fi(n);
        for(int i=0;i<n;i++)
        {
            double rate=1+getKsi(i,T-t);
            fi[i]=fi_ainf(x*rate,t-1);
        }
        double fiRes=mathE(fi,pi);
        if(!isInCache)
            CACHEfi_ainf.insert(arg,fiRes);
        return fiRes;
    }
}

double RecAlgCalc::calcPa(double initialPrice,int t)
{
    T=t;
    return fi_ainf(initialPrice,t);
}

double RecAlgCalc::calcPb(double initialPrice,int t)
{
    T=t;
    return fi_ainf(initialPrice,t)-fi_ab(initialPrice,t);
}

double RecAlgCalc::calcPab(double initialPrice,int t)
{
    T=t;
    return fi_ab(initialPrice,t);
}
