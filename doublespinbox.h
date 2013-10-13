#ifndef DOUBLESPINBOX_H
#define DOUBLESPINBOX_H

#include <QDoubleSpinBox>

class DoubleSpinBox : public QDoubleSpinBox
{
    Q_OBJECT
public:
    explicit DoubleSpinBox(QWidget *parent = 0);

private:
    void stepBy(int steps);
    
signals:
    void userStepBy(int steps);
    
public slots:
    
};

#endif // DOUBLESPINBOX_H
