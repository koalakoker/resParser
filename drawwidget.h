#ifndef DRAWWIDGET_H
#define DRAWWIDGET_H

#include <QWidget>
#include "hfloat.h"
#include <QVector>
#include "fpoint.h"
#include "qrecthf.h"

#define CURSOR_NUM 2

class DrawWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit DrawWidget(QWidget *parent = 0);
    ~DrawWidget();

    void setPoints(QVector<HPoint> p);
    QVector<HPoint>* Points(void);

    HPoint getXRange(void);
    HPoint getYRange(void);

    void setCursorX(int cursor, hfloat x);
    hfloat CursorY(int cursor);

private:

    int m_marginX;
    int m_marginY;
    QRectHF m_drawRect;
    hfloat m_xFactor;
    hfloat m_yFactor;

    QVector<HPoint> m_points;

    hfloat m_cursorX[CURSOR_NUM];
    hfloat m_cursorY[CURSOR_NUM];
    bool m_cursorDragged[CURSOR_NUM];

public:
    hfloat m_xmin;
    hfloat m_xmax;
    hfloat m_ymin;
    hfloat m_ymax;

private:
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);

    QPoint fromGlobalToLocal(HPoint global);
    int fromGlobalToLocalX(hfloat x);
    HPoint fromLocalToGlobal(QPoint local);

    bool cursorNear(int cursor, QPoint p);
    void int_setCursorX(int cursor, hfloat x);

signals:
    void cursorPositionChanged(int cursor, hfloat x, hfloat y);
};

#endif // DRAWWIDGET_H
