#include "recalgwindow.h"

RecAlgWindow::RecAlgWindow(QWidget *parent) : QMainWindow(parent)
{
    //this->setLocale(QLocale(QLocale::English, QLocale::UnitedKingdom));
    xi="xi"; p_="p_";
    maxInPrice=999999;
    prec=2;
    int defN=0;
    int defT=0;
    distrTableCellFrame=true;
    calculator=new RecAlgCalc(this);


    QLocale loc(QLocale::English, QLocale::UnitedStates);
    loc.setNumberOptions(QLocale::RejectGroupSeparator);
    QDoubleValidator *doubleValidator=new QDoubleValidator(0,maxInPrice, prec, this);
    doubleValidator->setLocale(loc);

    QHBoxLayout *mainLayout=new QHBoxLayout;
    QWidget *mainWidget=new QWidget(this);
    mainWidget->setLayout(mainLayout);
    this->setCentralWidget(mainWidget);
    this->setWindowTitle(parent->windowTitle());

    QGroupBox *parametersPane=new QGroupBox(tr("Parameters for calculating"),mainWidget);
    mainLayout->addWidget(parametersPane,1);
    QFormLayout *parametersLayout=new QFormLayout;
    parametersPane->setLayout(parametersLayout);

    nEdit=new QSpinBox(parametersPane);
    parametersLayout->addRow(tr("Length of ")+xi+tr(" distribution:"),nEdit);
    nEdit->setValue(defN);
    connect(nEdit,SIGNAL(valueChanged(int)),this,SLOT(nChanged(int)));

    tEdit=new QSpinBox(parametersPane);
    parametersLayout->addRow("Time slot:",tEdit);
    tEdit->setValue(defT);
    connect(tEdit,SIGNAL(valueChanged(int)),this,SLOT(tChanged(int)));

    XoEdit=new QLineEdit(parametersPane);
    XoEdit->setValidator(doubleValidator);
    parametersLayout->addRow("Initial price of asset:",XoEdit);
    XoEdit->setText("100.00");

    aEdit=new QLineEdit(parametersPane);
    aEdit->setValidator(doubleValidator);
    parametersLayout->addRow("Left boudary:",aEdit);
    aEdit->setText("85.57");

    bEdit=new QLineEdit(parametersPane);
    bEdit->setValidator(doubleValidator);
    parametersLayout->addRow("Right boudary:",bEdit);
    bEdit->setText("120.42");

    createRadioButton(parametersLayout);

    //======================================================
    distrTable=new QTableWidget;
    mainLayout->addWidget(distrTable,3);
    tChanged(defT);
    distrTable->setColumnCount(defN);
    connect(distrTable,SIGNAL(cellChanged(int,int)),this,SLOT(distrTableChanged(int,int)));

    //======================================================
    QGroupBox *resultsPane=new QGroupBox(tr("Calculated Probabilities"),mainWidget);
    mainLayout->addWidget(resultsPane,1);
    QFormLayout *resultsLayout=new QFormLayout;
    resultsPane->setLayout(resultsLayout);

    calcButton=new QPushButton(tr("Calculate"),resultsPane);
    resultsLayout->addWidget(calcButton);
    //calcButton->setEnabled(false);
    connect(calcButton,SIGNAL(clicked()),this,SLOT(calcButtonClicked()));

    PaOutEdit=new QLineEdit(resultsPane);
    resultsLayout->addRow("Pa:",PaOutEdit);
    PaOutEdit->setReadOnly(true);

    PbOutEdit=new QLineEdit(resultsPane);
    resultsLayout->addRow("Pb:",PbOutEdit);
    PbOutEdit->setReadOnly(true);

    //QLine

    //parametersLayout->addRow("Left boudery");




}
void RecAlgWindow::addDelRowColumn(int newCount, QString operation, QString axisName)
{
    int oldRowCount=distrTable->rowCount();
    int oldColumnCount=distrTable->columnCount();
    if(operation=="add")
    {
        if(axisName=="row")
            for(int i=oldRowCount;i<newCount;i++)
                addDistrRow(i);
        else
            if(axisName=="col")
                for(int i=oldColumnCount;i<newCount;i++)
                    addDistrColumn(i);
            else
                Q_ASSERT_X(0,"addDelRowColumn","row or col must be as input");
    }
    else
        if(operation=="del")
        {
            if(axisName=="row")
                for(int i=oldRowCount-1;i>=newCount;i--)
                    distrTable->removeRow(i);
            else
                if(axisName=="col")
                    for(int i=oldColumnCount-1;i>=newCount;i--)
                        distrTable->removeColumn(i);
                else
                    Q_ASSERT_X(0,"addDelRowColumn","row or col must be as input");

        }
        else
            Q_ASSERT_X(0,"addDelRowColumn","del or add must be as input");

}

void RecAlgWindow::nChanged(int newColumnCount)
{
    int oldColumnCount=distrTable->columnCount();
    if(newColumnCount<oldColumnCount)
        addDelRowColumn(newColumnCount,"del","col");
    else
        addDelRowColumn(newColumnCount,"add","col");
}
void RecAlgWindow::tChanged(int newT)
{
    int oldRowCount=distrTable->rowCount();
    if(newT<oldRowCount)
        addDelRowColumn(newT+1,"del","row");
    else
        addDelRowColumn(newT+1,"add","row");
}

void RecAlgWindow::addDistrRow(int ix)
{
    if(hasXiStationaryDistribution && (ix>1))
        return;

    distrTable->insertRow(ix);
    QLocale loc(QLocale::English, QLocale::UnitedStates);
    loc.setNumberOptions(QLocale::RejectGroupSeparator);
    QDoubleValidator *doubleValidator;
    QTableWidgetItem *header;

    if(ix==0)//we are insertinng row fore probablilities pi
    {
        doubleValidator=new QDoubleValidator(0,1, prec+2, distrTable);
        doubleValidator->setNotation(QDoubleValidator::StandardNotation);
        doubleValidator->setLocale(loc);
        header=new QTableWidgetItem(p_.append("i"));
        for(int i=0;i<distrTable->columnCount();i++)
        {
            QLineEdit *edit = new QLineEdit(distrTable);
            edit->setValidator(doubleValidator);
            edit->setFrame(distrTableCellFrame);
            distrTable->setCellWidget(ix, i, edit);
        }
    }
    else
    {
        doubleValidator=new QDoubleValidator(-1,maxInPrice, prec+2, distrTable);
        doubleValidator->setNotation(QDoubleValidator::StandardNotation);
        doubleValidator->setLocale(loc);
        header=new QTableWidgetItem(xi+qstr(ix));

        for(int i=0;i<distrTable->columnCount();i++)
        {
            QLineEdit *edit = new QLineEdit(distrTable);
            edit->setValidator(doubleValidator);
            edit->setFrame(distrTableCellFrame);
            distrTable->setCellWidget(ix, i, edit);
        }
    }
    distrTable->setVerticalHeaderItem(ix,header);
}
void RecAlgWindow::addDistrColumn(int ix)
{
    QString headerStr=qstr(ix+1);//p_+qstr(ix);
    QLocale loc(QLocale::English, QLocale::UnitedStates);
    loc.setNumberOptions(QLocale::RejectGroupSeparator);
    QDoubleValidator *doubleValidator;
    distrTable->insertColumn(ix);
    QTableWidgetItem *header=new QTableWidgetItem(headerStr);
    distrTable->setHorizontalHeaderItem(ix,header);
    if(distrTable->rowCount()>=1)//we are insertinng cell for  probabliliti pi in new column
    {
        doubleValidator=new QDoubleValidator(0,1, prec+2, distrTable);
        doubleValidator->setNotation(QDoubleValidator::StandardNotation);
        doubleValidator->setLocale(loc);
        QLineEdit *edit = new QLineEdit(distrTable);
        edit->setValidator(doubleValidator);
        edit->setFrame(distrTableCellFrame);
        distrTable->setCellWidget(0, ix, edit);
    }
    doubleValidator=new QDoubleValidator(-1,maxInPrice, prec+2, distrTable);
    doubleValidator->setNotation(QDoubleValidator::StandardNotation);
    doubleValidator->setLocale(loc);

    for(int i=1;i<distrTable->rowCount();i++)
    {
        QLineEdit *edit = new QLineEdit(distrTable);
        edit->setValidator(doubleValidator);
        edit->setFrame(distrTableCellFrame);
        distrTable->setCellWidget(i, ix, edit);
    }
}

QString RecAlgWindow::getDistrTableCellValue(int row, int column)
{
    int rowCount=distrTable->rowCount();
    int columnCount=distrTable->columnCount();
    Q_ASSERT_X(row<rowCount,
               "getDistrTableCellValue",
               (QString("row: ")+qstr(row).append(", but rowCount: ")+qstr(rowCount)).toStdString().c_str());
    Q_ASSERT_X(column<columnCount,
               "getDistrTableCellValue",
               (QString("column: ")+qstr(column).append(", but columnCount: ")+qstr(columnCount)).toStdString().c_str());
    QWidget *cellWidget=distrTable->cellWidget(row, column);
    QLineEdit *cellEdit=qobject_cast<QLineEdit*>(cellWidget);
    return cellEdit->text();
}

QString RecAlgWindow::qstr(int number)
{
    //this is just a "shorter" cover for QString::number(int)
    return QString::number(number);
}

void RecAlgWindow::createRadioButton(QFormLayout *layout)
{
    QGroupBox *radioPane = new QGroupBox;
    QHBoxLayout *radioLayout = new QHBoxLayout;
    QRadioButton *yes = new QRadioButton("&Yes");
    QRadioButton *no = new QRadioButton("&No");
    radioLayout->addWidget(yes);
    radioLayout->addWidget(no);
    radioPane->setLayout(radioLayout);
    QString xiQuestion=QString("Does ")+xi+QString(" have stationary distribution?");
    layout->addRow(xiQuestion,radioPane);


    yes->setChecked(true);
    hasXiStationaryDistribution=true;

    connect(no, SIGNAL(clicked()), this, SLOT(noRadioButtonClicked()));
    connect(yes, SIGNAL(clicked()), this, SLOT(yesRadioButtonClicked()));
}

void RecAlgWindow::noRadioButtonClicked()
{
    hasXiStationaryDistribution=false;
    tChanged(tEdit->value());
}

void RecAlgWindow::yesRadioButtonClicked()
{
    hasXiStationaryDistribution=true;
    tChanged(1);
}

void RecAlgWindow::distrTableChanged(int, int)
{
    //distrTableIsOk=validateDistrTable();
}

bool RecAlgWindow::validateDistrTable()
{/*
    int rowN=distrTable->rowCount();
    int columnN=distrTable->columnCount();
    double curVal;
    QString curValStr;
    double pSum=0;
    for(int i=0;i<rowN;i++)
        for(int j=0;j<columnN;j++)
        {
            curValStr==getDistrTableCellValue(i,j);
            if(curValStr.isEmpty())
            {
                this->statusBar()->showMessage("All cells in greed must be filled");
                this->statusBar()->set
                return false;
            }
            curVal=curValStr.toDouble();
            qDebug()<<curVal;
            //if(i==0)//validate probablities

        }*/
    return true;
}

void RecAlgWindow::calcButtonClicked()
{
    /*QTableWidgetItem *it=new QTableWidgetItem("val");
    distrTable->setItem(0,0,it);*/
    /*
    for(int i=0;i<distrTable->rowCount();i++)
        for(int j=0;j<distrTable->columnCount();j++)
            qDebug()<<"cell("<<num(i)<<","<<num(j)<<"): "<<getDistrTableCellValue(i,j);
            */
    //validateDistrTable();
    int m=distrTable->rowCount();
    int n=distrTable->columnCount();

    QVector<double> pi(n);
    for(int j=0;j<n;j++)
        pi[j]=getDistrTableCellValue(0,j).toDouble();

    QVector<QVector<double> >xi(m-1);
    for(int i=1;i<m;i++)
        for(int j=0;j<n;j++)
            xi[i-1].append(getDistrTableCellValue(i,j).toDouble());

    calculator->setXiPi(xi,pi);
    double a,b;
    a=aEdit->text().toDouble();
    b=bEdit->text().toDouble();
    calculator->setRange(a,b);

    int t=tEdit->value();
    double Xo=XoEdit->text().toDouble();
    double pa=calculator->calcPa(Xo,t);
    double pb=calculator->calcPb(Xo,t);

    PaOutEdit->setText(QString::number(pa));
    PbOutEdit->setText(QString::number(pb));

    //calculator->myShow();
}
