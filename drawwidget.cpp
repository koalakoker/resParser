#include "drawwidget.h"
#include "ui_drawwidget.h"
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

    setCursor1x(0);
    setCursor2x(0);
    m_cursor1dragged = false;
    m_cursor2dragged = false;
    setMouseTracking(true);
}

DrawWidget::~DrawWidget()
{
}

void DrawWidget::paintEvent(QPaintEvent *)
{
    QPainter p(this);

    int i;

    int w = geometry().width();
    int h = geometry().height();

    int fontH = p.fontMetrics().boundingRect("0").height();


    int top = m_marginY;
    int left = m_marginX;
    int bottom = h - m_marginY;
    int right = w - m_marginX;

    // Draw rect
    m_drawRect = QRect(QPoint(left,top),QPoint(right,bottom));
    p.setBrush(QBrush(QColor(255,255,255)));
    p.drawRect(m_drawRect);

    // Draw vertical elements
    int fontW;
    int delta = m_drawRect.width() / 10;
    float deltaLabelX = (m_xmax-m_xmin)/10;
    float deltaLabelY = (m_ymax-m_ymin)/10;
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
        QString label = QString("%1").arg(m_xmin + (deltaLabelX * i));
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
        QString label = QString("%1").arg(m_ymax - (deltaLabelY * i));
        fontW = p.fontMetrics().width(label);
        p.drawText(left-fontW-5,y+(fontH/4),label);
    }

    // Draw points
    p.setPen(QPen(Qt::SolidLine));
    for (i = 0; i < m_points.count() - 1; i++)
    {
        QPoint la = fromGlobalToLocal(m_points.at(i));
        QPoint lb = fromGlobalToLocal(m_points.at(i+1));
        p.drawLine(la.x(),la.y(),lb.x(),lb.y());
    }

    // Draw cursor
    p.setPen(QPen(QColor(Qt::darkBlue)));
    QPoint la = fromGlobalToLocal(FPoint(m_cursor1x,m_ymax));
    QPoint lb = fromGlobalToLocal(FPoint(m_cursor1x,m_ymin));
    p.drawLine(la.x(),la.y(),lb.x(),lb.y());
    la = fromGlobalToLocal(FPoint(m_cursor2x,m_ymax));
    lb = fromGlobalToLocal(FPoint(m_cursor2x,m_ymin));
    p.drawLine(la.x(),la.y(),lb.x(),lb.y());
    QPoint lc = fromGlobalToLocal(FPoint(m_cursor1x,m_cursor1y));
    p.setPen(QPen(QColor(Qt::red)));
    p.drawEllipse(QPoint(lc.x(),lc.y()),2,2);
    lc = fromGlobalToLocal(FPoint(m_cursor2x,m_cursor2y));
    p.drawEllipse(QPoint(lc.x(),lc.y()),2,2);
}

QPoint DrawWidget::fromGlobalToLocal(FPoint global)
{
    int locX, locY;
    locX = (int)(m_drawRect.left()+((m_drawRect.right() - m_drawRect.left())/(m_xmax-m_xmin))*(global.x()-m_xmin));
    locY = (int)(m_drawRect.top()+((m_drawRect.bottom() - m_drawRect.top())/(m_ymax-m_ymin))*(m_ymax-global.y()));
    return QPoint(locX,locY);
}

int DrawWidget::fromGlobalToLocalX(float x)
{
    int locX;
    locX = (int)(m_drawRect.left()+((m_drawRect.right() - m_drawRect.left())/(m_xmax-m_xmin))*(x-m_xmin));
    return locX;
}

FPoint DrawWidget::fromLocalToGlobal(QPoint local)
{
    float globX, globY;
    globX = m_xmin+((m_xmax-m_xmin)/(m_drawRect.right() - m_drawRect.left()))*((float)(local.x())-m_drawRect.left());
    globY = m_ymax-((m_ymax-m_ymin)/(m_drawRect.bottom() - m_drawRect.top()))*(m_drawRect.top()-(float)(local.y()));
    return FPoint(globX,globY);
}

void DrawWidget::setPoints(QVector<FPoint> p)
{
    m_points = p;
}

FPoint DrawWidget::getXRange(void)
{
    FPoint retVal(0,0);
    int l = m_points.count();
    if (l > 0)
    {
        float xmin,xmax;
        int i;
        FPoint p = m_points.at(0);
        xmin = p.x();
        xmax = xmin;
        for (i = 1; i < l; i ++)
        {
            p = m_points.at(i);
            float x = p.x();
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

FPoint DrawWidget::getYRange(void)
{
    FPoint retVal(0,0);
    int l = m_points.count();
    if (l > 0)
    {
        float ymin,ymax;
        int i;
        FPoint p = m_points.at(0);
        ymin = p.y();
        ymax = ymin;
        for (i = 1; i < l; i ++)
        {
            p = m_points.at(i);
            float y = p.y();
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

void DrawWidget::setCursor1x(float x)
{
    m_cursor1x = x;
    m_cursor1y = 0;
    int l = m_points.count();
    int i;
    FPoint p;
    for (i = 0; i < l; i++)
    {
        p = m_points.at(i);
        if (p.x() >= x)
        {
            m_cursor1y = p.y();
            break;
        }
    }
}

float DrawWidget::Cursor2y(void)
{
    return m_cursor2y;
}

void DrawWidget::setCursor2x(float x)
{
    m_cursor2x = x;
    m_cursor2y = 0;
    int l = m_points.count();
    int i;
    FPoint p;
    for (i = 0; i < l; i++)
    {
        p = m_points.at(i);
        if (p.x() >= x)
        {
            m_cursor2y = p.y();
            break;
        }
    }
}

float DrawWidget::Cursor1y(void)
{
    return m_cursor1y;
}

bool DrawWidget::cursor1Near(QPoint p)
{
    bool retVal = false;
    int x = fromGlobalToLocalX(m_cursor1x);
    if (abs(p.x()-x)<DRAG_LOCAL_MARGIN)
    {
        retVal = true;
    }
    return retVal;
}

bool DrawWidget::cursor2Near(QPoint p)
{
    bool retVal = false;
    int x = fromGlobalToLocalX(m_cursor2x);
    if (abs(p.x()-x)<DRAG_LOCAL_MARGIN)
    {
        retVal = true;
    }
    return retVal;
}

void DrawWidget::mousePressEvent(QMouseEvent* event)
{
    if (!(m_cursor1dragged = cursor1Near(event->pos())))
    {
        m_cursor2dragged = cursor2Near(event->pos());
    }
}

void DrawWidget::mouseMoveEvent(QMouseEvent* event)
{
    if (m_cursor1dragged)
    {
        setCursor1x(fromLocalToGlobal(event->pos()).x());
        repaint();
    }
    if (m_cursor2dragged)
    {
        setCursor2x(fromLocalToGlobal(event->pos()).x());
        repaint();
    }
    if ((cursor1Near(event->pos()))||(cursor2Near(event->pos())))
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
    m_cursor1dragged = false;
    m_cursor2dragged = false;
}
