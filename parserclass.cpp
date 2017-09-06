#include "parserclass.h"
#include "QRegExp"
#include "QStringList"
#include "drawwidgetbrowse.h"
#include <QFile>
#include "QDebug"

#define REGEXP6OPERAND "[-+*/:^]"
#define REGEXP2OPERAND "[-+]"

#define FIXED "%.%1Rf"
#define SCIENTIFIC "%.%1Re"
#define AUTO "%.%1Rg"
#define HEXADECIMAL "%lX"

#define HF_MAXRES "%.50Rf"

// To be removed after final integration with flex
keyWord::keyWord(keyWordCode_t code, QString str)
{
    m_code = code;
    m_str = str;
}

// To be removed after final integration with flex
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

// To be removed after final integration with flex
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

// To be removed after final integration with flex
QString ParserClass::RemoveKeyWord(QString str,keyWordCode_t code)
{
    QString retVal = str;
    int i;
    for (i = 0; i < m_keyWord.count(); i++)
    {
        if ((str.contains(m_keyWord.at(i).m_str))&&(m_keyWord.at(i).m_code==code))
        {
            retVal = str.remove(m_keyWord.at(i).m_str);
        }
    }
    return retVal;
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

    // To be removed after final integration with flex
    m_keyWord.append(keyWord(key_ClearHistory,"clearhistory"));
    m_keyWord.append(keyWord(key_ClearRaw,"clearraw"));
    m_keyWord.append(keyWord(key_Clear,"clear"));
    m_keyWord.append(keyWord(key_Clear,"delete"));
    m_keyWord.append(keyWord(key_Clear,"del"));
    m_keyWord.append(keyWord(key_List,"list"));
    m_keyWord.append(keyWord(key_List,"ls"));
    m_keyWord.append(keyWord(key_E12,"e12"));
    m_keyWord.append(keyWord(key_E24,"e24"));
    m_keyWord.append(keyWord(key_Usage,"usage"));
    m_keyWord.append(keyWord(key_Plot,"plot"));
    m_keyWord.append(keyWord(key_Plot,"draw"));

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

TableInfo ParserClass::UserDefinedFunctionsInfo(void)
{
    TableInfo list;
    list.setColumn(4);
    int i;
    for (i = 0; i < m_userdefinedFunctions.count(); i++)
    {
        TableInfoElement info;
        userdefinedFunctions func = m_userdefinedFunctions[i];
        info.m_value.append(func.NameArgs());
        info.m_value.append(func.functionSrt());
        info.m_value.append(func.HasRawData()?QString("RAW"):QString(""));
        info.m_value.append(func.RawRange().toString());
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

bool ParserClass::RemoveUserDefinedFunctionRawData(QString name)
{
    bool retVal = false;
    int i;
    for (i = 0; i < m_userdefinedFunctions.count(); i++)
    {
        if (m_userdefinedFunctions[i].Name() == name)
        {
            userdefinedFunctions func = m_userdefinedFunctions.at(i);
            delete func.RawPoints();
            func.setRawPoints(NULL);
            func.setRawRange(Range());
            m_userdefinedFunctions.replace(i,func);
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
            found = true;
            retVal = true;
            if (func.functionSrt() != newFuncStr)
            {
                func.setFunctionStr(newFuncStr);
                func.setArgs(args);
                // Reset RAW data
                delete func.RawPoints();
                func.setRawPoints(NULL);
                func.setRawRange(Range());
                m_userdefinedFunctions[i]=func;
                emit(functionListUpdate(builtInFunctionList()));
            }
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

// To be removed after final integration with flex
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
        if (str.toLower() == "clear" )
        {
            Clear();
        }
        else
        {
            str = RemoveKeyWord(str,key_Clear);
            if (IsVariableName(str))
            {
                RemoveVariable(str);
            }
            else if (IsUserDefinedFunctionName(str))
            {
                RemoveUserDefinedFunction(str);
            }
        }
    }
        break;
    case key_ClearRaw:
    {
        // Remove spaces
        str.replace(" ","");
        str = RemoveKeyWord(str,key_ClearRaw);
        if (IsUserDefinedFunctionName(str))
        {
            RemoveUserDefinedFunctionRawData(str);
        }
    }
        break;
    case key_ClearHistory:
    {
        emit ClearHistory();
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
        retVal.append("Function assignment: <b>func_name(arg,[arg]) = expression</b>.<br>");
        retVal.append("Show variables <b>list</b> or <b>ls</b>.<br>");
        retVal.append("<b>clear</b> to delete all variables.<br>");
        retVal.append("<b>clear var_name</b> to delete variable var_name.<br>");
        retVal.append("<b>clear func_name</b> to delete user function func_name.<br>");
        retVal.append("<b>clearraw func_name</b> to delete RAW data from user function func_name.<br>");
        retVal.append("<b>plot (or draw) func_name[range]</b> to plot user function func_name.<br>");
        retVal.append("<b>E12</b> to show all E12 resistor values.<br>");
        retVal.append("<b>E24</b> to show all E24 resistor values.<br>");
        retVal.append("<b>->E12</b> to round to nearest E12 resistor values.<br>");
        retVal.append("<b>^</b> power operator.<br>");
        retVal.append("<b>:</b> parallel operator between resistors.<br>");
    }
        break;
    case key_Plot:
    {
        // Remove spaces
        str.replace(" ","");
        str.remove("plot");
        Range r = EvaluateRange(str);
        if (r.isValid())
        {
            str.remove(ExtractRange(str));
        }
        int udFuncOrder;
        if ((udFuncOrder = HasUserDefinedFunction(str+"("))!= -1)
        {
            userdefinedFunctions udFunc = m_userdefinedFunctions[udFuncOrder];
            bool hasRaw = udFunc.HasRawData();
            if ((hasRaw) || ((!hasRaw)&&(udFunc.functionSrt()!="RAW")))
            {
                DrawWidgetBrowse* d = new DrawWidgetBrowse();
                RawData* points = new(RawData);
                if ((hasRaw) &&
                    ((r == udFunc.RawRange())||
                    (!r.isValid())))
                {
                    points = udFunc.RawPoints();
                    r = points->RawRange();
                }
                else
                {
                    if (!r.isValid())
                    {
                        // Set default range
                        r = Range(-10,10,0.1);
                    }
                    // Prepare for function computation
                    QString functionStr = udFunc.functionSrt();
                    QStringList funcionArgs = udFunc.args();

                    hfloat x;
                    for (x = r.m_min; x <= r.m_max; x += r.m_step)
                    {
                        QString tmpStr = functionStr;
                        tmpStr.replace(funcionArgs[0],x.toString(HF_MAXRES));
                        points->append(HPoint(x,Parse(tmpStr)));
                    }
                    // Just for last point
                    functionStr.replace(funcionArgs[0],r.m_max.toString(HF_MAXRES));
                    points->append(HPoint(r.m_max,Parse(functionStr)));

                    // Store RAW data in function
                    m_userdefinedFunctions[udFuncOrder].setRawRange(r);
                    m_userdefinedFunctions[udFuncOrder].setRawPoints(points);
                    emit(functionListUpdate(builtInFunctionList()));
                }

                points->updateRange();
                d->drawWidget()->setPoints(points);
                d->setXmin(points->xMin().toFloat());
                d->setXmax(points->xMax().toFloat());
                d->setYmin(points->yMin().toFloat());
                d->setYmax(points->yMax().toFloat());
                d->show();
            }
        }
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

QString ParserClass::Exec(keyWordCode_t code, QString param1, QString param2)
{
    QString retVal;

    /* execute command */
    switch (code)
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
            if (param1 == "")
            {
                Clear(); // Only variables!!
            }
            else
            {
                //retVal.append("Symbol not foud"); To be fixed
                if (IsVariableName(param1))
                {
                    RemoveVariable(param1);
                    m_flexParse.parse(param1+"=0");
                    retVal.append(FormatAnswer("Removed variable."));
                }
                else if (IsUserDefinedFunctionName(param1))
                {
                    RemoveUserDefinedFunction(param1);
                    m_flexParse.deleteFunc(param1);
                    retVal.append(FormatAnswer("Removed user function."));
                }
            }
        }
        break;

        case key_ClearRaw:
        {
            if (IsUserDefinedFunctionName(param1))
            {
                RemoveUserDefinedFunctionRawData(param1);
            }
        }
        break;

        case key_ClearHistory:
        {
            emit ClearHistory();
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
            retVal.append("Using built-in function <b>function_name(arg,[arg])</b>.<br>");
            retVal.append("Variable assignment: <b>var_name = expression</b>.<br>");
            retVal.append("Function assignment: <b>func_name(arg,[arg]) = expression</b>.<br>");
            retVal.append("Show variables <b>list</b> or <b>ls</b>.<br>");
            retVal.append("<b>clear</b> to delete all variables.<br>");
            retVal.append("<b>clear var_name</b> to delete variable var_name.<br>");
            retVal.append("<b>clear func_name</b> to delete user function func_name.<br>");
            retVal.append("<b>clearraw func_name</b> to delete RAW data from user function func_name.<br>");
            retVal.append("<b>plot (or draw) func_name[range]</b> to plot user function func_name.<br>");
            retVal.append("<b>E12</b> to show all E12 resistor values.<br>");
            retVal.append("<b>E24</b> to show all E24 resistor values.<br>");
            retVal.append("<b>->E12</b> to round to nearest E12 resistor values.<br>");
            retVal.append("<b>^</b> power operator.<br>");
            retVal.append("<b>:</b> parallel operator between resistors.");
        }
        break;

        case key_Plot:
        {
            Range r = EvaluateRange(param2); // To be Flexed

            int udFuncOrder;
            if ((udFuncOrder = HasUserDefinedFunction(param1+"("))!= -1) // To be Flexed
            {
                userdefinedFunctions udFunc = m_userdefinedFunctions[udFuncOrder];
                bool hasRaw = udFunc.HasRawData();
                if ((hasRaw) || ((!hasRaw)&&(udFunc.functionSrt()!="RAW")))
                {
                    DrawWidgetBrowse* d = new DrawWidgetBrowse();
                    RawData* points = new(RawData);
                    if ((hasRaw) &&
                        ((r == udFunc.RawRange())||
                        (!r.isValid())))
                    {
                        points = udFunc.RawPoints();
                        r = points->RawRange();
                    }
                    else
                    {
                        if (!r.isValid())
                        {
                            // Set default range
                            r = Range(-10,10,0.1);
                        }
                        // Prepare for function computation
                        QString functionStr = udFunc.functionSrt();
                        QStringList funcionArgs = udFunc.args();

                        hfloat x;
                        for (x = r.m_min; x <= r.m_max; x += r.m_step)
                        {
                            QString tmpStr = functionStr;
                            tmpStr.replace(funcionArgs[0],x.toString(HF_MAXRES));
                            points->append(HPoint(x,Parse(tmpStr)));
                        }
                        // Just for last point
                        functionStr.replace(funcionArgs[0],r.m_max.toString(HF_MAXRES));
                        points->append(HPoint(r.m_max,Parse(functionStr)));

                        // Store RAW data in function
                        m_userdefinedFunctions[udFuncOrder].setRawRange(r);
                        m_userdefinedFunctions[udFuncOrder].setRawPoints(points);
                        emit(functionListUpdate(builtInFunctionList()));
                    }

                    points->updateRange();
                    d->drawWidget()->setPoints(points);
                    d->setXmin(points->xMin().toFloat());
                    d->setXmax(points->xMax().toFloat());
                    d->setYmin(points->yMin().toFloat());
                    d->setYmax(points->yMax().toFloat());
                    d->show();
                }
            }
        }
        break;

        default:
        {
        }
        break;
    }

    return retVal;
}

QString ParserClass::ExtractFormulaFromAssignment(QString str)
{
    QString expression = "";
    int equalPos = str.indexOf("=");
    if (equalPos>0)
    {
        expression = str.mid(equalPos+1,str.length()-equalPos-1);
    }
    return expression;
}

hfloat ParserClass::Parse(QString str, bool preview)
{
    // Remove spaces
    str = str.replace(" ","");
    // Convert k and M
//    str = str.replace ("k","000");
//    str = str.replace ("K","000");
//    str = str.replace ("M","000000");

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
        str = EvaluatePow(str);
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
    QRegExp operand(REGEXP6OPERAND);
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

QString ParserClass::ExtractRange(QString str)
{
    int indexOfFirstParentesis = str.indexOf('[');
    int i;
    for (i = indexOfFirstParentesis + 1; i < str.length(); i++)
    {
        if (str[i] == ']')
        {
            QString expression = str.mid(indexOfFirstParentesis, i - indexOfFirstParentesis + 1);
            return expression;
        }
    }
    return "";
}

Range ParserClass::EvaluateRange(QString str)
{
    Range retVal = Range();
    hfloat min,max,step;
    int j = str.indexOf('[');
    int i = str.indexOf(':',j+1);
    if ((j != -1)&&(i != -1)&&(i>j))
    {
        QString expression = str.mid(j + 1, i - j -1);
        min = Parse(expression);
        j = str.indexOf(':',i+1);
        if (j != -1)
        {
            expression = str.mid(i + 1, j - i -1);
            max = Parse(expression);
            i = str.indexOf(']',j+1);
            if (i != -1)
            {
                expression = str.mid(j + 1, i - j -1);
                step = Parse(expression);
                retVal = Range(min,max,step);
            }
        }
    }
    return retVal;
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

        QRegExp operand(REGEXP6OPERAND);
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

QString ParserClass::EvaluatePow(QString str)
{
    QString retVal;
    if (HasOperand(str,'^'))
    {
        int operandPos = str.indexOf('^');
        QString beforeOp = str.mid(0,operandPos);
        QString afterOp = str.mid(operandPos+1,str.length()-operandPos-1);

        QRegExp operand(REGEXP6OPERAND);
        QStringList beforeOpSplitted = beforeOp.split(operand);
        QStringList afterOpSplitted = afterOp.split(operand);
        QString firstValueStr = beforeOpSplitted[beforeOpSplitted.count()-1];
        QString lastValueStr = afterOpSplitted[0];
        hfloat result = hfloat::pow(this->Parse(firstValueStr),this->Parse(lastValueStr));

        QString evaluated = str.replace(firstValueStr+"^"+lastValueStr,result.toString("%.50Rf"));
        retVal = EvaluatePow(evaluated);
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

        QRegExp operand("REGEXP6OPERAND");
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

        QRegExp operand("REGEXP6OPERAND");
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
        char digitA = str[(l-1)-i].toLatin1();
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
        char digitA = str[(l-1)-i].toLatin1();
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
        if (str.contains(m_userdefinedFunctions[i].Name()+QString("(")))
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
                    break;
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
        retVal = func.toString();
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
        m_flexParse.parse(name+"="+valueStr);
    }
    // Load User defined functions
    m_userdefinedFunctions.clear();
    in >> l;
    for (i = 0; i < l; i++)
    {
        userdefinedFunctions newUDF;
        newUDF.Load(in);
        m_userdefinedFunctions.append(newUDF);
        QString toBeParsed = "def "+ newUDF.Name() + "(";
        int argNum = newUDF.args().length();
        for (int i = 0; i < argNum; i++)
        {
            toBeParsed.append(newUDF.args().at(i));
            if (i < argNum - 1)
            {
                toBeParsed.append(",");
            }
        }
        toBeParsed.append (")=" + newUDF.functionSrt());
        m_flexParse.parse(toBeParsed);
    }
}

void ParserClass::ImportRawData(QString fileName)
{
    QFile file(fileName);
    file.open(QIODevice::ReadOnly);
    QDataStream in(&file);

    qint64 maxLen = 1000;
    char buff[maxLen];
    int readByte = 1;
    RawData* points = new(RawData);
    while (readByte > 0)
    {
        readByte = file.readLine(buff,maxLen);
        if (readByte > 0)
        {
            QString str(buff);
            if (str.indexOf(";")!=-1)
            {
                QStringList strSplit = str.split(";");
                if (strSplit.count()==2)
                {
                    hfloat x(strSplit.at(0));
                    QString yStr = strSplit.at(1);
                    yStr.remove('\r');
                    yStr.remove('\n');
                    hfloat y(yStr);
                    if ((!x.isNan())&&(!y.isNan()))
                    {
                        points->append(HPoint(x,y));
                    }
                }
            }
        }
    }

    // Store RAW data in function
    userdefinedFunctions funct;
    Range r(points->at(0).x(),points->at(points->count()-1).x(),points->at(1).x()-points->at(0).x());
    funct.setRawRange(r);
    funct.setRawPoints(points);
    funct.setName("Imported");
    funct.setFunctionStr("RAW");
    m_userdefinedFunctions.append(funct);
    emit(functionListUpdate(builtInFunctionList()));
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
