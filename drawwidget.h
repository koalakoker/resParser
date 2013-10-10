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

    void setCursor1x(float x);
    float Cursor1y(void);
    void setCursor2x(float x);
    float Cursor2y(void);

private:

    int m_marginX;
    int m_marginY;
    QRect m_drawRect;

    QVector<FPoint> m_points;

    float m_cursor1x;
    float m_cursor1y;
    bool m_cursor1dragged;

    float m_cursor2x;
    float m_cursor2y;
    bool m_cursor2dragged;

public:
    float m_xmin;
    float m_xmax;
    float m_ymin;
    float m_ymax;

private:
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);

    QPoint fromGlobalToLocal(FPoint global);
    int fromGlobalToLocalX(float x);
    FPoint fromLocalToGlobal(QPoint local);

    bool cursor1Near(QPoint p);
    bool cursor2Near(QPoint p);
};

#endif // DRAWWIDGET_H
