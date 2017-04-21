#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{

QString sec("OPHC");
QString from("2017-04-03");
QString to(2017-04-15);
QString domain("http://query.yahooapis.com/v1/public/yql?q=");
QString query(" select * from   yahoo.finance.historicaldata"
              " where symbol=%22"+sec+"%22 and startDate=%22"
              +from+"%22 and endDate=%22"+to+"%22");
QString urltail(" &format=json &diagnostics=true &env=store://datatables.org/alltableswithkeys &callback=");
QUrl url=QUrl(domain+query+urltail);

reply=qnam.get(QNetworkRequest(url));
   connect(reply, SIGNAL(finished()),
        this, SLOT(httpFinished()));
   connect(reply, SIGNAL(readyRead()),
            this, SLOT(httpReadyRead()));



}

void MainWindow::httpReadyRead()
{
    // this slot gets called every time the QNetworkReply has new data.
    // We read all of its new data and write it into the file.
    // That way we use less RAM than when reading it at the finished()
    // signal of the QNetworkReply

    //jsonData.append(reply->readAll());

}

void MainWindow::httpFinished()
{
    jsonData=reply->readAll();
    qDebug()<<jsonData.count();
    //qDebug()<<jsonData;
    QJsonParseError err;
    QJsonDocument document = QJsonDocument::fromJson(jsonData,&err);
        if (!document.isObject()) {
            qDebug() << "Document is not an object";
            qDebug()<<"err: "<<err.errorString();
            return;
        }
        QJsonObject object = document.object();

        QJsonValue query = object.value("query");
        if (query.isUndefined()) {
            qDebug() << "Key id does not exist";
            return;
        }

        QJsonValue results=query.toObject().value("results");
        if (results.isUndefined()) {
            qDebug() << "Key id does not exist";
            return;
        }

        QJsonValue quote=results.toObject().value("quote");
        if (quote.isUndefined()) {
            qDebug() << "Key id does not exist";
            return;
        }

        QJsonArray quotes=quote.toArray();
        QJsonObject row;
        for(size_t i=0;i<quotes.size();i++)
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
        }
       /* if (!qur.isString()) {
            qDebug() << "Value not string";
            //return;
        }*/

        //qDebug() << jsonValue.toString();

}

MainWindow::~MainWindow()
{

}
