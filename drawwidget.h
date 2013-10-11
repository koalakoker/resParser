#ifndef DRAWWIDGET_H
#define DRAWWIDGET_H

#include <QWidget>
#include "hfloat.h"
#include <QVector>
#include "fpoint.h"

#define CURSOR_NUM 2

class DrawWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit DrawWidget(QWidget *parent = 0);
    ~DrawWidget();

    void setPoints(QVector<FPoint> p);
    FPoint getXRange(void);
    FPoint getYRange(void);

    void setCursorX(int cursor, float x);
    float CursorY(int cursor);

private:

    int m_marginX;
    int m_marginY;
    QRect m_drawRect;

    QVector<FPoint> m_points;

    float m_cursorX[CURSOR_NUM];
    float m_cursorY[CURSOR_NUM];
    bool m_cursorDragged[CURSOR_NUM];

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

    bool cursorNear(int cursor, QPoint p);
    void int_setCursorX(int cursor, float x);

signals:
    void cursorPositionChanged(int cursor, float x, float y);
};

#endif // DRAWWIDGET_H
