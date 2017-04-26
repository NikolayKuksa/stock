#include "findatacollector.h"

FinDataCollector::FinDataCollector(QAbstractButton *callingButton, QWidget *parent) : QProgressBar(parent)
{
    this->callingButton=callingButton;
    blockSize=12;
    data=new QStandardItemModel;
    this->setTextVisible(false);
    this->setVisible(false);
    dataHeaders<<"Date"<<"Open"<<"High"<<"Low"<<"Close"<<"Volume"<<"Adj_Close";
}

QStandardItemModel* FinDataCollector::getDataModel()
{
    return data;
}

QJsonArray FinDataCollector::parseJson(QByteArray jsonData)
{
    qDebug()<<jsonData.count();
    //qDebug()<<jsonData;
    QJsonParseError err;
    QJsonArray empty;
    QJsonDocument document = QJsonDocument::fromJson(jsonData,&err);
        if (!document.isObject()) {
            qDebug() << "Document is not an object";
            qDebug()<<"err: "<<err.errorString();
            return empty;
        }
        QJsonObject object = document.object();

        QJsonValue query = object.value("query");
        if (query.isUndefined()) {
            qDebug() << "Key id does not exist";
            return empty;
        }

        QJsonValue results=query.toObject().value("results");
        if (results.isUndefined()) {
            qDebug() << "Key id does not exist";
            return empty;
        }

        QJsonValue quote=results.toObject().value("quote");
        if (quote.isUndefined()) {
            qDebug() << "Key id does not exist";
            return empty;
        }

        return quote.toArray();
}

void FinDataCollector::httpFinished()
{
    QByteArray jsonData=reply->readAll();
    QJsonArray quotes=parseJson(jsonData);
        QJsonObject row;
        /*for(size_t i=0;i<quotes.size();i++)
        {
            row=quotes.at(i).toObject();
            qDebug()<<"Symbol: "<<row.value("Symbol");
            qDebug()<<"Date: "<<row.value("Date");
            qDebug()<<"Open: "<<row.value("Open");
            qDebug()<<"High: "<<row.value("High");
            qDebug()<<"Low: "<<row.value("Low");
            qDebug()<<"Close: "<<row.value("Close");
            qDebug()<<"Volume: "<<row.value("Volume");
            qDebug()<<"Adj_Close: "<<row.value("Adj_Close");
            qDebug()<<"---------------------------------";
        }*/

       for(int i=0;i<quotes.size();i++){
           QList<QStandardItem *> newRow;
           row=quotes.at(i).toObject();
           newRow<<new QStandardItem((row.value("Date").toString()));
           newRow<<new QStandardItem((row.value("Open").toString()));
           newRow<<new QStandardItem((row.value("High").toString()));
           newRow<<new QStandardItem((row.value("Low").toString()));
           newRow<<new QStandardItem((row.value("Close").toString()));
           newRow<<new QStandardItem((row.value("Volume").toString()));
           newRow<<new QStandardItem((row.value("Adj_Close").toString()));
           data->appendRow(newRow);
       }

       httpRequestCall();
}

void FinDataCollector::httpRequest(QString from, QString to)
{
    qDebug()<<secId;
    QString domain("http://query.yahooapis.com/v1/public/yql?q=");
    QString query(" select * from   yahoo.finance.historicaldata"
                 " where symbol=%22"+secId+"%22 and startDate=%22"
                 +from+"%22 and endDate=%22"+to+"%22");
    QString urltail(" &format=json &diagnostics=true &env=store://datatables.org/alltableswithkeys &callback=");
    QUrl url=QUrl(domain+query+urltail);

    reply=qnam.get(QNetworkRequest(url));
    connect(reply, SIGNAL(finished()), this, SLOT(httpFinished()));
}

void FinDataCollector::fetchData(QString fromDate, QString toDate, QString secId, QString dateFormat)
{
    if (this->dateFormat==dateFormat)
        if (this->secId==secId)
            if (this->fromDate==fromDate)
                if (this->toDate==toDate)
                    return; //the same criteria for fetch as was before
    this->fromDate=fromDate;
    this->toDate=toDate;
    this->secId=secId;
    this->dateFormat=dateFormat;

    QDate startDate=QDate::fromString(fromDate,dateFormat);
    QDate endDate=QDate::fromString(toDate,dateFormat);
    if (endDate<startDate)
        return;

    if(callingButton!=0)
        callingButton->setEnabled(false);
    this->setVisible(true);
    data->clear();
    data->setHorizontalHeaderLabels(dataHeaders);
    QDate tmp=endDate;
    while (tmp>startDate) {
        httpRequestDates<<tmp;
        tmp=tmp.addMonths(-blockSize);
    }
    httpRequestDates<<startDate;

    this->reset();
    this->setMinimum(0);
    this->setMaximum(httpRequestDates.size()-1);
    this->setValue(0);

    httpRequestCall();
}

void FinDataCollector::httpRequestCall()
{
    if (httpRequestDates.size()<=1)
    {
        httpRequestDates.clear();
        return;
    }
    bool isLastInterval=httpRequestDates.size()==2;
    QString startDateStr;
    QString endDateStr=httpRequestDates.first().toString(dateFormat);
    httpRequestDates.pop_front();
    QDate tmp=httpRequestDates.first();
    if (isLastInterval)
    {
        startDateStr=tmp.toString(dateFormat);
        httpRequest(startDateStr, endDateStr);
        if(callingButton!=0)
                callingButton->setEnabled(true);
            this->setVisible(false);
        return;
    }
    else
        startDateStr=tmp.addDays(1).toString(dateFormat);

    httpRequest(startDateStr, endDateStr);

    this->setValue(this->value()+1);
}
