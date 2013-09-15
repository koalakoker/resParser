#ifndef BUILTINFUNCTION_H
#define BUILTINFUNCTION_H

#include "QString"
#include "hfloat.h"

class builtinFunction
{
public:
    builtinFunction(QString name, hfloat (*ptr1a)(hfloat a));
    QString name(void);
    hfloat exec(hfloat *arg, int argc);
private:
    QString m_name; // String used to call the function
    hfloat (*m_ptr1a)(hfloat a); // Pointer to the function to be executed
};

#endif // BUILTINFUNCTION_H
