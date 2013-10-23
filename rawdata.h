#ifndef RAWDATA_H
#define RAWDATA_H

#include <QVector>
#include "fpoint.h"
#include "range.h"

class RawData : public QVector<HPoint>
{
public:
    RawData();
    RawData(const RawData& obj);
    RawData(const QVector<HPoint>& obj);
    void append(const HPoint &t);
    void updateRange(void);
    Range RawRange(void) const;

    hfloat xMax(void) const;
    hfloat xMin(void) const;
    hfloat yMax(void) const;
    hfloat yMin(void) const;

    void setxMax(hfloat val);
    void setxMin(hfloat val);
    void setyMax(hfloat val);
    void setyMin(hfloat val);

private:
    hfloat m_xmax;
    hfloat m_ymax;
    hfloat m_xmin;
    hfloat m_ymin;

    void updateLimitFromPoint(const HPoint &t);
};

QDataStream &operator<<(QDataStream &ds, const RawData &obj);
QDataStream &operator>>(QDataStream &ds, RawData &obj);

#endif // RAWDATA_H
