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

class MainWindow : public QMainWindow
{
    Q_OBJECT
private slots:
    //void downloadFile();
    //void cancelDownload();
    //void httpFinished();
    void httpReadyRead();

private:
    QNetworkAccessManager qnam;
    QNetworkReply *reply;

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
};

#endif // MAINWINDOW_H
