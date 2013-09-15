#ifndef BUILTINFUNCTION_H
#define BUILTINFUNCTION_H

#include "hfloat.h"
#include "QString"
#include "QList"

class builtinFunction
{
public:
    builtinFunction(QString name, hfloat (*ptr0a)());
    builtinFunction(QString name, hfloat (*ptr1a)(hfloat a));
    QString name(void);
    hfloat exec(void);
    void clearArgs(void);
    void addArg(hfloat arg);

private:
    QString m_name; // String used to call the function
    hfloat (*m_ptr0a)(); // Pointer to the function to be executed
    hfloat (*m_ptr1a)(hfloat a); // Pointer to the function to be executed
    QList<hfloat> arg; // List of arguments

};

#endif // BUILTINFUNCTION_H
