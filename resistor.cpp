#include "resistor.h"
#include "math.h"
#include "QDebug"

Resistor::Resistor(void)
{
    resValue = 0;
}

Int64 Resistor::Value(void)
{
    return resValue;
}

void Resistor::setValue(Int64 value)
{
    resValue = value;
}


QString Resistor::ValueStr(void)


{
    return this->ToString ();
}

void Resistor::setValueStr(QString value)
{
    resValue = FromStringToValue(value);
}


QString Resistor::ToString()
{
    QString resString;
    resString = QString("%1").arg(resValue);

    int len = resString.length();
    if (len > 6)
    {
        if (resString.mid(len-6,6) == "000000")
        {
            resString = resString.mid(0,len-6);
            resString += "M";
        }
    }
    len = resString.length();
    if (len > 3)
    {
        if (resString.mid(len-3,3) == "000")
        {
            resString = resString.mid(0,len-3);
            resString += "k";
        }
    }
    return resString;
}

Int64 Resistor::FromStringToValue(QString str)
{
    Int64 retVal;
    str = str.replace("M","000000");
    str = str.replace("k","000");
    bool ok;
    retVal = str.toLong(&ok);
    if (!ok) retVal = 0;
    return retVal;
}

int* Resistor::GetE12Values(void)
{
    static int E_Values[] = {10,12,15,18,22,27,33,39,47,56,68,82};
    return E_Values;
}

QString Resistor::E12ValuesToString(void)
{
    QString retVal;
    int* values = GetE12Values();
    int i;
    for (i=0;i<12;i++)
    {
        retVal+=QString("%1").arg(values[i]);
        if (i<11)
        {
            retVal+=",";
        }
    }
    return retVal;
}

int* Resistor::GetE24Values(void)
{
    static int E_Values[] = {10,11,12,13,15,16,18,20,22,24,27,30,
                             33,36,39,43,47,51,56,62,68,75,82,91};
    return E_Values;
}

QString Resistor::E24ValuesToString(void)
{
    QString retVal;
    int* values = GetE24Values();
    int i;
    for (i=0;i<24;i++)
    {
        retVal+=QString("%1").arg(values[i]);
        if (i<23)
        {
            retVal+=",";
        }
    }
    return retVal;
}

bool Resistor::IsE12(Int64 val)
{
    int* E_Values = GetE12Values();
    return IsE (val,E_Values,12);
}

bool Resistor::IsE24(Int64 val)
{
    int* E_Values = GetE24Values();
    return IsE (val,E_Values,24);
}

int Resistor::GetTwoDigit(Int64 val)
{
    int retVal;
    if (val < 10)
    {
        retVal = (int)val;
    }
    else
    {
        QString str = QString("%1").arg(val);
        str = str.mid(0,2);
        bool ok;
        retVal = str.toLong(&ok);
        if (!ok) retVal = 0;
    }
    return retVal;
}

/* Order is the number of digit other that first two*/
int Resistor::GetOrder(Int64 val)
{
    int retVal = 0;
    QString str = QString("%1").arg(val);
    if (str.length()>2)
    {
        retVal = str.length()-2;
    }
    return (retVal);
}

bool Resistor::IsE(Int64 val,int* E_Values,int nValues)
{
    int TwoDigit = GetTwoDigit(val);

    bool found = false;
    int i,value;
    for (i = 0; i < nValues; i++)
    {
        value = E_Values[i];
        if (value == TwoDigit)
        {
            found = true;
        }
    }
    return found;
}

Int64 Resistor::ToNearE12(Int64 val)
{
    int order = GetOrder(val);
    qDebug()<<QString("Resistor::ToNearE12 - order:%1").arg(order);
    int TwoDigit = GetTwoDigit(val);
    qDebug()<<QString("Resistor::ToNearE12 - TwoDigit:%1").arg(TwoDigit);
    int* E_Values = GetE12Values();
    int E_Greater = 0;
    int E_Lesser = 0;
    int i,value;
    for (i = 0; i < 12; i++)
    {
        value = E_Values[i];
        if (value >= TwoDigit)
        {
            E_Greater = value;
            break;
        }
    }
    int j;
    for (j = 12 - 1; j > -1; j--)
    {
        if (E_Values[j] <= TwoDigit)
        {
            E_Lesser = E_Values[j];
            break;
        }
    }
    if (E_Lesser == 0)
    {
        return (Int64)(E_Greater * pow(10,order));
    }
    if (E_Greater == 0)
    {
        return (Int64)(E_Lesser * pow(10,order));
    }
    /* Calculate near */
    if ((TwoDigit-E_Lesser)<(E_Greater-TwoDigit))
    {
        return (Int64)(E_Lesser * pow(10,order));
    }
    else
    {
        return (Int64)(E_Greater * pow(10,order));
    }
}

