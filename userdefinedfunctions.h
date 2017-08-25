#ifndef USERDEFINEDFUNCTIONS_H
#define USERDEFINEDFUNCTIONS_H

#include <QString>
#include <QStringList>
#include <QVector>
#include "fpoint.h"
#include "range.h"
#include "rawdata.h"

class userdefinedFunctions
{
private:
    QString m_functionStr;
    QString m_name;
    QStringList m_args;
    RawData *m_points;
    Range m_range;

public:
    userdefinedFunctions();
    void setName(QString name);
    void setFunctionStr(QString str);
    void setArgs(QStringList args);
    QString Name(void);
    QString NameArgs(void);
    QString functionSrt(void);
    QStringList args(void);
    void setRawPoints(RawData *points);
    void setRawRange(Range range);
    bool HasRawData(void) const;
    RawData* RawPoints(void) const;
    Range RawRange(void) const;

    QString toString(void);

    void Save(QDataStream& out);
    void Load(QDataStream& in);
};

#endif // USERDEFINEDFUNCTIONS_H
