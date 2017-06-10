#include "pointforselection.h"

PointForSelection::PointForSelection()
{
}

PointForSelection::PointForSelection(int id, int n)
{
    this->id=id;
    this->n=n;
}

bool PointForSelection::operator >(const PointForSelection &other) const
{
    //qDebug()<<"operator > call"<<n;

    bool atListOneDominate=false;
    for(int i=0;i<n;i++)
    {
        //qDebug()<<this->maxDirection;
        //qDebug()<<"operator > this/other"<<this->getCriteria(i)<<other.getCriteria(i);
        if(this->getMaxDirection(i)){   //we need to maximize i-th criteria
            if(this->getCriteria(i)>=other.getCriteria(i))
            {

                if(this->getCriteria(i)>other.getCriteria(i))
                    atListOneDominate=true;
                else
                    continue;
            }
            else
                return false;
        }
        else    //we need to minimize i-th criteria
        {
            if(this->getCriteria(i)<=other.getCriteria(i))
            {
                if(this->getCriteria(i)<other.getCriteria(i))
                    atListOneDominate=true;
                else
                    continue;
            }
            else
                return false;
        }
    }
    return atListOneDominate;
}

double PointForSelection::getCriteria(int index) const
{
    //qDebug()<<"criteria: "<<criteria;
    return criteria.at(index);
}

QString PointForSelection::getCriteriaNames(int index) const
{
    return criteriaNames.at(index);
}

bool PointForSelection::getMaxDirection(int index) const
{
    return maxDirection.at(index);
}

void PointForSelection::setCriteria(QVector<double> values)
{
    this->criteria=values;
}

void PointForSelection::setCriteriaNames(QStringList names)
{
    this->criteriaNames=names;
}

void PointForSelection::setMaxDirection(QVector<bool> maxDirection)
{
    this->maxDirection=maxDirection;
}

int PointForSelection::getId()
{
    return id;
}











