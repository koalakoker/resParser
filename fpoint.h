#ifndef FPOINT_H
#define FPOINT_H

#include "hfloat.h"

class HPoint
{
private:
    hfloat m_x, m_y;

public:
    HPoint();
    HPoint(hfloat x, hfloat y);
    HPoint(float x, float y);
    HPoint(const HPoint& p);

    hfloat x(void) const;
    hfloat y(void) const;
    void setX(hfloat x);
    void setY(hfloat y);
};

QDataStream &operator<<(QDataStream &ds, const HPoint &obj);
QDataStream &operator>>(QDataStream &ds, HPoint &obj);

#endif // FPOINT_H
