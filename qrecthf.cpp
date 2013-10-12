#include "qrecthf.h"

QRectHF::QRectHF()
{
}

QRectHF::QRectHF(QPoint topleft, QPoint bottomright) : QRect(topleft,bottomright)
{
    m_topHF = hfloat(topleft.y());
    m_leftHF = hfloat(topleft.x());
    m_bottomHF = hfloat(bottomright.y());
    m_rightHF = hfloat(bottomright.x());
}

QRectHF::QRectHF(QRectHF& val) : QRect(val)
{
    m_topHF = val.topHF();
    m_bottomHF = val.bottomHF();
    m_rightHF = val.rightHF();
    m_leftHF = val.leftHF();
}

hfloat QRectHF::topHF(void)
{
    return m_topHF;
}

hfloat QRectHF::bottomHF(void)
{
    return m_bottomHF;
}

hfloat QRectHF::rightHF(void)
{
    return m_rightHF;
}

hfloat QRectHF::leftHF(void)
{
    return m_leftHF;
}
