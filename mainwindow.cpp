#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    this->setWindowTitle("Stock. Developed by Mykola Kuksa. ");

    QHBoxLayout *mainLayout=new QHBoxLayout;
    QWidget *mainWidget=new QWidget(this);
    mainWidget->setLayout(mainLayout);
    this->setCentralWidget(mainWidget);

    QPushButton *importViewButton=new QPushButton(tr("Import and view data"));
    mainLayout->addWidget(importViewButton);
    connect(importViewButton,SIGNAL(clicked()),this,SLOT(importViewButtonClicked()));

    QPushButton *recAlgButton=new QPushButton(tr("Recurrence relations"));
    mainLayout->addWidget(recAlgButton);
    connect(recAlgButton,SIGNAL(clicked()),this,SLOT(recAlgButtonClicked()));

    QPushButton *portOptimisationButton=new QPushButton(tr("Portfolio Optimisation"));
    mainLayout->addWidget(portOptimisationButton);
    connect(portOptimisationButton,SIGNAL(clicked()),this,SLOT(portOptimisationButtonClicked()));


}

void MainWindow::importViewButtonClicked()
{
    ImportViewDataWindow *w=new ImportViewDataWindow(this);
    w->show();
}

void MainWindow::recAlgButtonClicked()
{
    RecAlgWindow *w=new RecAlgWindow(this);
    w->show();
}

void MainWindow::portOptimisationButtonClicked()
{
    //PortfolioOptimisationWindow *w=new PortfolioOptimisationWindow(this);
    //w->show();
    //assert(0);
}
