#ifndef USERDEFINEDFUNCTIONS_H
#define USERDEFINEDFUNCTIONS_H

#include "QString"
#include "QStringList"

class userdefinedFunctions
{
private:
    QString m_functionStr;
    QString m_name;
    QStringList m_args;
public:
    userdefinedFunctions();
    void setName(QString name);
    void setFunctionStr(QString str);
    void setArgs(QStringList args);
    QString Name(void);
    QString functionSrt(void);
    QStringList args(void);

    QString toString(void);
};

#endif // USERDEFINEDFUNCTIONS_H
