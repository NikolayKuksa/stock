#include "importviewdatawindow.h"

ImportViewDataWindow::ImportViewDataWindow(QWidget *parent)
    : QMainWindow(parent)
{
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect  screenGeometry = screen->geometry();
    int height = screenGeometry.height()/2;
    int width = screenGeometry.width()/2;
    setGeometry(width/3,
                height/3,
                width, height);

    dateFormat=QString("yyyy-MM-dd");
    plotDateFormat=QString("d. MMMM\nyyyy");
    days=7;

    this->resize(QDesktopWidget().availableGeometry(this).size() * 0.7);
    mainLayout=new QBoxLayout(QBoxLayout::LeftToRight);
    this->setWindowTitle(parent->windowTitle()+QString("Import, View and Export data"));

    symbolDesc = new SecSymbolDescription;
    //==================================================
    calcPane = new QGroupBox(tr("Calculation"));
    QHBoxLayout *controlLayout = new QHBoxLayout;
    //add widgets here
    //calcPane->setLayout(controlLayout);
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

    //select fields for buiding plot
    createCheckBoxesPlotFields(historicalDataLayout);

    //hist table grid-----------
    histTableView=new QTableView;
    histTableView->setModel(yahooCollector->getDataModel());
    historicalDataLayout->addWidget(histTableView);
    connect(yahooCollector,SIGNAL(modelUpdated(int)), this, SLOT(histTableViewChanged(int)));

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

    finCalculator=new FinCalculator(this);
    finCalculator->setModel(yahooCollector->getDataModel());
    connect(yahooCollector,SIGNAL(modelUpdated(int)), finCalculator, SLOT(recalculate(int)));
    connect(finCalculator,SIGNAL(recalculated(int)),this,SLOT(updateCalcGrid(int)));

    QVBoxLayout *calcPaneLayout = new QVBoxLayout;
    calcPane->setLayout(calcPaneLayout);

    calculatedDataTableView=new QTableView(calcPane);
    calculatedDataTableView->setModel(finCalculator->getModel());
    calcPaneLayout->addWidget(calculatedDataTableView);

    returnPlotButton=new QPushButton("Make plot for return value",calcPane);
    returnPlotButton->setEnabled(false);
    connect(returnPlotButton,SIGNAL(clicked()),this,SLOT(returnPlotButtonClicked()));
    calcPaneLayout->addWidget(returnPlotButton);





    //===================================

    //set layout on window
    QWidget* wlayout=new QWidget;
    wlayout->setLayout(mainLayout);
    this->setCentralWidget(wlayout);

    histGridHasData=false;
}

void ImportViewDataWindow::createDateEdit(QHBoxLayout *layout)
{
    QGroupBox *fromToDatePane = new QGroupBox;
    QHBoxLayout *fromToDateLayout = new QHBoxLayout;
    fromToDatePane->setLayout(fromToDateLayout);

    QFormLayout *fromLayout=new QFormLayout;
    QLabel *fromLabel =new QLabel("From",fromToDatePane);
    fromDateEdit = new QDateEdit(QDate::currentDate().addDays(-1500),fromToDatePane);
    connect(fromDateEdit,SIGNAL(dateChanged(QDate)),this,SLOT(fromDateEditValidate(QDate)));
    fromLayout->addRow(fromLabel,fromDateEdit);
    fromDateEdit->setDisplayFormat(dateFormat);

    QFormLayout *toLayout=new QFormLayout;
    QLabel *toLabel = new QLabel("To",fromToDatePane);
    toDateEdit = new QDateEdit(QDate::currentDate(),toLabel);
    connect(toDateEdit,SIGNAL(dateChanged(QDate)),this,SLOT(toDateEditValidate(QDate)));
    toLayout->addRow(toLabel,toDateEdit);
    toDateEdit->setDisplayFormat(dateFormat);

    //dateEdit->setMinimumDate(QDate::currentDate().addDays(-365));
    //dateEdit->setMaximumDate(QDate::currentDate().addDays(365));
    fromToDateLayout->addLayout(fromLayout);
    fromToDateLayout->addLayout(toLayout);

    layout->addWidget(fromToDatePane);
}

void ImportViewDataWindow::createRadioButton(QHBoxLayout *layout)
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

void ImportViewDataWindow::createSearchButton(QHBoxLayout *layout, QWidget *parent)
{
    searchButton=new QPushButton(tr("&Search"),parent);
    layout->addWidget(searchButton);
    connect(searchButton, SIGNAL(clicked()), this, SLOT(searchButtonClicked()));
}

void ImportViewDataWindow::searchButtonClicked(){
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

void ImportViewDataWindow::createSelectSecComboBox(QGridLayout *layout, QWidget *parent, int row, int column)
{
    selectSecComboBox = new QComboBox(parent);
    layout->addWidget(selectSecComboBox,row,column);
}

void ImportViewDataWindow::createSelectSecComboBox(QBoxLayout *layout, QWidget *parent)
{
    selectSecComboBox = new QComboBox(parent);
    layout->addWidget(selectSecComboBox);
}

void ImportViewDataWindow::shareClicked()
{
    secType=security;
    selectSecComboBox->clear();
    selectSecComboBox->addItems(symbolDesc->getAllDescriptions(secType));
}

void ImportViewDataWindow::indexClicked()
{
    secType=index;
    selectSecComboBox->clear();
    selectSecComboBox->addItems(symbolDesc->getAllDescriptions(secType));
}

void ImportViewDataWindow::histTableViewChanged(int rowCount)
{
    qDebug()<<rowCount;
    histGridHasData=rowCount!=0;
    bool plotButtonEnabled=histGridHasData&&atLeastOnePlotFieldsChecked;
    histPlotButton->setEnabled(plotButtonEnabled);
    histDataToFileButton->setEnabled(histGridHasData);
}

void ImportViewDataWindow::histPlotButtonClicked()
{
    QString format("dd.MM.yyyy");
    histDataPlot=new PlotByModel(dateFormat,plotDateFormat,"Price",this);
    histDataPlot->setYscaleRange(20,10);
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

void ImportViewDataWindow::returnPlotButtonClicked()
{
    QString format("dd.MM.yyyy");
    PlotByModel *plot=new PlotByModel(dateFormat,plotDateFormat,"Historical return of asset",this);
    QString plotTitle(selectSecComboBox->currentText());
    plotTitle.append("  ");
    //plotTitle.append("");
    plotTitle.append(fromDateEdit->date().toString(format));
    plotTitle.append("-");
    plotTitle.append(toDateEdit->date().toString(format));
    plot->setWindowTitle(plotTitle);
    QStringList headers;
    headers<<"Date"<<"Return";
    QStandardItemModel *plotModel=cutModelChangeDirection(finCalculator->getModel(),headers,back);
    plot->setDataModel(plotModel);
    plot->setYscaleRange(0.5,0.5);
    plot->show();
    plot->makePlot();
}

void ImportViewDataWindow::histDataToFileButtonClicked()
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

void ImportViewDataWindow::fromDateEditValidate(QDate newDate)
{
    if (newDate.addDays(days)>toDateEdit->date())
        fromDateEdit->setDate(toDateEdit->date().addDays(-days));
}

void ImportViewDataWindow::toDateEditValidate(QDate newDate)
{
    if (fromDateEdit->date()>=newDate.addDays(-days))
        toDateEdit->setDate(fromDateEdit->date().addDays(days));
}

void ImportViewDataWindow::priceFieldsForPlotCheckBoxClicked(bool isChecked)
{
    if (isChecked)
    {
        plotFieldsCount++;
        histPlotButton->setEnabled(histGridHasData);
        atLeastOnePlotFieldsChecked=true;
    }
    else
    {
        plotFieldsCount--;
        if(plotFieldsCount==0)
            histPlotButton->setEnabled(atLeastOnePlotFieldsChecked=false);
    }
}

void ImportViewDataWindow::selectAllPlotFieldsButtonClicked()
{
    bool b=true;
    openPriceCheckBox->setChecked(b);
    closePriceCheckBox->setChecked(b);
    lowPriceCheckBox->setChecked(b);
    highPriceCheckBox->setChecked(b);
    plotFieldsCount=4;
    histPlotButton->setEnabled(histGridHasData);
    atLeastOnePlotFieldsChecked=b;
}

void ImportViewDataWindow::deselectAllPlotFieldsButtonClicked()
{
    bool b=false;
    openPriceCheckBox->setChecked(b);
    closePriceCheckBox->setChecked(b);
    lowPriceCheckBox->setChecked(b);
    highPriceCheckBox->setChecked(b);
    plotFieldsCount=0;
    histPlotButton->setEnabled(atLeastOnePlotFieldsChecked=b);
}

QStandardItemModel* ImportViewDataWindow::prepareModelForPlot(QStandardItemModel *model)
{
    QStringList plotFields=getHistPlotFields();
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

QStringList ImportViewDataWindow::getHistPlotFields()
{
    QStringList plotFields;
    plotFields<<"Date";
    if(openPriceCheckBox->isChecked())
        plotFields<<"Open";
    if(closePriceCheckBox->isChecked())
        plotFields<<"Close";
    if(lowPriceCheckBox->isChecked())
        plotFields<<"Low";
    if(highPriceCheckBox->isChecked())
        plotFields<<"High";
    return plotFields;
}

void ImportViewDataWindow::createCheckBoxesPlotFields(QVBoxLayout *layout)
{
    QHBoxLayout *plotFieldsLayout=new QHBoxLayout;
    QGroupBox *plotFieldsPane = new QGroupBox(tr("Which prices would you like to see on plot?"));
    plotFieldsPane->setLayout(plotFieldsLayout);
    layout->addWidget(plotFieldsPane);

    openPriceCheckBox=new QCheckBox(tr("Open"),plotFieldsPane);
    plotFieldsLayout->addWidget(openPriceCheckBox);
    connect(openPriceCheckBox,SIGNAL(clicked(bool)),this,SLOT(priceFieldsForPlotCheckBoxClicked(bool)));

    closePriceCheckBox=new QCheckBox(tr("Close"),plotFieldsPane);
    plotFieldsLayout->addWidget(closePriceCheckBox);
    connect(closePriceCheckBox,SIGNAL(clicked(bool)),this,SLOT(priceFieldsForPlotCheckBoxClicked(bool)));

    lowPriceCheckBox=new QCheckBox(tr("Low"),plotFieldsPane);
    plotFieldsLayout->addWidget(lowPriceCheckBox);
    connect(lowPriceCheckBox,SIGNAL(clicked(bool)),this,SLOT(priceFieldsForPlotCheckBoxClicked(bool)));

    highPriceCheckBox=new QCheckBox(tr("High"),plotFieldsPane);
    plotFieldsLayout->addWidget(highPriceCheckBox);
    connect(highPriceCheckBox,SIGNAL(clicked(bool)),this,SLOT(priceFieldsForPlotCheckBoxClicked(bool)));

    QPushButton *selectAllPlotFieldsButton=new QPushButton(tr("Select all"),plotFieldsPane);
    plotFieldsLayout->addWidget(selectAllPlotFieldsButton);
    connect(selectAllPlotFieldsButton,SIGNAL(clicked(bool)),this,SLOT(selectAllPlotFieldsButtonClicked()));

    QPushButton *deselectAllPlotFieldsButton=new QPushButton(tr("Deselect all"),plotFieldsPane);
    plotFieldsLayout->addWidget(deselectAllPlotFieldsButton);
    connect(deselectAllPlotFieldsButton,SIGNAL(clicked(bool)),this,SLOT(deselectAllPlotFieldsButtonClicked()));

    openPriceCheckBox->setChecked(true);
    plotFieldsCount=1;
    atLeastOnePlotFieldsChecked=true;
}

void ImportViewDataWindow::updateCalcGrid(int rowCount)
{
    returnPlotButton->setEnabled(rowCount!=0);
    calculatedDataTableView->setModel(finCalculator->getModel());
}

ImportViewDataWindow::~ImportViewDataWindow()
{
    delete symbolDesc;
}
