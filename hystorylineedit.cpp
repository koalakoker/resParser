#include "hystorylineedit.h"
#include <QKeyEvent>

HystoryLineEdit::HystoryLineEdit(QWidget *parent) :
    QLineEdit(parent)
{
}

void HystoryLineEdit::keyPressEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_Up)
    {
        emit keyUpPressed();
    }
    if (event->key() == Qt::Key_Down)
    {
        emit keyDownPressed();
    }
    if (event->key() == Qt::Key_Escape)
    {
        emit keyEscPressed();
    }
    QLineEdit::keyPressEvent(event);
}
