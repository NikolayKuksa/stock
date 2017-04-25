#include "secsymboldescription.h"

SecSymbolDescription::SecSymbolDescription()
{
    addSecurity("AAPL","Apple Inc.",security);
    addSecurity("AMZN","Amazon.com, Inc.",security);
    addSecurity("INTC","Intel Corporation",security);
    addSecurity("AMD","Advanced Micro Devices, Inc. (AMD)",security);
    addSecurity("IBM","International Business Machines Corporation (IBM)",security);
    addSecurity("AAL","American Airlines Group Inc.",security);
    addSecurity("KO","The Coca-Cola Company",security);
    addSecurity("GM","General Motors Company",security);
    addSecurity("TM","Toyota Motor Corporation",security);
    //addSecurity("symbol","desc",security);
    addSecurity("ESRX","Express Scripts Holding Company",security);
    addSecurity("AA","Alcoa Corporation",security);
    addSecurity("PZRX","PhaseRx",security);
    addSecurity("TMUS","T-Mobile US, Inc.",security);
    addSecurity("MU","Micron Technology, Inc.",security);
    addSecurity("FUEL","Rocket Fuel Inc.",security);
    addSecurity("OPTT","Ocean Power Technologies, Inc.",security);
    addSecurity("HAS","Hasbro, Inc.",security);
    addSecurity("AKS","AK Steel Holding Corporation",security);
    addSecurity("CMG","Chipotle Mexican Grill, Inc.",security);
    addSecurity("RGSE","Real Goods Solar, Inc.",security);
    addSecurity("HEB","Hemispherx Biopharma, Inc.",security);
    addSecurity("WY","Weyerhaeuser Co.",security);
    addSecurity("WFM","Whole Foods Market, Inc.",security);
    addSecurity("BAC","Bank of America Corporation",security);
    addSecurity("GPRO","GOPRO, INC.",security);
    addSecurity("BIIB","Biogen Inc.",security);
    addSecurity("UVXY","ProShares Ultra VIX Short-Term Futures",security);
    addSecurity("ABX","Barrick Gold Corporation",security);
    addSecurity("WHR","Whirlpool Corporation",security);
    addSecurity("WDC","Western Digital Corporation",security);
    addSecurity("CAT","Caterpillar Inc.",security);
    addSecurity("NE","Noble Corporation plc",security);
    addSecurity("DB","Deutsche Bank AG",security);
    addSecurity("DOW","The Dow Chemical Company",security);
    addSecurity("ILMN","Illumina, Inc.",security);
    addSecurity("AVGO","Broadcom Limited",security);

    //=========================================
    addSecurity("^GSPC","S&P 500",index);
    addSecurity("^DJI","Dow Jones Industrial Average",index);
    addSecurity("^IXIC","NASDAQ Composite",index);
    addSecurity("^NYA","NYSE COMPOSITE (DJ)",index);
    addSecurity("^XAX","NYSE AMEX COMPOSITE INDEX",index);
    addSecurity("^RUT","RUSSELL 2000 INDEX",index);
    addSecurity("^VIX","VOLATILITY S&P 500",index);
    addSecurity("^FTSE","FTSE 100",index);
    addSecurity("^GDAXI","DAX",index);
    addSecurity("^FCHI","CAC 40",index);
    addSecurity("^STOXX50E","ESTX50 EUR P",index);
    addSecurity("^N100","EURONEXT 100",index);
    addSecurity("^BFX","BEL 20",index);
    addSecurity("^N225","Nikkei 225",index);
    addSecurity("^HSI","HANG SENG INDEX",index);
    addSecurity("^STI","STI Index",index);
    addSecurity("^AXJO","S&P/ASX 200",index);
    addSecurity("^AORD","ALL ORDINARIES",index);
    addSecurity("^BSESN","S&P BSE SENSEX",index);
    addSecurity("^JKSE","Jakarta Composite Index",index);
    addSecurity("^KLSE","FTSE Bursa Malaysia KLCI",index);
    addSecurity("^NZ50","S&P/NZX 50 INDEX GROSS",index);
    addSecurity("^KS11","KOSPI Composite Index",index);
    addSecurity("^TWII","TSEC weighted index",index);
    addSecurity("^GSPTSE","S&P/TSX Composite index",index);
    addSecurity("^BVSP","IBOVESPA",index);
    addSecurity("^MXX","IPC",index);
    addSecurity("^IPSA","IPSA SANTIAGO DE CHILE",index);
    addSecurity("^MERV","MERVAL BUENOS AIRES",index);
    addSecurity("^TA100","TA-125",index);
    addSecurity("^CASE30","EGX 30 INDEX",index);
}

int SecSymbolDescription::addSecurity(QString symbol, QString  description, SecurityType secType)
{
    if(secType==security)
    {
        securitySymbols.push_back(symbol);
        securityDescriptions.push_back(description);
        return 0;
    }
    else if(secType==index)
    {
        indexSymbols.push_back(symbol);
        indexDescriptions.push_back(description);
        return 0;
    }
    return 1;
}

QString SecSymbolDescription::getSymbol(int ix, SecurityType secType)
{
    if(secType==security)
    {
        if (ix<securitySymbols.length())
            return securitySymbols.at(ix);
    }
    else if(secType==index)
    {
        if (ix<indexSymbols.length())
            return indexSymbols.at(ix);
    }
    return QString("error!");
}

QStringList SecSymbolDescription::getAllDescriptions(SecurityType secType)
{
    if (secType==security)
        return QStringList(securityDescriptions.toList());
    else if(secType==index)
        return QStringList(indexDescriptions.toList());
    return QStringList("error!");
}
