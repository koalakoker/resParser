#ifndef DRAWWIDGET_H
#define DRAWWIDGET_H

#include <QWidget>
#include "hfloat.h"
#include <QVector>
#include "fpoint.h"
#include "qrecthf.h"
#include "rawdata.h"

#define CURSOR_NUM 2

class DrawWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit DrawWidget(QWidget *parent = 0);
    ~DrawWidget();

    void setPoints(RawData *p);
    RawData* Points(void);

    HPoint getXRange(void);
    HPoint getYRange(void);

    void setCursorX(int cursor, hfloat x);
    hfloat CursorY(int cursor);

    void setXmin(hfloat val);
    void setXmax(hfloat val);
    void setYmin(hfloat val);
    void setYmax(hfloat val);

    // Options
    void setHighlightPoints(bool state);
    void setShowLabelX(bool state);
    void setShowLabelY(bool state);
    void setShowGridOrizzontal(bool state);
    void setShowGridVertical(bool state);

signals:
    void cursorPositionChanged(int cursor, hfloat x, hfloat y);

private:
    int m_marginX;
    int m_marginY;
    QRectHF m_drawRect;
    hfloat m_xFactor;
    hfloat m_yFactor;

    RawData *m_points;
    QVector<QPoint> m_pointsFloat;
    bool m_dataPointUpdate;

    hfloat m_cursorX[CURSOR_NUM];
    hfloat m_cursorY[CURSOR_NUM];
    bool m_cursorDragged[CURSOR_NUM];

    hfloat m_xmin;
    hfloat m_xmax;
    hfloat m_ymin;
    hfloat m_ymax;

    // Options
    bool m_highlightPoints;
    bool m_showLabelsX;
    bool m_showLabelsY;
    bool m_showGridOrizzontal;
    bool m_showGridVertical;

private:
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
    void resizeEvent(QResizeEvent *);

    void updateDataPoint(void);
    QPoint fromGlobalToLocal(HPoint global);
    int fromGlobalToLocalX(hfloat x);
    HPoint fromLocalToGlobal(QPoint local);

    bool cursorNear(int cursor, QPoint p);
    void int_setCursorX(int cursor, hfloat x);

    void setMarginX(int marginX);
    void setMarginY(int marginY);
    int getXMarginRequiredByLabels(QPainter *p);
};

#endif // DRAWWIDGET_H
