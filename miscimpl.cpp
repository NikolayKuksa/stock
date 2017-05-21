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
    QDate date1(QDate::fromString(dateStr1,inDateFormat));
    QDate date2(QDate::fromString(dateStr2,inDateFormat));
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
    ModelDirection curModelDirection;//=calcDirection(model,QString("yyyy-mm-dd"));
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
                if(curModelDirection==direction)
                    data.push_back(value);
                else
                    data.push_front(value);
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

int modelFromCSV(QStandardItemModel *model, QString fileName)
{
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << file.errorString();
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
        //qDebug()<<endl;
    }

    //qDebug() << wordList;

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
