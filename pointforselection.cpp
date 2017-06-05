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
    bool atListOneDominate=false;
    for(int i=0;i<n;i++)
    {
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

void PointForSelection::setMaxDirection(QVector<bool> maxDirecton)
{
    this->maxDirection=maxDirection;
}

bool PointForSelection::hasDominator(QVector<PointForSelection> points) const
{
    for(int i=0;i<points.length();i++)
        if(points.at(i)>(*this))
            return true;
}

int PointForSelection::getId()
{
    return id;
}











