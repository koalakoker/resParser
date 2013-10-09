#ifndef DRAWWIDGET_H
#define DRAWWIDGET_H

#include <QWidget>
#include "hfloat.h"
#include <QVector>
#include "fpoint.h"

class DrawWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit DrawWidget(QWidget *parent = 0);
    ~DrawWidget();

    void setPoints(QVector<FPoint> p);
    FPoint getXRange(void);
    FPoint getYRange(void);

private:

    int m_marginX;
    int m_marginY;
    QRect m_drawRect;

    QVector<FPoint> m_points;

public:
    float m_xmin;
    float m_xmax;
    float m_ymin;
    float m_ymax;

private:
    void paintEvent(QPaintEvent *);

    FPoint fromGlobalToLocal(FPoint global);
};

#endif // DRAWWIDGET_H
