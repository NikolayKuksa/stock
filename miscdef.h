#ifndef MISCDEF
#define MISCDEF

#include <math.h>
#include <QString>
#include <QStringList>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QDate>
#include <QDebug>
#include <QMap>
#include <QPair>
#include <QFile>
#include <algorithm>
#include "pointforselection.h"

enum SecurityType{security, index};
enum ModelDirection{forward,back,oneRow};

double myRound(double arg);
QString numberFormat(QString number,int accurancy=6);
QStandardItemModel *cutModelChangeDirection(QStandardItemModel *model, QStringList headers, bool changeDirection);
QList<QStandardItem *> doubleVectorToModelColumn(QVector<double> values, ModelDirection direction=forward);
QVector<double> fetchValuesFromModel(QStandardItemModel *model, QString header, ModelDirection direction=forward);
ModelDirection calcDirection(QStandardItemModel *model, QString inDateFormat, int column=0);
void appendValuesToModel(QStandardItemModel *model, QString header, QVector<double> values);
double mathE(QVector<double> xi,QVector<double> pi);
int modelFromCSV(QStandardItemModel *model,QString fileName,bool reverseRowOrder=false);
QString getFileNameFromFullPath(QString path,bool excludeFileExt=true);
QVector<double> reverse(QVector<double> vec);
double mathSemiD(QVector<double> xi,QVector<double> pi, double E);
QStandardItemModel *reverseRowsOrder(QStandardItemModel* model);

class PortfolioParam
{
public:
    double E;
    double D;
    double Pa;
    double Pb;
    double ro;
    bool dominateP(PortfolioParam p);
    bool hasDominator(QVector<PointForSelection> points);
    QString toString();
    PointForSelection toPointForSelection(QVector<bool> checkedCriteria, int id=0);
};

QVector<PortfolioParam> getParetoSet(QVector<PortfolioParam> pors, QVector<bool> lx);
double roundPrec(double num,int prec=6);
void moveData(QStandardItemModel *from, QStandardItemModel *to);
bool hasDominatorInVector(PointForSelection point, QVector<PointForSelection> points);

//void plot(QVector<double> x, QVector<QVector<double> > y, QVector<Q)
#endif // MISCDEF

