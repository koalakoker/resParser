#include "drawwidget.h"
#include "ui_drawwidget.h"
#include "QPainter"

DrawWidget::DrawWidget(QWidget *parent) :
    QWidget(parent)
{
    m_marginX = 40;
    m_marginY = 30;

    m_xmin = -10.0;
    m_xmax = 10.0;
    m_ymin = -10.0;
    m_ymax = 10.0;
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
        FPoint la = fromGlobalToLocal(m_points.at(i));
        FPoint lb = fromGlobalToLocal(m_points.at(i+1));
        p.drawLine(la.x(),la.y(),lb.x(),lb.y());
    }
}

FPoint DrawWidget::fromGlobalToLocal(FPoint global)
{
    float locX, locY;
    locX = m_drawRect.left()+((m_drawRect.right() - m_drawRect.left())/(m_xmax-m_xmin))*(global.x()-m_xmin);
    locY = m_drawRect.top()+((m_drawRect.bottom() - m_drawRect.top())/(m_ymax-m_ymin))*(m_ymax-global.y());
    return FPoint(locX,locY);
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
