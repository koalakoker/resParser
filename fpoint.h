#ifndef FPOINT_H
#define FPOINT_H

class FPoint
{
private:
    float m_x, m_y;

public:
    FPoint();
    FPoint(float x, float y);
    FPoint(const FPoint& p);

    float x(void);
    float y(void);
    void setX(float x);
    void setY(float y);
};

#endif // FPOINT_H
