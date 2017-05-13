#include "plotbymodel.h"

PlotByModel::PlotByModel(QString inDateFormat, QString outDateFormat, QString yAxisLabel,QWidget *parent)
    : QDialog(parent)
{
    this->inDateFormat=inDateFormat;
    this->outDateFormat=outDateFormat;
    this->yAxisLabel=yAxisLabel;
    plotColors.push_back(QColor(Qt::red));
    plotColors.push_back(QColor(Qt::darkGreen));
    plotColors.push_back(QColor(Qt::darkBlue));
    plotColors.push_back(QColor(Qt::black));
    plotColors.push_back(QColor(Qt::yellow));
    plotColors.push_back(QColor(Qt::cyan));

    QScreen *screen = QGuiApplication::primaryScreen();
    QRect  screenGeometry = screen->geometry();
    int height = screenGeometry.height();
    int width = screenGeometry.width();
    int nWidth = width;
    int nHeight = height/2;
    if (parent != NULL)
        setGeometry(parent->x() + parent->width()/2 - nWidth/2,
                    parent->y() + parent->height()/2 - nHeight/2,
                    nWidth, nHeight);
    else
        resize(nWidth, nHeight);

    this->setWindowFlags(Qt::Window);
    //------------------------------

    QHBoxLayout *layout=new QHBoxLayout();
    this->setLayout(layout);

    customPlot=new QCustomPlot(this);
    layout->addWidget(customPlot);



    /*
    customPlot->setLocale(QLocale(QLocale::English, QLocale::UnitedKingdom)); // period as decimal separator and comma as thousand separator
    customPlot->legend->setVisible(true);
    customPlot->xAxis->setLabel("xxx");
    customPlot->yAxis->setLabel("yyy");

    // add two new graphs and set their look:
    customPlot->addGraph();
    customPlot->graph(0)->setPen(QPen(Qt::blue)); // line color blue for first graph
    customPlot->graph(0)->setName("first !!!");
    customPlot->graph(0)->setLineStyle(QCPGraph::lsNone);
    customPlot->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, 5));

    customPlot->addGraph();
    customPlot->graph(1)->setPen(QPen(Qt::red)); // line color red for second graph

    // generate some points of data (y0 for first, y1 for second graph):
    QVector<double> x(251), y0(251), y1(251);
    for (int i=0; i<251; ++i)
    {
      x[i] = i;
      y0[i] = qExp(-i/150.0)*qCos(i/10.0); // exponentially decaying cosine
      y1[i] = qExp(-i/150.0);              // exponential envelope
    }

//QCPAxisTickerDateTime
    // pass data points to graphs:
    customPlot->graph(0)->setData(x, y0,false);
    customPlot->graph(1)->setData(x, y1,false);

    // let the ranges scale themselves so graph 0 fits perfectly in the visible area:
    customPlot->graph(0)->rescaleAxes();
    // same thing for graph 1, but only enlarge ranges (in case graph 1 is smaller than graph 0):
    customPlot->graph(1)->rescaleAxes(true);//rescaleAxes(true)
    // Note: we could have also just called customPlot->rescaleAxes(); instead
    // Allow user to drag axis ranges with mouse, zoom with mouse wheel and select graphs by clicking:
    customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
    */




}

void PlotByModel::setDataModel(QStandardItemModel *model)
{
    dataModel=model;
}

void PlotByModel::makePlot()
{
    customPlot->setLocale(QLocale(QLocale::English, QLocale::UnitedKingdom));
    int xAxisColumn=0;

    QString dateStr;
    int i=0;
    QModelIndex ix= dataModel->index(i,xAxisColumn);
    QVector<double> xAxisData;
    while(ix.isValid())
    {
        dateStr=dataModel->itemFromIndex(ix)->data(Qt::DisplayRole).toString();
        xAxisData.push_back(QDateTime(QDate::fromString(dateStr,inDateFormat)).toTime_t());
        ix=dataModel->index(++i,xAxisColumn);
    }

    double Ymin=9999999,Ymax=0,tmp;
    int xlen=xAxisData.length();

    int j=1;
    ix=dataModel->index(0,j);
    while(ix.isValid())
    {
        customPlot->addGraph();
        customPlot->graph()->setLineStyle(QCPGraph::lsLine);
        customPlot->graph()->setPen(plotColors.at(j-1));
        customPlot->graph()->setName(dataModel->headerData(j,Qt::Horizontal).toString());
        QVector<double> yAxisData(xlen);
        for (int i=0; i<xlen; i++)
        {
            //double tmp= dataModel->item(i,j)->data(Qt::DisplayRole).toDouble();
            //qDebug()<<"tmp: "<<tmp<<"  j="<<j;
            yAxisData[i]=dataModel->item(i,j)->data(Qt::DisplayRole).toDouble();;
        }
        customPlot->graph()->setData(xAxisData,yAxisData,true);
        customPlot->graph()->rescaleAxes(true);
        tmp=*std::min_element(yAxisData.constBegin(), yAxisData.constEnd());
        Ymin=tmp<Ymin?tmp:Ymin;
        tmp=*std::max_element(yAxisData.constBegin(), yAxisData.constEnd());
        Ymax=tmp>Ymax?tmp:Ymax;

        ix=dataModel->index(0,++j);
    }

    QSharedPointer<QCPAxisTickerDateTime> dateTicker(new QCPAxisTickerDateTime);
    dateTicker->setDateTimeFormat(outDateFormat);
    customPlot->xAxis->setTicker(dateTicker);
    // configure left axis text labels:
    // set a more compact font size for bottom and left axis tick labels:
    customPlot->xAxis->setTickLabelFont(QFont(QFont().family(), 8));

    // set axis labels:
    customPlot->xAxis->setLabel(dataModel->headerData(0,Qt::Horizontal).toString());
    customPlot->yAxis->setLabel(yAxisLabel);

    // set axis ranges to show all data:
    customPlot->xAxis->setRange(xAxisData.at(0), xAxisData.at(xlen-1));
    customPlot->yAxis->setRange(Ymin-yScaleMinMinus, Ymax+yScaleMaxPlus);

    customPlot->legend->setVisible(true);
    customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
}

PlotByModel::~PlotByModel()
{
    delete customPlot;
}

void PlotByModel::setYscaleRange(int maxPlus, int minMinus)
{
    this->yScaleMaxPlus=maxPlus;
    this->yScaleMinMinus=minMinus;
}
