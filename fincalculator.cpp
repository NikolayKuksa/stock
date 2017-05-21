#include "fincalculator.h"

FinCalculator::FinCalculator(QObject *parent) : QObject(parent)
{
    inDateFormat="yyyy-mm-dd";
    outModel=Q_NULLPTR;
}

void FinCalculator::setModel(QStandardItemModel *rawModel)
{
    this->rawModel=rawModel;
    directionSetting=back;
    recalculate(0);

}

QStandardItemModel* FinCalculator::getModel()
{
    return outModel;
}

void FinCalculator::recalculate(int rowCount)
{
    if(rowCount<=0)
        outModel=new QStandardItemModel;
    else
    {
        //delete outModel; //probably have memory leek
        outModel=cutModelChangeDirection(rawModel,QStringList("Date"),false);

        prices=calcPrices();
        appendValuesToModel(outModel,QString("AVG price per day"),prices);

        returns=calcReturns();
        appendValuesToModel(outModel,QString("Return"),returns);

        emit recalculated(prices.length());
    }
}

QVector<double> FinCalculator::calcPrices()
{
    QVector<double> open=fetchValuesFromModel(rawModel,"Open");
    QVector<double> close=fetchValuesFromModel(rawModel,"Close");
    /*if(calcDirection(rawModel,inDateFormat)==back)
    {
        open=reverse(open);
        close=reverse(close);
    }*/
    int n=open.length();
    QVector<double> res(n);

    for(int i=0;i<n;i++)
        res[i]=(open.at(i)+close.at(i))/2;
    //qDebug()<<res;
    return res;
}

QVector<double> FinCalculator::calcReturns()
{
    int n=prices.length()-1;
    QVector<double> res(n);
    double price_i;
    for(int i=0;i<n;i++)
    {
        price_i=prices.at(i);
        res[i]=(prices.at(i+1)-price_i)/price_i;
    }
    res.append(0);
    return res;
}

QVector<double> FinCalculator::getPrices()
{
    return prices;
}

QVector<double> FinCalculator::getReturns()
{
    return returns;
}
