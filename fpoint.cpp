#include "fpoint.h"

HPoint::HPoint()
{
    m_x = 0.0;
    m_y = 0.0;
}

HPoint::HPoint(hfloat x, hfloat y)
{
    m_x = x;
    m_y = y;
}

HPoint::HPoint(float x, float y)
{
    m_x = hfloat(x);
    m_y = hfloat(y);
}

HPoint::HPoint(const HPoint& p)
{
    m_x = p.m_x;
    m_y = p.m_y;
}

hfloat HPoint::x(void) const
{
    return m_x;
}

hfloat HPoint::y(void) const
{
    return m_y;
}

void HPoint::setX(hfloat x)
{
    m_x = x;
}

void HPoint::setY(hfloat y)
{
    m_y = y;
}

QDataStream &operator<<(QDataStream &ds, const HPoint &obj)
{
    ds << obj.x();
    ds << obj.y();
    return ds;
}

QDataStream &operator>>(QDataStream &ds, HPoint &obj)
{
    hfloat in;
    ds >> in;
    obj.setX(in);
    ds >> in;
    obj.setY(in);
    return ds;
}
