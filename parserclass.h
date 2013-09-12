#ifndef PARSERCLASS_H
#define PARSERCLASS_H

#include "variable.h"
#include "QString"

class ParserClass
{
private:
    Variable* m_variables;
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
    static double EvaluateSumAndDifference(QString str);

public:
    ParserClass();
    int VariableCreated(void);
    double LoadVariable(QString name);
    void StoreVariable(QString name,double newValue);
    Variable GetVariableAtIndex(int i);
    void Clear(void);
    double Parse(QString str);
};

#endif // PARSERCLASS_H
