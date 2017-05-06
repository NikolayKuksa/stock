#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    dateFormat=QString("yyyy-MM-dd");
    plotDateFormat=QString("d. MMMM\nyyyy");

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
    searchPane->setLayout(searchLayout);

    historicalDataPane = new QGroupBox(tr("Historical data"));
    QVBoxLayout *historicalDataLayout = new QVBoxLayout;
    historicalDataPane->setLayout(historicalDataLayout);
    historicalDataLayout->addWidget(searchPane);

    QHBoxLayout *dateSecLayout = new QHBoxLayout;
    createDateEdit(dateSecLayout);
    searchLayout->addLayout(dateSecLayout,0,0,1,1);

    createSelectSecComboBox(searchLayout,searchPane,1,0);
    createRadioButton(dateSecLayout);
    createSearchButton(dateSecLayout,searchPane);

    yahooCollector=new FinDataCollector(searchButton,this);
    connect(this,SIGNAL(needData(QString, QString,QString,QString)),
            yahooCollector,SLOT(fetchData(QString,QString,QString,QString)));


    histTableView=new QTableView;
    histTableView->setModel(yahooCollector->getDataModel());
    historicalDataLayout->addWidget(histTableView);
    connect(yahooCollector,SIGNAL(modelUpdated(int)), this, SLOT(histTableViewChanged(int)));
    qDebug()<<"0 0 index: "<<yahooCollector->getDataModel()->index(0,0).isValid();

    QHBoxLayout *exportHistTableViewDataLayout=new QHBoxLayout;
    historicalDataLayout->addLayout(exportHistTableViewDataLayout);

    histPlotButton=new QPushButton(tr("&Make Plot"),historicalDataPane);
    exportHistTableViewDataLayout->addWidget(histPlotButton);
    histPlotButton->setEnabled(false);
    connect(histPlotButton, SIGNAL(clicked()), this, SLOT(histPlotButtonClicked()));

    histDataToFileButton=new QPushButton(tr("&Export to File"),historicalDataPane);
    exportHistTableViewDataLayout->addWidget(histDataToFileButton);
    histDataToFileButton->setEnabled(false);
    connect(histDataToFileButton, SIGNAL(clicked()), this, SLOT(histDataToFileButtonClicked()));


    //==================================================
    mainLayout->addWidget(calcPane,1);
    mainLayout->addWidget(historicalDataPane,1);

    //set layout on window
    QWidget* wlayout=new QWidget;
    wlayout->setLayout(mainLayout);
    this->setCentralWidget(wlayout);

    plotFields<<"Open"<<"Close"<<"High"<<"Low"<<"Date";
    //histDataPlot=new PlotByModel(dateFormat,plotDateFormat,this);
    //pl->setModal(true);
    //pl->show();
}

void MainWindow::createDateEdit(QHBoxLayout *layout)
{
    QGroupBox *fromToDatePane = new QGroupBox;
    QHBoxLayout *fromToDateLayout = new QHBoxLayout;
    fromToDatePane->setLayout(fromToDateLayout);

    QFormLayout *fromLayout=new QFormLayout;
    QLabel *fromLabel =new QLabel("From",fromToDatePane);
    fromDateEdit = new QDateEdit(QDate::currentDate().addDays(-1500),fromToDatePane);
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
    searchButton=new QPushButton(tr("&Search"),parent);
    layout->addWidget(searchButton);
    connect(searchButton, SIGNAL(clicked()), this, SLOT(searchButtonClicked()));
}

void MainWindow::searchButtonClicked(){
    QString sec=symbolDesc->getSymbol(selectSecComboBox->currentIndex(),secType);
    QString from=fromDateEdit->date().toString(dateFormat);
    QString to=toDateEdit->date().toString(dateFormat);
/*
    QStringList headers;
    headers<<"Date";
    headers<<"Volume"<<"Adj_Close";
    yahooCollector->setDataHeaders(headers);

    */

    emit needData(from,to,sec,dateFormat);

}

void MainWindow::createSelectSecComboBox(QGridLayout *layout, QWidget *parent, int row, int column)
{
    selectSecComboBox = new QComboBox(parent);
    layout->addWidget(selectSecComboBox,row,column);
}

void MainWindow::createSelectSecComboBox(QBoxLayout *layout, QWidget *parent)
{
    selectSecComboBox = new QComboBox(parent);
    layout->addWidget(selectSecComboBox);
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

void MainWindow::histTableViewChanged(int rowCount)
{
    bool flag=rowCount!=0;
    histPlotButton->setEnabled(flag);
    histDataToFileButton->setEnabled(flag);

}

void MainWindow::histPlotButtonClicked()
{
    qDebug()<<"Plot button is clicked";
    QStandardItemModel *model=yahooCollector->getDataModel();
    QModelIndex ix= model->index(2,1);
    qDebug()<<model->headerData(0,Qt::Horizontal).toString();
    //qDebug()<<yahooCollector->getDataModel()->item(1,1)->data(Qt::DisplayRole).toString();
     //qDebug()<<"By index: "<<ix.isValid()<<"    "<<ix.data(Qt::DisplayRole).toString();
     //qDebug()<<"bad index: "<<yahooCollector->getDataModel()->index(20,0).isValid();
    /*List<QStandardItem*> items= yahooCollector->getDataModel()
    for (int i=0; i<items.length();i++)
    {
        qDebug()<<items.first()->data(Qt::DisplayRole).toString();
        items.pop_front();
    }*/
    QString format("dd.MM.yyyy");
    histDataPlot=new PlotByModel(dateFormat,plotDateFormat,"Price",this);
    QString plotTitle(selectSecComboBox->currentText());
    plotTitle.append("  ");
    plotTitle.append(fromDateEdit->date().toString(format));
    plotTitle.append("-");
    plotTitle.append(toDateEdit->date().toString(format));
    histDataPlot->setWindowTitle(plotTitle);
    histDataPlot->setDataModel(prepareModelForPlot(yahooCollector->getDataModel()));
    histDataPlot->show();
    histDataPlot->makePlot();
}

void MainWindow::histDataToFileButtonClicked()
{
    QString sep(" , ");
    QStandardItemModel *model=yahooCollector->getDataModel();
    QString format("dd.MM.yyyy");
    QString fileName(selectSecComboBox->currentText());
    fileName.append(" ");
    fileName.append(fromDateEdit->date().toString(format));
    fileName.append("-");
    fileName.append(toDateEdit->date().toString(format));
    QFile f( fileName.append(".csv"));
    if(f.open( QIODevice::WriteOnly | QFile::Text))
    {
        QTextStream ts(&f);
        QStringList newRow;
        int i=0,j=0;
        QModelIndex ix=model->index(i,j=0);
        while(ix.isValid())
        {
            newRow<<model->headerData(j,Qt::Horizontal).toString();
            ix=model->index(i,++j);
        }
        ts << newRow.join(sep)<<endl;
        newRow.clear();

        ix=model->index(i,j=0);
        while(ix.isValid())
        {
            while(ix.isValid())
            {
                newRow<<model->itemFromIndex(ix)->data(Qt::DisplayRole).toString();
                ix=model->index(i,++j);
            }
                ts << newRow.join(sep)<<endl;
            newRow.clear();
            ix=model->index(++i,j=0);
        }
        f.close();
        QString title("Historical data. Export to file");
        QString msg("The historical data has been saved to ");
        msg.append("\n");
        msg.append(fileName);
        QMessageBox msgBox(QMessageBox::Information,title,msg);
        //msgBox.setText(msg);
        msgBox.exec();
    }
}

QStandardItemModel* MainWindow::prepareModelForPlot(QStandardItemModel *model)
{
    QStandardItemModel *resModel=new QStandardItemModel;
    QList<QStandardItem *> newModelColumn;
    QString curHeader;
    int i=0,j=0,shift=0;
    QModelIndex ix=model->index(i,j);
    while(ix.isValid())
    {
        curHeader=model->headerData(j,Qt::Horizontal).toString();
        if(plotFields.contains(curHeader))
        {
            while(ix.isValid())
            {
                newModelColumn.push_front(model->itemFromIndex(ix)->clone());
                ix=model->index(++i,j);
            }
            resModel->appendColumn(newModelColumn);
            QStandardItem * headerItem=new QStandardItem(curHeader);
            resModel->setHorizontalHeaderItem(j-shift,headerItem);
            newModelColumn.clear();
        }
        else
            shift++;
        ix=model->index(i=0,++j);
    }
    return resModel;
}

MainWindow::~MainWindow()
{
    delete symbolDesc;
}
