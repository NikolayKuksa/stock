#ifndef RECALGWINDOW_H
#define RECALGWINDOW_H

#include <QMainWindow>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QGroupBox>
#include <QLineEdit>
#include <QString>
#include <QIntValidator>
#include <QDoubleValidator>
#include <QSpinBox>
#include <QTableWidget>
#include <QPushButton>
#include <QDebug>
#include <QRadioButton>
#include <QLabel>
#include <QStatusBar>
#include "recalgcalc.h"

class RecAlgWindow : public QMainWindow
{
    Q_OBJECT
private:
    QString xi;
    QString p_;
    double maxInPrice;
    int prec;
    bool hasXiStationaryDistribution;
    bool distrTableIsOk;

    RecAlgCalc *calculator;

    QPushButton *calcButton;
    QLineEdit *PaOutEdit;
    QLineEdit *PbOutEdit;
    QTableWidget *distrTable;
    QSpinBox *nEdit;
    QSpinBox *tEdit;
    QLineEdit *XoEdit;
    QLineEdit *aEdit;
    QLineEdit *bEdit;
    bool distrTableCellFrame;

    void addDistrRow(int ix);
    void addDistrColumn(int ix);
    QString getDistrTableCellValue(int row,int column);
    QString qstr(int number);
    void createRadioButton(QFormLayout *layout);
    bool validateDistrTable();

private slots:
    void nChanged(int newColumnCount);
    void tChanged(int newT);
    void addDelRowColumn(int newCount,QString operation, QString axisName);
    void calcButtonClicked();
    void noRadioButtonClicked();
    void yesRadioButtonClicked();
    void distrTableChanged(int,int);
public:
    explicit RecAlgWindow(QWidget *parent = 0);

signals:

public slots:
};

#endif // RECALGWINDOW_H
