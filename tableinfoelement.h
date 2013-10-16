#ifndef TABLEINFOELEMENT_H
#define TABLEINFOELEMENT_H

#include <QStringList>
#include <QVector>

class TableInfoElement
{
public:
    TableInfoElement();

    QStringList m_value;
};

class TableInfo : public QVector<TableInfoElement>
{
public:
    TableInfo();
    TableInfo(TableInfo const &ti);

    int column(void) const;
    int row(void) const;

    void setColumn(int val);

private:
    int m_column;

};

#endif // TABLEINFOELEMENT_H
