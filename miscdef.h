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

enum SecurityType{security, index};
enum ModelDirection{forward,back,oneRow};

double myRound(double arg);
QString numberFormat(QString number,int accurancy=6);
QStandardItemModel *cutModelChangeDirection(QStandardItemModel *model, QStringList headers, ModelDirection direction);
QList<QStandardItem *> doubleVectorToModelColumn(QVector<double> values, ModelDirection direction=forward);
QVector<double> fetchValuesFromModel(QStandardItemModel *model, QString header, ModelDirection direction=forward);
ModelDirection calcDirection(QStandardItemModel *model, QString inDateFormat, int column=0);
void appendValuesToModel(QStandardItemModel *model, QString header, QVector<double> values);
double mathE(QVector<double> xi,QVector<double> pi);

#endif // MISCDEF

