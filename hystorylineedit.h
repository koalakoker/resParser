#ifndef HYSTORYLINEEDIT_H
#define HYSTORYLINEEDIT_H

#include <QLineEdit>

class HystoryLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    explicit HystoryLineEdit(QWidget *parent = 0);
    
signals:
    void keyUpPressed(void);
    void keyDownPressed(void);
    void keyEscPressed(void);
    void keyOperator(void);
public slots:
    
protected:
    void keyPressEvent(QKeyEvent* event);
};

#endif // HYSTORYLINEEDIT_H
