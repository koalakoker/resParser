#include "variable.h"
#include "global.h"

Variable::Variable()
{
}

void Variable::setValue(hfloat value)
{
    m_value = value;
    if (m_name.toLower()[0] == 'r')
    {
        m_resistor.setValue((Int64)(m_value.toString("%.50Rf").toFloat()));
    }
}

hfloat Variable::Value(void)
{
    hfloat retVal;
    if (m_name.toLower()[0] == 'r')
    {
        retVal = hfloat(QString("%1").arg(m_resistor.Value()));
    }
    else
    {
        retVal = m_value;
    }
    return retVal;
}


void Variable::setName(QString name)
{
    m_name = name;
    if (m_name.toLower()[0] == 'r')
    {
        m_resistor.setValue((Int64)(m_value.toString("%.50Rf").toFloat()));
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
        retVal = QString("[Resistor: Name={%1}, Value={%2}]").arg(Name()).arg(m_resistor.ToString());
    }
    else
    {
        retVal = QString("[Variable: Name={%1}, Value={%2}]").arg(Name()).arg(Value().toString("%.50Rg"));
    }
    return retVal;
}

void Variable::Save(QDataStream& out)
{
    out << m_name;
    out << m_value.toString(FIXED_MAX_PRECISION);
}

void Variable::Load(QDataStream& in)
{
    in >> m_name;
    QString str;
    in >> str;
    m_value = hfloat(str);
}
