#include "parserclass.h"
#include "QRegExp"
#include "QStringList"
#include "QDebug"

#define REGEXP5OPERAND "[-+*/:]"
#define REGEXP2OPERAND "[-+]"

ParserClass::ParserClass()
{
    m_VariableCreated = 0;
    m_builtinfunctionCreated = 0;

    addBuiltInFunction("sqrt",hfloat::sqrt);
    addBuiltInFunction("sqr",hfloat::sqr);
    addBuiltInFunction("cbrt",hfloat::cbrt);
    addBuiltInFunction("pow",hfloat::pow);
    addBuiltInFunction("abs",hfloat::abs);
    addBuiltInFunction("log",hfloat::log);
    addBuiltInFunction("log2",hfloat::log2);
    addBuiltInFunction("log10",hfloat::log10);
    addBuiltInFunction("exp",hfloat::exp);
    addBuiltInFunction("exp2",hfloat::exp2);
    addBuiltInFunction("exp10",hfloat::exp10);

    addBuiltInFunction("sec",hfloat::sec);
    addBuiltInFunction("csc",hfloat::csc);
    addBuiltInFunction("cot",hfloat::cot);

    addBuiltInFunction("sech",hfloat::sech);
    addBuiltInFunction("csch",hfloat::csch);
    addBuiltInFunction("coth",hfloat::coth);

    addBuiltInFunction("asin",hfloat::asin);
    addBuiltInFunction("acos",hfloat::acos);
    addBuiltInFunction("atan",hfloat::atan);
    addBuiltInFunction("atan2",hfloat::atan2);

    addBuiltInFunction("sin",hfloat::sin);
    addBuiltInFunction("cos",hfloat::cos);
    addBuiltInFunction("tan",hfloat::tan);

    addBuiltInFunction("acosh",hfloat::acosh);
    addBuiltInFunction("asinh",hfloat::asinh);
    addBuiltInFunction("atanh",hfloat::atanh);

    addBuiltInFunction("cosh",hfloat::cosh);
    addBuiltInFunction("sinh",hfloat::sinh);
    addBuiltInFunction("tanh",hfloat::tanh);
}

void ParserClass::addBuiltInFunction(QString name, hfloat (*ptr1a)(hfloat a))
{
    if (m_builtinfunctionCreated < MAX_BUILTINFUNCTIONS)
    {
        m_functions[m_builtinfunctionCreated] = new builtinFunction(name,ptr1a);
        m_builtinfunctionCreated++;
    }
}

void ParserClass::addBuiltInFunction(QString name, hfloat (*ptr2a)(hfloat a,hfloat b))
{
    if (m_builtinfunctionCreated < MAX_BUILTINFUNCTIONS)
    {
        m_functions[m_builtinfunctionCreated] = new builtinFunction(name,ptr2a);
        m_builtinfunctionCreated++;
    }
}

int ParserClass::VariableCreated(void)
{
    return m_VariableCreated;
}

hfloat ParserClass::LoadVariable(QString name)
{
    hfloat retVal;
    int i;
    for (i = 0; i < m_VariableCreated; i++)
    {
        Variable* var = &m_variables[i];
        if (var->Name() == name)
        {
            retVal = var->Value();
        }
    }
    return retVal;
}

bool ParserClass::StoreVariable(QString name,hfloat newValue)
{
    bool retVal = false, found = false;
    int i;
    for (i = 0; i < m_VariableCreated; i++)
    {
        Variable* var = &(m_variables[i]);
        if (var->Name() == name)
        {
            var->setValue(newValue);
            found = true;
            retVal = true;
        }
    }
    if (!found)
    {
        if (m_VariableCreated < MAX_VARIABLES)
        {
            m_variables[m_VariableCreated].setName(name);
            m_variables[m_VariableCreated].setValue(newValue);
            m_VariableCreated++;
            retVal = true;
        }
    }
    return retVal;
}

Variable* ParserClass::GetVariableAtIndex(int i)
{
    Variable* retVal = 0;
    if (i < m_VariableCreated)
    {
        retVal = &(m_variables[i]);
    }
    return retVal;
}

void ParserClass::Clear(void)
{
    m_VariableCreated = 0;
}

hfloat ParserClass::Parse(QString str)
{
    // Remove spaces
    str = str.replace(" ","");
    // Convert k and M
    str = str.replace ("k","000");
    str = str.replace ("K","000");
    str = str.replace ("M","000000");

    hfloat retVal;

    // Builtin function parsing
    int biFunc = HasFunction(str);
    if (biFunc!=-1)
    {
        QString from,to;
        ExtractFunction(str,biFunc,from,to);
        if (from != "")
        {
            if (str.contains(from))
            {
                str = str.replace(from,to);
                retVal = this->Parse(str);
            }
            else
            {
                retVal.setNan();
            }
        }
    } else if (str.contains("->E12"))
    {
        int pos = str.indexOf("->E12");
        QString res = str.mid(0,pos);
        Int64 val = (Int64)Parse(res).toString().toFloat();
        retVal = hfloat(QString("%1").arg(Resistor::ToNearE12(val)));
    } else if (IsNumeric(str))
    {
        retVal = hfloat(str);
    }
    else if (IsVariableName(str))
    {
        retVal = LoadVariable(str);
    }
    else if (IsAssignment(str))
    {
        int equalPos = str.indexOf("=");
        QString VarName = str.mid(0,equalPos);
        QString expression = str.mid(equalPos+1,str.length()-equalPos-1);
        hfloat expressionValue = this->Parse(expression);
        StoreVariable(VarName,expressionValue); // Not used return value (False if no more variables available)
        retVal = expressionValue;
    }
    else if (HasParentesis(str))
    {
        QString expression = ExtractExpressionFromParentesis(str);
        if (expression != "")
        {
            str = str.replace("("+expression+")",this->Parse(expression).toString());
            retVal = this->Parse(str);
        }
    }
    else if (HasOperand(str))
    {
        str = EvaluateParallel(str);
        str = EvaluateDivision(str);
        str = EvaluateMultiply(str);
        retVal = EvaluateSumAndDifference(str);
    }
    else
    {
    }

    return retVal;
}

bool ParserClass::IsNumeric(QString str)
{
    int quoteNum = 0;
    int i;
    for (i = 0; i < str.length(); i++)
    {
        if ((str[i] == '-') && (i == 0))
        {
            i = 1;
        }
        if ((str[i] < '0') || (str[i] > '9'))
        {
            if (((str[i] == ',') || (str[i] == '.')) && (quoteNum == 0))
            {
                quoteNum++;
                continue;
            }
            return false;
        }
    }
    return true;
}

bool ParserClass::IsVariableName(QString str)
{
    int i;
    for (i = 0; i < m_VariableCreated; i++)
    {
        Variable var = m_variables[i];
        if (var.Name() == str)
        {
            return true;
        }
    }
    return false;
}

bool ParserClass::IsAssignment(QString str)
{
    int equalPos = str.indexOf("=");
    if (equalPos != -1)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool ParserClass::HasOperand(QString str)
{
    QRegExp operand(REGEXP5OPERAND);
    int i = str.indexOf(operand);
    if (i == -1)
    {
        return false;
    }
    if ((str[i] == '-') && (i == 0))
    {
        i = str.indexOf(operand,i+1);
    }
    if (i == -1)
    {
        return false;
    }
    else
    {
        return true;
    }
}

bool ParserClass::HasParentesis(QString str)
{
    int i;
    for (i = 0; i < str.length(); i ++)
    {
        if (str[i] == '(')
        {
            return true;
        }
    }
    return false;
}

QString ParserClass::ExtractExpressionFromParentesis(QString str)
{
    int indexOfFirstParentesis = str.indexOf('(');
    int i;
    int numberOfParentesisOpen = 1;
    int numberOfParentesisClose = 0;
    for (i = indexOfFirstParentesis + 1; i < str.length(); i++)
    {
        if (str[i] == '(')
        {
            numberOfParentesisOpen++;
        }
        if (str[i] == ')')
        {
            numberOfParentesisClose++;
            if (numberOfParentesisOpen == numberOfParentesisClose)
            {
                QString expression = str.mid(indexOfFirstParentesis + 1, i - indexOfFirstParentesis -1);
                return expression;
            }
        }
    }
    return "";
}

bool ParserClass::HasOperand(QString str,char operand)
{
    int operandPos = str.indexOf(operand);
    /* Skip first '-' */
    if ((operand == '-') && (operandPos == 0))
    {
        operandPos = str.indexOf(operand,operandPos+1);
    }
    if (operandPos != -1)
    {
        return true;
    }
    else
    {
        return false;
    }
}

QString ParserClass::EvaluateParallel(QString str)
{
    QString retVal;
    if (HasOperand(str,':'))
    {
        int operandPos = str.indexOf(':');
        QString beforeOp = str.mid(0,operandPos);
        QString afterOp = str.mid(operandPos+1,str.length()-operandPos-1);

        QRegExp operand(REGEXP5OPERAND);
        QStringList beforeOpSplitted = beforeOp.split(operand);
        QStringList afterOpSplitted = afterOp.split(operand);
        QString lastValueStr = beforeOpSplitted[beforeOpSplitted.count()-1];
        QString firstValueStr = afterOpSplitted[0];
        hfloat result = hfloat("1")/((hfloat("1")/this->Parse(lastValueStr))+(hfloat("1")/this->Parse(firstValueStr)));

        QString evaluated = str.replace(lastValueStr+":"+firstValueStr,result.toString());
        retVal = EvaluateParallel(evaluated);
    }
    else
    {
        retVal = str;
    }
    return retVal;
}

QString ParserClass::EvaluateMultiply(QString str)
{
    QString retVal;
    if (HasOperand(str,'*'))
    {
        int operandPos = str.indexOf('*');
        QString beforeOp = str.mid(0,operandPos);
        QString afterOp = str.mid(operandPos+1,str.length()-operandPos-1);

        QRegExp operand("REGEXP5OPERAND");
        QStringList beforeOpSplitted = beforeOp.split(operand);
        QStringList afterOpSplitted = afterOp.split(operand);
        QString lastValueStr = beforeOpSplitted[beforeOpSplitted.count()-1];
        QString firstValueStr = afterOpSplitted[0];
        hfloat result = this->Parse(lastValueStr)*this->Parse(firstValueStr);

        QString evaluated = str.replace(lastValueStr+"*"+firstValueStr,result.toString());
        retVal = EvaluateMultiply(evaluated);
    }
    else
    {
        retVal = str;
    }
    return retVal;
}

QString ParserClass::EvaluateDivision(QString str)
{
    QString retVal;
    if (HasOperand(str,'/'))
    {
        int operandPos = str.indexOf('/');
        QString beforeOp = str.mid(0,operandPos);
        QString afterOp = str.mid(operandPos+1,str.length()-operandPos-1);

        QRegExp operand("REGEXP5OPERAND");
        QStringList beforeOpSplitted = beforeOp.split(operand);
        QStringList afterOpSplitted = afterOp.split(operand);
        QString lastValueStr = beforeOpSplitted[beforeOpSplitted.count()-1];
        QString firstValueStr = afterOpSplitted[0];
        hfloat result = this->Parse(lastValueStr)/this->Parse(firstValueStr);

        QString evaluated = str.replace(lastValueStr+"/"+firstValueStr,result.toString());
        retVal = EvaluateDivision(evaluated);
    }
    else
    {
        retVal = str;
    }
    return retVal;
}

hfloat ParserClass::EvaluateSumAndDifference(QString str)
{
    QRegExp operand(REGEXP2OPERAND);
    QStringList splittedStr = str.split(operand,QString::SkipEmptyParts);
    int numAddend = splittedStr.count();
    hfloat tot("0");
    int operatorPos = str.indexOf(operand);
    if (operatorPos == 0)
    {
        tot = tot - Parse(splittedStr[0]);
    }
    else
    {
        tot = tot + Parse(splittedStr[0]);
        operatorPos = -1;
    }
    int i;
    for (i = 1; i < numAddend; i++)
    {
        operatorPos = str.indexOf(operand,operatorPos+1);
        if (str[operatorPos] == '-')
        {
            tot = tot - Parse(splittedStr[i]);
        }
        else
        {
            tot = tot + Parse(splittedStr[i]);
        }
    }
    return tot;
}

int ParserClass::HasFunction(QString str)
{
    int retVal = -1;
    int i;
    for (i = 0; i < m_builtinfunctionCreated; i++)
    {
        if (str.toLower().contains(m_functions[i]->name()+QString("(")))
        {
            retVal = i;
            break;
        }
    }
    return retVal;
}

void ParserClass::ExtractFunction(QString str, int biFuncOrder, QString& from, QString& to)
{
    from=QString("");
    to=QString("");
    if ((biFuncOrder >= 0)&&(biFuncOrder < m_builtinfunctionCreated))
    {
        builtinFunction* biFunc = m_functions[biFuncOrder];
        biFunc->clearArgs();
        QString functionName = biFunc->name() + QString("(");
        int functionPos = str.indexOf(functionName);
        if (functionPos >= 0)
        {
            functionPos+=functionName.length()-1;
            QStringList argumentStr = ExtractFunctionArguments(str.mid(functionPos,str.length()-functionPos),from);
            int i;
            for (i = 0; i < argumentStr.count(); i++)
            {
                biFunc->addArg(Parse(argumentStr[i]));
            }
            hfloat result = biFunc->exec();
            if (!result.isNan())
            {
                from = biFunc->name() + from;
                to = result.toString();
            }
            else
            {
                from = QString("");
                to=QString("");
            }
        }
    }
}

QStringList ParserClass::ExtractFunctionArguments(QString str,QString& from)
{
    QStringList expression;
    if (str[0] != '(')
    {
        return expression;
    }
    int indexOfFirstParentesis = 0;
    int i;
    int numberOfParentesisOpen = 1;
    int numberOfParentesisClose = 0;
    for (i = indexOfFirstParentesis + 1; i < str.length(); i++)
    {
        if (str[i] == '(')
        {
            numberOfParentesisOpen++;
        }
        if (str[i] == ')')
        {
            numberOfParentesisClose++;
            if (numberOfParentesisOpen == numberOfParentesisClose)
            {
                QString extractStr = str.mid(indexOfFirstParentesis + 1, i - indexOfFirstParentesis -1);
                expression = extractStr.split(",");
                from = QString("(") + extractStr + QString(")");
                return expression;
            }
        }
    }
    return expression;
}
