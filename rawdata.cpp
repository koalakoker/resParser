#include "rawdata.h"

RawData::RawData() :
    QVector<HPoint>()
{
}

RawData::RawData(const RawData& obj) :
    QVector<HPoint>(obj)
{
    m_xmin = obj.m_xmin;
    m_xmax = obj.m_xmax;
    m_ymin = obj.m_ymin;
    m_ymax = obj.m_ymax;
}

RawData::RawData(const QVector<HPoint>& obj) :
    QVector<HPoint>(obj)
{
}

void RawData::append(const HPoint &t)
{
    hfloat x = t.x();
    if (!x.isNan())
    {
        if (m_xmin.isNan())
        {
            m_xmin = x;
        }
        else if (x < m_xmin)
        {
            m_xmin = x;
        }
        if (m_xmax.isNan())
        {
            m_xmax = x;
        }
        else if (x > m_xmax)
        {
            m_xmax = x;
        }
    }
    hfloat y = t.y();
    if (!y.isNan())
    {
        if (m_ymin.isNan())
        {
            m_ymin = y;
        }
        else if (y < m_ymin)
        {
            m_ymin = y;
        }
        if (m_ymax.isNan())
        {
            m_ymax = y;
        }
        else if (y > m_ymax)
        {
            m_ymax = y;
        }
    }
    QVector<HPoint>::append(t);
}

Range RawData::RawRange(void) const
{
    Range r;
    r.m_min = m_xmin;
    r.m_max = m_xmax;
    r.m_step = (m_xmax-m_xmin)/count();
    return r;
}

hfloat RawData::xMax(void) const
{
    return m_xmax;
}

hfloat RawData::xMin(void) const
{
    return m_xmin;
}

hfloat RawData::yMax(void) const
{
    return m_ymax;
}

hfloat RawData::yMin(void) const
{
    return m_ymin;
}

void RawData::setxMax(hfloat val)
{
    m_xmax = val;
}

void RawData::setxMin(hfloat val)
{
    m_xmin = val;
}

void RawData::setyMax(hfloat val)
{
    m_ymax = val;
}

void RawData::setyMin(hfloat val)
{
    m_ymin = val;
}

QDataStream &operator<<(QDataStream &ds, const RawData &obj)
{
    ds << (QVector<HPoint>)(obj);
    ds << obj.xMin();
    ds << obj.xMax();
    ds << obj.yMin();
    ds << obj.yMax();
    return ds;
}

QDataStream &operator>>(QDataStream &ds, RawData &obj)
{
    QVector<HPoint> qv;
    ds >> qv;
    obj = RawData(qv);
    hfloat in;
    ds >> in;
    obj.setxMin(in);
    ds >> in;
    obj.setxMax(in);
    ds >> in;
    obj.setyMin(in);
    ds >> in;
    obj.setyMax(in);
    return ds;
}
