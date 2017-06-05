#ifndef POINTFORSELECTION_H
#define POINTFORSELECTION_H

#include <QVector>
#include <QString>
#include <QStringList>

class PointForSelection
{
public:
    PointForSelection();
    PointForSelection(int id, int n);
    bool operator >(const PointForSelection &other) const;

    double getCriteria(int index) const;
    QString getCriteriaNames(int index) const;
    bool getMaxDirection(int index) const;
    int getId();

    void setCriteria(QVector<double> values);
    void setCriteriaNames(QStringList names);
    void setMaxDirection(QVector<bool> maxDirecton);

    bool hasDominator(QVector<PointForSelection> points) const;

private:
    int n;
    int id;
    QVector<double> criteria;
    QStringList criteriaNames;
    QVector<bool> maxDirection;
};

#endif // POINTFORSELECTION_H
