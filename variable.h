#ifndef VARIABLE_H
#define VARIABLE_H

#include "QString"
#include "resistor.h"

class Variable
{
private:
    double m_value;
    QString m_name;
    Resistor* m_resistor;

public:
    Variable();

    void setValue(double value);
    double Value(void);
    void setName(QString value);
    QString Name(void);
    QString ToString (void);
};

#endif // VARIABLE_H
