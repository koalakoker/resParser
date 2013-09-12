#include "variable.h"

Variable::Variable()
{
    m_resistor = new Resistor();
}

void Variable::setValue(double value)
{
    m_value = value;
    if (m_name.toLower()[0] == 'r')
    {
        m_resistor->setValue((Int64)(m_value));
    }
}

double Variable::Value(void)
{
    double retVal;
    if (m_name.toLower()[0] == 'r')
    {
        retVal = (double)m_resistor->Value();
    }
    else
    {
        retVal = m_value;
    }
    return retVal;
}


void Variable::setName(QString value)
{
    m_name = value;
    if (m_name.toLower()[0] == 'r')
    {
        m_resistor->setValue((Int64)(m_value));
    }
}

QString Variable::Name(void)
{
    return m_name;
}

QString Variable::ToString (void)
{
    QString retVal;
    if (m_name.toLower()[0] == 'r')
    {
        retVal = QString("[Resistor: Name={%1}, Value={%2}]").arg(Name()).arg(m_resistor->ToString());
    }
    else
    {
        retVal = QString("[Variable: Name={%1}, Value={%2}]").arg(Name()).arg(Value());
    }
    return retVal;
}
