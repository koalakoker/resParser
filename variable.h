#ifndef VARIABLE_H
#define VARIABLE_H

#include "QString"
#include "resistor.h"
#include "hfloat.h"

class Variable
{
private:
    hfloat m_value;
    QString m_name;
    Resistor m_resistor;

public:
    Variable();

    void setValue(hfloat value);
    hfloat Value(void);
    void setName(QString name);
    QString Name(void);
    QString ToString (void);
};

#endif // VARIABLE_H
