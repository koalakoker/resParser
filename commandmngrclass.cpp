#include "commandmngrclass.h"

CommandMngrClass::CommandMngrClass()
{
   lastCommand = 0;
   commandIndex = 0;
}

void CommandMngrClass::AddNewCommand(QString str)
{
    commands.append(str);
    commandIndex = lastCommand;
    lastCommand++;
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
