#include "drawwidget.h"
#include "QPainter"
#include <QMouseEvent>

#define DRAG_LOCAL_MARGIN 10

DrawWidget::DrawWidget(QWidget *parent) :
    QWidget(parent)
{
    m_marginX = 40;
    m_marginY = 30;

    m_xmin = -10.0;
    m_xmax = 10.0;
    m_ymin = -10.0;
    m_ymax = 10.0;

    m_dataPointUpdate = true;

    // Options
    m_highlightPoints = false;

    int_setCursorX(0,0);
    int_setCursorX(1,0);
    int i;
    for (i = 0; i < CURSOR_NUM; i++)
    {
        m_cursorDragged[i] = false;
    }
    setMouseTracking(true);
}

DrawWidget::~DrawWidget()
{
}

void DrawWidget::setXmin(hfloat val)
{
    m_xmin = val;
    m_dataPointUpdate = true;
}

void DrawWidget::setXmax(hfloat val)
{
    m_xmax = val;
    m_dataPointUpdate = true;
}

void DrawWidget::setYmin(hfloat val)
{
    m_ymin = val;
    m_dataPointUpdate = true;
}

void DrawWidget::setYmax(hfloat val)
{
    m_ymax = val;
    m_dataPointUpdate = true;
}

void DrawWidget::setMarginX(int marginX)
{
    m_marginX = marginX;
}

void DrawWidget::setMarginY(int marginY)
{
    m_marginY = marginY;
}

void DrawWidget::paintEvent(QPaintEvent *)
{
    QPainter p(this);

    int i;

    int w = geometry().width();
    int h = geometry().height();

    int fontH = p.fontMetrics().boundingRect("0").height();

    m_marginX = getXMarginRequiredByLabels(&p) + 5;

    int top = m_marginY;
    int left = m_marginX;
    int bottom = h - m_marginY;
    int right = w - m_marginX;

    // Draw rect
    m_drawRect = QRectHF(QPoint(left,top),QPoint(right,bottom));
    m_xFactor = (m_drawRect.rightHF() - m_drawRect.leftHF())/(m_xmax-m_xmin); // Speed up plot
    m_yFactor = (m_drawRect.bottomHF() - m_drawRect.topHF())/(m_ymax-m_ymin); // Speed up plot
    p.setBrush(QBrush(QColor(255,255,255)));
    p.drawRect(m_drawRect);

    // Draw vertical elements
    int fontW;
    int delta = m_drawRect.width() / 10;
    hfloat deltaLabelX = (m_xmax-m_xmin)/10;
    hfloat deltaLabelY = (m_ymax-m_ymin)/10;
    for (i = 0;  i < 11; i++)
    {
        int x = left+(delta*i);
        // Vertical Axe
        if (i == 5)
        {
            p.setPen(QPen(Qt::SolidLine));
        }
        else
        {
            p.setPen(QPen(Qt::DotLine));
        }
        // Grid
        if ((i > 0) && (i < 10))
        {
            p.drawLine(x,top,x,bottom);
        }
        // Orizontal labels
        hfloat tmp = m_xmin + (deltaLabelX * i);
        QString label = tmp.toString("%.2Rf");
        fontW = p.fontMetrics().width(label);
        p.drawText(x-(fontW/2),bottom+fontH,label);
    }

    // Draw orizzontal elements
    delta = m_drawRect.height() / 10;
    for (i = 0;  i < 11; i++)
    {
        int y = top+(delta*i);
        // Orizontal axe
        if (i == 5)
        {
            p.setPen(QPen(Qt::SolidLine));
        }
        else
        {
            p.setPen(QPen(Qt::DotLine));
        }
        // Grid
        if ((i > 0) && (i < 10))
        {
            p.drawLine(left,y,right,y);
        }
        // Vertical labels
        hfloat tmp = m_ymax - (deltaLabelY * i);
        QString label = tmp.toString("%.2Rf");
        fontW = p.fontMetrics().width(label);
        p.drawText(left-fontW-5,y+(fontH/4),label);
    }

    // Draw points
    if (m_dataPointUpdate)
    {
        updateDataPoint();
    }
    p.setClipRegion(QRegion(m_drawRect));
    p.setPen(QPen(Qt::SolidLine));
    for (i = 0; i < m_points.count() - 1; i++)
    {
        QPoint la = m_pointsFloat.at(i);
        QPoint lb = m_pointsFloat.at(i+1);
        p.drawLine(la.x(),la.y(),lb.x(),lb.y());
        if (m_highlightPoints)
        {
            p.drawEllipse(lb,3,3);
        }
    }

    // Draw cursor
    p.setPen(QPen(QColor(Qt::darkBlue)));
    for (i = 0; i < CURSOR_NUM; i++)
    {
        QPoint la = fromGlobalToLocal(HPoint(m_cursorX[i],m_ymax));
        QPoint lb = fromGlobalToLocal(HPoint(m_cursorX[i],m_ymin));
        p.drawLine(la.x(),la.y(),lb.x(),lb.y());
        QPoint lc = fromGlobalToLocal(HPoint(m_cursorX[i],m_cursorY[i]));
        p.setPen(QPen(QColor(Qt::red)));
        p.drawEllipse(QPoint(lc.x(),lc.y()),2,2);
    }
}

int DrawWidget::getXMarginRequiredByLabels(QPainter* p)
{
    int fontW;
    int maxFontWidth = 0;
    int i;
    hfloat deltaLabelY = (m_ymax-m_ymin)/10;
    for (i = 0;  i < 11; i++)
    {
        // Vertical labels
        hfloat tmp = m_ymax - (deltaLabelY * i);
        QString label = tmp.toString("%.2Rf");
        fontW = p->fontMetrics().width(label);
        if (fontW > maxFontWidth)
        {
            maxFontWidth = fontW;
        }
    }
    return maxFontWidth;
}

QPoint DrawWidget::fromGlobalToLocal(HPoint global)
{
    int locX, locY;
    hfloat tmp = (m_drawRect.leftHF()+m_xFactor*(global.x()-m_xmin));
    locX = (int)tmp.toInt();
    tmp = (m_drawRect.topHF()+m_yFactor*(m_ymax-global.y()));
    locY = (int)tmp.toInt();
    return QPoint(locX,locY);
}

int DrawWidget::fromGlobalToLocalX(hfloat x)
{
    int locX;
    hfloat tmp = m_drawRect.leftHF()+((m_drawRect.rightHF() - m_drawRect.leftHF())/(m_xmax-m_xmin))*(x-m_xmin);
    locX = (int)(tmp.toInt());
    return locX;
}

HPoint DrawWidget::fromLocalToGlobal(QPoint local)
{
    hfloat globX, globY;
    globX = m_xmin+((m_xmax-m_xmin)/(m_drawRect.rightHF() - m_drawRect.leftHF()))*(hfloat(local.x())-m_drawRect.leftHF());
    globY = m_ymax-((m_ymax-m_ymin)/(m_drawRect.bottomHF() - m_drawRect.topHF()))*(m_drawRect.topHF()-hfloat(local.y()));
    return HPoint(globX,globY);
}

void DrawWidget::setPoints(QVector<HPoint> p)
{
    m_points = p;
    m_dataPointUpdate = true;
}

QVector<HPoint>* DrawWidget::Points(void)
{
    return &m_points;
}

void DrawWidget::updateDataPoint()
{
    // Update points
    m_pointsFloat.clear();
    int i;
    for (i = 0; i < m_points.count(); i++)
    {
        m_pointsFloat.append(fromGlobalToLocal(m_points.at(i)));
    }
    m_dataPointUpdate = false;
}

HPoint DrawWidget::getXRange(void)
{
    HPoint retVal(0,0);
    int l = m_points.count();
    if (l > 0)
    {
        hfloat xmin,xmax;
        int i;
        HPoint p = m_points.at(0);
        xmin = p.x();
        xmax = xmin;
        for (i = 1; i < l; i ++)
        {
            p = m_points.at(i);
            hfloat x = p.x();
            if (x < xmin)
            {
                xmin = x;
            }
            if (x > xmax)
            {
                xmax = x;
            }
        }
        retVal.setX(xmin);
        retVal.setY(xmax);
    }
    return retVal;
}

HPoint DrawWidget::getYRange(void)
{
    HPoint retVal(0,0);
    int l = m_points.count();
    if (l > 0)
    {
        hfloat ymin,ymax;
        int i;
        HPoint p = m_points.at(0);
        ymin = p.y();
        ymax = ymin;
        for (i = 1; i < l; i ++)
        {
            p = m_points.at(i);
            hfloat y = p.y();
            if (y < ymin)
            {
                ymin = y;
            }
            if (y > ymax)
            {
                ymax = y;
            }
        }
        retVal.setX(ymin);
        retVal.setY(ymax);
    }
    return retVal;
}

void DrawWidget::setCursorX(int cursor, hfloat x)
{
    if (x < m_xmin)
    {
        x = m_xmin;
    }
    if (x > m_xmax)
    {
        x = m_xmax;
    }
    m_cursorX[cursor] = x;
    m_cursorY[cursor] = 0;
    int l = m_points.count();
    int i;
    HPoint p;
    for (i = 0; i < l; i++)
    {
        p = m_points.at(i);
        if (p.x() >= x)
        {
            m_cursorY[cursor] = p.y();
            break;
        }
    }
}

void DrawWidget::int_setCursorX(int cursor, hfloat x)
{
    setCursorX(cursor,x);
    emit cursorPositionChanged(cursor, m_cursorX[cursor],m_cursorY[cursor]);
}

hfloat DrawWidget::CursorY(int cursor)
{
    return m_cursorY[cursor];
}

bool DrawWidget::cursorNear(int cursor, QPoint p)
{
    bool retVal = false;
    int x = fromGlobalToLocalX(m_cursorX[cursor]);
    if (abs(p.x()-x)<DRAG_LOCAL_MARGIN)
    {
        retVal = true;
    }
    return retVal;
}

void DrawWidget::mousePressEvent(QMouseEvent* event)
{
    int i;
    for (i = 0; i < CURSOR_NUM; i++)
    {
        if ((m_cursorDragged[i] = cursorNear(i, event->pos())))
        {
            break;
        }
    }
}

void DrawWidget::mouseMoveEvent(QMouseEvent* event)
{
    bool near = false;
    int i;
    for (i = 0; i < CURSOR_NUM; i++)
    {
        if (m_cursorDragged[i])
        {
            int_setCursorX(i,fromLocalToGlobal(event->pos()).x());
            repaint();
        }
        if (cursorNear(i,event->pos()))
        {
            near = true;
        }
    }
    if (near)
    {
        setCursor(QCursor(Qt::SizeHorCursor));
    }
    else
    {
        setCursor(QCursor(Qt::ArrowCursor));
    }
}

void DrawWidget::mouseReleaseEvent(QMouseEvent* event)
{
    int i;
    for (i = 0; i < CURSOR_NUM; i++)
    {
        m_cursorDragged[i] = false;
    }
}

void DrawWidget::resizeEvent(QResizeEvent *)
{
    m_dataPointUpdate = true;
}

void DrawWidget::setHighlightPoints(bool state)
{
    if (m_highlightPoints != state)
    {
        m_highlightPoints = state;
        repaint();
    }
}
