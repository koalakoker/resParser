#include "tableinfoelement.h"

TableInfoElement::TableInfoElement()
{
}

TableInfo::TableInfo()
{
    m_column = 0;
}

TableInfo::TableInfo(TableInfo const &ti) : QVector<TableInfoElement>(ti)
{
    m_column = ti.column();
}

int TableInfo::column(void) const
{
    return m_column;
}

int TableInfo::row(void) const
{
    return this->count();
}

void TableInfo::setColumn(int val)
{
    m_column = val;
}
