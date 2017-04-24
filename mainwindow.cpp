#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    this->resize(QDesktopWidget().availableGeometry(this).size() * 0.7);
    mainLayout=new QBoxLayout(QBoxLayout::LeftToRight);
    this->setWindowTitle("Stock. Developed by Mykola Kuksa");

    //==================================================
    calcPane = new QGroupBox(tr("Calculation"));
    QHBoxLayout *controlLayout = new QHBoxLayout;
    //add widgets here
    calcPane->setLayout(controlLayout);
    //==================================================

    //==========Historical data pane===================
    searchPane = new QGroupBox(tr("Search criteria"));
    QGridLayout *searchLayout = new QGridLayout;
    QHBoxLayout *dateSecLayout = new QHBoxLayout;
    createDateEdit(dateSecLayout);
    createRadioButton(dateSecLayout);
    createSearchButton(dateSecLayout,searchPane);

    searchLayout->addLayout(dateSecLayout,0,0,1,1);
    searchPane->setLayout(searchLayout);
    historicalDataPane = new QGroupBox(tr("Historical data"));
    QVBoxLayout *historicalDataLayout = new QVBoxLayout;
    historicalDataLayout->addWidget(searchPane);
    table=new QTableWidget();
    historicalDataLayout->addWidget(table);
    historicalDataPane->setLayout(historicalDataLayout);
    //==================================================
//qDebug()<<fromEdit->date().toString(dateFormat);
    mainLayout->addWidget(calcPane,1);
    mainLayout->addWidget(historicalDataPane,1);

    //set layout on window
    QWidget* wlayout=new QWidget;
    wlayout->setLayout(mainLayout);
    this->setCentralWidget(wlayout);

//this->set

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
    QByteArray jsonData=reply->readAll();
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

       table->setColumnCount(8);
       QStringList headers;
       headers<<"Symbol"<<"Date"<<"Open"<<"High"<<"Low"<<"Close"<<"Volume"<<"Adj_Close";
       table->setHorizontalHeaderLabels(headers);
       for(int i=0;i<quotes.size();i++){
            table->insertRow(i);
            QTableWidgetItem* celli0=new QTableWidgetItem();
            QTableWidgetItem* celli1=new QTableWidgetItem();
            QTableWidgetItem* celli2=new QTableWidgetItem();
            QTableWidgetItem* celli3=new QTableWidgetItem();
            QTableWidgetItem* celli4=new QTableWidgetItem();
            QTableWidgetItem* celli5=new QTableWidgetItem();
            QTableWidgetItem* celli6=new QTableWidgetItem();
            QTableWidgetItem* celli7=new QTableWidgetItem();

            row=quotes.at(i).toObject();
            celli0->setText(row.value("Symbol").toString());
            celli1->setText(row.value("Date").toString());
            celli2->setText(row.value("Open").toString());
            celli3->setText(row.value("High").toString());
            celli4->setText(row.value("Low").toString());
            celli5->setText(row.value("Close").toString());
            celli6->setText(row.value("Volume").toString());
            celli7->setText(row.value("Adj_Close").toString());

            table->setItem(i,0,celli0);
            table->setItem(i,1,celli1);
            table->setItem(i,2,celli2);
            table->setItem(i,3,celli3);
            table->setItem(i,4,celli4);
            table->setItem(i,5,celli5);
            table->setItem(i,6,celli6);
            table->setItem(i,7,celli7);
        }

}

void MainWindow::createDateEdit(QHBoxLayout *layout)
{
    QString dateFormat("yyyy-MM-dd");

    QGroupBox *fromToDatePane = new QGroupBox;
    QHBoxLayout *fromToDateLayout = new QHBoxLayout;
    fromToDatePane->setLayout(fromToDateLayout);

    QFormLayout *fromLayout=new QFormLayout;
    QLabel *fromLabel =new QLabel("From",fromToDatePane);
    fromDateEdit = new QDateEdit(QDate::currentDate(),fromToDatePane);
    fromLayout->addRow(fromLabel,fromDateEdit);
    fromDateEdit->setDisplayFormat(dateFormat);

    QFormLayout *toLayout=new QFormLayout;
    QLabel *toLabel = new QLabel("To",fromToDatePane);
    toDateEdit = new QDateEdit(QDate::currentDate(),toLabel);
    toLayout->addRow(toLabel,toDateEdit);
    toDateEdit->setDisplayFormat(dateFormat);

    //dateEdit->setMinimumDate(QDate::currentDate().addDays(-365));
    //dateEdit->setMaximumDate(QDate::currentDate().addDays(365));
    fromToDateLayout->addLayout(fromLayout);
    fromToDateLayout->addLayout(toLayout);

    layout->addWidget(fromToDatePane);
}

void MainWindow::createRadioButton(QHBoxLayout *layout)
{
    QGroupBox *radioPane = new QGroupBox;
    QHBoxLayout *radioLayout = new QHBoxLayout;
    QRadioButton *shareRadioButton = new QRadioButton(tr("&Share"));
    QRadioButton *indexRadioButton = new QRadioButton(tr("Market index"));
    radioLayout->addWidget(shareRadioButton);
    radioLayout->addWidget(indexRadioButton);
    radioPane->setLayout(radioLayout);
    layout->addWidget(radioPane);

    shareRadioButton->setChecked(true);
    secType=security;
    connect(shareRadioButton, SIGNAL(clicked(bool)),
             this, SLOT(shareClicked(bool)));
    connect(indexRadioButton, SIGNAL(clicked(bool)),
             this, SLOT(indexClicked(bool)));
}

void MainWindow::createSearchButton(QHBoxLayout *layout, QWidget *parent)
{
    QPushButton *searchButton=new QPushButton("search",parent);
    layout->addWidget(searchButton);
    connect(searchButton, SIGNAL(clicked()), this, SLOT(httpRequest()));
}

void MainWindow::shareClicked(bool is)
{
    secType=security;
}

void MainWindow::indexClicked(bool is)
{
    secType=index;
}

void MainWindow::httpRequest()
{
    QString dateFormat("yyyy-MM-dd");

    QString sec("OPHC");
    QString from=fromDateEdit->date().toString(dateFormat);
    QString to=toDateEdit->date().toString(dateFormat);
    QString domain("http://query.yahooapis.com/v1/public/yql?q=");
    QString query(" select * from   yahoo.finance.historicaldata"
                 " where symbol=%22"+sec+"%22 and startDate=%22"
                 +from+"%22 and endDate=%22"+to+"%22");
    QString urltail(" &format=json &diagnostics=true &env=store://datatables.org/alltableswithkeys &callback=");
    QUrl url=QUrl(domain+query+urltail);

    reply=qnam.get(QNetworkRequest(url));
    connect(reply, SIGNAL(finished()), this, SLOT(httpFinished()));
    connect(reply, SIGNAL(readyRead()), this, SLOT(httpReadyRead()));
}

MainWindow::~MainWindow()
{

}
