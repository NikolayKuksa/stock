#ifndef FINDATACOLLECTOR_H
#define FINDATACOLLECTOR_H

#include <QProgressBar>
#include <QLabel>
#include <QHBoxLayout>
#include <QStandardItemModel>
#include <QString>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonParseError>
#include <QAbstractButton>
#include <QJsonArray>
#include <QJsonObject>
#include <QList>
#include <QDate>
#include <QStringList>
#include <QDialog>

#include "securitytype.h"

class FinDataCollector : public QDialog
{
    Q_OBJECT

private:
    QProgressBar *progressBar;
    QAbstractButton *callingButton;
    QString fromDate;
    QString toDate;
    QString secId;
    QString dateFormat;
    std::size_t blockSize;
    QStandardItemModel *data;
    QNetworkAccessManager qnam;
    QNetworkReply *reply;
    QStringList dataHeaders;
    QList<QDate> httpRequestDates;
    int rowCount;
    bool isLastInterval;

    void httpRequest(QString from, QString to);
    QJsonArray parseJson(QByteArray jsonData);
    void httpRequestCall();

public:
    explicit FinDataCollector(QAbstractButton *callingButton=0,QWidget *parent = 0);
    QStandardItemModel* getDataModel();
    void setDataHeaders(QStringList headers);

signals:
    void modelUpdated(int);

private slots:
    //void downloadFile();
    //void cancelDownload();
    void httpFinished();
    //void httpReadyRead();


public slots:
    void fetchData(QString fromDate, QString toDate, QString secId, QString dateFormat);
};

#endif // FINDATACOLLECTOR_H
