#include "recalgcalc.h"

RecAlgCalc::RecAlgCalc(QWidget *parent) : QDialog(parent)
{
    mother=parent;
    calcProgressBar=new QProgressBar(this);
    calcProgressBar->setValue(0);
    calcProgressBar->setMinimum(0);

    QHBoxLayout *layout = new QHBoxLayout;
    this->setLayout(layout);
    layout->addWidget(calcProgressBar);
    connect(this,SIGNAL(updateProgress(int)),this,SLOT(updateProgressBar(int)));
}

void RecAlgCalc::setRange(double left, double right)
{
    if((a!=b)||(b!=right))
    {
        a=left;
        b=right;
        CACHEfi_ab.clear();
        CACHEfi_ainf.clear();
    }
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
    int tt=ksi.length()>1?t:1;
    return ksi.at(tt-1).at(i);
}

double RecAlgCalc::fi_ab(double x, int t)
{
    QPair<double,int> arg;
    arg.first=x;
    arg.second=t;
    bool isInCache=CACHEfi_ab.contains(arg);

    //int progress=T-t;
    //if(calcProgressBar->value()!=progress)
      //  emit updateProgress(progress);

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
            double rate=1+getKsi(i,t);
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

    //int progress=T-t;
    //if(calcProgressBar->value()!=progress)
      //  emit updateProgress(progress);

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
            double rate=1+getKsi(i,t);
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
    //calcProgressBar->setMaximum(T);
    return fi_ainf(initialPrice,t);
}

double RecAlgCalc::calcPb(double initialPrice,int t)
{
    T=t;
    /*calcProgressBar->setMaximum(T);
    calcProgressBar->setMinimum(0);
    calcProgressBar->setValue(0);*/
    return fi_ainf(initialPrice,t)-fi_ab(initialPrice,t);
}

double RecAlgCalc::calcPab(double initialPrice,int t)
{
    T=t;
    return fi_ab(initialPrice,t);
}

void RecAlgCalc::setXiPi(QVector<QVector<double> > xi, QVector<double> pi)
{
    if((this->ksi!=xi)||(this->pi!=pi))
    {
        this->ksi=xi;
        this->pi=pi;
        n=pi.length();
    }
}
void RecAlgCalc::myShow()
{
    int nWidth = 200;
    int nHeight = 100;
    if (mother != NULL)
        setGeometry(mother->x() + mother->width()/2 - nWidth/2,
                    mother->y() + mother->height()/2 - nHeight/2,
                    nWidth, nHeight);
    else
        resize(nWidth, nHeight);
    this->setWindowFlags(Qt::Window|Qt::WindowTitleHint);
    this->setWindowTitle("Calculating probabilities...");
    this->setMinimumSize(this->size());
    this->setMaximumSize(this->size());

    this->setModal(true);
    show();
}

void RecAlgCalc::updateProgressBar(int value)
{
    if(value==0)
        myShow();
    else
        if(value!=calcProgressBar->maximum())
            calcProgressBar->setValue(value);
        else
        {
            calcProgressBar->setValue(value);
            calcProgressBar->setVisible(false);
            calcProgressBar->reset();
        }
}
