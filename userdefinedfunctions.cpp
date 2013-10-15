#include "userdefinedfunctions.h"

userdefinedFunctions::userdefinedFunctions()
{
}

void userdefinedFunctions::setName(QString name)
{
    this->m_name = name;
}

void userdefinedFunctions::setFunctionStr(QString str)
{
    this->m_functionStr = str;
}

void userdefinedFunctions::setArgs(QStringList args)
{
    this->m_args = args;
}

QString userdefinedFunctions::Name(void)
{
    return m_name;
}

QString userdefinedFunctions::functionSrt(void)
{
    return m_functionStr;
}

QStringList userdefinedFunctions::args(void)
{
    return m_args;
}

QString userdefinedFunctions::toString(void)
{
    QString retVal = m_name + QString("(");
    int i;
    for (i = 0; i < m_args.count(); i++)
    {
        retVal.append(m_args[i]);
        if (i < m_args.count()-1)
        {
            retVal.append(",");
        }
    }
    retVal.append(")=");
    retVal.append(m_functionStr);
    if (m_points.count())
    {
        retVal.append(" RAW");
        retVal.append(m_range.toString());
    }
    return retVal;
}

void userdefinedFunctions::setRawPoints(QVector<HPoint> points)
{
    m_points = points;
}

void userdefinedFunctions::setRawRange(Range range)
{
    m_range = range;
}

bool userdefinedFunctions::RawData(void) const
{
    if (m_points.count())
        return true;
    return false;
}

QVector<HPoint> userdefinedFunctions::RawPoints(void) const
{
    return m_points;
}

Range userdefinedFunctions::RawRange(void) const
{
    return m_range;
}

void userdefinedFunctions::Save(QDataStream& out)
{
    out << m_functionStr;
    out << m_name;
    out << m_args;
    out << m_points;
    out << m_range;
}

void userdefinedFunctions::Load(QDataStream& in)
{
    in >> m_functionStr;
    in >> m_name;
    in >> m_args;
    in >> m_points;
    in >> m_range;
}
