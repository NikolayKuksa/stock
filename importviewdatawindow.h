#ifndef IMPORTVIEWDATAWINDOW_H
#define IMPORTVIEWDATAWINDOW_H

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
#include <QCheckBox>

#include "secsymboldescription.h"
#include "findatacollector.h"
#include "plotbymodel.h"
#include "miscdef.h"
#include "fincalculator.h"

class ImportViewDataWindow : public QMainWindow
{
    Q_OBJECT
private slots:
    void shareClicked();
    void indexClicked();
    void searchButtonClicked();
    void histTableViewChanged(int rowCount);
    void histPlotButtonClicked();
    void histDataToFileButtonClicked();
    void fromDateEditValidate(QDate newDate);
    void toDateEditValidate(QDate newDate);
    void priceFieldsForPlotCheckBoxClicked(bool isChecked);
    void selectAllPlotFieldsButtonClicked();
    void deselectAllPlotFieldsButtonClicked();
    void returnPlotButtonClicked();
    void loadFileButtonClicked();

private:
    SecurityType secType;
    QNetworkAccessManager qnam;
    QNetworkReply *reply;
    SecSymbolDescription *symbolDesc;
    FinDataCollector *yahooCollector;
    PlotByModel *histDataPlot;
    FinCalculator *finCalculator;
    QString dateFormat;
    QString plotDateFormat;
    bool atLeastOnePlotFieldsChecked;
    int days;
    int plotFieldsCount;
    bool histGridHasData;
    bool plotFromFile;
    QString fromFileNamePath;

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
    QCheckBox *openPriceCheckBox;
    QCheckBox *closePriceCheckBox;
    QCheckBox *lowPriceCheckBox;
    QCheckBox *highPriceCheckBox;
    QTableView *calculatedDataTableView;
    QPushButton *returnPlotButton;

    void createRadioButton(QHBoxLayout *layout);
    void createDateEdit(QHBoxLayout *layout);
    void createSearchButton(QHBoxLayout *layout,QWidget *parent = 0);
    void createSelectSecComboBox(QBoxLayout *layout, QWidget *parent = 0);
    void createSelectSecComboBox(QGridLayout *layout, QWidget *parent = 0, int row=0, int column=0);
    QStandardItemModel *prepareModelForPlot(QStandardItemModel *model);
    QStringList getHistPlotFields();
    void createCheckBoxesPlotFields(QVBoxLayout *layout);

signals:
    void needData(QString, QString, QString, QString);
    void showCalcGridFromCSV(int);
public:
    ImportViewDataWindow(QWidget *parent = 0);
    ~ImportViewDataWindow();

public slots:
    void updateCalcGrid(int rowCount);
};

#endif // IMPORTVIEWDATAWINDOW_H
