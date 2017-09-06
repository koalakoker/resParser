#ifndef FLEXPARSE_H
#define FLEXPARSE_H

#include <QString>
#include <QStringList>
#include "keywordcode.h"

class FlexParse
{
public:
    FlexParse();

    QString parse(QString inp);
    void deleteFunc(QString name);

    keyWordCode_t m_kwc;

    bool m_newAssignVar;
    bool m_newAssignFunc;

    QString m_symbol;
    QStringList m_args;
};

#endif // FLEXPARSE_H
