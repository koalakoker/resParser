#include "doublespinbox.h"

DoubleSpinBox::DoubleSpinBox(QWidget *parent) :
    QDoubleSpinBox(parent)
{
}

void DoubleSpinBox::stepBy(int steps)
{
    QDoubleSpinBox::stepBy(steps);
    emit userStepBy(steps);
}
