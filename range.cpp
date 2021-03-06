#include "range.h"

Range::Range()
{
    m_min = 0;
    m_max = 0;
    m_step = 0;
}

Range::Range(hfloat min, hfloat max, hfloat step)
{
    m_min = min;
    m_max = max;
    m_step = step;
}

Range::Range(const Range& r)
{
    m_min = r.m_min;
    m_max = r.m_max;
    m_step = r.m_step;
}

QString Range::toString(void) const
{
    QString retVal;
    if ((m_min==0)&&(m_max==0)&&(m_step==0))
    {
        retVal = "";
    }
    else if ((m_min.isNan())||(m_min.isNan())||m_step.isNan())
    {
        retVal = "";
    }
    else
    {
        retVal = QString("[%1:%2:%3]").arg(m_min.toString("%.5Rg")).arg(m_max.toString("%.5Rg")).arg(m_step.toString("%.5Rg"));
    }
    return retVal;
}

bool Range::isValid(void) const
{
    bool retVal = true;
    if ((m_min==0)&&(m_max==0)&&(m_step==0))
    {
        retVal = false;
    }
    if (((m_min.isNan())||(m_min.isNan())||m_step.isNan()))
    {
        retVal = false;
    }
    return retVal;
}

bool Range::operator==(const Range& op)
{
    return ((op.m_min == this->m_min)&&(op.m_max == this->m_max)&&(op.m_step == this->m_step));
}

QDataStream &operator<<(QDataStream &ds, const Range &obj)
{
    ds << obj.m_min;
    ds << obj.m_max;
    ds << obj.m_step;
    return ds;
}

QDataStream &operator>>(QDataStream &ds, Range &obj)
{
    hfloat in;
    ds >> in;
    obj.m_min = hfloat(in);
    ds >> in;
    obj.m_max = hfloat(in);
    ds >> in;
    obj.m_step = hfloat(in);
    return ds;
}
