#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QNetworkAccessManager>
#include <QUrl>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QString>
#include <QLineEdit>
#include <QByteArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonArray>

class MainWindow : public QMainWindow
{
    Q_OBJECT
private slots:
    //void downloadFile();
    //void cancelDownload();
    void httpFinished();
    void httpReadyRead();

private:
    QNetworkAccessManager qnam;
    QNetworkReply *reply;
    QByteArray jsonData;

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
};

#endif // MAINWINDOW_H
