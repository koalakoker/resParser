#ifndef RANGE_H
#define RANGE_H

#include <QDataStream>
#include <QString>
#include "hfloat.h"

class Range
{
public:
    Range();
    Range(hfloat min,hfloat max, hfloat step);
    Range(const Range& r);

    QString toString(void) const;
    bool isValid(void) const;

public:
    hfloat m_min;
    hfloat m_max;
    hfloat m_step;

    bool operator==(const Range& op);
};

QDataStream &operator<<(QDataStream &ds, const Range &obj);
QDataStream &operator>>(QDataStream &ds, Range &obj);

#endif // RANGE_H
