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
   connect(reply, SIGNAL(readyRead()),
            this, SLOT(httpReadyRead()));



}

void MainWindow::httpReadyRead()
{
    // this slot gets called every time the QNetworkReply has new data.
    // We read all of its new data and write it into the file.
    // That way we use less RAM than when reading it at the finished()
    // signal of the QNetworkReply

    QByteArray jsonData = reply->readAll();
    QJsonParseError *err;
    qDebug()<<"err: "<<err->errorString();
        QJsonDocument document = QJsonDocument::fromJson(jsonData,err);
        if (!document.isObject()) {
            qDebug() << "Document is not an object";
            //return 4;
        }
        QJsonObject object = document.object();
        QJsonValue jsonValue = object.value("query");
        if (jsonValue.isUndefined()) {
            qDebug() << "Key id does not exist";
            //return 5;
        }
        if (!jsonValue.isString()) {
            qDebug() << "Value not string";
           // return 6;
        }

        qDebug() << jsonValue.toString();

}

MainWindow::~MainWindow()
{

}
