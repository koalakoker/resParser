#ifndef FLEXPARSE_H
#define FLEXPARSE_H

#include <QString>
#include "keywordcode.h"

class FlexParse
{
public:
    FlexParse();

    QString parse(QString inp);

    keyWordCode_t m_kwc;
};

#endif // FLEXPARSE_H
