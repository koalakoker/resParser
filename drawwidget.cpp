#include "drawwidget.h"
#include "ui_drawwidget.h"
#include "QPainter"

FPoint::FPoint()
{
    m_x = 0.0;
    m_y = 0.0;
}

FPoint::FPoint(float x, float y)
{
    m_x = x;
    m_y = y;
}

FPoint::FPoint(const FPoint& p)
{
    m_x = p.m_x;
    m_y = p.m_y;
}

float FPoint::x(void)
{
    return m_x;
}

float FPoint::y(void)
{
    return m_y;
}

void FPoint::setX(float x)
{
    m_x = x;
}

void FPoint::setY(float y)
{
    m_y = y;
}

DrawWidget::DrawWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DrawWidget)
{
    ui->setupUi(this);

    m_marginX = 20;
    m_marginY = 20;

    m_xmin = -10.0;
    m_xmax = 10.0;
    m_ymin = -10.0;
    m_ymax = 10.0;
}

DrawWidget::~DrawWidget()
{
    delete ui;
}

void DrawWidget::paintEvent(QPaintEvent *)
{
    QPainter p(this);

    int w = geometry().width();
    int h = geometry().height();

    int top = m_marginY;
    int left = m_marginX;
    int bottom = h - m_marginY;
    int right = w - m_marginX;

    // Draw grid
    m_drawRect = QRect(QPoint(left,top),QPoint(right,bottom));
    p.setBrush(QBrush(QColor(255,255,255)));
    p.drawRect(m_drawRect);

    // Draw points
    int i;
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
