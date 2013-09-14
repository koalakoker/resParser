#ifndef PARSERCLASS_H
#define PARSERCLASS_H

#include "variable.h"
#include "QString"
#include "hfloat.h"

#define MAX_VARIABLES 3

class ParserClass
{
private:
    Variable m_variables[MAX_VARIABLES];
    int m_VariableCreated;

    bool IsVariableName(QString str);
    static bool IsNumeric(QString str);
    static bool IsAssignment(QString str);
    static bool HasParentesis(QString str);
    static bool HasOperand(QString str);
    static bool HasOperand(QString str,char operand);
    static QString ExtractExpressionFromParentesis(QString str);
    QString EvaluateParallel(QString str);
    QString EvaluateDivision(QString str);
    QString EvaluateMultiply(QString str);
    hfloat EvaluateSumAndDifference(QString str);

public:
    ParserClass();
    int VariableCreated(void);
    hfloat LoadVariable(QString name);
    bool StoreVariable(QString name,hfloat newValue);
    Variable *GetVariableAtIndex(int i);
    void Clear(void);
    hfloat Parse(QString str);
};

#endif // PARSERCLASS_H
