#include "commandmngrclass.h"

CommandMngrClass::CommandMngrClass()
{
    lastCommand = 0;
    commandIndex = 0;
}

QString CommandMngrClass::AddNewCommand(QString qsInput)
{
    commands.append(qsInput);
    commandIndex = lastCommand;
    lastCommand++;

    /* execute command */
    QString retVal;
    if ((qsInput.contains("list"))||(qsInput.contains("ls")))
    {
        int i;
        for (i = 0; i < parser.VariableCreated(); i++)
        {
            Variable* var = parser.GetVariableAtIndex(i);
            retVal.append(var->ToString());
            retVal.append("<br>");
        }
    }
    else if (qsInput.contains("clear"))
    {
        parser.Clear();
    }
    else if (qsInput == "E12")
    {
        retVal.append(Resistor::E12ValuesToString());
        retVal.append("<br>");
    }
    else if (qsInput == "E24")
    {
        retVal.append(Resistor::E24ValuesToString());
        retVal.append("<br>");
    }
    else if (qsInput.toLower() == "usage")
    {
        retVal.append("- Using built-in function function_name(arg,[arg])<br>");
        retVal.append("- Variable assignment: var_name = expression<br>");
        retVal.append("- Show variables ""list"" or ""ls"".<br>");
        retVal.append("- ""clear"" to delete all varaibles.<br>");
        retVal.append("- ""E12"" to show all E12 resitor values.<br>");
        retVal.append("- ""E24"" to show all E24 resitor values.<br>");
        retVal.append("- ""->E12"" to round to nearest E12 resitor values.<br>");
        retVal.append("- "":"" parallel operator between resistors.<br>");
    }
    else
    {
        hfloat result = parser.Parse(qsInput);
        parser.StoreVariable("ans",result); // Store the last result
        retVal.append(QString("ans=%1<br>").arg(result.toString()));
    }
    return retVal;
}

QString CommandMngrClass::GetPreviousCommand(void)
{
    QString retVal = "";
    if ((commandIndex < commands.count()) && (commandIndex >= 0))
    {
        retVal = commands[commandIndex];
        if (commandIndex > 0)
        {
            commandIndex--;
        }
    }
    return  retVal;
}

QString CommandMngrClass::GetNextCommand(void)
{
    QString retVal = "";
    if (commandIndex < lastCommand - 1)
    {
        commandIndex++;
    }
    if ((commandIndex < commands.count()) && (commandIndex >= 0))
    {
        retVal = commands[commandIndex];
    }
    return  retVal;
}

hfloat CommandMngrClass::PreviewResult(QString qsInput)
{
    return parser.Parse(qsInput);
}

QStringList CommandMngrClass::builtInFunctionList(void)
{
    return parser.builtInFunctionList();
}
