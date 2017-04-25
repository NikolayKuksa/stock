#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    this->resize(QDesktopWidget().availableGeometry(this).size() * 0.7);
    mainLayout=new QBoxLayout(QBoxLayout::LeftToRight);
    this->setWindowTitle("Stock. Developed by Mykola Kuksa");

    symbolDesc = new SecSymbolDescription;
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

    selectSecComboBox = createSelectSecComboBox(searchPane);
    searchLayout->addWidget(selectSecComboBox,1,0);

    createRadioButton(dateSecLayout);
    createSearchButton(dateSecLayout,searchPane);

    searchLayout->addLayout(dateSecLayout,0,0,1,1);
    searchPane->setLayout(searchLayout);
    historicalDataPane = new QGroupBox(tr("Historical data"));
    QVBoxLayout *historicalDataLayout = new QVBoxLayout;
    historicalDataLayout->addWidget(searchPane);

    yahooCollector=new FinDataCollector(searchButton,searchPane);
    connect(this,SIGNAL(needData(QString, QString,QString,QString)),
            yahooCollector,SLOT(fetchData(QString,QString,QString,QString)));
    connect(yahooCollector,SIGNAL(modelUpdated()),
            this,SLOT());

    historicalDataLayout->addWidget(yahooCollector);

    QTableView *histView=new QTableView;
    histView->setModel(yahooCollector->getDataModel());

    historicalDataLayout->addWidget(histView);
    historicalDataPane->setLayout(historicalDataLayout);
    //==================================================
    mainLayout->addWidget(calcPane,1);
    mainLayout->addWidget(historicalDataPane,1);

    //set layout on window
    QWidget* wlayout=new QWidget;
    wlayout->setLayout(mainLayout);
    this->setCentralWidget(wlayout);
}

void MainWindow::createDateEdit(QHBoxLayout *layout)
{
    QString dateFormat("yyyy-MM-dd");

    QGroupBox *fromToDatePane = new QGroupBox;
    QHBoxLayout *fromToDateLayout = new QHBoxLayout;
    fromToDatePane->setLayout(fromToDateLayout);

    QFormLayout *fromLayout=new QFormLayout;
    QLabel *fromLabel =new QLabel("From",fromToDatePane);
    fromDateEdit = new QDateEdit(QDate::currentDate().addDays(-14),fromToDatePane);
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
    selectSecComboBox->addItems(symbolDesc->getAllDescriptions(secType));

    connect(shareRadioButton, SIGNAL(clicked()), this, SLOT(shareClicked()));
    connect(indexRadioButton, SIGNAL(clicked()), this, SLOT(indexClicked()));
}

void MainWindow::createSearchButton(QHBoxLayout *layout, QWidget *parent)
{
    searchButton=new QPushButton("search",parent);
    layout->addWidget(searchButton);
    connect(searchButton, SIGNAL(clicked()), this, SLOT(searchButtonClicked()));
}

void MainWindow::searchButtonClicked(){
    QString dateFormat("yyyy-MM-dd");
    QString sec=symbolDesc->getSymbol(selectSecComboBox->currentIndex(),secType);
    QString from=fromDateEdit->date().toString(dateFormat);
    QString to=toDateEdit->date().toString(dateFormat);
    emit needData(from,to,sec,dateFormat);
}

QComboBox* MainWindow::createSelectSecComboBox(QWidget *parent)
{
    QComboBox *combo = new QComboBox(parent);
    return combo;
}

void MainWindow::shareClicked()
{
    secType=security;
    selectSecComboBox->clear();
    selectSecComboBox->addItems(symbolDesc->getAllDescriptions(secType));
}

void MainWindow::indexClicked()
{
    secType=index;
    selectSecComboBox->clear();
    selectSecComboBox->addItems(symbolDesc->getAllDescriptions(secType));
}

MainWindow::~MainWindow()
{
    delete symbolDesc;
}
