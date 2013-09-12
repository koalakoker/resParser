#ifndef COMMANDMNGRCLASS_H
#define COMMANDMNGRCLASS_H

#include "QStringList"

class CommandMngrClass
{
private:
    QStringList commands;
    int lastCommand;
    int commandIndex;
public:
    CommandMngrClass();
    void AddNewCommand(QString str);
    QString GetPreviousCommand(void);
    QString GetNextCommand(void);
};

#endif // COMMANDMNGRCLASS_H
