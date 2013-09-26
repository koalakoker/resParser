#ifndef PARSERCLASS_H
#define PARSERCLASS_H

#include <QObject>

#include "variable.h"
#include "QString"
#include "hfloat.h"
#include "builtinfunction.h"
#include "userdefinedfunctions.h"

#define MAX_VARIABLES 100
#define MAX_BUILTINFUNCTIONS 40

class ParserClass : public QObject
{
    Q_OBJECT
private:
    Variable m_variables[MAX_VARIABLES];
    int m_VariableCreated;

    builtinFunction* m_functions[MAX_BUILTINFUNCTIONS];
    int m_builtinfunctionCreated;

    QList<userdefinedFunctions> m_userdefinedFunctions;

    bool IsVariableName(QString str);
    static bool IsNumeric(QString str);
    static bool IsHexadecimal(QString str);
    static bool IsBinary(QString str);
    static bool IsAssignment(QString str);
    static bool HasParentesis(QString str);
    static bool HasOperand(QString str);
    static bool HasOperand(QString str,char operand);
    int HasFunction(QString str);

    static QString ExtractExpressionFromParentesis(QString str);
    QString EvaluateParallel(QString str);
    QString EvaluateDivision(QString str);
    QString EvaluateMultiply(QString str);
    static QString EvaluateExadecimal(QString str);
    static QString EvaluateBinary(QString str);
    static long int FromAsciiHexDigitToNUmber(char digitA);
    static long int FromAsciiBinDigitToNUmber(char digitA);
    static long int pow(int base, int exponent);
    hfloat EvaluateSumAndDifference(QString str);
    void ExtractBuiltInFunction(QString str, int biFuncOrder, QString& from, QString& to);
    void ExtractUserDefinedFunction(QString str, int udFuncOrder, QString& from, QString& to);
    bool ExtractFunctionArguments(QString str,QStringList &args, QString &from);
    int HasUserDefinedFunction(QString str);

    void addBuiltInFunction(QString name, hfloat (*ptr1a)(hfloat a));
    void addBuiltInFunction(QString name, hfloat (*ptr2a)(hfloat a,hfloat b));

public:
    explicit ParserClass(QObject *parent = 0);
    int VariableCreated(void);
    hfloat LoadVariable(QString name);
    bool StoreVariable(QString name,hfloat newValue);
    bool StoreFunction(QString name,QStringList args,QString newValue);
    Variable *GetVariableAtIndex(int i);
    void Clear(void);
    hfloat Parse(QString str,bool preview  = false);
    QStringList builtInFunctionList(void);
    QString UserDefineFunctionFormulaFromName(QString name);

    void Save(QDataStream& out);
    void Load(QDataStream& in);

signals:
    void functionListUpdate(QStringList list);

public slots:
};

#endif // PARSERCLASS_H
