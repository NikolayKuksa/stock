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
#include <QDesktopWidget>
#include <QBoxLayout>
#include <QTableWidget>
#include <QGroupBox>
#include <QDateEdit>
#include <QLabel>
#include <QRadioButton>
#include <QFormLayout>
#include <QPushButton>
#include <QComboBox>

#include "secsymboldescription.h"
#include "findatacollector.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
private slots:
    void shareClicked();
    void indexClicked();
    void searchButtonClicked();

private:
    QNetworkAccessManager qnam;
    QNetworkReply *reply;
    SecSymbolDescription *symbolDesc;
    FinDataCollector *yahooCollector;
   //Gui
    QGroupBox *calcPane;
    QGroupBox *searchPane;
    QGroupBox *historicalDataPane;
    QBoxLayout* mainLayout;
    QTableWidget* table;
    QDateEdit *fromDateEdit;
    QDateEdit *toDateEdit;
    QComboBox *selectSecComboBox;
    SecurityType secType;
    QPushButton *searchButton;

    void createRadioButton(QHBoxLayout *layout);
    void createDateEdit(QHBoxLayout *layout);
    void createSearchButton(QHBoxLayout *layout,QWidget *parent = 0);
    QComboBox* createSelectSecComboBox(QWidget *parent = 0);

signals:
    void needData(QString, QString, QString, QString);
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
};

#endif // MAINWINDOW_H
