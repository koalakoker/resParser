#ifndef QRECTHF_H
#define QRECTHF_H

#include <QRect>
#include "hfloat.h"

class QRectHF : public QRect
{
public:
    QRectHF();
    QRectHF(QPoint topleft, QPoint bottomright);
    QRectHF(QRectHF &val);

    hfloat topHF(void);
    hfloat bottomHF(void);
    hfloat rightHF(void);
    hfloat leftHF(void);

private:
    hfloat m_topHF;
    hfloat m_bottomHF;
    hfloat m_rightHF;
    hfloat m_leftHF;
};

#endif // QRECTHF_H
