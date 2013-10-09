#include "fpoint.h"

FPoint::FPoint()
{
    m_x = 0.0;
    m_y = 0.0;
}

FPoint::FPoint(float x, float y)
{
    m_x = x;
    m_y = y;
}

FPoint::FPoint(const FPoint& p)
{
    m_x = p.m_x;
    m_y = p.m_y;
}

float FPoint::x(void)
{
    return m_x;
}

float FPoint::y(void)
{
    return m_y;
}

void FPoint::setX(float x)
{
    m_x = x;
}

void FPoint::setY(float y)
{
    m_y = y;
}
