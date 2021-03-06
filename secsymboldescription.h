#ifndef SECSYMBOLDESCRIPTION_H
#define SECSYMBOLDESCRIPTION_H

#include <QVector>
#include "securitytype.h"
#include "miscdef.h"

class SecSymbolDescription
{
private:
    QVector<QString>  securitySymbols;
    QVector<QString>  indexSymbols;
    QVector<QString>  securityDescriptions;
    QVector<QString>  indexDescriptions;

public:
    SecSymbolDescription();
    QString getSymbol(int ix, SecurityType secType);
    int addSecurity(QString symbol, QString description,SecurityType);
    QStringList getAllDescriptions(SecurityType secType);

};

#endif // SECSYMBOLDESCRIPTION_H
