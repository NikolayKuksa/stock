#include "miscdef.h"

double myRound(double arg)
{
    return round(arg);
}

QString numberFormat(QString number,int accurancy)
{
    if (number.contains('.'))
    {
        QStringList partsOfNumber=number.split(".");
        QString intPart,fraction;
        intPart=partsOfNumber.at(0);
        fraction=partsOfNumber.at(1);
        int fractionLen=fraction.length();
        if(fractionLen>accurancy)
            fraction=fraction.left(accurancy);
        else
            for (int i=0;i<accurancy-fractionLen;i++)
                fraction.append("0");
        QString tmp=intPart.append(".");
        number=tmp.append(fraction);
    }
    else
    {
        number.append(".");
        for (int i=0;i<accurancy;i++)
            number.append("0");
    }
    return number;
}

QStandardItemModel *cutModelChangeDirection(QStandardItemModel *model, QStringList headers, bool changeDirection)
{
    QStandardItemModel *resModel=new QStandardItemModel;
    QList<QStandardItem *> newModelColumn;
    QString curHeader;
    int i=0,j=0,shift=0;
    QModelIndex ix=model->index(i,j);
    while(ix.isValid())
    {
        curHeader=model->headerData(j,Qt::Horizontal).toString();
        if(headers.contains(curHeader))
        {
            while(ix.isValid())
            {
                if(changeDirection)
                    newModelColumn.push_front(model->itemFromIndex(ix)->clone());
                else
                    newModelColumn.push_back(model->itemFromIndex(ix)->clone());
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

QList<QStandardItem *> doubleVectorToModelColumn(QVector<double> values, ModelDirection direction)
{
    QList<QStandardItem *> newModelColumn;
    QStandardItem *item;
    for(int i=0;i<values.length();i++)
    {
        item=new QStandardItem(numberFormat(numberFormat(QString::number(values.at(i),'g',3))));
        if(direction==forward)
            newModelColumn.push_front(item);//change direction
        else
            newModelColumn.push_back(item);
    }
    return newModelColumn;
}

ModelDirection calcDirection(QStandardItemModel *model, QString inDateFormat,int column)
{
    QModelIndex ix=model->index(1,column);
    if(!ix.isValid())
        return forward;
    QString dateStr1=model->item(0,column)->data(Qt::DisplayRole).toString();
    QString dateStr2=model->item(1,column)->data(Qt::DisplayRole).toString();
    QDate date1=QDate::fromString(dateStr1,inDateFormat);
    QDate date2=QDate::fromString(dateStr2,inDateFormat);
    //qDebug()<<date1;
    //qDebug()<<date2;
    if(date1<=date2)
        return forward;
    else
        return back;
}

QVector<double> fetchValuesFromModel(QStandardItemModel *model, QString header, ModelDirection direction)
{
    QVector<double> data;
    double value;
    QString curHeader;
    ModelDirection curModelDirection=back;//=calcDirection(model,QString("yyyy-MM-dd"));
    //qDebug()<<"cur dir "<<curModelDirection;
    //qDebug()<<"cacl dir "<<calcDirection(model,QString("yyyy-MM-dd"));
    int i=0,j=0;
    QModelIndex ix=model->index(i,j);
    while(ix.isValid())
    {
        curHeader=model->headerData(j,Qt::Horizontal).toString();
        if(curHeader==header)
        {
            while(ix.isValid())
            {
                value=model->itemFromIndex(ix)->data(Qt::DisplayRole).toString().toDouble();
                //qDebug()<<"!!!"<<value;
                if(curModelDirection==direction)//{
                    data.push_back(value); ////qDebug()<<"push back";}
                else//{
                    data.push_front(value); ////qDebug()<<"push front";}
                ix=model->index(++i,j);
            }
            return data;
        }
        ix=model->index(i=0,++j);
    }
    return data;
}

int calcModelColumnCount(QStandardItemModel *model)
{
    int j=0;
    QModelIndex ix=model->index(0,j);
    while(ix.isValid())
        ix=model->index(0,++j);
    return j;
}

void appendValuesToModel(QStandardItemModel *model,QString header, QVector<double> values)
{
    int columnCount=calcModelColumnCount(model);
    QList<QStandardItem *> column=doubleVectorToModelColumn(values);
    model->appendColumn(column);
    QStandardItem * headerItem=new QStandardItem(header);
    model->setHorizontalHeaderItem(columnCount,headerItem);
}

double mathE(QVector<double> xi, QVector<double> pi)
{
    double res=0;
    int n=std::min(xi.length(),pi.length());
    for(int i=0;i<n;i++)
        res+=xi.at(i)*pi.at(i);
    return res;
}

int modelFromCSV(QStandardItemModel *model, QString fileName, bool reverseRowOrder)
{
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        //qDebug() << file.errorString();
        return 0;
    }
    if(model!=Q_NULLPTR)
        model->clear();
    QList<QByteArray> wordList;

    int rowCount=0;
    bool isFirstLine=true;
    bool isFirstWord;
    QString word;
    QByteArray line;
    QString newVal;
    QStringList headers;
    while (!file.atEnd()) {
        QList<QStandardItem *> newModelRow;
        line = file.readLine();
        if(!isFirstLine)
            rowCount++;
        wordList=line.split(',');
        isFirstWord=true;
        while (!wordList.isEmpty()) {
            word=wordList.first();
            word = word.simplified();
            word.replace( " ", "" );

            if(isFirstLine)
            {
                headers<<word;
            }
            else
            {
                if(!isFirstWord)
                    newVal= numberFormat(word);
                else
                    newVal=word; //Date has been read
                newModelRow<<new QStandardItem(newVal);
            }
            //qDebug()<<word;
            wordList.pop_front();
            isFirstWord=false;
        }

        if(isFirstLine)
        {
            isFirstLine=false;
            model->setHorizontalHeaderLabels(headers);
        }
        else
            model->appendRow(newModelRow);
        ////qDebug()<<endl;
    }

    ////qDebug() << wordList;

    return rowCount;
}

QString getFileNameFromFullPath(QString path, bool excludeFileExt)
{
    QString fileName=path.split("/").last();
    if(excludeFileExt){
        QStringList tmp=fileName.split(".");
        tmp.pop_back();
        fileName=tmp.join(".");
    }
    return fileName;
}

QVector<double> reverse(QVector<double> vec)
{
    int n=vec.length();
    QVector<double> rv(n);
    for(int i=0;i<n;i++)
        rv[n-(i+1)]=vec.at(i);
    return rv;
}

double mathSemiD(QVector<double> xi, QVector<double> pi, double E)
{
    double tmp;
    for(int i=0;i<xi.length();i++)
    {
        tmp=std::min(0.0,xi.at(i)-E);
        xi[i]=tmp*tmp;
    }
    return mathE(xi,pi);
}

QString PortfolioParam::toString()
{
    //qDebug()<<"ro="<<ro<<"  E="<<E<<"  D="<<D<<"  Pa="<<Pa<<"  Pb="<<Pb;
    return QString("");
}

PointForSelection PortfolioParam::toPointForSelection(QVector<bool> checkedCriteria,int id)
{
    //order of criteria: E D Pa Pb
    int n=checkedCriteria.length();
    QStringList tmpNames;
    tmpNames<<"mathE"<<"semi-D"<<"Pa"<<"Pb";
    QVector<double> tmpValues;
    tmpValues<<this->E<<this->D<<this->Pa<<this->Pb;
    QVector<bool> tmpMaxDirection;
    tmpMaxDirection<<true<<false<<true<<true;

    ////qDebug()<<"tmpValues: "<<tmpValues;
    QStringList names;
    QVector<double> values;
    QVector<bool> maxDirection;

    for(int i=0;i<n;i++)
        if(checkedCriteria.at(i)){
            names.append(tmpNames.at(i));
            values.append(tmpValues.at(i));
            maxDirection.append(tmpMaxDirection.at(i));
        }
    ////qDebug()<<"values "<<values;
    PointForSelection point(id,names.length());
    point.setCriteria(values);
    point.setCriteriaNames(names);
    point.setMaxDirection(maxDirection);

    return point;
}

/*bool PortfolioParam::dominateP(PortfolioParam p)
{
    bool flag=false;
    double ourE=this->E;
    double ourD=this->D;
    double ourPa=this->Pa;
    double ourPb=this->Pb;
    if(ourE>=p.E)
    {
        flag=flag||(ourE>p.E);
        if(ourD<=p.D)
        {
            flag=flag||(ourD<p.D);
            if(ourPa>=p.Pa)
            {
                flag=flag||(ourPa>p.Pa);
                if(ourPb>=p.Pb)
                {
                    flag=flag||(ourPb>p.Pb);
                    return flag;
                }
            }
        }
    }
    return false;
}

bool PortfolioParam::hasDominator(QVector<PointForSelection> points)
{
    PortfolioParam tmp;
    tmp.D=this->D;
    tmp.E=this->E;
    tmp.Pa=this->Pa;
    tmp.Pb=this->Pb;
    for(int i=0;i<pors.length();i++)
        if(pors[i].dominateP(tmp))
            return true;
    return false;
}*/

QVector<PortfolioParam> getParetoSet(QVector<PortfolioParam> pors,QVector<bool> lx)
{
    int n=pors.length();
    QVector<PointForSelection> points(n);
    for(int i=0;i<n;i++)
    {
        points[i]=pors[i].toPointForSelection(lx,i);
    }
    //if(points.length()>=1)
        //qDebug()<<points[1].getId()<<points[1].getCriteria(0);
    QVector<PointForSelection> paretoSetPoints;
    PointForSelection curP;
    while(!points.isEmpty())
    {
        curP=points.first();
        //qDebug()<<"curP "<<curP.getCriteria(0);
        points.pop_front();
        if(hasDominatorInVector(curP,points)||hasDominatorInVector(curP,paretoSetPoints))
            ;  //do nothing
        else
            paretoSetPoints.push_back(curP);
    }
    n=paretoSetPoints.length();
    //qDebug()<<"pareto length "<<n;
    QVector<PortfolioParam> paretoSetPorts(n);
    for(int i=0;i<n;i++)
        paretoSetPorts[i]=pors[paretoSetPoints[i].getId()];
    return paretoSetPorts;
}

QStandardItemModel *reverseRowsOrder(QStandardItemModel *model)
{
    QStandardItemModel *resModel=new QStandardItemModel;
    QList<QStandardItem *> newModelColumn;
    int i=0,j=0;
    QModelIndex ix=model->index(i,j);
    while(ix.isValid())
    {
        while(ix.isValid())
        {
            newModelColumn.push_front(model->itemFromIndex(ix)->clone());
            ix=model->index(++i,j);
        }
        resModel->appendColumn(newModelColumn);
        newModelColumn.clear();
        //copy headers to reversed model
        resModel->setHorizontalHeaderItem(j,model->horizontalHeaderItem(j)->clone());
        ix=model->index(i=0,++j);
    }
    return resModel;
}

double roundPrec(double num, int prec)
{
    double mul=1;
    for(int i=1;i<=prec;i++)
        mul*=10;
    num=num*mul;
    int tmp=static_cast<int>(num);
    return tmp/mul;
}

void moveData(QStandardItemModel *from, QStandardItemModel *to)
{
    QList<QStandardItem *> newModelColumn;
    to->clear();
    int i=0,j=0;
    QModelIndex ix=from->index(i,j);
    while(ix.isValid())
    {
        while(ix.isValid())
        {
            newModelColumn.push_back(from->itemFromIndex(ix)->clone());
            ix=from->index(++i,j);
        }
        to->appendColumn(newModelColumn);
        newModelColumn.clear();
        //copy headers to reversed model
        to->setHorizontalHeaderItem(j,from->horizontalHeaderItem(j)->clone());
        ix=from->index(i=0,++j);
    }
}

bool hasDominatorInVector(PointForSelection point, QVector<PointForSelection> points)
{
    //qDebug()<<"hasDominator call start------------ ";
    //qDebug()<<"point "<<point.getCriteria(0);
    QVector<double> tmp; //for debug reason in Release mode
    for(int i=0;i<points.length();i++)
    {
        tmp.append(points.at(i).getCriteria(0)); //for debug reason in Release mode
        if(points.at(i)>point)
        {
            //qDebug()<<"vector "<<tmp;
            //qDebug()<<"result "<<true;
            //qDebug()<<"hasDominator call end------------ ";
            return true;
        }

    }
    //qDebug()<<"vector "<<tmp;
    //qDebug()<<"result"<<false;
    //qDebug()<<"hasDominator call end------------ ";
    return false;
}
