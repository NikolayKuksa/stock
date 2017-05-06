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
#include <QStandardItemModel>

#include "secsymboldescription.h"
#include "findatacollector.h"
#include "plotbymodel.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
private slots:
    void shareClicked();
    void indexClicked();
    void searchButtonClicked();
    void histTableViewChanged(int rowCount);
    void histPlotButtonClicked();
    void histDataToFileButtonClicked();

private:
    SecurityType secType;
    QNetworkAccessManager qnam;
    QNetworkReply *reply;
    SecSymbolDescription *symbolDesc;
    FinDataCollector *yahooCollector;
    PlotByModel *histDataPlot;
    QString dateFormat;
    QString plotDateFormat;
    QStringList plotFields;

   //Gui
    QGroupBox *calcPane;
    QGroupBox *searchPane;
    QGroupBox *historicalDataPane;
    QBoxLayout* mainLayout;
    QDateEdit *fromDateEdit;
    QDateEdit *toDateEdit;
    QComboBox *selectSecComboBox;
    QPushButton *searchButton;
    QPushButton *histPlotButton;
    QPushButton *histDataToFileButton;
    QTableView *histTableView;

    void createRadioButton(QHBoxLayout *layout);
    void createDateEdit(QHBoxLayout *layout);
    void createSearchButton(QHBoxLayout *layout,QWidget *parent = 0);
    void createSelectSecComboBox(QBoxLayout *layout, QWidget *parent = 0);
    void createSelectSecComboBox(QGridLayout *layout, QWidget *parent = 0, int row=0, int column=0);
    QStandardItemModel *prepareModelForPlot(QStandardItemModel *model);

signals:
    void needData(QString, QString, QString, QString);
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
};

#endif // MAINWINDOW_H
