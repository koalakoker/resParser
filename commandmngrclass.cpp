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
        retVal.append("list or ls to show varaibles.");
        retVal.append("clear to delete all varaibles.");
        retVal.append("E12 to show all E12 resitor values.");
        retVal.append("E24 to show all E24 resitor values.");
        retVal.append("->E12 to round to nearest E12 resitor values.");
        retVal.append(": parallel operator.<br>");
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
