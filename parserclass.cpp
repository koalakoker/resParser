#include "parserclass.h"
#include "QRegExp"
#include "QStringList"
//#include "QDebug"

#define REGEXP5OPERAND "[-+*/:]"
#define REGEXP2OPERAND "[-+]"

#define FIXED "%.%1Rf"
#define SCIENTIFIC "%.%1Re"
#define AUTO "%.%1Rg"
#define HEXADECIMAL "%lX"

keyWord::keyWord(keyWordCode_t code, QString str)
{
    m_code = code;
    m_str = str;
}

bool ParserClass::IsKeyWord(QString str)
{
    int i;
    for (i = 0; i < m_keyWord.count(); i++)
    {
        if (str.contains(m_keyWord.at(i).m_str))
        {
            return true;
        }
    }
    return false;
}

keyWordCode_t ParserClass::KeyWordCode(QString str)
{
    int i;
    for (i = 0; i < m_keyWord.count(); i++)
    {
        if (str.contains(m_keyWord.at(i).m_str))
        {
            return m_keyWord.at(i).m_code;
        }
    }
    return key_None;
}

ParserClass::ParserClass(QObject *parent):
    QObject(parent)
{
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

    m_keyWord.append(keyWord(key_Clear,"clear"));
    m_keyWord.append(keyWord(key_List,"list"));
    m_keyWord.append(keyWord(key_List,"ls"));
    m_keyWord.append(keyWord(key_E12,"E12"));
    m_keyWord.append(keyWord(key_E24,"E24"));
    m_keyWord.append(keyWord(key_Usage,"usage"));

    m_formatOutput = Auto;
    m_precision = 20;
}

void ParserClass::addBuiltInFunction(QString name, hfloat (*ptr1a)(hfloat a))
{
    m_functions.append(builtinFunction(name,ptr1a));
}

void ParserClass::addBuiltInFunction(QString name, hfloat (*ptr2a)(hfloat a,hfloat b))
{
    m_functions.append(builtinFunction(name,ptr2a));
}

QStringList ParserClass::builtInFunctionList(void)
{
    QStringList list;
    int i;
    for (i = 0; i < m_functions.count(); i++)
    {
        list.append(m_functions[i].name());
    }
    return list;
}

QList<TableInfoElement> ParserClass::UserDefinedFunctionsInfo(void)
{
    QList<TableInfoElement> list;
    int i;
    for (i = 0; i < m_userdefinedFunctions.count(); i++)
    {
        TableInfoElement info;
        info.m_name = m_userdefinedFunctions[i].Name();
        info.m_value = m_userdefinedFunctions[i].functionSrt();
        list.append(info);
    }
    return list;
}

int ParserClass::VariableCreated(void)
{
    return m_variables.count();
}

hfloat ParserClass::LoadVariable(QString name)
{
    hfloat retVal;
    int i;
    for (i = 0; i < m_variables.count(); i++)
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
    for (i = 0; i < m_variables.count(); i++)
    {
        Variable* var = &(m_variables[i]);
        if (var->Name() == name)
        {
            var->setValue(newValue);
            found = true;
            retVal = true;
            emit variablesUpdate();
        }
    }
    if (!found)
    {
        Variable newVar;
        newVar.setName(name);
        newVar.setValue(newValue);
        m_variables.append(newVar);
        retVal = true;
        emit variablesUpdate();
    }
    return retVal;
}

bool ParserClass::RemoveVariable(QString name)
{
    bool retVal = false;
    int i;
    for (i = 0; i < m_variables.count(); i++)
    {
        if (m_variables[i].Name() == name)
        {
            m_variables.removeAt(i);
            retVal = true;
            emit variablesUpdate();
            break;
        }
    }
    return retVal;
}

bool ParserClass::RemoveUserDefinedFunction(QString name)
{
    bool retVal = false;
    int i;
    for (i = 0; i < m_userdefinedFunctions.count(); i++)
    {
        if (m_userdefinedFunctions[i].Name() == name)
        {
            m_userdefinedFunctions.removeAt(i);
            retVal = true;
            emit(functionListUpdate(builtInFunctionList()));
            break;
        }
    }
    return retVal;
}

bool ParserClass::StoreFunction(QString name,QStringList args,QString newFuncStr)
{
    bool retVal = false, found = false;
    int i;
    for (i = 0; i < m_userdefinedFunctions.count(); i++)
    {
        userdefinedFunctions func = m_userdefinedFunctions.at(i);
        if (func.Name() == name)
        {
            func.setFunctionStr(newFuncStr);
            func.setArgs(args);
            m_userdefinedFunctions[i]=func;
            found = true;
            retVal = true;
            emit(functionListUpdate(builtInFunctionList()));
        }
    }
    if (!found)
    {
        userdefinedFunctions newFunc;
        newFunc.setName(name);
        newFunc.setArgs(args);
        newFunc.setFunctionStr(newFuncStr);
        m_userdefinedFunctions.append(newFunc);
        retVal = true;
        emit(functionListUpdate(builtInFunctionList()));
    }
    return retVal;
}

Variable* ParserClass::GetVariableAtIndex(int i)
{
    Variable* retVal = 0;
    if (i < m_variables.count())
    {
        retVal = &(m_variables[i]);
    }
    return retVal;
}

void ParserClass::Clear(void)
{
    m_variables.clear();
}

QString ParserClass::Exec(QString str, hfloat &result)
{
    QString retVal = str + QString("<br>");

    /* execute command */
    switch (KeyWordCode(str.toLower()))
    {
    case key_List:
    {
        int i;
        for (i = 0; i < VariableCreated(); i++)
        {
            Variable* var = GetVariableAtIndex(i);
            retVal.append(var->ToString());
            retVal.append("<br>");
        }
    }
        break;
    case key_Clear:
    {
        // Remove spaces
        str.replace(" ","");
        if (IsVariableName(str.remove("clear")))
        {
            RemoveVariable(str);
        }
        else if (IsUserDefinedFunctionName(str))
        {
            RemoveUserDefinedFunction(str);
        }
        else if (str.toLower() == "clear" )
        {
            Clear();
        }
    }
        break;
    case key_E12:
    {
        retVal.append(Resistor::E12ValuesToString());
        retVal.append("<br>");
    }
        break;
    case key_E24:
    {
        retVal.append(Resistor::E24ValuesToString());
        retVal.append("<br>");
    }
        break;
    case key_Usage:
    {
        retVal.append("<br>");
        retVal.append("Using built-in function <b>function_name(arg,[arg])</b>.<br>");
        retVal.append("Variable assignment: <b>var_name = expression</b>.<br>");
        retVal.append("Show variables <b>list</b> or <b>ls</b>.<br>");
        retVal.append("<b>clear</b> to delete all varaibles.<br>");
        retVal.append("<b>E12</b> to show all E12 resitor values.<br>");
        retVal.append("<b>E24</b> to show all E24 resitor values.<br>");
        retVal.append("<b>->E12</b> to round to nearest E12 resitor values.<br>");
        retVal.append("<b>:</b> parallel operator between resistors.<br>");
    }
        break;
    default:
    {
        QString tmpVal;
        if ((tmpVal = UserDefineFunctionFormulaFromName(str))!="")
        {
            /* Append user defined formula if name is in str */
            retVal.append(FormatAnswer(tmpVal).append("<br>"));
        }
        else
        {
            result = Parse(str);
            QString ansStr;
            if (!result.isNan())
            {
                StoreVariable("ans",result); // Store the last result
                ansStr = QString("ans=");
            }
            retVal.append(FormatAnswer(QString("%1%2<br>").arg(ansStr).arg(result.toString(FormatOutput()))));
        }
    }
        break;
    }
    return retVal;
}

hfloat ParserClass::Parse(QString str, bool preview)
{
    // Remove spaces
    str = str.replace(" ","");
    // Convert k and M
    str = str.replace ("k","000");
    str = str.replace ("K","000");
    str = str.replace ("M","000000");

    hfloat retVal;
    int biFunc,udFunc;

    if (IsAssignment(str))
    {
        // Variable or function?
        int equalPos = str.indexOf("=");
        if (equalPos>0)
        {
            if (str[equalPos-1] != ')')
            {
                // Variable assignment
                QString VarName = str.mid(0,equalPos);
                QString expression = str.mid(equalPos+1,str.length()-equalPos-1);
                hfloat expressionValue = this->Parse(expression);
                if (!preview)
                {
                    StoreVariable(VarName,expressionValue); // Not used return value (False if no more variables available)
                }
                retVal = expressionValue;
            }
            else
            {
                // Function assignment
                QString beforeAssignStr = str.mid(0,equalPos);
                QString functionName = beforeAssignStr.mid(0,str.indexOf("("));
                QString fromPar = ExtractExpressionFromParentesis(beforeAssignStr);
                fromPar = QString("(")+fromPar+QString(")");
                QString from;
                QStringList args;
                if (ExtractFunctionArguments(fromPar,args,from))
                {
                    QString expression = str.mid(equalPos+1,str.length()-equalPos-1);
                    if (expression.length()>0)
                    {
                        if (!preview)
                        {
                            StoreFunction(functionName,args,expression);
                        }
                    }
                }
            }
        }
    } else if ((biFunc = HasFunction(str))!=-1)
    {
        // Builtin function parsing
        QString from,to;
        ExtractBuiltInFunction(str,biFunc,from,to);
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
    } else if ((udFunc = HasUserDefinedFunction(str))!=-1)
    {
        // User defined function parsing
        QString from,to;
        ExtractUserDefinedFunction(str,udFunc,from,to);
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
        Int64 val = (Int64)Parse(res).toString("%.50Rf").toFloat();
        retVal = hfloat(QString("%1").arg(Resistor::ToNearE12(val)));
    } else if (IsNumeric(str))
    {
        retVal = hfloat(str);
    }
    else if (IsHexadecimal(str))
    {
        retVal = hfloat(EvaluateExadecimal(str));
    }
    else if (IsBinary(str))
    {
        retVal = hfloat(EvaluateBinary(str));
    }
    else if (IsVariableName(str))
    {
        retVal = LoadVariable(str);
    }
    else if (HasParentesis(str))
    {
        QString expression = ExtractExpressionFromParentesis(str);
        if (expression != "")
        {
            str = str.replace("("+expression+")",this->Parse(expression).toString("%.50Rf"));
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

bool ParserClass::IsHexadecimal(QString str)
{
    return (str.indexOf("0x") == 0);
}

bool ParserClass::IsBinary(QString str)
{
    return (str.indexOf("0b") == 0);
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
    for (i = 0; i < m_variables.count(); i++)
    {
        Variable var = m_variables[i];
        if (var.Name() == str)
        {
            return true;
        }
    }
    return false;
}

bool ParserClass::IsUserDefinedFunctionName(QString str)
{
    int i;
    for (i = 0; i < m_userdefinedFunctions.count(); i++)
    {
        if (m_userdefinedFunctions[i].Name() == str)
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

        QString evaluated = str.replace(lastValueStr+":"+firstValueStr,result.toString("%.50Rf"));
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

        QString evaluated = str.replace(lastValueStr+"*"+firstValueStr,result.toString("%.50Rf"));
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

        QString evaluated = str.replace(lastValueStr+"/"+firstValueStr,result.toString("%.50Rf"));
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

long int ParserClass::FromAsciiHexDigitToNUmber(char digitA)
{
    long int retVal = 0;
    if ((digitA >= '0') && (digitA <='9'))
    {
        retVal = digitA - '0';
    }
    else if ((digitA >= 'a') && (digitA <='f'))
    {
        retVal = 10 + (digitA - 'a');
    }
    return retVal;
}

long int ParserClass::FromAsciiBinDigitToNUmber(char digitA)
{
    long int retVal = 0;
    if ((digitA >= '0') && (digitA <='1'))
    {
        retVal = digitA - '0';
    }
    return retVal;
}

long int ParserClass::pow(int base, int exponent)
{
    long int retVal = base;
    if (exponent == 0)
    {
        retVal = 1;
    }
    else
    {
        int i;
        for (i = 0;  i < (exponent - 1); i++)
        {
            retVal *= base;
        }
    }
    return retVal;
}

QString ParserClass::EvaluateExadecimal(QString str)
{
    str = str.mid(2).toLower();
    int i,l = str.length();
    long int result = 0;
    for (i = 0; i < l; i++)
    {
        char digitA = str[(l-1)-i].toAscii();
        long int digit = FromAsciiHexDigitToNUmber(digitA);
        result += digit * pow(16,i);
    }
    return QString("%1").arg(result);
}

QString ParserClass::EvaluateBinary(QString str)
{
    str = str.mid(2).toLower();
    int i,l = str.length();
    long int result = 0;
    for (i = 0; i < l; i++)
    {
        char digitA = str[(l-1)-i].toAscii();
        long int digit = FromAsciiBinDigitToNUmber(digitA);
        result += digit * pow(2,i);
    }
    return QString("%1").arg(result);
}

int ParserClass::HasFunction(QString str)
{
    int retVal = -1;
    int i;
    for (i = 0; i < m_functions.count(); i++)
    {
        if (str.toLower().contains(m_functions[i].name()+QString("(")))
        {
            retVal = i;
            break;
        }
    }
    return retVal;
}

int ParserClass::HasUserDefinedFunction(QString str)
{
    int retVal = -1;
    int i;
    for (i = 0; i < m_userdefinedFunctions.count(); i++)
    {
        if (str.toLower().contains(m_userdefinedFunctions[i].Name()+QString("(")))
        {
            retVal = i;
            break;
        }
    }
    return retVal;
}

void ParserClass::ExtractBuiltInFunction(QString str, int biFuncOrder, QString& from, QString& to)
{
    from=QString("");
    to=QString("");
    if ((biFuncOrder >= 0)&&(biFuncOrder < m_functions.count()))
    {
        builtinFunction biFunc = m_functions[biFuncOrder];
        biFunc.clearArgs();
        QString functionName = biFunc.name() + QString("(");
        int functionPos = str.indexOf(functionName);
        if (functionPos >= 0)
        {
            functionPos+=functionName.length()-1;
            QStringList argumentStr;
            if (ExtractFunctionArguments(str.mid(functionPos,str.length()-functionPos),argumentStr,from))
            {
                int i;
                for (i = 0; i < argumentStr.count(); i++)
                {
                    biFunc.addArg(Parse(argumentStr[i]));
                }
                hfloat result = biFunc.exec();
                if (!result.isNan())
                {
                    from = biFunc.name() + from;
                    to = result.toString("%.50Rf");
                }
                else
                {
                    from = QString("");
                    to=QString("");
                }
            }
        }
    }
}

void ParserClass::ExtractUserDefinedFunction(QString str, int udFuncOrder, QString& from, QString& to)
{
    from=QString("");
    to=QString("");
    if ((udFuncOrder >= 0)&&(udFuncOrder < m_userdefinedFunctions.count()))
    {
        userdefinedFunctions udFunc = m_userdefinedFunctions[udFuncOrder];
        QString functionName = udFunc.Name() + QString("(");
        int functionPos = str.indexOf(functionName);
        if (functionPos >= 0)
        {
            functionPos+=functionName.length()-1;
            QStringList argument;
            if (ExtractFunctionArguments(str.mid(functionPos,str.length()-functionPos),argument,from))
            {
                QString functionStr = udFunc.functionSrt();
                QStringList funcionArgs = udFunc.args();
                if (funcionArgs.count()==argument.count())
                {
                    // Replace arguments
                    int i;
                    for (i = 0; i < argument.count(); i++)
                    {
                        functionStr.replace(funcionArgs[i],argument[i]);
                    }
                }
                hfloat result = Parse(functionStr);
                if (!result.isNan())
                {
                    from = udFunc.Name() + from;
                    to = result.toString("%.50Rf");
                }
                else
                {
                    from = QString("");
                    to=QString("");
                }
            }
        }
    }
}

bool ParserClass::ExtractFunctionArguments(QString str, QStringList &args, QString& from)
{
    bool retVal = false;
    if (str[0] == '(')
    {
        int i;
        int numberOfParentesisOpen = 1;
        int numberOfParentesisClose = 0;
        for (i = 1; i < str.length(); i++)
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
                    QString extractStr = str.mid(1, i-1);
                    args = extractStr.split(",");
                    from = QString("(") + extractStr + QString(")");
                    retVal = true;
                }
            }
        }
    }
    return retVal;
}

QString ParserClass::UserDefineFunctionFormulaFromName(QString name)
{
    QString retVal = QString("");
    int udFuncOrder = -1;
    int i;
    for (i = 0; i < m_userdefinedFunctions.count(); i++)
    {
        if (name == m_userdefinedFunctions[i].Name())
        {
            udFuncOrder = i;
            break;
        }
    }
    if ((udFuncOrder >= 0) && (udFuncOrder < m_userdefinedFunctions.count()))
    {
        userdefinedFunctions func = m_userdefinedFunctions.at(udFuncOrder);
        retVal = func.functionSrt();
    }
    return retVal;
}

void ParserClass::Save(QDataStream& out)
{
    int i, l = m_variables.count();
    // Save Variables
    out << (qint32)(l);
    for (i = 0; i < l; i++)
    {
        m_variables[i].Save(out);
    }
    // Save User defined functions
    l = m_userdefinedFunctions.count();
    out << (qint32)(l);
    for (i = 0; i < l; i++)
    {
        m_userdefinedFunctions[i].Save(out);
    }
}

void ParserClass::Load(QDataStream& in)
{
    // Load Variables
    Clear();
    int i,l;
    in >> l;
    for (i = 0; i < l; i++)
    {
        QString name,valueStr;
        in >> name;
        in >> valueStr;
        hfloat newValue = hfloat(valueStr);
        StoreVariable(name,newValue);
    }
    // Load User defined functions
    m_userdefinedFunctions.clear();
    in >> l;
    for (i = 0; i < l; i++)
    {
        userdefinedFunctions newUDF;
        newUDF.Load(in);
        m_userdefinedFunctions.append(newUDF);
    }
}

formatOutput_t ParserClass::Format(void)
{
    return m_formatOutput;
}

void ParserClass::SetFormat(formatOutput_t format)
{
    m_formatOutput = format;
}

int ParserClass::Precision(void)
{
    return m_precision;
}

void ParserClass::SetPrecision(int precision)
{
    m_precision = precision;
}

QString ParserClass::FormatAnswer(QString str)
{
    return QString("<font size=""4""><b>&nbsp;&nbsp;")+str+QString("</b></font>");
}

QString ParserClass::FormatOutput(void)
{
    QString retVal;
    switch (m_formatOutput)
    {
    case Hexadecimal:
    {
        retVal = QString(HEXADECIMAL);
    }
        break;
    case Fixed:
    {
        retVal = QString(FIXED).arg(m_precision);
    }
        break;
    case Scientific:
    {
        retVal = QString(SCIENTIFIC).arg(m_precision);
    }
        break;
    case Auto:
    default:
    {
        retVal = QString(AUTO).arg(m_precision);
    }
        break;
    }
    return retVal;
}
