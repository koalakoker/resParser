#ifndef PARSERCLASS_H
#define PARSERCLASS_H

#include <QObject>

#include "variable.h"
#include "QString"
#include "hfloat.h"
#include "builtinfunction.h"
#include "userdefinedfunctions.h"
#include "tableinfoelement.h"
#include "range.h"

typedef enum {Fixed, Scientific, Auto, Hexadecimal} formatOutput_t;

typedef enum {key_None, key_Clear, key_List, key_E12, key_E24, key_Usage, key_Plot} keyWordCode_t;

class keyWord
{
public:
    keyWord(keyWordCode_t code, QString str);
    keyWordCode_t m_code;
    QString m_str;
};

class ParserClass : public QObject
{
    Q_OBJECT
private:
    QList<Variable> m_variables;
    QList<builtinFunction> m_functions;
    QList<userdefinedFunctions> m_userdefinedFunctions;
    QList<keyWord> m_keyWord;

    formatOutput_t m_formatOutput;
    int m_precision;

public:
    bool IsVariableName(QString str);
    bool IsUserDefinedFunctionName(QString str);

private:
    static bool IsNumeric(QString str);
    static bool IsHexadecimal(QString str);
    static bool IsBinary(QString str);
    static bool IsAssignment(QString str);
    static bool HasParentesis(QString str);
    static bool HasOperand(QString str);
    static bool HasOperand(QString str,char operand);
    int HasFunction(QString str);

    static QString ExtractExpressionFromParentesis(QString str);
    static QString ExtractRange(QString str);
    Range EvaluateRange(QString str);
    QString EvaluateParallel(QString str);
    QString EvaluateDivision(QString str);
    QString EvaluateMultiply(QString str);
    QString EvaluatePow(QString str);
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

    bool IsKeyWord(QString str);
    keyWordCode_t KeyWordCode(QString str);
    QString RemoveKeyWord(QString str,keyWordCode_t code);

public:
    explicit ParserClass(QObject *parent = 0);
    int VariableCreated(void);
    hfloat LoadVariable(QString name);
    bool StoreVariable(QString name,hfloat newValue);
    bool RemoveVariable(QString name);
    bool RemoveUserDefinedFunction(QString name);
    bool StoreFunction(QString name,QStringList args,QString newValue);
    Variable *GetVariableAtIndex(int i);
    void Clear(void);
    hfloat Parse(QString str,bool preview  = false);
    QString Exec(QString str,hfloat& result);
    QStringList builtInFunctionList(void);
    TableInfo UserDefinedFunctionsInfo(void);
    QString UserDefineFunctionFormulaFromName(QString name);

    formatOutput_t Format(void); // Returns the current selected format
    int Precision(void); // Returns the current select precision in digit
    void SetFormat(formatOutput_t format); // Sets the format
    void SetPrecision(int precision); // Sets the precision in digit
    QString FormatOutput(void); // Return the format string to pass to hfloat::toString()
    QString FormatAnswer(QString str);

    void Save(QDataStream& out);
    void Load(QDataStream& in);

signals:
    void functionListUpdate(QStringList list);
    void variablesUpdate(void);

public slots:
};

#endif // PARSERCLASS_H
