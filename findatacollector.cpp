#include "findatacollector.h"

FinDataCollector::FinDataCollector(QAbstractButton *callingButton, QWidget *parent) : QDialog(parent)
{
    int nWidth = 200;
    int nHeight = 100;
    if (parent != NULL)
        setGeometry(parent->x() + parent->width()/2 - nWidth/2,
                    parent->y() + parent->height()/2 - nHeight/2,
                    nWidth, nHeight);
    else
        resize(nWidth, nHeight);
    this->setWindowFlags(Qt::Window|Qt::WindowTitleHint);
    this->setWindowTitle("Data is importing");
    this->setMinimumSize(this->size());
    this->setMaximumSize(this->size());

    this->setModal(true);
    progressBar=new QProgressBar(this);

    rowCount=0;
    isLastInterval=false;

    QHBoxLayout *layout = new QHBoxLayout;
    this->setLayout(layout);
    layout->addWidget(progressBar);

    this->callingButton=callingButton;
    blockSize=12;
    data=new QStandardItemModel(this);

    dataHeaders<<"Date"<<"Open"<<"High"<<"Low"<<"Close"<<"Volume"<<"Adj_Close";
}

QStandardItemModel* FinDataCollector::getDataModel()
{
    return data;
}

QJsonArray FinDataCollector::parseJson(QByteArray jsonData)
{
    //qDebug()<<jsonData.count();
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
    QJsonObject newJsonRow;

    for(int i=0;i<quotes.size();i++){
        QList<QStandardItem *> newModelRow;
        newJsonRow=quotes.at(i).toObject();
        for (int j=0;j<dataHeaders.size();j++)
            newModelRow<<new QStandardItem(newJsonRow.value(dataHeaders.at(j)).toString());
        data->appendRow(newModelRow);
        rowCount++;
    }
    if (isLastInterval)
        emit modelUpdated(rowCount);
    else
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
    this->show();
    isLastInterval=false;
    rowCount=0;
    data->clear();
    data->setHorizontalHeaderLabels(dataHeaders);
    QDate tmp=endDate;
    while (tmp>startDate) {
        httpRequestDates<<tmp;
        tmp=tmp.addMonths(-blockSize);
    }
    httpRequestDates<<startDate;

    progressBar->reset();
    progressBar->setMinimum(0);
    progressBar->setMaximum(httpRequestDates.size()-1);
    progressBar->setValue(0);

    httpRequestCall();
}

void FinDataCollector::httpRequestCall()
{
    if (httpRequestDates.size()<=1)
    {
        httpRequestDates.clear();
        return;
    }
    isLastInterval=httpRequestDates.size()==2;
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
        this->hide();
        return;
    }
    else
        startDateStr=tmp.addDays(1).toString(dateFormat);

    httpRequest(startDateStr, endDateStr);

    progressBar->setValue(progressBar->value()+1);
}

void FinDataCollector::setDataHeaders(QStringList headers)
{
    this->dataHeaders=headers;
}
