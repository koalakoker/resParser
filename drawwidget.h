#ifndef DRAWWIDGET_H
#define DRAWWIDGET_H

#include <QWidget>
#include "hfloat.h"
#include <QVector>

namespace Ui {
class DrawWidget;
}

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

class DrawWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit DrawWidget(QWidget *parent = 0);
    ~DrawWidget();

    void setPoints(QVector<FPoint> p);
    
private:
    Ui::DrawWidget *ui;

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
