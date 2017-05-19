#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "importviewdatawindow.h"
#include "recalgwindow.h"
#include "portfoliooptimisationwindow.h"
#include <assert.h>

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);

signals:

public slots:

private slots:
    void importViewButtonClicked();
    void recAlgButtonClicked();
    void portOptimisationButtonClicked();
};

#endif // MAINWINDOW_H
