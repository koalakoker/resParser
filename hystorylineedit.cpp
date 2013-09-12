#include "hystorylineedit.h"
#include <QKeyEvent>

HystoryLineEdit::HystoryLineEdit(QWidget *parent) :
    QLineEdit(parent)
{
}

void HystoryLineEdit::keyPressEvent(QKeyEvent* event)
{
    switch (event->key())
    {
    case Qt::Key_Up:
    {
        emit keyUpPressed();
        break;
    }
    case Qt::Key_Down:
    {
        emit keyDownPressed();
        break;
    }
    case Qt::Key_Escape:
    {
        emit keyEscPressed();
        break;
    }
    case Qt::Key_Plus:
    case Qt::Key_Minus:
    case Qt::Key_Asterisk:
    case Qt::Key_Slash:
    case Qt::Key_Colon:
    {
        emit keyOperator();
        break;
    }
    }

    QLineEdit::keyPressEvent(event);
}
